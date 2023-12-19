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

// ORB 기반 특징점 검출 및 descriptors 계산
void getDescriptors(const Mat& image, vector<KeyPoint>& keypoints, Mat& descriptors);
// 두 이미지의 descriptor간의 매칭
void matchDescriptors(const Mat& descriptors1, const Mat& descriptors2, vector<DMatch>& matches);
// RANSAC 기반 두 이미지 간의 homographyomography 행렬 계산
Mat computeHomography(const vector<KeyPoint>& keypoints1, const vector<KeyPoint>& keypoints2, const vector<DMatch>& matches);
// 두 이미지를 병합
Mat merge(const Mat& image1, const Mat& image2, const Mat& homography);
// 파노라마에 연결
Mat mergePanorama(const Mat& panorama, const Mat& image, const Mat& homography);

int main()
{
    Mat pano;
    Mat homography = Mat::eye(3, 3, CV_64F);
    vector<DMatch> matches;

    Mat pano1= imread("pano1.jpg");
    vector<KeyPoint> keypoints1;
    Mat descriptors1;
    getDescriptors(pano1, keypoints1, descriptors1);
    pano = pano1;

    Mat pano2 = imread("pano2.jpg");
    vector<KeyPoint> keypoints2;
    Mat descriptors2;
    getDescriptors(pano2, keypoints2, descriptors2);
    matchDescriptors(descriptors1, descriptors2, matches);
    homography *= computeHomography(keypoints1, keypoints2, matches);
    pano = mergePanorama(pano, pano2, homography);

    Mat pano3 = imread("pano3.jpg");
    vector<KeyPoint> keypoints3;
    Mat descriptors3;
    getDescriptors(pano3, keypoints3, descriptors3);
    matchDescriptors(descriptors2, descriptors3, matches);
    homography *= computeHomography(keypoints2, keypoints3, matches);
    pano = mergePanorama(pano, pano3, homography);

    Mat pano4 = imread("pano4.jpg");
    vector<KeyPoint> keypoints4;
    Mat descriptors4;
    getDescriptors(pano4, keypoints4, descriptors4);
    matchDescriptors(descriptors3, descriptors4, matches);
    homography *= computeHomography(keypoints3, keypoints4, matches);
    pano = mergePanorama(pano, pano4, homography);

    imshow("Panorama", pano);
    imwrite("Panorama.jpg", pano);
    waitKey(0);

    return 0;
}

// ORB 기반의 특징점 검출 및 서술자 추출
void getDescriptors(const Mat& image, vector<KeyPoint>& keypoints, Mat& descriptors)
{
    Ptr<ORB> orb = ORB::create(1000);
    orb->detectAndCompute(image, Mat(), keypoints, descriptors);
}

// 특징점 매칭
void matchDescriptors(const Mat& descriptors1, const Mat& descriptors2, vector<DMatch>& matches)
{
    Ptr<BFMatcher> matcher = BFMatcher::create(NORM_HAMMING);
    vector<vector<DMatch>> knn_matches;
    matcher->knnMatch(descriptors1, descriptors2, knn_matches, 2);
    // 필터링
    const float ratio_thresh = 0.7f;
    for (size_t i = 0; i < knn_matches.size(); i++)
        if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
            matches.push_back(knn_matches[i][0]);
}

// 호모그래피 행렬 계산
Mat computeHomography(const vector<KeyPoint>& keypoints1, const vector<KeyPoint>& keypoints2, const vector<DMatch>& matches)
{
    vector<Point2f> points1, points2;
    for (const auto& match : matches) {
        points1.push_back(keypoints1[match.queryIdx].pt);
        points2.push_back(keypoints2[match.trainIdx].pt);
    }
    return findHomography(points2, points1, RANSAC);
}

// 이미지 변환 및 스티칭
Mat merge(const Mat& image1, const Mat& image2, const Mat& homography)
{
    Mat result;
    warpPerspective(image2, result, homography, Size(image1.cols + image2.cols, image1.rows));
    Mat half(result, Rect(0, 0, image1.cols, image1.rows));
    image1.copyTo(half);
    return result;
}

Mat mergePanorama(const Mat& panorama, const Mat& image, const Mat& homography)
{
    // 결과 이미지의 크기
    vector<Point2f> corners(4);
    corners[0] = Point2f(0, 0);
    corners[1] = Point2f((float)image.cols, 0);
    corners[2] = Point2f((float)image.cols, (float)image.rows);
    corners[3] = Point2f(0, (float)image.rows);
    vector<Point2f> transformed_corners(4);
    perspectiveTransform(corners, transformed_corners, homography);

    // 변환된 이미지의 최소 사이즈를 계산
    double max_x = 0, max_y = 0;
    for (const auto& pt : transformed_corners)
    {
        max_x = max(max_x, (double)pt.x);
        max_y = max(max_y, (double)pt.y);
    }
    Size result_size = Size(max((int)max_x, panorama.cols), max((int)max_y, panorama.rows));
    Mat warpedImage;
    warpPerspective(image, warpedImage, homography, result_size);

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
    warpPerspective(maskNew, warpedMaskNew, homography, result_size);
    warpPerspective(maskPanorama, warpedMaskPanorama, homography, result_size);

    // Calculate the overlap region mask
    Mat overlapMask;
    bitwise_and(warpedMaskNew, warpedMaskPanorama, overlapMask);

    // Perform blending
    for (int y = 0; y < result.rows; ++y)
    {
        for (int x = 0; x < result.cols; ++x)
        {
            if (overlapMask.at<uchar>(y, x))
            {
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
