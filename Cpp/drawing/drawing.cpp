#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char ** argv)
{
    // 1. circle
    Mat img = imread("../data/test.png", CV_LOAD_IMAGE_COLOR);

    circle(img,Point(img.cols/2,img.rows/2),100, Scalar(255,0,0),10,LINE_AA);
    ellipse(img, Point(img.cols/4,img.rows/2), Size(100,50),60,0,360,Scalar(0,255,0),5,LINE_AA);

    Rect rct(0,0,100,50);
    rectangle(img,rct,Scalar(0,0,255));

    Point pt1(99,49), pt2(200,200);
    line(img,pt1,pt2,Scalar(255,0,0),1,LINE_AA);
    cout << clipLine(rct,pt1,pt2) << endl;
    imshow("Output", img);
    waitKey();

    // FPS

    /*VideoCapture cap(1);

    Mat frame;
    int64 tc_64,tc2_64;
    tc_64 = cv::getTickCount();
    int framecounter = 0, framecounter2 = 0;
    while(true)
    {
        cap >> frame;
        if (frame.empty())
            break;
        framecounter++;

        tc2_64 = cv::getTickCount();
        int time_passed = (tc2_64-tc_64)/cv::getTickFrequency();
        cout << time_passed << endl;
        if (time_passed >= 1)
        {

            framecounter2 = framecounter;
            framecounter = 0;
            tc_64 = cv::getTickCount();

        }
        stringstream conv;
        conv << framecounter2;
        putText(frame,conv.str(),Point(200,200),CV_FONT_HERSHEY_PLAIN,1,Scalar(0,255,0), 1);
        imshow("Output", frame);
        waitKey(3);
    }*/

    Mat img2(300,300,CV_8UC1);
    img2.setTo(Scalar(0));

    Mat colimg;
    cvtColor(img2,colimg,COLOR_GRAY2BGR);

    pt1.x = 0;
    pt1.y = 40;
    pt2.x = 40;
    pt2.y = 100;
    line(colimg,pt1,pt2,Scalar(255,255,0),1,LINE_4);
    LineIterator litr(colimg,pt1,pt2,4);
    LineIterator litr2(colimg,pt1,pt2,8);
    cout << "LineIter1: " << litr.count << endl <<
            "LineIter2: " << litr2.count << endl;

    imshow("Output", colimg);
    waitKey();
    destroyAllWindows();
	return 0;
}
