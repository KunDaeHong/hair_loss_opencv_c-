// Packages
#include <iostream>
#include <opencv2/opencv.hpp>

// Utils
#include <hair_filter.hpp>

// Namespace
using namespace cv;
using namespace std;
using namespace BaldCV;

int main()
{
    string path = "/Users/hongdaehyeon/Desktop/project/cv/bayabasHairLossCV/Resources/errorData/errorData6.png";
    HairFilter test = HairFilter(path);

    /* // 이미지 투영 및 명암 기반 컨투어
    HairFilter test = HairFilter(path);
    test.warpHairContours();
    waitKey(0);
    //*/

    /* // 바이레터럴과 이미지 컨투어 찾기
    HairFilter test = HairFilter(path);
    std::vector<std::vector<cv::Point>> outputContours = {};
    Mat bilateralFiltered = test.bilateralFilterHair(test.imgRead);

    test.findContour(bilateralFiltered, outputContours);
    //*/

    /* //명암기반 이미지 컨투어
    BayabasHairFilter test = BayabasHairFilter(path);
    std::vector<std::vector<cv::Point>> outputContours = {};

    test.findContourWithContrast(test.imgRead, outputContours);
    //*/

    //* //실제 작동되는 테스트 데이터(실제 데이터로 넣어두었습니다.)
    vector<BaldArea> area = {};
    BaldCV::Point analyzeSize = {720, 720};
    vector<BaldCV::Point> headParts = {
        {static_cast<int>(56.445993031358881), static_cast<int>(56.445993031358881)},
        {static_cast<int>(663.55400696864103), static_cast<int>(56.445993031358881)},
        {static_cast<int>(663.55400696864103), static_cast<int>(504.72196383243727)},
        {static_cast<int>(56.445993031358881), static_cast<int>(504.72196383243727)},
    };

    vector<BaldCV::Point> foreheadParts = {
        {static_cast<int>(174.11228851611048), static_cast<int>(504.72196383243727)},
        {static_cast<int>(553.39832608929919), static_cast<int>(504.72196383243727)},
        {static_cast<int>(553.39832608929919), static_cast<int>(636.45765998707179)},
        {static_cast<int>(174.11228851611048), static_cast<int>(636.45765998707179)},
    };

    vector<std::vector<BaldCV::Point>> result = test.getHairLossContourList(analyzeSize, foreheadParts, headParts, area);
    std::printf("fin");
    //*/

    /*
    // 전용 파장 카메라로 테스트를 위한 코드
    VideoCapture cap(0);

    if (!cap.isOpened())
    {
        cout << "카메라 사용불가";
        return -1;
    }

    bool isCaptured = false;
    bool isPressed = false;

    Mat frame;
    Mat whiteSkinFrame, greenSkinFrame;

    while (true)
    {
        cap.read(frame);

        if (frame.empty())
        {
            cout << "프레임 읽기 불가" << endl;
            break;
        }

        imshow("Camera Viewer", frame);

        int key = waitKey(1);

        if (key == 32 && !isPressed)
        {
            isPressed = true;
            cout << "Key Pressed" << endl;

            if (!isCaptured)
            {
                BayabasBasicUtils::cvWait(800);
                cap.read(frame);
                whiteSkinFrame = frame.clone();
                isCaptured = true;
                cout << "White Skin Captured" << endl;
            }
            else
            {
                BayabasBasicUtils::cvWait(100);
                cap.read(frame);
                greenSkinFrame = frame.clone();
                cout << "Green Skin Captured" << endl;
                break;
            }
        }

        if (key != 32 || key != 113)
        {
            isPressed = false;
        }

        if (key == 113)
            break;
    }
    cap.release();
    imshow("Camera Viewer", whiteSkinFrame);
    waitKey(0);
    imshow("Camera Viewer", greenSkinFrame);
    waitKey(0);
    destroyWindow("Camera Viewer");

    // CLAHE
    Mat whiteSkinYcrcb, whiteSkinFrameClahe, greenSkinYcrcb, greenSkinFrameClahe;
    cvtColor(whiteSkinFrame, whiteSkinYcrcb, COLOR_BGR2YCrCb);
    cvtColor(greenSkinFrame, greenSkinYcrcb, COLOR_BGR2YCrCb);

    vector<Mat> wsChannel;
    vector<Mat> gsChannel;
    split(whiteSkinYcrcb, wsChannel);
    split(greenSkinYcrcb, gsChannel);

    Ptr<CLAHE> clahe = createCLAHE(1.5, Size(5, 5));
    clahe->apply(wsChannel[0], whiteSkinFrameClahe);
    clahe->apply(gsChannel[0], greenSkinFrameClahe);
    wsChannel[0] = whiteSkinFrameClahe;
    gsChannel[0] = greenSkinFrameClahe;

    merge(wsChannel, whiteSkinYcrcb);
    merge(gsChannel, greenSkinYcrcb);

    Mat grayG, grayW;
    cvtColor(whiteSkinYcrcb, whiteSkinFrame, COLOR_YCrCb2BGR);
    cvtColor(greenSkinYcrcb, greenSkinFrame, COLOR_YCrCb2BGR);
    cvtColor(whiteSkinFrame, grayW, COLOR_BGR2GRAY);
    cvtColor(greenSkinFrame, grayG, COLOR_BGR2GRAY);

    Mat finalMask;
    bitwise_and(grayW, grayG, finalMask);

    imshow("Mask Result", finalMask);
    waitKey(0);
    destroyWindow("Mask Result");

    return 0;
    //*/
}
