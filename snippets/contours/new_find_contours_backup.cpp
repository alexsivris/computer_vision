/*
void cv::findContours(
  cv::InputOutputArray    image,         // Input "binary" 8-bit single channel
  cv::OutputArrayOfArrays contours,      // Vector of vectors or points
  cv::OutputArray         hierarchy,     // (optional) topology information
  int                     mode,          // Contour retrieval mode (Figure 14-2)
  int                     method,              // Approximation method
  cv::Point               offset = cv::Point() // (optional) Offset every point
);

void cv::findContours(
  cv::InputOutputArray    image,         // Input "binary" 8-bit single channel
  cv::OutputArrayOfArrays contours,      // Vector of vectors or points
  int                     mode,          // Contour retrieval mode (Figure 14-2)
  int                     method,        // Approximation method
  cv::Point               offset = cv::Point() // (optional) Offset every point
);

void  cv::drawContours(
  cv::InputOutputArray   image,                  // Will draw on input image
  cv::InputArrayOfArrays contours,               // Vector of vectors or points
  int                    contourIdx,             // Contour to draw (-1 is "all")
  const cv::Scalar&      color,                  // Color for contours
  int                    thickness = 1,          // Thickness for contour lines
  int                    lineType  = 8,          // Connectedness ('4' or '8')
  cv::InputArray         hierarchy = noArray(),  // optional (from findContours)
  int                    maxLevel  = INT_MAX,    // Max descent in hierarchy
  cv::Point              offset    = cv::Point() // (optional) Offset all points
)


*/
#include <iostream>
#include <opencv2/opencv.hpp>
#define NO_GOOD_MATCH -5
using namespace std;
using namespace cv;

Ptr<ShapeContextDistanceExtractor> mysc = createShapeContextDistanceExtractor();
Ptr<HausdorffDistanceExtractor> hde = createHausdorffDistanceExtractor();

Mat g_gray, g_binary;
int g_thresh = 128;
string g_output_win = "Output contours";
Point2f center;
float radius;

Rect bound_rect;
Mat contourRegion;
void on_trackbar(int, void*)
{
    cv::threshold(g_gray, g_binary,g_thresh, 255, cv::THRESH_BINARY);
    vector< vector< cv::Point > > contours;
    cv::findContours(
                g_binary,
                contours,
                cv::noArray(),
                cv::RETR_LIST,
                cv::CHAIN_APPROX_SIMPLE);
    g_binary = cv::Scalar::all(0);
    cv::drawContours(g_binary,contours,-1, cv::Scalar::all(255));
    //imshow(g_output_win, g_binary);
}
void on_hue (int, void*)
{

}

int findBestMatch(vector< vector< Point> > &_img_cnt, vector< Point> &_tpl_strong_cnt, Mat &_tpl_hist)
{
    int best=NO_GOOD_MATCH; //
    float dist=99;
    // Ptr<ShapeContextDistanceExtractor> mysc = createShapeContextDistanceExtractor();

    ///  Get the mass centers:

    for (int i=0; i<_img_cnt.size();i++)
    {


        if (matchShapes(_img_cnt.at(i),_tpl_strong_cnt,CV_CONTOURS_MATCH_I1,2) < dist
                && contourArea(_img_cnt.at(i)) >= 800/* &&
                _img_cnt.at(i).size() >= (_tpl_strong_cnt.size()/5) &&
                _img_cnt.at(i).size() <= 1300 &&*/
                && contourArea(_img_cnt.at(i)) <= 50000 && contourArea(_img_cnt.at(i)) >= 100 )
        {
            dist = matchShapes(_img_cnt.at(i),_tpl_strong_cnt,CV_CONTOURS_MATCH_I1,2);
            cv::minAreaRect(_img_cnt.at(i));
            minEnclosingCircle(_img_cnt.at(i),center,radius);
            bound_rect = boundingRect(_img_cnt.at(i));
            // set after histogram is ok

            /*// MASK
            Mat mask = Mat::zeros(g_gray.size(),CV_8UC1);
            drawContours(mask,_img_cnt,i,Scalar(255), CV_FILLED);
            Mat masked_gray;
            g_gray.copyTo(masked_gray,mask);
            imshow("Mask",masked_gray);*/
            // MASK
            Mat mask = Mat::zeros(g_gray.size(),CV_8UC1);
            vector<vector<Point>> mask_hull(1), tpl_hull(1);
            convexHull(_img_cnt.at(i),mask_hull.at(0));
            convexHull(_tpl_strong_cnt,tpl_hull.at(0));
            drawContours(mask,mask_hull,-1,Scalar(255), CV_FILLED);
            Mat masked_gray;
            g_gray.copyTo(masked_gray,mask);
            imshow("Mask",masked_gray);

            // masked histogram
            Mat mask_hist;
            int histSize = 256;
            const float range[] = {0,256};
            const float * histRange = { range };
            calcHist(&masked_gray,1,0,Mat(),mask_hist,1,&histSize,&histRange,true,false);
            normalize( mask_hist, mask_hist, 0, 1, NORM_MINMAX, -1, Mat() );
            normalize( _tpl_hist, _tpl_hist, 0, 1, NORM_MINMAX, -1, Mat() );

            //

            double comp = compareHist(mask_hist,_tpl_hist,CV_COMP_CHISQR_ALT );
//            cout << "Correlation 1: " << comp << endl;

            /*Arc length
            double tpl_arclen = arcLength(_tpl_strong_cnt, true)/_tpl_strong_cnt.size();
            cout << "Tpl arclen: " << tpl_arclen << endl;
            double scene_arclen = arcLength(_img_cnt.at(i), true)/_img_cnt.at(i).size();
            cout << "Scene arclen: " << scene_arclen << endl;*/
            //float dis = hde->computeDistance(_tpl_strong_cnt, _img_cnt.at(i));//mysc->computeDistance( _tpl_strong_cnt, _img_cnt.at(i) );
            //cout << "Contour distance: " << dis << endl;

            /*if (comp < 5.25 /*&& dis < 300)*/
                best = i;
            /*else
                best = NO_GOOD_MATCH;*/



        }
    }
    return best;
}

int main (int argc, char **argv)
{
    namedWindow(g_output_win,1);
    namedWindow("HSV Settings");
    VideoCapture cap(1);
    cout << CV_VERSION << endl;

    cv::createTrackbar(
                "Threshold",
                g_output_win,
                &g_thresh,
                255,
                on_trackbar
                );

    int hue_low = 150;
    int hue_high = 179;
    int sat_low = 50;
    int sat_high = 255;
    int lum_low = 50;
    int lum_high = 255;
    cv::createTrackbar(
                "Hue (lower bound)",
                "HSV Settings",
                &hue_low,
                179
                );
    cv::createTrackbar(
                "Hue (upper bound)",
                "HSV Settings",
                &hue_high,
                179
                );
    cv::createTrackbar(
                "Sat (lower bound)",
                "HSV Settings",
                &sat_low,
                255
                );
    cv::createTrackbar(
                "Sat (upper bound)",
                "HSV Settings",
                &sat_high,
                255
                );
    cv::createTrackbar(
                "Lum (lower bound)",
                "HSV Settings",
                &lum_low,
                255
                );
    cv::createTrackbar(
                "Lum (upper bound)",
                "HSV Settings",
                &lum_high,
                255
                );
    // Load template
    string stpl = "../data/shape_sample/";
    string p1 = stpl + argv[1];
    Mat tpl = imread(p1,CV_LOAD_IMAGE_GRAYSCALE);
    Mat tpl_binary;
    cv::threshold(tpl, tpl_binary,g_thresh, 255, cv::THRESH_BINARY);
    vector< vector< cv::Point > > tpl_contours; // all contours in template
    int strongContour = 2; // mario 31 red_star 2


    cv::findContours(
                tpl_binary,
                tpl_contours,
                cv::noArray(),
                cv::RETR_LIST,
                cv::CHAIN_APPROX_SIMPLE);
    tpl_binary = cv::Scalar::all(0);
    drawContours(tpl_binary,tpl_contours,strongContour,cv::Scalar::all(255)); //
    imshow("Template", tpl_binary);

    Mat tpl_hist;
    int histSize = 256;
    const float range[] = {0,256};
    const float * histRange = { range };
    calcHist(&tpl,1,0,Mat(),tpl_hist,1,&histSize,&histRange,true,false);
    //



    Mat frame; // rgb
    Mat hsv_frame;
    Mat red_frame;
    Mat white_frame;
    while (1)
    {
        cap >> frame;
        if (frame.empty())
            break;
        cvtColor(frame,hsv_frame,COLOR_BGR2HSV);
        //inRange(hsv_frame, Scalar(150,50, 50), Scalar(179, 255, 255), red_frame);
        inRange(hsv_frame, Scalar(hue_low,sat_low, lum_low), Scalar(hue_high, sat_high, lum_high), red_frame);
        Mat er_kernel = Mat::ones(Size(10,10),CV_8UC1);
        Mat dil_kernel = Mat::ones(Size(10,10),CV_8UC1);
        /**/
        erode(red_frame,red_frame,er_kernel);

        dilate(red_frame,red_frame,Mat());
        Mat tmp;
        frame.copyTo(tmp,red_frame);
        imshow("hsv mask", red_frame);
        imshow("hsv color", tmp);
        // red mask

        //cvtColor(red_frame,tmp,COLOR_HSV2BGR);
        //cvtColor(tmp,red_frame,COLOR_BGR2GRAY);

        //threshold(red_frame,red_frame,128,255,THRESH_BINARY);

        imshow("RED", tmp);

        vector< vector< cv::Point > > contours; // all contours in img

        cv::cvtColor(frame, g_gray,cv::COLOR_BGR2GRAY);
        cv::threshold(g_gray, g_binary,g_thresh, 255, cv::THRESH_BINARY);
        threshold(g_gray,white_frame,g_thresh,255,THRESH_BINARY);
        //imshow("RED", white_frame);
        cvtColor(white_frame,white_frame,COLOR_GRAY2BGR);

        // use red mask
        red_frame.copyTo(g_binary);


        cv::findContours(
                    g_binary,
                    contours,
                    cv::noArray(),
                    cv::RETR_LIST,
                    cv::CHAIN_APPROX_SIMPLE);
        int idxBest = findBestMatch(contours, tpl_contours.at(strongContour),tpl_hist);

        g_binary = cv::Scalar::all(0);
        resize(g_binary,g_binary,Size(frame.cols,frame.rows));
        if (idxBest != NO_GOOD_MATCH)
        {
            cv::drawContours(g_binary,contours,idxBest, cv::Scalar::all(255));
            circle(g_binary,center,radius,Scalar::all(255),3);
            rectangle(g_binary,bound_rect,Scalar::all(255),3);
            rectangle(frame,center,center,Scalar(0,255,0),50);
            //rectangle(frame,bound_rect,Scalar(0,0,255),10);
            cout << "Best contour size: " << contours.at(idxBest).size()
                 << " Center: " << center << " Area: " << contourArea(contours.at(idxBest)) << endl;
            imshow(g_output_win, g_binary);
        }
        else
        {
            imshow(g_output_win, g_binary);
        }
        frame.copyTo(tmp,red_frame);

        bitwise_or(tmp,white_frame,tmp);
        bitwise_not(white_frame,white_frame);
        bitwise_and(tmp,white_frame,tmp);
        //bitwise_or(no_bg,tmp,tmp);
        imshow("BGR", frame);
        waitKey(33);
    }
    destroyAllWindows();
    return 0;
}
