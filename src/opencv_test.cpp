#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{

    char *imgName = "../images/bmp_test.bmp";
    Mat image;

    image = imread(imgName, 1);
    if (!image.data)
    {
        cout << "No image data" << endl;
        return -1;
    }
    Mat gray_img;

    cvtColor(image, gray_img, COLOR_BGR2GRAY);
   // imwrite("~/template/result.jpg", gray_img);
    imshow("gray",gray_img);
   cv::waitKey(0);

    return 0;
}