//
// Created by tk on 2021/8/18.
//

#ifndef CIDP_RM_CV_HIKCAMERA_WRAPPER_H
#define CIDP_RM_CV_HIKCAMERA_WRAPPER_H

#endif //CIDP_RM_CV_HIKCAMERA_WRAPPER_H

#include <armor_finder/armor_finder.h>
#include <options.h>
#include <additions.h>
#include "MvCameraControl.h"
#include "camera/Hikcamera_debug.h"

int RGB2BGR(unsigned char *pRgbData, unsigned int nWidth, unsigned int nHeight);

bool Convert2Mat(MV_FRAME_OUT_INFO_EX *pstImageInfo, unsigned char *pData);

bool PrintDeviceInfo(MV_CC_DEVICE_INFO *pstMVDevInfo);

void CamInfoShow(IN void *handle);

void CamInfoSet(IN void *handle);

void CamInit(unsigned int nIndex);

void StopImageStream(void *handle);

void GetOneImage();