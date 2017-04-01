#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Point pt_start, pt_end;
Point scope_start, scope_end;
bool finished_rect = false;
bool scoping_rect = false;

void cbMouse(int event, int x, int y, int flags, void * userdata)
{
    Point pt;
    pt.x =x;
    pt.y = y;
    if (event == EVENT_LBUTTONDOWN)
    {
        finished_rect = false;
        scoping_rect = true;
        cout << "clicked point: " << pt << endl;
        pt_start = pt;
        scope_start = pt;
    }
    if (event == EVENT_LBUTTONUP)
    {
        scoping_rect = false;
        finished_rect = true;
        cout << "release point: " << pt << endl;
        pt_end = pt;
    }
    if (scoping_rect)
        scope_end = pt;
}

int main(int argc, char ** argv)
{
    Mat img = imread("../data/test.png", CV_LOAD_IMAGE_COLOR);

    namedWindow("Image");
    setMouseCallback("Image", cbMouse);
    Mat scope_frame;
    while (true)
    {
        img.copyTo(scope_frame);
        if (finished_rect)
        {
            Rect rct(pt_start,pt_end);
            rectangle(img,rct,Scalar(255,0,0),1,LINE_AA);
        }
        if (scoping_rect)
        {
            Rect rct(scope_start, scope_end);
            Mat shiftmat(scope_frame.rows,scope_frame.cols, scope_frame.type());
            shiftmat.setTo(Scalar(255,255,255));
            rectangle(scope_frame,rct,Scalar(0,255,0),3, LINE_AA);
            addWeighted(shiftmat,0.2,scope_frame,1.0,0.1,scope_frame);
            imshow("Image", scope_frame);

        }
        else
        {
            imshow("Image", img);
        }
        waitKey(1);
    }
    destroyAllWindows();

	return 0;
}
