#include <opencv2/opencv.hpp>
#include <algorithm>
#include <iostream>
using namespace std;
using namespace cv;

struct AreaCmp {
    AreaCmp(const vector<float>& _areas) : areas(&_areas) {}
    bool operator()(int a, int b) const {
        return (*areas)[a] >(*areas)[b];
    }
    const vector<float>* areas;
};

int main(int argc, char ** argv)
{
    Mat img, img_edge, img_color;
    //load img
    img = imread("../data/shape_sample/red_star.png",CV_LOAD_IMAGE_GRAYSCALE);

    namedWindow("Img after thresh",CV_WINDOW_NORMAL);
    namedWindow("Output",CV_WINDOW_NORMAL);

    threshold(img,img_edge,128,255,cv::THRESH_BINARY);
    imshow("Img after thresh", img_edge);
    vector< vector< Point > > contours;
    vector< Vec4i > hierarchy;

    findContours(img_edge,contours,hierarchy,cv::RETR_LIST,cv::CHAIN_APPROX_SIMPLE);

    cout << "Hit any key to draw the next contour, ESC to quit." << endl;
    cout << "Total contours detected: " << contours.size() << endl;

    vector<int> sortIdx(contours.size());
    vector<float>areas(contours.size());
    for (int n=0;n<(int)contours.size(); n++)
    {
        sortIdx[n] = n;
        areas[n] = contourArea(contours[n], false);
    }
    // sort contours -> largest go first
    std::sort(sortIdx.begin(), sortIdx.end(), AreaCmp(areas));
    for (int n=0; n<(int)contours.size(); n++)
    {
        int idx = sortIdx[n];
        cv::cvtColor(img, img_color,cv::COLOR_GRAY2BGR);
        cv::drawContours(
                    img_color,
                    contours, idx,
                    cv::Scalar(0,0,255),2,8,hierarchy,
                    20 // try different values of max_level
                    );
        cout << "Contour #" << idx << ": area=" << areas[idx] <<
                ", nvertices=" << contours[idx].size() << endl;
        imshow("Output", img_color);
        int k;
        if ((k=cv::waitKey()&255) == 27)
            break;
    }
    destroyAllWindows();
    cout << "Finished all contours" << endl;

    return 0;
}
