#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

namespace BaldCV
{
    extern "C"
    {
        struct BaldArea
        {
            double area;
            double headArea;
            double baldRatio;
        };
        struct Point
        {
            int x;
            int y;
        };
    };
}
