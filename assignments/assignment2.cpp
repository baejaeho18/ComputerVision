#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main()
{
	Mat gray_image;
	Mat rotated_image;
	Mat result_image;
	MatIterator_<uchar> it, end;
	float gamma;
	unsigned char pix[256];

	gray_image = imread("lena.png", 0);
	// rotate the image and gamma-transformation
	rotated_image = gray_image.clone();
	for (int j = 0; j < gray_image.rows; j++)
		for (int i = 0; i < gray_image.cols; i++)
			rotated_image.at<uchar>(j, i) = gray_image.at<uchar>(i, gray_image.rows - 1 - j);

	gamma = 10.0;
	result_image = rotated_image.clone();
	for (int i = 0; i < 256; i++) {
		pix[i] = saturate_cast<uchar>(pow((float)(i / 255.0), gamma) * 255.0f);
	}
	for (int j = 0; j < rotated_image.rows; j++){
		for (int i = 0; i < rotated_image.cols; i++) {
			if (rotated_image.at<uchar>(j, i) < 127)
				result_image.at<uchar>(j, i) = 255 - rotated_image.at<uchar>(j, i);
			else
				result_image.at<uchar>(j, i) = pix[rotated_image.at<uchar>(j, i)];
		}
	}
	//imshow("rotated", rotated_image);
	//mshow("result_1", result_image);

	//do it at once!
	//result_image = gray_image.clone();
	//for (int i = 0; i < 256; i++) {
	//	pix[i] = saturate_cast<uchar>(pow((float)(i / 255.0), gamma) * 255.0f);
	//}
	//for (int j = 0; j < gray_image.rows; j++) {
	//	for (int i = 0; i < gray_image.cols; i++) {
	//		if (gray_image.at<uchar>(j, i) < 127)
	//			result_image.at<uchar>(gray_image.cols -1 - i, j) = 255 - gray_image.at<uchar>(j, i);
	//		else
	//			result_image.at<uchar>(gray_image.cols - 1 - i, j) = pix[gray_image.at<uchar>(j, i)];
	//	}
	//}

	imshow("gray image", gray_image);
	imshow("result", result_image);
	waitKey(0);
}