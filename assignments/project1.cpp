#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat negativeTransformation(Mat img) {
    Mat result = img.clone();
    for (int j = 0; j < img.rows; j++)
        for (int i = 0; i < img.cols*3; i++)
            img.at<uchar>(j, i) = 255 - img.at<uchar>(j, i);
    return img;
}

Mat gammaTransformation(Mat img, float gamma) {
    Mat result = img.clone();
    MatIterator_<uchar> it, end;
    unsigned char pix[256];

    for (int i = 0; i < 256; i++) {
        pix[i] = saturate_cast<uchar>(pow((float)(i / 255.0), gamma) * 255.0f);
    }
    for (int j = 0; j < img.rows; j++)
        for (int i = 0; i < img.cols*3; i++)
            result.at<uchar>(j, i) = pix[result.at<uchar>(j, i)];

    return result;
}

Mat histogramEqualization(Mat img) {
    Mat result = img.clone();
    vector<Mat> ic(3);
    cvtColor(img, img, CV_BGR2HSV);
    split(img, ic);
    equalizeHist(ic[2], ic[2]);
    merge(ic, result);
    cvtColor(result, result, CV_HSV2BGR);
    return result;
}

Mat colorSlicing(Mat img) {
    Mat result = img.clone();
    vector<Mat> cs(3);  // color slicing
    uchar* h;
    uchar* s;

    cvtColor(result, result, CV_BGR2HSV);
    split(img, cs);
    for (int i = 0; i < result.rows; i++) {
        h = cs[0].ptr<uchar>(i);
        s = cs[1].ptr<uchar>(i);
        for (int j = 0; j < result.cols; j++) 
            s[j] = (h[j] < 9 || h[j] > 23) ? s[j] : 0;
    }
    merge(cs, result);
    cvtColor(result, result, CV_HSV2BGR);
    return result;
}

Mat colorConversion(Mat img) {
    Mat result = img.clone();
    vector<Mat> cc(3); // color conversion
    uchar* h;

    cvtColor(result, result, CV_BGR2HSV);
    for (int i = 0; i < result.rows; i++) {
        h = cc[0].ptr<uchar>(i);
        for (int j = 0; j < result.cols; j++) 
            h[j] = (h[j] > 129) ? h[j] - 129 : h[j] + 50;
    }
    merge(cc, result);
    cvtColor(result, result, CV_HSV2BGR);
    return result;
}

Mat averageFiltering(Mat img) {
    Mat result;
    cvtColor(img, img, CV_BGR2HSV);
    blur(img, result, Size(9, 9));
    cvtColor(result, result, CV_HSV2BGR);
    return result;
}

Mat unsharpMasking(Mat img) {
    Mat result;
    Mat unsharp = img - averageFiltering(img);
    result = img + 3 * unsharp;
    return img;
}

Mat whiteBalancing(Mat img) {
    int sum, temp;
    double avg;
    Mat bgr_channels[3];
    split(img, bgr_channels);

    for (int c = 0; c < img.channels(); c++) {
        sum = 0;
        avg = 0.0f;
        for (int i = 0; i < img.rows; i++) {
            for (int j = 0; j < img.cols; j++) {
                sum += bgr_channels[c].at<uchar>(i, j);
            }
        }
        avg = sum / (img.rows * img.cols);
        for (int i = 0; i < img.rows; i++) {
            for (int j = 0; j < img.cols; j++) {
                temp = (128 / avg) * bgr_channels[c].at<uchar>(i, j);
                bgr_channels[c].at<uchar>(i, j) = (temp > 255) ? 255 : temp;
            }
        }
    }
    merge(bgr_channels, 3, img);
    return img;
}

Mat resetFrame(Mat img) {
    return img;
}

int main()
{
	VideoCapture cap;
	int delay;
	Mat frame;
	Mat cvtFrame;
    char key = '\0';
    char steadyKey = 'r';
    char beforeSteadyKey = 'r';

	// Read 'vidoe.mp4'
	if (cap.open("video.mp4") == 0)
	{
		cout << "no such file!" << endl;
		waitKey(0);
	}
	delay = 1000 / cap.get(CAP_PROP_FPS);

    while (1) {
        cap >> frame;
        if (frame.empty()) {
            cout << "end of video" << endl;
            break;
        }

        switch (steadyKey) {
        case 'n':   // good?
            cvtFrame = negativeTransformation(frame);
            break;
        case 'g':   // good
            cvtFrame = gammaTransformation(frame, 2.5);
            break;
        case 'h':   // good
            cvtFrame = histogramEqualization(frame);
            break;
        case 's':   // not so white
            cvtFrame = colorSlicing(frame);
            break;
        case 'c':   // not work
            cvtFrame = colorConversion(frame);
            break;
        case 'a':   // good
            cvtFrame = averageFiltering(frame);
            break;
        case 'u':   // so highlighted
            cvtFrame = unsharpMasking(frame);
            break;
        case 'w':   // good
            cvtFrame = whiteBalancing(frame);
            break;
        case 'r':   //good
            cvtFrame = resetFrame(frame);
            break;
        default:
            cout << "wrong input" << endl;
            steadyKey = beforeSteadyKey;
            break;
        }

        // Display one window 'video'
        imshow("video", cvtFrame);
        key = waitKey(delay);

        // Use waitKey for user interation
        if (key == 27) { // 'Esc' key to exit
            break;
        }
        else if (key != -1) {
            beforeSteadyKey = steadyKey;
            steadyKey = key;
        }
    }
    waitKey(0);
}
