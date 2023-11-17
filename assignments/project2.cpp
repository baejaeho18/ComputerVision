#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// Function definition: Function to count the number of non-zero pixels
int countNonZeroManual(const Mat& image) {
    int count = 0;
    for (int i = 0; i < image.rows; ++i) {
        for (int j = 0; j < image.cols; ++j) {
            // Check for non-zero pixels and increment the coefficient.
            if (image.at<uchar>(i, j) != 0) {
                ++count;
            }
        }
    }
    return count;
}

int main(){

    //lane departure 
    Mat frame;
    Mat canny_left, canny_right;
    VideoCapture cap;
    bool showText = false; // Variable that determines whether the "Lane Departure" message is displayed
    int frameCount = 0;    // Variable to store the number of last frames after displaying the message

    float rho, theta, a, b, x0, y0, total_rho, total_theta;
    float rho1, theta1, a1, b1, x1, y1, total_rho1, total_theta1;
    Point p1, p2;
    Point p3,p4;
    vector<Vec2f> lines1, lines2;

    Rect lane_roi(305, 230, 85, 200);  // ROI to detect lane departure

    if(cap.open("Project2_video.mp4")==0){
        cout << "no such file!" << endl;
        waitKey(0);
    }

    //start moving
    bool startText = false; 
    int startframeCount = 0;    
    Mat prevFrame;
    Rect roi(240, 250, 210, 220); // ROI to detect vehicle departure

    int fps = cap.get(CAP_PROP_FPS);

    while(1){
        cap >> frame;
        if(frame.empty()){
            cout << "end of video" << endl;
            break;
        }


        // lane departure
        canny_left = frame(lane_roi);

        cvtColor(canny_left,canny_left,COLOR_BGR2GRAY);
        GaussianBlur(canny_left,canny_left,Size(7,7),0);
        Canny(canny_left,canny_left,100,200,3);
        //rectangle(frame, rect_left, Scalar(255, 0, 0), 2);

        HoughLines(canny_left, lines1, 1, CV_PI /180, 60, 0, 0, -CV_PI/180*15, CV_PI/180 * 20);
        
        if(lines1.size()){
            total_rho=0;
            total_theta=0;
            for(int i=0;i<lines1.size();i++){
                rho = lines1[i][0];
                theta = lines1[i][1];

                total_rho +=rho;
                total_theta +=theta;
            }
            total_rho /= lines1.size();
            total_theta /= lines1.size();

            a= cos(total_theta);
            b= sin(total_theta);
            x0 = a*total_rho;
            y0= b*total_rho;
            p1 = Point(cvRound(x0+1000*(-b))+305, cvRound(y0+1000*a)+305);
            p2 = Point(cvRound(x0-1000*(-b))+305, cvRound(y0-1000*a)+305);
            //line(frame,p1,p2,Scalar(0,0,255),3,8);

            showText = true;   
            frameCount = 0;    
        }

        if(showText) {
            putText(frame, "Lane Departure", Point(50, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
            frameCount++;
            if(frameCount > fps) {  
                showText = false;   
            }
        }


        //Start moving!!
        if (!prevFrame.empty())
        {
            Mat totalDiff, roiDiff;
            cvtColor(abs(frame - prevFrame), totalDiff, COLOR_BGR2GRAY);
            cvtColor(abs(frame(roi) - prevFrame(roi)), roiDiff, COLOR_BGR2GRAY);

            threshold(totalDiff, totalDiff, 50, 255, THRESH_BINARY);
            threshold(roiDiff, roiDiff, 50, 255, THRESH_BINARY);

            int totalNonZeroCount = countNonZeroManual(totalDiff);
            int roiNonZeroCount = countNonZeroManual(roiDiff);

            // If the movement is mostly within the ROI and not much in the total frame
            if (roiNonZeroCount > 500 && totalNonZeroCount < 5000)
            {
                startText = true;  
                startframeCount = 0;    
            }
        }

        if(startText) {
            putText(frame, "Start Moving!", Point(50, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 2);
            startframeCount++;
            if(startframeCount > fps) {  
                startText = false;   
            }
        }

        prevFrame = frame.clone();

        imshow("Frame", frame);
        waitKey(1000/fps);
    }
}
