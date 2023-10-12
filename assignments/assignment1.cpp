#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main()
{
	Mat frame;
	int total_frame;
	int current_time = 0;
	int post_frame;
	int fps;
	int delay;
	VideoCapture cap;

	if (cap.open("background.mp4") == 0)
	{
		cout << "no such file!" << endl;
		waitKey(0);
	}

	total_frame = cap.get(CAP_PROP_FRAME_COUNT);
	fps = cap.get(CAP_PROP_FPS);
	delay = 1000 / fps;
	cout << "FPS : " << fps << endl;

	while (current_time < 3000)
	{
		cap >> frame;
		if (frame.empty())
		{
			cout << "end of video" << endl;
			break;
		}
		imshow("video", frame);

		current_time = cap.get(CAP_PROP_POS_MSEC);
		post_frame = cap.get(CAP_PROP_POS_FRAMES);

		cout << "Frame: " << post_frame - 1 << " / " << total_frame << endl;
		waitKey(delay);
	}
}