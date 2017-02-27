#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{

 cout << "TEST" << endl;
  Mat img = imread( argv[1], -1 ); // can read BMP, DIB, JPEG, JPE, PNG, PBM, PGM, PPM, SR, RAS, and TIFF
  if( img.empty() ) return -1;

  namedWindow( "Example1", cv::WINDOW_AUTOSIZE );
  imshow( "Example1", img );
  waitKey( 3000 ); // >0 in ms; =0 wait for any key press;
    // mat variables are automatically deallocated
  destroyWindow( "Example1" );
  return 0;
}
