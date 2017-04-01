#include "integralhistogram.h"


IntegralHistogram::IntegralHistogram(Mat &_img, HistogramType _htype, vector<DominantBin> &_db) :
    m_img(_img), m_htype(_htype),dominant_bins(_db)
{

    computeHistogram();
}

Mat IntegralHistogram::getWholeHistogram()
{
    if (m_htype == HistogramType::gray)
        return gray_histogram.at(m_img.rows-1).row(m_img.cols-1);
    else
        return dominant_histogram[m_img.rows-1][m_img.cols-1];
}

Mat IntegralHistogram::getRoiHistogram(Rect &_roi)
{
    int x = _roi.x;
    int y = _roi.y;
    if (!x)
        x=1;
    if (!y)
        y=1;

    Mat region_hist;
    if (m_htype == HistogramType::gray)
    {
        Mat region_hist0 = gray_histogram.at(y+_roi.height-2).row(x+_roi.width-2)
                - gray_histogram.at(y+_roi.height-2).row(x);
        Mat region_hist1 = region_hist0 - gray_histogram.at(y).row(x+_roi.width-2);
        region_hist = region_hist1 + gray_histogram.at(y).row(x);
    }
    else
    {
        Mat region_hist0 = dominant_histogram[y+_roi.height-2][x+_roi.width-2]
                - dominant_histogram[y + _roi.height-2][x];
        Mat region_hist1 = region_hist0 - dominant_histogram[y][x+_roi.width-2];
        region_hist = region_hist1 + dominant_histogram[y][x];
    }
    return region_hist;

}

void IntegralHistogram::computeGrayHistogram()
{
    int bin_no = 256;
    vector<Mat> gray_hist_vec(1,Mat(1,bin_no,CV_32FC1,Scalar::all(0)));
    //gray_histogram.create(2,);

    Mat current_hist(1,bin_no,CV_32FC1,Scalar::all(0));
    Mat left_hist(1,bin_no,CV_32FC1,Scalar::all(0));
    Mat upper_hist(1,bin_no,CV_32FC1,Scalar::all(0));
    Mat upper_left_hist(1,bin_no,CV_32FC1,Scalar::all(0));


    for (int y=0;y<m_img.rows; y++) // vector
    {
        for (int x=0;x<m_img.cols; x++) // column
        {
            // TODO: only if pixel value a dominant color

            if ((x-1)>=0)
            {
                gray_hist_vec[y].row(x-1).copyTo(left_hist);
            }

            if ((y-1)>=0)
            {
                gray_hist_vec[y-1].row(x).copyTo(upper_hist);
                if ((x-1)>=0)
                    gray_hist_vec[y-1].row(x-1).copyTo(upper_left_hist);
            }
            current_hist.at<float>(0,m_img.at<uchar>(y,x)) += 1;

            Mat prop_hist = left_hist + upper_hist - upper_left_hist + current_hist;

            prop_hist.copyTo(gray_hist_vec[y].row(x));

            // reset
            current_hist.setTo(Scalar::all(0));
            left_hist.setTo(Scalar::all(0));
            upper_hist.setTo(Scalar::all(0));
            upper_left_hist.setTo(Scalar::all(0));
            if (x!=m_img.cols-1)
                gray_hist_vec[y].push_back(Mat(1,bin_no,CV_32FC1,Scalar::all(0)));
        }
        if (y!=m_img.rows-1)
            gray_hist_vec.push_back(Mat(1,bin_no,CV_32FC1,Scalar::all(0)));


    }

    // Histogram of region - just need center point and rct size
    /*Mat region_hist0 = gray_hist_vec.at(2).row(2) - gray_hist_vec.at(0).row(2);
    Mat region_hist1 = region_hist0 - gray_hist_vec.at(2).row(0);
    Mat region_hist = region_hist1 + gray_hist_vec.at(0).row(0);*/

    //gray_hist_vec.at(m_img.rows-1).row(m_img.cols-1).copyTo(gray_histogram);
    gray_histogram = gray_hist_vec;
    gray_hist_vec.clear();
    /*cout << "Integral histogram" << endl;
    cout << gray_histogram.at(m_img.rows-1).row(m_img.cols-1) << endl;
    cout << "Sum" << sum(gray_histogram.at(m_img.rows-1).row(m_img.cols-1)) << endl;*/
}


void IntegralHistogram::computeDominantHistogram()
{
    int count = dominant_bins.size();

    vector<Mat> vec_entry(1,Mat(1,count,CV_32FC1,Scalar::all(0)));

    vector<vector<Mat>> hist_vec(1,vec_entry);

    Mat left_hist(Mat(1,count,CV_32FC1,Scalar::all(0)));
    Mat current_hist(Mat(1,count,CV_32FC1,Scalar::all(0)));
    Mat upper_left_hist(Mat(1,count,CV_32FC1,Scalar::all(0)));
    Mat upper_hist( Mat(1,count,CV_32FC1,Scalar::all(0)));


    for (int y=0;y<m_img.rows; y++) // vector
    {
        for (int x=0;x<m_img.cols; x++) // column
        {
            int pxl_val_hue = hsv_split[0].at<uchar>(y,x);
            int pxl_val_sat = hsv_split[1].at<uchar>(y,x);
            int found_idx = -1;
            for (int i=0;i<dominant_bins.size();i++)
            {
                if ((pxl_val_hue == dominant_bins.at(i).location.x) &&
                        (pxl_val_sat == dominant_bins.at(i).location.y))
                    found_idx = i;
            }

            // hue
            if ((x-1)>=0)
            {
                hist_vec[y][x-1].copyTo(left_hist);
            }

            if ((y-1)>=0)
            {
                hist_vec[y-1][x].copyTo(upper_hist);
                if ((x-1)>=0)
                {
                    hist_vec[y-1][x-1].copyTo(upper_left_hist);
                }
            }
            if (found_idx < 0)
                found_idx = 10;
            current_hist.at<float>(0,found_idx) += 1;

            Mat prop_hist= left_hist + upper_hist - upper_left_hist + current_hist;
            prop_hist.copyTo(hist_vec[y][x]);
            hist_vec[y][x].at<float>(0,10) = 0;

            // reset
            current_hist.setTo(Scalar::all(0));
            left_hist.setTo(Scalar::all(0));
            upper_hist.setTo(Scalar::all(0));
            upper_left_hist.setTo(Scalar::all(0));
            if (x!=m_img.cols-1)
                hist_vec.at(y).push_back(Mat(1,count,CV_32FC1,Scalar::all(0)));
        }
        if (y!=m_img.rows-1)
            hist_vec.push_back(vec_entry);

    }
    /*imshow("INTEGRAL HISTOGRAM", hist_vec[m_img.rows-1][m_img.cols-1]);
    cout << "INTEGRAL HISTOGRAM" << endl
         << hist_vec[13][303] << endl;
    cout << cv::sum(hist_vec[13][303]) << endl;
    FOR TEST
    cout << "INTEGRAL HISTOGRAM WHOLE" << endl
         << hist_vec[m_img.rows-1][m_img.cols-1] << endl;
    cout << cv::sum(hist_vec[m_img.rows-1][m_img.cols-1]) << endl;*/
    dominant_histogram = hist_vec;
    hist_vec.clear();
    return;
}

void IntegralHistogram::computeHistogram()
{
    if (m_htype == HistogramType::gray)
    {
        // Grayscale - Wavefront approach
        computeGrayHistogram();
    }
    else
    {
        // HS - wavefront approach
        hsv_split.resize(3);
        split(m_img,hsv_split);

        for (int y=0;y<m_img.rows;y++)
        {
            for (int x=0;x<m_img.cols;x++)
            {
                int pxl_val_hue = hsv_split[0].at<uchar>(y,x);
                int pxl_val_sat = hsv_split[1].at<uchar>(y,x);
                int found_idx = -1;
                for (int i=0;i<dominant_bins.size();i++)
                {
                    if ((pxl_val_hue == dominant_bins.at(i).location.x) &&
                            (pxl_val_sat == dominant_bins.at(i).location.y))
                        found_idx = i;
                }
                // only dominant bins sur
                if (found_idx < 0)
                {
                    hsv_split[0].at<uchar>(y,x) = 0;
                    hsv_split[1].at<uchar>(y,x) = 0;
                }
            }
        }
        computeDominantHistogram();
    }

}
