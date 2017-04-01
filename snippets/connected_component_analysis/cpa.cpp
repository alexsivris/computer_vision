/*
int  cv::connectedComponents (
  cv::InputArrayn image,                // input 8-bit single-channel (binary)
  cv::OutputArray labels,               // output label map
  int             connectivity = 8,     // 4- or 8-connected components
  int             ltype        = CV_32S // Output label type (CV_32S or CV_16U)
);

int  cv::connectedComponentsWithStats (
  cv::InputArrayn image,                // input 8-bit single-channel (binary)
  cv::OutputArray labels,               // output label map
  cv::OutputArray stats,                // Nx5 matrix (CV_32S) of statistics:
                                        // [x0, y0, width0, height0, area0;
                                        //  ... ; x(N-1), y(N-1), width(N-1),
                                        // height(N-1), area(N-1)]
  cv::OutputArray centroids,            // Nx2 CV_64F matrix of centroids:
                                        // [ cx0, cy0; ... ; cx(N-1), cy(N-1)]
  int             connectivity = 8,     // 4- or 8-connected components
  int             ltype        = CV_32S // Output label type (CV_32S or CV_16U)
);
*/

#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int main(int argc, char ** argv)
{
    cv::Mat img, img_edge, labels, img_color, stats;
    img = cv::imread("test.png", CV_LOAD_IMAGE_GRAYSCALE);
    if (img.empty())
    {    cout << "image is emoty" << endl; return -1; }

    namedWindow("Img after thresh");
    cv::threshold(img,img_edge,128,255,cv::THRESH_BINARY);
    cv::imshow("Img after thresh", img_edge);

    vector<Point> centroids;
    int i, nccomp = connectedComponentsWithStats(img_edge,labels, stats,noArray());
    cout << "total connected components detected: " << nccomp << endl;

    vector<cv::Vec3b> colors(nccomp+1);
    colors[0] = cv::Vec3b(0,0,0); //bg remains black
    for (int i=0;i<=nccomp;i++)
    {
        colors[i] = cv::Vec3b(rand()%256, rand()%256,rand()%256);
        if (stats.at<int>(i-1, cv::CC_STAT_AREA) < 100)
        {
            colors[i] = cv::Vec3b(0,0,0); // small regions painted black
        }
    }
    img_color = cv::Mat::zeros(img.size(), CV_8UC3);
    for (int y=0; y<img_color.rows; y++)
    {
        for (int x=0;x<img_color.cols; y++)
        {
            int label = labels.at<int>(x,y);
            CV_Assert(0 <= label && label <= nccomp);
            img_color.at<cv::Vec3b>(y,x) = colors[label];
        }
    }
    imshow("Labeled map", img_color);
    waitKey();
    destroyAllWindows();
    return 0;

	return 0;
}
