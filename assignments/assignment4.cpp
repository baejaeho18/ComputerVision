#include <iostream>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main()
{
    Mat frame;
    Mat frame_copy;
    Mat left_roi, right_roi;
    Mat left_canny, right_canny;
    VideoCapture cap;
    // Lt and RB of left_roi : (200,400)-(600,600)
    Rect left(200, 400, 400, 200);
    // Lt and RB of right_roi : (600,400)-(1000,600)
    Rect right(600, 400, 400, 200);
    int delay;
    float left_average_rho, left_average_theta, right_average_rho, right_average_theta;
    float left_cos = 0, left_sin = 0, left_x = 0, left_y = 0;
    float right_cos = 0, right_sin = 0, right_x = 0, right_y = 0;
    Point left_p1, left_p2, right_p1, right_p2;
    vector<Vec2f> left_lines;
    vector<Vec2f> right_lines;

    // 1-1) Read 'Road.mp4'
    if (cap.open("road.mp4") == 0) 
    {
        cout << "no such file!" << endl;
        waitKey(0);
    }
    delay = 1000 / cap.get(CAP_PROP_FPS);

    // Display frames that are between 0~20 seconds
    while (cap.get(CAP_PROP_POS_MSEC) < 20000) 
    {
        cap >> frame;
        if (frame.empty()) 
        {
            cout << "end of video" << endl;
            break;
        }
        frame_copy = frame.clone();

        // 1-2) Set left_roi and right_roi
        left_roi = frame_copy(left);
        right_roi = frame_copy(right);

        // 1-3) For each ROI, display canny edge
        // Use cvtColor, CV_BGR2GRAY to make a gray-scale image
        cvtColor(left_roi, left_roi, CV_BGR2GRAY);
        cvtColor(right_roi, right_roi, CV_BGR2GRAY);
        // Use blur function with mask size as 5x5
        blur(left_roi, left_canny, Size(5, 5));
        blur(right_roi, right_canny, Size(5, 5));
        // For THresholds for canny, use 10 and 60
        Canny(left_canny, left_canny, 10, 60, 3);
        Canny(right_canny, right_canny, 10, 60, 3);
        // 1-4) 0Display two windows 
        namedWindow("Left canny");
        moveWindow("Left canny", 200, 0);
        imshow("Left canny", left_canny);
        namedWindow("Right canny");
        moveWindow("Right canny", 600, 0);
        imshow("Right canny", right_canny);

        // 2) Draw lines corresponding to lanes
        // Line Filtering : Use angle information of lines
        HoughLines(left_canny, left_lines, 1, CV_PI / 180, 60, 0, 0, 30 * (CV_PI / 180), 60 * (CV_PI / 180));
        HoughLines(right_canny, right_lines, 1, CV_PI / 180, 60, 0, 0, 120 * (CV_PI / 180), 150 * (CV_PI / 180));
        // Line Merging : Take average of rho and theta of filtered lines
        left_average_rho = 0;
        left_average_theta = 0;
        for (int i = 0; i < left_lines.size(); i++) {
            left_average_rho = left_average_rho + left_lines[i][0];
            left_average_theta = left_average_theta + left_lines[i][1];
        }
        left_average_rho = left_average_rho / left_lines.size();
        left_average_theta = left_average_theta / left_lines.size();
        left_cos = cos(left_average_theta);
        left_sin = sin(left_average_theta);
        left_x = left_cos * left_average_rho + 200;
        left_y = left_sin * left_average_rho + 400;
        left_p1 = Point(cvRound(left_x + 1000 * (-left_sin)), cvRound(left_y + 1000 * left_cos));
        left_p2 = Point(cvRound(left_x - 1000 * (-left_sin)), cvRound(left_y - 1000 * left_cos));

        right_average_rho = 0;
        right_average_theta = 0;
        for (int i = 0; i < right_lines.size(); i++) {
            right_average_rho = right_average_rho + right_lines[i][0];
            right_average_theta = right_average_theta + right_lines[i][1];
        }
        right_average_rho = right_average_rho / right_lines.size();
        right_average_theta = right_average_theta / right_lines.size();
        right_cos = cos(right_average_theta);
        right_sin = sin(right_average_theta);
        right_x = right_cos * right_average_rho + 600;
        right_y = right_sin * right_average_rho + 400;
        right_p1 = Point(cvRound(right_x + 1000 * (-right_sin)), cvRound(right_y + 1000 * right_cos));
        right_p2 = Point(cvRound(right_x - 1000 * (-right_sin)), cvRound(right_y - 1000 * right_cos));

        // The color of Lines should be red
        line(frame, left_p1, left_p2, Scalar(0, 0, 255), 3, 8);
        line(frame, right_p1, right_p2, Scalar(0, 0, 255), 3, 8);

        // Didplay 'Frame' Window
        imshow("Frame", frame);
        waitKey(delay);
    }
    waitKey(0);
}