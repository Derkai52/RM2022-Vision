#include "energy/energy.h"
#include<opencv2/opencv.hpp>


using namespace std;
using namespace cv;

extern McuData mcu_data;


//----------------------------------------------------------------------------------------------------------------------
// 此函数用于清空各vector
// ---------------------------------------------------------------------------------------------------------------------
void Energy::clearAll() {
    fans.clear();
    armors.clear();
    flow_strip_fans.clear();
    target_armors.clear();
    flow_strips.clear();
}


//----------------------------------------------------------------------------------------------------------------------
// 此函数用于图像预处理
// ---------------------------------------------------------------------------------------------------------------------
void Energy::initImage(cv::Mat &src) {
// 相机标定参数
    cameraMatrix.at<double>(0, 0) = 6.287564830671486e+02;
    cameraMatrix.at<double>(0, 1) = 0.524894206937962;
    cameraMatrix.at<double>(0, 2) = 3.194092032648542e+02;
    cameraMatrix.at<double>(1, 1) = 6.283593581676873e+02;
    cameraMatrix.at<double>(1, 2) = 2.485110761090757e+02;

    distCoeffs.at<double>(0, 0) = -0.476158594402853;
    distCoeffs.at<double>(1, 0) = 0.260183206476090;
    distCoeffs.at<double>(2, 0) = 0.002073886671081;
    distCoeffs.at<double>(3, 0) = -0.002552103371703;
    distCoeffs.at<double>(4, 0) = 0;

    initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat(),
        getOptimalNewCameraMatrix(cameraMatrix, distCoeffs, imageSize, 1, imageSize, 0),
        imageSize, CV_16SC2, map1, map2);

    remap(src, src, map1, map2, INTER_LINEAR);
    imshow("new",src);

    if (src.type() == CV_8UC3){
        cvtColor(src, src, COLOR_BGR2GRAY);
    }
    if (mcu_data.enemy_color == ENEMY_BLUE){
        threshold(src, src, energy_part_param_.RED_GRAY_THRESH, 255, THRESH_BINARY);
    } else if(mcu_data.enemy_color == ENEMY_RED){
        threshold(src, src, energy_part_param_.BLUE_GRAY_THRESH, 255, THRESH_BINARY);
    }
    if (show_process) imshow("bin", src);
    if (show_energy || show_process)waitKey(1);
}

