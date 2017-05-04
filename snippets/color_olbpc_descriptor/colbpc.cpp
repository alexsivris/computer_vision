#include <opencv2/opencv.hpp>
#include <bitset>
using namespace std;
using namespace cv;

int main(int argc, char ** argv)
{
    cout << CV_VERSION_MAJOR << CV_VERSION_MINOR << endl;
    string tpl_path = "../data/tpl_mentos.png";
    Mat tpl_bgr = imread(tpl_path,IMREAD_COLOR);
    blur(tpl_bgr,tpl_bgr,Size(5,5));
    Mat tpl_hsv;
    cvtColor(tpl_bgr,tpl_hsv,COLOR_BGR2HSV);


    Mat tpl_colbpc(tpl_hsv.rows,tpl_hsv.cols,CV_8UC2, Scalar::all(0));

    /* 1. calculate OLBP code for the hue channel */

    vector<Mat> hsv_split;
    split(tpl_hsv,hsv_split);

    for (int y=1; y<hsv_split[0].rows-1;y++)
    {
        for (int x=1; x<hsv_split[0].cols-1;x++)
        {
            unsigned char lbp = 0x00;

            // diagonal
            if (hsv_split[0].at<uchar>(y-1,x+1) > hsv_split[0].at<uchar>(y,x)) lbp |= (0x01 << 4);
            if (hsv_split[0].at<uchar>(y-1,x-1) > hsv_split[0].at<uchar>(y,x)) lbp |= (0x01 << 5);
            if (hsv_split[0].at<uchar>(y+1,x-1) > hsv_split[0].at<uchar>(y,x)) lbp |= (0x01 << 6);
            if (hsv_split[0].at<uchar>(y+1,x+1) > hsv_split[0].at<uchar>(y,x)) lbp |= (0x01 << 7);
            tpl_colbpc.at<Vec2i>(y,x)[0] = lbp;
            //cout << "lbp (diag.): " << bitset<8>(tpl_colbpc.at<Vec2i>(y,x)[0]) << endl;


            lbp = 0x00;
            // orthogonal
            if (hsv_split[0].at<uchar>(y,x+1) > hsv_split[0].at<uchar>(y,x)) lbp |= (0x01 << 4);
            if (hsv_split[0].at<uchar>(y-1,x) > hsv_split[0].at<uchar>(y,x)) lbp |= (0x01 << 5);
            if (hsv_split[0].at<uchar>(y,x-1) > hsv_split[0].at<uchar>(y,x)) lbp |= (0x01 << 6);
            if (hsv_split[0].at<uchar>(y+1,x) > hsv_split[0].at<uchar>(y,x)) lbp |= (0x01 << 7);
            tpl_colbpc.at<Vec2i>(y,x)[1] = lbp;
            //cout << (unsigned int)lbp << endl << bitset<8>(tpl_colbpc.at<Vec2i>(y,x)[1]) << endl;

        }
    }

    imshow("Template", tpl_bgr);

    vector<Mat> tpl_split(2);
    split(tpl_colbpc,tpl_split);
    cout << bitset<8>(tpl_colbpc.at<Vec2i>(1,7)[0]) << " " << bitset<8>(tpl_colbpc.at<Vec2i>(1,7)[1]) << endl;
    cout << bitset<8>(tpl_split[0].at<unsigned int>(1,7)) << " " << bitset<8>(tpl_split[1].at<int>(1,7)) << endl;
    imshow("Binary pattern[0]", tpl_split[0]);
    imshow("Binary pattern[1]", tpl_split[1]);


    /* 2. Histogram of COLBPC */
    Mat lbp_hist,lbp_hist2;
    vector<Mat> in_lbp = {tpl_split[0]};
    int bins = 16;
    vector<int> chans = {0};
    vector<int> sizes = {bins};
    vector<float> ranges = {0,16};
    calcHist(in_lbp,chans,noArray(),lbp_hist,sizes,ranges);
    transpose(lbp_hist,lbp_hist);
    cout << lbp_hist << endl;


    in_lbp.clear();
    in_lbp.push_back(tpl_split[1]);
    vector<int> chans2 = {0};
    vector<float> ranges2 = {0,255};
    calcHist(in_lbp,chans2,noArray(),lbp_hist2,sizes,ranges2);
    transpose(lbp_hist2,lbp_hist2);

    cout << lbp_hist2 << endl;



    waitKey();

    return 0;
}
