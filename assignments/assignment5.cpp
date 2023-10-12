#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main()
{
	// For 'finger_print.png', set finger print region to 0 and background region to 255
	Mat finger_print_before, finger_print_after;
	finger_print_before = imread("finger_print.png", 0);
	threshold(finger_print_before, finger_print_after, 0, 255, THRESH_BINARY | THRESH_OTSU);
	imshow("finger_print", finger_print_after);

	// For 'adatpve_1.jpg', set character region to 0 and background region to 255
	Mat adaptive_1_before, adaptive_1_after;
	adaptive_1_before = imread("adaptive_1.jpg", 0);
	adaptiveThreshold(adaptive_1_before, adaptive_1_after, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 85, 15);
	imshow("adaptive_1", adaptive_1_after);

	// For 'adaptive.png', set character region to 0 and background region to 255
	Mat adaptive_before, adaptive_after;
	adaptive_before = imread("adaptive.png", 0);
	adaptiveThreshold(adaptive_before, adaptive_after, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 15, 15);
	imshow("adaptive", adaptive_after);

    waitKey(0);
}