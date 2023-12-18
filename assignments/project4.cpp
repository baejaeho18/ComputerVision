#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

// ORB 기반의 특징점 검출 및 서술자 추출
void detectAndDescribe(const Mat& image, vector<KeyPoint>& keypoints, Mat& descriptors) {
    Ptr<ORB> orb = ORB::create(1000);
    orb->detectAndCompute(image, Mat(), keypoints, descriptors);
}

// 특징점 매칭 (Brute-Force Matcher)
void matchFeatures(const Mat& desc1, const Mat& desc2, vector<DMatch>& matches) {
    Ptr<BFMatcher> matcher = BFMatcher::create(NORM_HAMMING);
    vector<vector<DMatch>> knn_matches;
    matcher->knnMatch(desc1, desc2, knn_matches, 2); // 2 for knn
    // 필터링: Lowe's ratio test
    const float ratio_thresh = 0.7f;
    for (size_t i = 0; i < knn_matches.size(); i++) {
        if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance) {
            matches.push_back(knn_matches[i][0]);
        }
    }
}

// 호모그래피 행렬 계산
Mat computeHomography(const vector<KeyPoint>& keypoints1, const vector<KeyPoint>& keypoints2, const vector<DMatch>& matches) {
    vector<Point2f> points1, points2;
    for (const auto& match : matches) {
        points1.push_back(keypoints1[match.queryIdx].pt);
        points2.push_back(keypoints2[match.trainIdx].pt);
    }
    return findHomography(points2, points1, RANSAC); // 주의: 순서 변경
}

// 이미지 변환 및 스티칭
Mat warpAndStitch(const Mat& image1, const Mat& image2, const Mat& H) {
    // Create a black image that will fit both the images
    Mat result;
    warpPerspective(image2, result, H, Size(image1.cols + image2.cols, image1.rows));
    // Copy image1 on the first half of full image
    Mat half(result, Rect(0, 0, image1.cols, image1.rows));
    image1.copyTo(half);
    return result;
}

Mat stitchTwoImages(const Mat& panorama, const Mat& image, const Mat& H) {
    // 이미지 변환 시 결과 이미지의 크기를 계산
    vector<Point2f> corners(4);
    corners[0] = Point2f(0, 0);
    corners[1] = Point2f((float)image.cols, 0);
    corners[2] = Point2f((float)image.cols, (float)image.rows);
    corners[3] = Point2f(0, (float)image.rows);

    // 변환된 코너 포인트 계산
    vector<Point2f> transformed_corners(4);
    perspectiveTransform(corners, transformed_corners, H);

    // 변환된 이미지가 들어갈 수 있는 최소 사이즈를 계산
    double max_x = 0, max_y = 0;
    for (const auto& pt : transformed_corners) {
        max_x = max(max_x, (double)pt.x);
        max_y = max(max_y, (double)pt.y);
    }
    Size result_size = Size(max((int)max_x, panorama.cols), max((int)max_y, panorama.rows));
    Mat warpedImage;
    warpPerspective(image, warpedImage, H, result_size);

    // Create an empty panorama image
    Mat result = Mat::zeros(result_size, panorama.type());

    // Copy the original panorama into the result
    panorama.copyTo(result(Rect(0, 0, panorama.cols, panorama.rows)));

    // Create a mask for the new image and a mask for the existing panorama
    Mat maskNew = Mat::zeros(image.size(), CV_8U);
    maskNew.setTo(255);
    Mat maskPanorama = Mat::zeros(panorama.size(), CV_8U);
    maskPanorama.setTo(255);

    // Warp the masks
    Mat warpedMaskNew, warpedMaskPanorama;
    warpPerspective(maskNew, warpedMaskNew, H, result_size);
    warpPerspective(maskPanorama, warpedMaskPanorama, H, result_size);

    // Calculate the overlap region mask
    Mat overlapMask;
    bitwise_and(warpedMaskNew, warpedMaskPanorama, overlapMask);

    // Perform blending
    for (int y = 0; y < result.rows; ++y) {
        for (int x = 0; x < result.cols; ++x) {
            if (overlapMask.at<uchar>(y, x)) {
                float alpha = 0.5; // Simple average for blending
                Vec3b colorNew = warpedImage.at<Vec3b>(y, x);
                Vec3b colorPanorama = result.at<Vec3b>(y, x);
                result.at<Vec3b>(y, x) = alpha * colorNew + (1 - alpha) * colorPanorama;
            }
        }
    }

    // Copy non-overlapped regions from the new image to the result
    warpedImage.copyTo(result, warpedMaskNew);

    return result;
}

int main() 
{
    vector<string> filenames = { "pano1.jpg", "pano2.jpg", "pano3.jpg", "pano4.jpg" };
    vector<Mat> images;
    vector<vector<KeyPoint>> keypoints_list;
    vector<Mat> descriptors_list;

    // 이미지 로딩 및 특징점 검출과 서술자 추출
    for (const auto& filename : filenames) {
        Mat image = imread(filename, IMREAD_COLOR);
        if (image.empty()) {
            cout << "Could not read the image: " << filename << endl;
            return -1;
        }
        images.push_back(image);

        vector<KeyPoint> keypoints;
        Mat descriptors;
        detectAndDescribe(image, keypoints, descriptors);
        keypoints_list.push_back(keypoints);
        descriptors_list.push_back(descriptors);
    }

    // 첫 번째 이미지를 기준으로 설정
    Mat panorama = images[0];
    Mat accumulatedHomography = Mat::eye(3, 3, CV_64F);

    // 나머지 이미지들과 스티칭
    for (size_t i = 1; i < images.size(); ++i) {
        vector<DMatch> matches;
        matchFeatures(descriptors_list[i - 1], descriptors_list[i], matches);
        Mat H = computeHomography(keypoints_list[i - 1], keypoints_list[i], matches);
        accumulatedHomography = H * accumulatedHomography;
        panorama = stitchTwoImages(panorama, images[i], accumulatedHomography);
    }

    // 결과 출력
    imshow("Panorama", panorama);
    imwrite("Panorama.jpg", panorama);
    waitKey(0);

    return 0;
}
