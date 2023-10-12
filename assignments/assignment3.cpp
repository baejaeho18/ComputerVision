#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main()
{
	Mat moon_image;
	Mat moon_filtered_image;
	Mat saltnpepper_image;
	Mat saltnpepper_filtered_image;

	moon_image = imread("Moon.jpeg", 0);
	imshow("moon", moon_image);

	int moon_width = moon_image.cols;
	int moon_height = moon_image.rows;
	Rect moon_rect = Rect(moon_width / 2, 0, moon_width / 2, moon_height);
	moon_filtered_image = moon_image.clone();
	Mat original_image = moon_image(moon_rect);
	Mat blurred_image;
	blur(original_image, blurred_image, Size(3, 3));
	Mat sharpened_image = original_image + 3 * (original_image - blurred_image);
	sharpened_image.copyTo(moon_filtered_image(moon_rect));
	imshow("moon_filtered", moon_filtered_image);

	saltnpepper_image = imread("saltnpepper.png", 0);
	imshow("saltnpepper", saltnpepper_image);

	int snp_width = saltnpepper_image.cols;
	int snp_height = saltnpepper_image.rows;
	Rect snp_rect = Rect(0, 0, snp_width / 2, snp_height);
	saltnpepper_filtered_image = saltnpepper_image.clone();
	medianBlur(saltnpepper_image(snp_rect), saltnpepper_filtered_image(snp_rect), 9);
	imshow("saltnpepper_filtered", saltnpepper_filtered_image);

	waitKey(0);
}