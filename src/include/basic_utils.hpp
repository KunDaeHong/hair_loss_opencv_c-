#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

namespace BayabasCV
{
  
    class BayabasBasicUtils{
        public:
        
        static void cvWait(int ms) {
            int64 start = cv::getTickCount();
            double freq = cv::getTickFrequency();

            while(true) {
                int64 now = cv::getTickCount();
                double elapsed_ms = (now - start) * 1000.0 / freq;

                if(elapsed_ms >= ms) break;

                cv::waitKey(1);
            }
        };
    };
}
