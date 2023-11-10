#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void printLaneDepartureAlarm(Mat frame);
void printMovingCarAlarm(Mat frame);

int main() 
{
	VideoCapture cap;
	int key = '\0';
	int delay;
	Mat frame;

	// Read 'Project2_Vidoe.mp4'
	if (cap.open("Project2_video.mp4") == 0)
	{
		cout << "No such file!" << endl;
		waitKey(0);
	}
	delay = 1000 / cap.get(CAP_PROP_FPS);
	// Lane
	Rect left(150, 200, 150, 300);
	Rect center(300, 200, 150, 300);
	Rect right(450, 200, 150, 300);
	Mat left_roi, center_roi, right_roi;
	Mat left_canny, center_canny, right_canny;
	Mat frame_copy;
	vector<Vec2f> left_line;
	vector<Vec2f> center_line;
	vector<Vec2f> right_line;
	// Moving


	while (1)
	{
		cap >> frame;
		if (frame.empty())
		{
			cout << "end of video" << endl;
			waitKey();
			break;
		}
		frame_copy = frame.clone();

		// Lane Departure
		left_roi = frame_copy(left);
		right_roi = frame_copy(right);
		cvtColor(left_roi, left_roi, CV_BGR2GRAY);
		cvtColor(right_roi, right_roi, CV_BGR2GRAY);
		blur(left_roi, left_canny, Size(5, 5));
		blur(right_roi, right_canny, Size(5, 5));
		Canny(left_canny, left_canny, 10, 70, 3);
		Canny(right_canny, right_canny, 10, 70, 3);
		HoughLines(left_canny, left_line, 1, (CV_PI / 180), 60, 0, 0, 30 * (CV_PI / 180), 60 * (CV_PI / 180));
		HoughLines(right_canny, right_line, 1, (CV_PI / 180), 60, 0, 0, 30 * (CV_PI / 180), 60 * (CV_PI / 180));
		
		if (left_line.empty() && right_line.empty())
		{
			center_roi = frame_copy(center);
			cvtColor(center_roi, center_roi, CV_BGR2GRAY);
			blur(center_roi, center_canny, Size(5, 5));
			Canny(center_canny, center_canny, 10, 70, 3);
			HoughLines(center_canny, center_line, 1, (CV_PI / 180), 65, 0, 0, 150 * (CV_PI / 180), 177 * (CV_PI / 180));
			if (!center_line.empty())
				printLaneDepartureAlarm(frame);
		}

		// Car Moving



		imshow("Project2", frame);
		key = waitKey(delay);
	}
	return 0;
}

void printLaneDepartureAlarm(Mat frame)
{
	putText(frame, format("Lane Departure!"), Point(frame.cols / 2, 40), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 2);
}
void printMovingCarAlarm(Mat frame)
{
	putText(frame, format("Car is Moving!"), Point(frame.cols / 2, 160), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 244, 0), 2);
}
