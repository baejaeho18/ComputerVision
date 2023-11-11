#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

void lookROI(Mat frame, Rect left, Rect center, Rect right, Rect top);
void drawLeftHoughLine(Mat frame, vector<Vec2f> lines, Rect roi);
void drawCenterHoughLine(Mat frame, vector<Vec2f> lines, Rect roi);
void drawRightHoughLine(Mat frame, vector<Vec2f> lines, Rect roi);

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
	Rect left(150, 280, 150, 200);
	Rect center(250, 250, 200, 200);
	Rect right(400, 280, 150, 200);
	Mat left_roi, center_roi, right_roi;
	Mat left_canny, center_canny, right_canny;
	Mat frame_copy;
	vector<Vec2f> left_line;
	vector<Vec2f> center_line;
	vector<Vec2f> right_line;
	vector<int> frameBuffer(21, 0);
	int onesCount;

	// Moving
	Rect top(0, 0, 720, 200);
	Rect front_car(230, 250, 200, 200);
	Mat background, car_roi;
	Ptr<BackgroundSubtractor> pMOG2 = createBackgroundSubtractorMOG2();

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
		onesCount = 0;
		left_roi = frame_copy(left);
		right_roi = frame_copy(right);
		cvtColor(left_roi, left_roi, CV_BGR2GRAY);
		cvtColor(right_roi, right_roi, CV_BGR2GRAY);
		blur(left_roi, left_canny, Size(5, 5));
		blur(right_roi, right_canny, Size(5, 5));
		Canny(left_canny, left_canny, 10, 70, 3);
		Canny(right_canny, right_canny, 10, 70, 3);
		HoughLines(left_canny, left_line, 1, (CV_PI / 180), 60, 0, 0, 20 * (CV_PI / 180), 60 * (CV_PI / 180));
		HoughLines(right_canny, right_line, 1, (CV_PI / 180), 60, 0, 0, 120 * (CV_PI / 180), 150 * (CV_PI / 180));

		if (left_line.empty() && right_line.empty())
		{
			center_roi = frame_copy(center);
			cvtColor(center_roi, center_roi, CV_BGR2GRAY);
			blur(center_roi, center_canny, Size(5, 5));
			Canny(center_canny, center_canny, 10, 70, 3);
			HoughLines(center_canny, center_line, 1, (CV_PI / 180), 60, 0, 0, 150 * (CV_PI / 180), 220 * (CV_PI / 180));
			if (!center_line.empty())
			{
				//printLaneDepartureAlarm(frame);
				cout << 1 << endl;
				frameBuffer.push_back(1);
			}
			else
			{
				cout << 0 << endl;
				frameBuffer.push_back(0);
			}
		}
		else {
			frameBuffer.push_back(0);
		}
		frameBuffer.erase(frameBuffer.begin());
		onesCount = count(frameBuffer.begin(), frameBuffer.end(), 1);
		if (onesCount >= 5)
		{
			printLaneDepartureAlarm(frame);
			cout << "Lane Departure" << endl;
		}
			
		lookROI(frame, left, center, right, top);
		drawLeftHoughLine(frame, left_line, left);
		drawCenterHoughLine(frame, center_line, center);
		drawRightHoughLine(frame, right_line, right);

		// Car Moving
		background = frame_copy(top);
		car_roi = frame_copy(front_car);
		cvtColor(background, background, CV_BGR2GRAY);



		imshow("Project2", frame);
		key = waitKey(delay);
	}
	return 0;
}

void printLaneDepartureAlarm(Mat frame)
{
	putText(frame, format("Lane Departure!"), Point(frame.cols / 2 - 240, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
}
void printMovingCarAlarm(Mat frame)
{
	putText(frame, format("Car is Moving!"), Point(frame.cols / 2, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
}

void lookROI(Mat frame, Rect left, Rect center, Rect right, Rect top)
{
	rectangle(frame, left, Scalar(0, 0, 122), 2);
	rectangle(frame, center, Scalar(0, 0, 122), 2);
	rectangle(frame, right, Scalar(0, 0, 122), 2);
	rectangle(frame, top, Scalar(0, 0, 122), 2);
}
void drawLeftHoughLine(Mat frame, vector<Vec2f> lines, Rect roi)
{
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0];
		float theta = lines[i][1];
		double a = cos(theta);
		double b = sin(theta);
		double x0 = a * rho + roi.x;
		double y0 = b * rho + roi.y;
		Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
		Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)));
		line(frame, pt1, pt2, Scalar(0, 145, 0), 1, 8, 0);
	}
}
void drawCenterHoughLine(Mat frame, vector<Vec2f> lines, Rect roi)
{
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0];
		float theta = lines[i][1];
		double a = cos(theta);
		double b = sin(theta);
		double x0 = a * rho + roi.x;
		double y0 = b * rho + roi.y;
		Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
		Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)));
		line(frame, pt1, pt2, Scalar(145, 0, 0), 1, 8, 0);
	}
}
void drawRightHoughLine(Mat frame, vector<Vec2f> lines, Rect roi)
{
	for (size_t i = 0; i < lines.size(); i++)
	{
		float rho = lines[i][0];
		float theta = lines[i][1];
		double a = cos(theta);
		double b = sin(theta);
		double x0 = a * rho + roi.x;
		double y0 = b * rho + roi.y;
		Point pt1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * (a)));
		Point pt2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * (a)));
		line(frame, pt1, pt2, Scalar(145, 145, 0), 1, 8, 0);
	}
}
