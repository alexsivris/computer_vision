#include <opencv2/opencv.hpp>
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;
using namespace cv;

static vector<Point> sampleContour( const Mat &image, int n=300)
{
    vector<vector<Point> > _contours;
    vector<Point> all_points;

    findContours(image,_contours,RETR_LIST,CHAIN_APPROX_NONE);
    for (size_t i=0; i<_contours.size(); i++)
    {
        for (size_t j=0; j<_contours[i].size(); j++)
        {
            all_points.push_back(_contours[i][j]);
        }
    }

    // if too few points, replicate them
    int dummy=0;
    for (int add=(int)all_points.size(); add<n;add++)
    {
        all_points.push_back(all_points[dummy++]);
    }
    // Sample uniformly
    random_shuffle(all_points.begin(),all_points.end());
    vector<Point> sampled;
    for (int i=0;i<n;i++)
    {
        sampled.push_back(all_points[i]);
    }

    return sampled;
}


int main(int argc, char ** argv)
{
    string path = "../data/shape_sample/";
    string p1 = path+argv[1];
    cout << "Loaded template: " << p1 <<  endl;

    Ptr<ShapeContextDistanceExtractor> mysc = createShapeContextDistanceExtractor();

    Mat img2 = imread(p1, CV_LOAD_IMAGE_GRAYSCALE);
    int bin_thresh = 50;
    threshold(img2,img2,bin_thresh,255,cv::THRESH_BINARY);
    VideoCapture cap(1);

    Mat img1;
    Mat img_rgb(img1.size(), CV_8UC3);
    //invert matix
    Mat inv_mat = Mat::ones(Size(640,480), CV_8UC1)*255;
    namedWindow("Image");
    namedWindow("Template shape");

    vector<Point> c1 = sampleContour(img2);
    float dist=0;
    imshow("Template shape", img2);
    Mat imres;
    while (true)
    {
        cap >> img1;
        if (img1.empty())
            break;
        cvtColor(img1,img1,cv::COLOR_BGR2GRAY);
        threshold(img1,img1,bin_thresh,255,cv::THRESH_BINARY);
        subtract(inv_mat,img1, imres);
        vector<Point> c2 = sampleContour(imres);
        dist = mysc->computeDistance(c1,c2);
        cout << "shape context distance btwn " << argv[1] << " and " << argv[2] <<
                " is: " << dist << endl;
        if (dist < 2)
        {
            // draw contours in image
            vector< vector<Point> > vc1;
            findContours(imres,vc1,RETR_LIST,CHAIN_APPROX_NONE);
            cvtColor(imres,img_rgb,COLOR_GRAY2BGR);
            drawContours(
                        img_rgb,
                        vc1,
                        5,
                        cv::Scalar(0,0,255),2,8);
            imshow("Image", img_rgb);
        }
        else
        {
            imshow("Image", imres);
        }
        waitKey(33);
    }



    destroyAllWindows();

    return 0;
}
