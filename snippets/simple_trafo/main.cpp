#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>


using namespace std;
using namespace cv;

const string g_input_window = "Input";
const string g_output_window = "Output";

void example2_5(Mat &img)
{
    Mat out;
    // smoothing
    GaussianBlur(img, out, Size(5,5),3,3);
    GaussianBlur(out, out, Size(5,5),3,3);
    imshow("Output",out);
}

//Downsampling->convolving with delta fct series
void example2_6(Mat &img)
{
    Mat img2;
    pyrDown(img, img2);
    imshow(g_output_window, img2);
}

// Canny edge - pyramid pipeline
void example2_7(Mat &img)
{
    Mat gray, cny;
    cvtColor(img,gray,COLOR_BGR2GRAY);
    pyrDown(gray,cny); // example2_8
    pyrDown(cny,cny);

    // example2_9
    int x=16, y=32;
    Vec3b intensity = img.at<Vec3b>(y,x);

    uchar blue = intensity[0];
    uchar green = intensity[1];
    uchar red = intensity[2];
    cout << "At (x,y) = (" << x << ", " << y <<
              "): (blue, green, red) = (" <<
              (unsigned int)blue <<
              ", " << (unsigned int)green << ", " <<
              (unsigned int)red << ")" << endl;

    cout << "Gray pixel there is: " <<
                 (unsigned int)gray.at<uchar>(y, x) << endl;
    x /= 4; y /= 4;
    std::cout << "Pyramid2 pixel there is: " <<
                 (unsigned int)cny.at<uchar>(y, x) << std::endl;
    cny.at<uchar>(x, y) = 128;

    Canny(cny,cny, 10, 100, 3, true);
    imshow(g_output_window,cny);

}

// access pixels
void example2_9(Mat &img)
{
    namedWindow(g_input_window, cv::WINDOW_AUTOSIZE);
    namedWindow(g_output_window, cv::WINDOW_AUTOSIZE);
}
void example2_11(Mat &img, Mat &lpol, VideoWriter &wr)
{
    logPolar(img,lpol,Point2f(img.cols/2, img.rows/2), 40, WARP_FILL_OUTLIERS);
    imshow(g_output_window,lpol);
    wr << lpol;
}

int main(int argc, char ** argv)
{
    namedWindow(g_input_window, cv::WINDOW_AUTOSIZE);
    namedWindow(g_output_window, cv::WINDOW_AUTOSIZE);
    VideoCapture cap;
    if (argc == 1)
    {
        cap.open(1);
    } else
    {
        cap.open(argv[1]);
    }
    if (!cap.isOpened())
    {
        cerr << "Could not open capture !" << endl;
    }

    double fps = cap.get(CAP_PROP_FPS);
    Size size(
                (int)cap.get(CAP_PROP_FRAME_WIDTH),
                (int)cap.get(CAP_PROP_FRAME_HEIGHT)
    );

    VideoWriter writer;
    writer.open("test.avi", CV_FOURCC('M','J','P','G'),fps, size);


    Mat frame, log_polar;


    while (1)
    {
        cap >> frame;

        if (frame.empty())
            break;
        if(waitKey(33) >=0)
            break;
        imshow(g_input_window, frame);

        //example2_5(frame);
        //example2_6(frame);
        //example2_7(frame);
        //example2_9(frame);
        example2_11(frame, log_polar, writer);
    }
    cap.release();
    waitKey(0);
    destroyAllWindows();
    return 0;
}
