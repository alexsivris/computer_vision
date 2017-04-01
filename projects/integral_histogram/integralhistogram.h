#ifndef INTEGRALHISTOGRAM_H
#define INTEGRALHISTOGRAM_H
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
struct BestHistogram {
    float dist=0;
    int scale=1;
    Rect rect;
    Point position;
};
struct Candidate {
    double dist=0;
    int index=0;
};

struct HSVColorRange {
    int hue_val_low=0;
    int hue_val_high=179;
    int sat_val_low=0;
    int sat_val_high=255;
    int val_val_low=0;
    int val_val_high=255;
};
struct DominantBin
{
    Point location; // x: hue, y: sat
    double count=0;

};
struct HistogramData {
    Mat histogram;
    Point location;
};

class IntegralHistogram
{
public:
    enum HistogramType {gray, hs};

    IntegralHistogram(Mat &_img, HistogramType _htype, vector<DominantBin> &_db);
    //~IntegralHistogram();

    // compute histogram of certain region
    Mat getRoiHistogram(Rect &_roi);

    // get histogram of entire image
    Mat getWholeHistogram();

private:
    // Compute histogram of whole image
    void computeHistogram();

    // Compute histogram for grayscale image
    void computeGrayHistogram();

    // Compute histogram for dominant color bins
    void computeDominantHistogram();

    Mat &m_img;
    vector<Mat> hsv_split;
    vector<DominantBin> &dominant_bins;
    vector<Mat>  gray_histogram;
    vector<vector<Mat>> dominant_histogram;
    HistogramType m_htype;

};

#endif // INTEGRALHISTOGRAM_H
