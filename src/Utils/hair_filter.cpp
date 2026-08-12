#include "hair_filter.hpp"

/*
**NOTE**
The image size should be 128x128. Resize the image for hair disease vit model.
**/

namespace BaldCV
{

    // Constructor
    HairFilter::HairFilter(string path, int flags)
    {
        imgPath = path;
        imgRead = cv::imread(path, flags);
    }

    // MARK: Filter

    Mat HairFilter::bilateralFilterHair(Mat &targetImg)
    {
        Mat imgFiltered;
        // 이미지, 출력될 이미지, 이웃픽셀거리, 컬러당 공간정의, 컬러 범위(d < 범위 일 시 d값에 비례함)
        cv::bilateralFilter(targetImg, imgFiltered, 15, 50, 50);
        return imgFiltered;
    }

    Mat HairFilter::equalHistFilter()
    {
        Mat imgFiltered;
        equalizeHist(imgRead, imgFiltered);

        return imgFiltered;
    }

    cv::Scalar HairFilter::getKmeansColor(const Mat &target, int k)
    {
        Mat samples;
        target.convertTo(samples, CV_32F);
        samples = samples.reshape(1, target.rows * target.cols);

        Mat labels, centers;
        cv::kmeans(
            samples,
            k,
            labels,
            TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 10, 1.0),
            3,
            KMEANS_PP_CENTERS,
            centers);

        vector<int> cnts(k, 0);
        for (int i = 0; i < labels.rows; ++i)
        {
            cnts[labels.at<int>(i)]++;
        }

        int maxClusterIdx = 0;
        int maxCnt = 0;
        for (int i = 0; i < k; ++i)
        {
            if (cnts[i] > maxCnt)
            {
                maxCnt = cnts[i];
                maxClusterIdx = i;
            }
        }

        return cv::Scalar(centers.at<float>(maxClusterIdx, 0), centers.at<float>(maxClusterIdx, 1), centers.at<float>(maxClusterIdx, 2));
    }

    Scalar HairFilter::scalarBGR2Ycrcb(Scalar bgr)
    {
        Mat bgrMat(1, 1, CV_8UC3, bgr);
        Mat ycrcbMat;
        cvtColor(bgrMat, ycrcbMat, cv::COLOR_BGR2YCrCb);
        Vec3b pixel = ycrcbMat.at<Vec3b>(0, 0);
        return Scalar(pixel[0], pixel[1], pixel[2]);
    }

    // MARK: Draw

    void HairFilter::drawingPoint(Mat &targetImg, int x, int y, cv::Scalar color)
    {
        cv::Point center(x, y);
        cv::circle(targetImg, center, 5, color, 5);
    }

    void HairFilter::drawingText(Mat &targetImg, String text, int x, int y, cv::Scalar color)
    {
        cv::Point center(x, y);
        cv::putText(targetImg, text, center, FONT_HERSHEY_SIMPLEX, 1, color);
    }

    void HairFilter::findContour(Mat &targetImg, std::vector<std::vector<cv::Point>> &outputContours)
    {
        Mat targetImgCopy;
        cv::RNG rng(12345);
        int mode = RETR_EXTERNAL;
        int method = CHAIN_APPROX_SIMPLE;
        Mat grayscaleTargetImg, thresholdImg;

        if (targetImg.type() == COLOR_BGR2GRAY)
        {
            cvtColor(targetImg, grayscaleTargetImg, COLOR_BGR2GRAY);
        }
        else
        {
            grayscaleTargetImg = targetImg; // 레퍼런스 얕은 복사로 누수 안됨
        }

        cv::threshold(grayscaleTargetImg, thresholdImg, 100, 255, cv::THRESH_BINARY);
        cv::findContours(thresholdImg, outputContours, mode, method, cv::Point());

#ifdef DEBUG
        cvtColor(targetImg, targetImgCopy, COLOR_GRAY2BGR);
#endif

        for (int c = 0; c < outputContours.size(); ++c)
        {
            double area = cv::contourArea(outputContours[c]);

            if (area < 300)
            {
                outputContours.erase(outputContours.begin() + c);
                --c;
                continue;
            }

#ifdef DEBUG
            cv::drawContours(targetImgCopy, outputContours, c, cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256)), 2);
#endif
        }

#ifdef DEBUG
        // imshow("contour", targetImgCopy);
        // waitKey(0);
#endif
    }

    void HairFilter::findContourWithContrast(Mat &targetImg, std::vector<std::vector<cv::Point>> &outputContours, cv::Scalar skin, cv::Scalar head)
    {
        // Step 1:  Make brighter on dark side ONLY.
        Mat yuvImgRead, afterClahe;
        cv::cvtColor(targetImg, yuvImgRead, COLOR_BGR2YUV);

        vector<Mat> channels;
        cv::split(yuvImgRead, channels);

        Mat img_clahe_y;
        Ptr<cv::CLAHE> clahe = cv::createCLAHE(2.0, cv::Size(5, 5));
        clahe->apply(channels[0], img_clahe_y);
        channels[0] = img_clahe_y;

        cv::merge(channels, yuvImgRead);
        cv::cvtColor(yuvImgRead, afterClahe, COLOR_YUV2BGR);

        // Mat hairYChannel, hairEdges;
        // cv::cvtColor(afterClahe, hairYChannel, COLOR_BGR2GRAY);
        // cv::Canny(hairYChannel, hairEdges, 50, 90);

        // Step 2: Remove noise and change to image color to yuv color standard.
        Mat ycrcbImg;
        vector<Mat> ycrcbImgChannels;
        Mat waterHairShadeImg = HairFilter::bilateralFilterHair(afterClahe);

        cv::cvtColor(waterHairShadeImg, ycrcbImg, cv::COLOR_BGR2YCrCb);
        cv::split(ycrcbImg, ycrcbImgChannels);

#ifdef DEBUG
        imshow("Clean image based on yuv", ycrcbImg);
        waitKey(0);
#endif

        // Step 3: Detecting head color only (whole head)
        // 2 Channels YcbcrImg

        int deltaHairY = 50;                      // 백인의 금발인 경우 측정이 꽤 어려움.
        int deltaHairCr = 15;                     // Cr(붉은기) 채널의 허용 오차 범위
        int deltaHairCb = 15;                     // Cb(푸른기) 채널의 허용 오차 범위
        Scalar headYcrcb = scalarBGR2Ycrcb(head); // 실제 머리카락 색상

        Scalar lowHead(headYcrcb[0] - deltaHairY, std::max(0, (int)(headYcrcb[1] - deltaHairCr)), std::max(0, (int)(headYcrcb[2] - deltaHairCb)));
        Scalar highHead(headYcrcb[0] + deltaHairY, std::min(255, (int)(headYcrcb[1] + deltaHairCr)), std::min(255, (int)(headYcrcb[2] + deltaHairCb)));
        Mat maskHeadYcrcb;
        cv::inRange(ycrcbImg, lowHead, highHead, maskHeadYcrcb);

#ifdef DEBUG
        imshow("hair mask", maskHeadYcrcb);
        waitKey(0);
#endif

        // Step 4: Detecting skin color only and remove skin color on mask
        Mat removedHairImg;
        removedHairImg.setTo(Scalar(0, 0), maskHeadYcrcb);

        // 피부
        int deltaCr = 15; // Cr(붉은기) 채널의 허용 오차 범위
        int deltaCb = 15; // Cb(푸른기) 채널의 허용 오차 범위
        Scalar skinYcrcb = scalarBGR2Ycrcb(skin);
        Scalar lowSkin(std::max(0, (int)(skinYcrcb[1] - deltaCr)), std::max(0, (int)(skinYcrcb[2] - deltaCb)));
        Scalar highSkin(std::min(255, (int)(skinYcrcb[1] + deltaCr)), std::min(255, (int)(skinYcrcb[2] + deltaCb)));

        Mat maskSkin;
        cv::inRange(removedHairImg, lowSkin, highSkin, maskSkin);
#ifdef DEBUG
        imshow("skin mask", maskSkin);
        waitKey(0);
#endif

        // 조도에 의한 피부
        Mat skinGray;
        cv::cvtColor(targetImg, skinGray, COLOR_BGR2GRAY);
        Mat brightSkinMask;
        double lowThreshold = 100;
        double highThreshold = 230;
        cv::inRange(skinGray, Scalar(lowThreshold), Scalar(highThreshold), brightSkinMask);

#ifdef DEBUG
        imshow("bright skin mask", brightSkinMask);
        waitKey(0);
#endif

        Mat finalMaskSkin;
        cv::bitwise_and(maskSkin, brightSkinMask, finalMaskSkin);

#ifdef DEBUG
        imshow("final skin mask", finalMaskSkin);
        waitKey(0);
#endif

        Mat morph = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(4, 4));
        cv::morphologyEx(finalMaskSkin, finalMaskSkin, cv::MORPH_OPEN, morph);
        cv::morphologyEx(finalMaskSkin, finalMaskSkin, cv::MORPH_CLOSE, morph);
#ifdef DEBUG
        imshow("final mask", finalMaskSkin);
        waitKey(0);
#endif

        // 컨투어 찾기
        findContour(finalMaskSkin, outputContours);
    }

    vector<uchar> HairFilter::drawingNoise(vector<Point> headParts, vector<vector<cv::Point>> &outputContours, int noisePercent)
    {
        srand((unsigned int)time(NULL));
        Mat targetImg = imgRead.clone();

        Mat headWarp = HairFilter::wrappingImg(targetImg, headParts, Size_<int>(targetImg.cols, targetImg.rows));
        cv::Scalar meanHeadColor = HairFilter::getKmeansColor(headWarp, 5);

        for (const vector<cv::Point> &path : outputContours)
        {
            cv::Rect boundingBox = cv::boundingRect(path);
            for (int y = boundingBox.y; y < boundingBox.y + boundingBox.height; ++y)
            {
                for (int x = boundingBox.x; x < boundingBox.x + boundingBox.width; ++x)
                {

                    if (cv::pointPolygonTest(path, cv::Point(x, y), false) > 0)
                    {
                        if (rand() % 100 < noisePercent)
                        {
                            targetImg.at<Vec3b>(cv::Point(x, y))[0] = meanHeadColor[0];
                            targetImg.at<Vec3b>(cv::Point(x, y))[1] = meanHeadColor[1];
                            targetImg.at<Vec3b>(cv::Point(x, y))[2] = meanHeadColor[2];
                        }
                    }
                }
            }
        }

        vector<uchar> buffer;
        cv::imencode(".png", targetImg, buffer);

        return buffer;
    }

    // MARK: Projection
    Mat HairFilter::wrappingImg(Mat &target, vector<Point> warpPoints, Size_<int> size)
    {
        // Open CV는 모든 구조가 좌상, 우상, 우하, 좌하로 이루어짐
        vector<Point2f> warpCorners = {
            Point2f(0, 0),                    // Left Top
            Point2f(size.width, 0),           // Right Top
            Point2f(size.width, size.height), // Right Bottom
            Point2f(0, size.height),          // Left Bottom
        };

        vector<Point2f> warpPoint2Point2f = {
            Point2f(warpPoints[0].x, warpPoints[0].y),
            Point2f(warpPoints[1].x, warpPoints[1].y),
            Point2f(warpPoints[2].x, warpPoints[2].y),
            Point2f(warpPoints[3].x, warpPoints[3].y)};

        Mat warpOutput(size, target.type());
        // 사용법: 프로젝션될 이미지 원본 좌표 4개만 사용합니다. 투영될 좌표, 투영사이즈
        Mat transCoord = getPerspectiveTransform(warpPoint2Point2f, warpCorners);
        // 사용법: 원본이미지, 추출될 이미지, 투영된 좌표, 투영 후 이미지 사이즈
        cv::warpPerspective(target, warpOutput, transCoord, size);

        return warpOutput;
    }

    // MARK: Custom

    vector<std::vector<Point>> HairFilter::getHairLossContourList(Point analyzeImgSize, vector<Point> forheadParts, vector<Point> headParts, vector<BaldArea> &baldRatioByContour)
    {
#ifdef DEBUG
        imshow("target", imgRead);
        waitKey(0);
#endif

        // Mat yuvImgRead, afterClahe;
        // cv::cvtColor(imgRead, yuvImgRead, COLOR_BGR2YUV);

        // vector<Mat> channels;
        // cv::split(yuvImgRead, channels);

        // Mat img_clahe_y;
        // Ptr<cv::CLAHE> clahe = cv::createCLAHE(1.0, cv::Size(5,5));
        // clahe->apply(channels[0], img_clahe_y);
        // channels[0] = img_clahe_y;

        // cv::merge(channels, yuvImgRead);
        // cv::cvtColor(yuvImgRead, afterClahe, COLOR_YUV2BGR);

        // #ifdef DEBUG
        // imshow("target", afterClahe);
        // waitKey(0);
        // #endif

        std::printf("Start Warp");
        Mat headWarp = HairFilter::wrappingImg(imgRead, headParts, Size_<int>(analyzeImgSize.x, analyzeImgSize.y));
        Mat foreheadWarp = HairFilter::wrappingImg(imgRead, forheadParts, Size_<int>(analyzeImgSize.x, analyzeImgSize.y));
        std::printf("Fin Warp");

#ifdef DEBUG
        imshow("target", headWarp);
        waitKey(0);
#endif

        std::printf("Start mean");
        cv::Scalar meanHeadColor = HairFilter::getKmeansColor(headWarp, 5);
        cv::Scalar meanForeheadColor = cv::mean(foreheadWarp);
        std::printf("Fin mean");

        std::printf("Start Contour");
        double totalBaldRatio = 0.0;
        vector<vector<cv::Point>> outputContours = {};
        vector<vector<Point>> outputContoursPoints = {};         // return
        vector<vector<cv::Point>> outputContoursPointsTest = {}; // Debug

        findContourWithContrast(headWarp, outputContours, meanForeheadColor, meanHeadColor);

        const double headArea = headWarp.cols * headWarp.rows;

        for (const vector<cv::Point> &i : outputContours)
        {
            vector<Point> points;
            vector<cv::Point> pointsDebug; // Debug
            double area = cv::contourArea(i);
            double baldRatio = (area / headArea) * 100;
            double scaleX = (headParts[1].x - headParts[0].x) / double(analyzeImgSize.x);
            double scaleY = (headParts[2].y - headParts[1].y) / double(analyzeImgSize.y);

            BaldCV::BaldArea baldInfo = {area, headArea, baldRatio};
            baldRatioByContour.push_back(baldInfo);

            for (const cv::Point &j : i)
            {
                int originX = j.x * scaleX + headParts[0].x;
                int originY = j.y * scaleY + headParts[0].y;

                points.push_back(Point{originX, originY});
                pointsDebug.push_back(cv::Point{originX, originY}); // Debug
            }

            totalBaldRatio += baldRatio;
            outputContoursPoints.push_back(points);
            outputContoursPointsTest.push_back(pointsDebug);

#ifdef DEBUG
            std::printf("Head Area: %f Contour Area: %f Bald Ratio: %f\n", headArea, area, baldRatio);
#endif
        }

#ifdef DEBUG
        std::printf("\n Total Bald Ratio: %f \n", totalBaldRatio);
        cv::RNG rng(12345);
        for (int c = 0; c < outputContours.size(); ++c)
        {
            cv::drawContours(imgRead, outputContoursPointsTest, c, cv::Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256)), 2);
        }
        imshow("result", imgRead);
        waitKey(0);
#endif

        std::printf("Fin Contour");
        return outputContoursPoints;
    }

    // MARK: Callbacks
    void HairFilter::onHandleWarpCallback(int event, int x, int y, int flags, HairFilter *self)
    {
        if (event == EVENT_LBUTTONDOWN)
        {

#ifdef DEBUG
            std::printf("eventType: %d debug coord x: %d, y: %d\n", event, x, y);
            fflush(stdout);
#endif
            imgWrappingPoints.push_back(Point2f(x, y));
            self->isDrawOnImg = true;
        }
        else if (event == EVENT_LBUTTONUP)
        {
            self->isDrawOnImg = false;

            if (imgWrappingPoints.size() == 4)
            {
#ifdef DEBUG
                std::printf("Finish eventType: %d debug coord x: %d, y: %d\n", event, x, y);
                fflush(stdout);
#endif
                self->isFinishDetect = true;
            }
        }
    }

    void HairFilter::onMouseWarpCallback(int event, int x, int y, int flags, void *userdata)
    {
        if (userdata)
        {
            auto *self = static_cast<HairFilter *>(userdata);
            self->onHandleWarpCallback(event, x, y, flags, self);
        }
    }
}