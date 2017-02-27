#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char ** argv)
{
    // RNG
    cv::RNG r;
    cout << "float" << endl;
    cout << "1.) " << r.uniform(0.0f,1.0f) << endl <<
            "2.) " << r.uniform(0.0f,1.0f) << endl <<
            "3.) " << r.uniform(0.0f,1.0f) << endl;

    cout << "double" << endl;
    cout << "1.) " << r.gaussian(1.0f) << endl <<
            "2.) " << r.gaussian(1.0f) << endl <<
            "3.) " << r.gaussian(1.0f) << endl;

    cout << "unsigned bytes" << endl;
    cout << "1.) " << r.uniform(int(0),int(255)) << endl <<
            "2.) " << r.uniform(int(0),int(255)) << endl <<
            "3.) " << r.uniform(int(0),int(255)) << endl;


    cout << "fill" << endl;

    Mat rand_bytes(Size(20,1),CV_32FC1);
    rand_bytes.setTo(Scalar(0));
    //cv::Range rg(2,4);
    //rand_bytes.colRange(rg).setTo(Scalar(1));
    //rand_bytes.at<uchar>(rg) = 1;

    //r.fill(rand_bytes,RNG::UNIFORM);
    r.fill(rand_bytes,cv::RNG::UNIFORM,float(0.0), float(1.0));
    cout << rand_bytes << endl;
    r.fill(rand_bytes,cv::RNG::NORMAL,float(0.0), float(1.0));
    cout << rand_bytes.type() << endl;

    rand_bytes.setTo(Scalar(0));
    rand_bytes.convertTo(rand_bytes,CV_8UC1);
    r.fill(rand_bytes,cv::RNG::UNIFORM,int(0),int(255));
    cout << rand_bytes << endl;

    cout << "color triplets" << endl;
    rand_bytes.create(500,500,CV_8UC3);
    cout << rand_bytes.type() << endl;

    //resize(rand_bytes,rand_bytes,Size(20,1));
    Mat lower(1,3,CV_8UC1);
    lower.setTo(Scalar(0));
    Mat upper(1,3,CV_8UC1);
    upper.setTo(Scalar(255));
    r.fill(rand_bytes,cv::RNG::UNIFORM,lower,upper);
    //cout << rand_bytes << endl;
    /*imshow("Output",rand_bytes);
    waitKey();
    destroyAllWindows();*/

    rand_bytes.create(3,100,CV_8UC1);
    lower.create(3,1,CV_8UC1);
    lower.row(0).setTo(Scalar(64));
    lower.row(1).setTo(Scalar(192));
    lower.row(2).setTo(Scalar(128));

    upper.create(3,3,CV_8UC1);
    upper.row(0).setTo(Scalar(10));
    upper.row(1).setTo(Scalar(10));
    upper.row(2).setTo(Scalar(2));

    r.fill(rand_bytes.row(0),cv::RNG::NORMAL,64,10);

    r.fill(rand_bytes.row(1),cv::RNG::NORMAL,192,10);

    r.fill(rand_bytes.row(2),cv::RNG::NORMAL,128,2);

    PCA p(rand_bytes,lower,cv::PCA::DATA_AS_COL,2);

    cout << p.mean << endl <<
            p.eigenvectors << endl <<
            p.eigenvalues << endl;

    // SVD
    Mat A = (Mat_<float>(3,2) << 1,1,0,1,-1,1);
    cv::SVD s(A,cv::SVD::FULL_UV);

    cout << "SVD" << endl;
    cout << s.vt << endl;
    cout << s.u << endl <<
            s.w << endl;
    return 0;
}
