//
// Created by tk on 2021/8/18.
//

#ifndef CIDP_RM_CV_CAMERA_DEBUG_H
#define CIDP_RM_CV_CAMERA_DEBUG_H
#endif //CIDP_RM_CV_CAMERA_DEBUG_H

#include <iostream>
#include <opencv2/highgui.hpp>
#include "MvCameraControl.h"

using namespace std;
using namespace cv;

// 滑动条函数声明
static void exposureTrackbar(int, void *);

static void gainTrackbar(int, void *);

static void gammasTrackbar(int, void *);

static void balanceRatioRedTrackbar(int, void *);

static void balanceRatioGreenTrackbar(int, void *);

static void balanceRatioBlueTrackbar(int, void *);

void debugImg();
