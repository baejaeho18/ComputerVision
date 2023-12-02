#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat genrate_qm(Mat y, Mat quantization_mat);
double PSNR(Mat original, Mat frame);

int main() {
    Mat quantization_mat1 = (Mat_<double>(8, 8) << 16, 11, 10, 16, 24, 40, 51, 61,
                                                   12, 12, 14, 19, 26, 58, 60, 55,
                                                   14, 13, 16, 24, 40, 57, 69, 56,
                                                   14, 17, 22, 29, 51, 87, 80, 62,
                                                   18, 22, 37, 56, 68, 109, 103, 77, 
                                                   24, 35, 55, 64, 81, 104, 113, 92, 
                                                   49, 64, 78, 87, 103, 121, 120, 101,
                                                   72, 92, 95, 98, 112, 100, 103, 99);

    Mat quantization_mat2 = (Mat_<double>(8, 8) << 1, 1, 1, 1, 1, 1, 1, 1,
                                                    1, 1, 1, 1, 1, 1, 1, 1,
                                                    1, 1, 1, 1, 1, 1, 1, 1,
                                                    1, 1, 1, 1, 1, 1, 1, 1, 
                                                    1, 1, 1, 1, 1, 1, 1, 1, 
                                                    1, 1, 1, 1, 1, 1, 1, 1, 
                                                    1, 1, 1, 1, 1, 1, 1, 1, 
                                                    1, 1, 1, 1, 1, 1, 1, 1);

    Mat quantization_mat3 = (Mat_<double>(8, 8) << 100, 100, 100, 100, 100, 100, 100, 100,
                                                    100, 100, 100, 100, 100, 100, 100, 100,
                                                    100, 100, 100, 100, 100, 100, 100, 100,
                                                    100, 100, 100, 100, 100, 100, 100, 100, 
                                                    100, 100, 100, 100, 100, 100, 100, 100, 
                                                    100, 100, 100, 100, 100, 100, 100, 100, 
                                                    100, 100, 100, 100, 100, 100, 100, 100, 
                                                    100, 100, 100, 100, 100, 100, 100, 100);

    Mat image;
    Mat ycbcr_image;
    Mat ycbcr_channels[3];
    Mat y(512, 512, CV_8UC1);
    Mat dct_block;

    image = imread("Lena.png");
    cvtColor(image, ycbcr_image, CV_BGR2YCrCb);
    split(ycbcr_image, ycbcr_channels);
    for (int j = 0; j < 512; j++) {
        for (int i = 0; i < 512; i++) {
            y.at<uchar>(j, i) = 0;
            y.at<uchar>(j, i) = ycbcr_channels[0].at<uchar>(j, i);
        }
    }
    y.convertTo(y, CV_32FC1);

    for (int k = 0; k < 512; k = k + 8) {
        for (int j = 0; j < 512; j = j + 8) {
            Rect rect_dct_block(k, j, 8, 8);
            dct_block = y(rect_dct_block);
            dct(dct_block, dct_block, 0);
        }
    }

    Mat qm1 = genrate_qm(y, quantization_mat1);
    imshow("QM1", qm1);
    Mat qm2 = genrate_qm(y, quantization_mat2);
    imshow("QM2", qm2);
    Mat qm3 = genrate_qm(y, quantization_mat3);
    imshow("QM3", qm3);

    for (int k = 0; k < 512; k = k + 8) {
        for (int j = 0; j < 512; j = j + 8) {
            Rect rect_dct_block(k, j, 8, 8);
            dct_block = y(rect_dct_block);
            dct(dct_block, dct_block, 1);
        }
    }
    y.convertTo(y, CV_8UC1);
    imshow("Original Y", y);

    cout.precision(8);  // 유효숫자 수?
    cout << "QM1: psnr = " << PSNR(y, qm1) << endl;
    cout << "QM2: psnr = " << PSNR(y, qm2) << endl;
    cout << "QM3: psnr = " << PSNR(y, qm3) << endl;

    waitKey(0);
    return 0;
}

Mat genrate_qm(Mat y, Mat quantization_mat) {
    Mat dct_block;
    Mat qm(512, 512, CV_32FC1);

    for (int k = 0; k < 512; k = k + 8) {
        for (int a = 0; a < 512; a = a + 8) {
            for (int i = k; i < k + 8; i++) {
                for (int j = a; j < a + 8; j++) {
                    qm.at<float>(i, j) = round(y.at<float>(i, j) / quantization_mat.at<double>(i - k, j - a));
                    qm.at<float>(i, j) = qm.at<float>(i, j) * quantization_mat.at<double>(i - k, j - a);
                }
            }
        }
    }

    for (int k = 0; k < 512; k = k + 8) {
        for (int j = 0; j < 512; j = j + 8) {
            Rect rect_dct_block(k, j, 8, 8);
            dct_block = qm(rect_dct_block);
            dct(dct_block, dct_block, 1);
        }
    }
    qm.convertTo(qm, CV_8UC1);
    return qm;
}

double PSNR(Mat original, Mat frame) {

    double PSNR = 0.0;
    double MSE = 0.0;
    double value = 0.0;
    for (int i = 0; i < 512; i++) {
        for (int j = 0; j < 512; j++) {
            value = original.at<uchar>(i, j) - frame.at<uchar>(i, j);
            MSE = value * value + MSE;
        }
    }
    MSE /= (512 * 512);
    PSNR = 20 * log10(255.0) - 10 * log10(MSE);

    return PSNR;
}