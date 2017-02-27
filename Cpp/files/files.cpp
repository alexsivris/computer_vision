#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char ** argv)
{
    VideoCapture cap;
    cap.open("../data/test.avi");
    if (!cap.isOpened())
    {
        cout << "An error occured when loading the video!" << endl;
        return -1;
    }

    Mat frame, gray, canny, result;

    while(cap.read(frame))
    {
        cvtColor(frame,gray,COLOR_BGR2GRAY);
        cvtColor(gray,gray,COLOR_GRAY2BGR);
        Canny(gray, canny,0,128);
        cvtColor(canny,canny,COLOR_GRAY2BGR);
        putText(frame,"Original Frame",Point(frame.cols/4, frame.rows/4),CV_FONT_HERSHEY_PLAIN,2,Scalar(0,255,0),2);
        putText(gray,"Grayscale Frame",Point(frame.cols/4, frame.rows/4),CV_FONT_HERSHEY_PLAIN,2,Scalar(0,255,0),2);
        putText(canny,"Canny Frame",Point(frame.cols/4, frame.rows/4),CV_FONT_HERSHEY_PLAIN,2,Scalar(0,255,0),2);

        result.create(frame.rows,frame.cols*3,frame.type());
        frame.copyTo(result.colRange(Range(1,frame.cols+1)));
        gray.copyTo(result.colRange(Range(gray.cols+1,2*gray.cols+1)));
        canny.copyTo(result.colRange(Range(2*canny.cols, 3*canny.cols)));
        imshow("Result", result);

        waitKey((1/cap.get(cv::CAP_PROP_FPS))*1000);
    }
    destroyAllWindows();

	return 0;
}
