#include <iostream>
#include <opencv2/opencv.hpp>

// Entities
#include <Entities/area_entity.hpp>

using namespace cv;
using namespace std;

namespace BaldCV
{
    class HairFilter
    {
    private:
        Mat cacheImg;
        string imgPath = "";
        vector<Point2f> imgWrappingPoints;

        bool isDrawOnImg = false;
        bool isFinishDetect = false;

        // MARK: Draw
        void drawingPoint(Mat &targetImg, int x, int y, cv::Scalar color = cv::Scalar(0, 255, 0));
        void drawingText(Mat &targetImg, String text, int x, int y, cv::Scalar color = cv::Scalar(0, 255, 0));

        // MARK: Callbacks
        static void onMouseWarpCallback(int event, int x, int y, int flags, void *userdata);
        void onHandleWarpCallback(int event, int x, int y, int flags, HairFilter *self);

    public:
        Mat imgRead;

        // MARK: Constructor
        HairFilter(string path = "", int flags = IMREAD_COLOR_BGR);

        // MARK: Draw
        void findContour(Mat &targetImg, std::vector<std::vector<cv::Point>> &outputContours);
        void findContourWithContrast(Mat &targetImg, std::vector<std::vector<cv::Point>> &outputContours, cv::Scalar skin, cv::Scalar head);
        vector<uchar> drawingNoise(vector<Point> headParts, vector<vector<cv::Point>> &outputContours, int noisePercent);

        // MARK: Filters
        Scalar scalarBGR2Ycrcb(Scalar bgr);
        Scalar getKmeansColor(const Mat &target, int k);
        Mat bilateralFilterHair(Mat &targetImg);
        Mat equalHistFilter();

        // MARK: Projection
        Mat wrappingImg(Mat &target, vector<Point> warpPoints, Size_<int> size);

        // MARK: Custom
        // void warpHairContours();
        vector<std::vector<Point>> getHairLossContourList(Point analyzeImgSize, vector<Point> forheadParts, vector<Point> headParts, vector<BaldArea> &baldRatioByContour);
    };
}