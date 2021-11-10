#include "camera/Hikcamera_wrapper.h"

extern ArmorFinder armor_finder;
unsigned int g_nPayloadSize = 0;
void *handle = NULL;  //句柄初始化(通过操作句柄来完成对数据流的操作)

// RGB 转换为 BGR
int RGB2BGR(unsigned char *pRgbData, unsigned int nWidth, unsigned int nHeight) {
    if (NULL == pRgbData) {
        return MV_E_PARAMETER;
    }

    for (unsigned int j = 0; j < nHeight; j++) {
        for (unsigned int i = 0; i < nWidth; i++) {
            unsigned char red = pRgbData[j * (nWidth * 3) + i * 3];
            pRgbData[j * (nWidth * 3) + i * 3] = pRgbData[j * (nWidth * 3) + i * 3 + 2];
            pRgbData[j * (nWidth * 3) + i * 3 + 2] = red;
        }
    }
    return MV_OK;
}

// 转换数据流为Mat类型
bool Convert2Mat(MV_FRAME_OUT_INFO_EX *pstImageInfo, unsigned char *pData) {
    cv::Mat srcImage;
    if (pstImageInfo->enPixelType == PixelType_Gvsp_Mono8) {
        srcImage = cv::Mat(pstImageInfo->nHeight, pstImageInfo->nWidth, CV_8UC1, pData);
    } else if (pstImageInfo->enPixelType == PixelType_Gvsp_RGB8_Packed) {
        RGB2BGR(pData, pstImageInfo->nWidth, pstImageInfo->nHeight);
        srcImage = cv::Mat(pstImageInfo->nHeight, pstImageInfo->nWidth, CV_8UC3, pData);
    } else if (pstImageInfo->enPixelType == PixelType_Gvsp_BGR8_Packed) {
        srcImage = cv::Mat(pstImageInfo->nHeight, pstImageInfo->nWidth, CV_8UC3, pData);  // 平均耗时 0.005ms
    } else { // 无法识别的像素格式
        printf("unsupported pixel format\n");
        return false;
    }
    resize(srcImage, srcImage, Size(640, 480)); // 统一resize图像为640*480大小
    if (save_video) saveVideos(srcImage); // 保存视频

    armor_finder.run(srcImage); // 自动瞄准主程序入口

    if (NULL == srcImage.data) {
        return false;
    }
    return true;
}

// 显示设备信息
bool PrintDeviceInfo(MV_CC_DEVICE_INFO *pstMVDevInfo) {
    if (NULL == pstMVDevInfo) {
        printf("The Pointer of pstMVDevInfo is NULL!\n");
        return false;
    }
    if (pstMVDevInfo->nTLayerType == MV_GIGE_DEVICE) {
        int nIp1 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
        int nIp2 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
        int nIp3 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
        int nIp4 = (pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);

        // ch:打印当前相机ip和用户自定义名字 | en:print current ip and user defined name
        printf("Device Model Name: %s\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chModelName);
        printf("CurrentIp: %d.%d.%d.%d\n", nIp1, nIp2, nIp3, nIp4);
        printf("UserDefinedName: %s\n\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chUserDefinedName);
    } else if (pstMVDevInfo->nTLayerType == MV_USB_DEVICE) {
        printf("Device Model Name: %s\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chModelName);
        printf("UserDefinedName: %s\n\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName);

    } else {
        printf("Not support.\n");
    }

    return true;
}

// 显示图像属性
void CamInfoShow(IN void *handle) {
    int nRet = 0;
    // 数据类型详见海康工业相机常用节点数据类型
    MVCC_INTVALUE stParam;
    MVCC_FLOATVALUE fValue = {0};
    MVCC_ENUMVALUE stTriggerMode = {0};
    printf("\n------------------------------------------------\n");
    // 查看相机采集帧率
    nRet = MV_CC_GetFloatValue(handle, "ResultingFrameRate", &fValue);
    printf("图像采集帧率:%f FPS\n", fValue.fCurValue);
    // 查看曝光时间
    nRet = MV_CC_GetFloatValue(handle, "ExposureTime", &fValue);
    printf("曝光时间:%f us\n", fValue.fCurValue);
    // 查看Gain增益值
    nRet = MV_CC_GetFloatValue(handle, "Gain", &fValue);
    printf("Gain :%f\n", fValue.fCurValue);
    // 查看Gamma
    nRet = MV_CC_GetFloatValue(handle, "Gamma", &fValue);
    printf("Gamma:%f\n", fValue.fCurValue);
    // 查看亮度值
    nRet = MV_CC_GetIntValue(handle, "Brightness", &stParam);
    printf("亮度 :%d\n", stParam.nCurValue);
    // 查看白平衡值
    nRet = MV_CC_GetIntValue(handle, "BalanceRatio", &stParam);
    printf("白平衡值:%d\n", stParam.nCurValue);
    // 查看传输速率
    nRet = MV_CC_GetIntValue(handle, "DeviceConnectionSpeed", &stParam);
    printf("传输速率 :%d Mbps\n", stParam.nCurValue);
    // 查看连接选择
    nRet = MV_CC_GetIntValue(handle, "DeviceLinkSelector", &stParam);
    printf("连接选择 :%d Mbps\n", stParam.nCurValue);
    // 查看链路速度
    nRet = MV_CC_GetIntValue(handle, "DeviceLinkSpeed", &stParam);
    printf("链路速度 :%d Mbps\n", stParam.nCurValue);
    // 查看色度
    nRet = MV_CC_GetIntValue(handle, "Hue", &stParam);
    printf("色度 :%d\n", stParam.nCurValue);
    // 查看像素格式
    nRet = MV_CC_GetEnumValue(handle, "PixelFormat", &stTriggerMode);
    printf("像素尺寸:%d\n", stTriggerMode.nCurValue);
    // 查看像素尺寸
    nRet = MV_CC_GetEnumValue(handle, "PixelSize", &stTriggerMode);
    printf("像素尺寸:%d\n", stTriggerMode.nCurValue);

    printf("\n------------------------------------------------\n");
}

// 图像属性设置
void CamInfoSet(IN void *handle) {
    int nRet = 0;
    // 设置触发模式为off
    nRet = MV_CC_SetEnumValue(handle, "TriggerMode", 0);
    if (MV_OK != nRet) {
        printf("MV_CC_SetTriggerMode fail! nRet [%x]\n", nRet);
    }
    // 设置像素格式为RGB
    nRet = MV_CC_SetEnumValue(handle, "PixelFormat", PixelType_Gvsp_RGB8_Packed);
    if (MV_OK != nRet) {
        printf("Set Pixel Format fail! nRet [0x%x]\n", nRet);
    }
    // 设置亮度为200
    MV_CC_SetBrightness(handle, 250);
    if (MV_OK != nRet) {
        printf("Set Brightness fail! nRet [0x%x]\n", nRet);
    }
    // 控制使能帧率为100
    MV_CC_SetFrameRate(handle, 100);
    if (MV_OK != nRet) {
        printf("Set Frame Rate fail! nRet [0x%x]\n", nRet);
    }
    // 设置曝光模式
    nRet = MV_CC_SetExposureAutoMode(handle, 2);
    if (MV_OK != nRet) {
        printf("Set Exposure Auto Mode fail! nRet [0x%x]\n", nRet);
    }
    MV_CC_SetGainMode(handle, 2);
    if (MV_OK != nRet) {
        printf("Set Gain Mode fail! nRet [0x%x]\n", nRet);
    }
}

MV_CC_DEVICE_INFO *pDeviceInfo;

// 相机初始化
void CamInit(unsigned int nIndex) {
    int nRet = MV_OK;
    do {
        MV_CC_DEVICE_INFO_LIST stDeviceList;
        memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

        // 1、枚举设备 enum device
        nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &stDeviceList);
        if (MV_OK != nRet) {
            printf("MV_CC_EnumDevices fail! nRet [%x]\n", nRet);
            break;
        }

        // 2、遍历设备 device list
        if (stDeviceList.nDeviceNum > 0) {
            for (int i = 0; i < stDeviceList.nDeviceNum; i++) {
                printf("[device %d]:\n", i);
                pDeviceInfo = stDeviceList.pDeviceInfo[i];
                if (NULL == pDeviceInfo) {
                    break;
                }
                PrintDeviceInfo(pDeviceInfo);
            }
        } else {
            printf("Find No Devices!\n");
            break;
        }

        unsigned int nIndex = 0; // 默认相机设备号为0
        // printf("Please Intput camera index: ");
        // scanf("%d", &nIndex); // 输入设备号(适用于多相机设备)

        if (nIndex >= stDeviceList.nDeviceNum) {
            printf("Intput error!\n");
            break;
        }

        // 3、选择设备并创建句柄 select device and create handle
        nRet = MV_CC_CreateHandle(&handle, stDeviceList.pDeviceInfo[nIndex]);
        if (MV_OK != nRet) {
            printf("MV_CC_CreateHandle fail! nRet [%x]\n", nRet);
            break;
        }

        // 4、启动设备 open device
        nRet = MV_CC_OpenDevice(handle);
        if (MV_OK != nRet) {
            printf("MV_CC_OpenDevice fail! nRet [%x]\n", nRet);
            break;
        }

        // 5、获取有效载荷大小 Get payload size
        MVCC_INTVALUE stParam;
        memset(&stParam, 0, sizeof(MVCC_INTVALUE));
        nRet = MV_CC_GetIntValue(handle, "PayloadSize", &stParam);
        if (MV_OK != nRet) {
            printf("Get PayloadSize fail! nRet [0x%x]\n", nRet);
        }
        g_nPayloadSize = stParam.nCurValue;

        // 6、图像属性初始化设置 initializing image config
        CamInfoSet(handle); // 可以通过MVS设置图像属性信息，也可通过此函数设置

        // 7、查看图像信息 show image info
        CamInfoShow(handle);

        // 8、开始取流 start grab image
        nRet = MV_CC_StartGrabbing(handle);
        if (MV_OK != nRet) {
            printf("MV_CC_StartGrabbing fail! nRet [%x]\n", nRet);
            break;
        }

        if (debug_img) debugImg(); // 调试图像

    } while (0);
}

// 关闭推流
void StopImageStream(void *handle) {
    int nRet = MV_OK;
    do {
        // 1、停止取流 stop grab image
        nRet = MV_CC_StopGrabbing(handle);
        if (MV_OK != nRet) {
            printf("MV_CC_StopGrabbing fail! nRet [%x] ,If it`s initializing now, ignore it pleaes.\n", nRet);
            break;
        }
        // 2、关闭设备 close device
        nRet = MV_CC_CloseDevice(handle);
        if (MV_OK != nRet) {
            printf("MV_CC_CloseDevice fail! nRet [%x] ,If it`s initializing now, ignore it pleaes.\n", nRet);
            break;
        }
        // 3、销毁句柄 destroy handle
        nRet = MV_CC_DestroyHandle(handle);
        if (MV_OK != nRet) {
            printf("MV_CC_DestroyHandle fail! nRet [%x] ,If it`s initializing now, ignore it pleaes.\n", nRet);
            break;
        }

        if (nRet != MV_OK) { // 强制销毁
            if (handle != NULL) {
                MV_CC_DestroyHandle(handle);
                handle = NULL;
            }
        }
    } while (0);
}

// 图像采集
void GetOneImage() {
    int nRet = MV_OK;
    do {
        // 1、内存初始化
        MV_FRAME_OUT_INFO_EX stImageInfo = {0};
        memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
        unsigned char *pData = (unsigned char *) malloc(sizeof(unsigned char) * (g_nPayloadSize));
        if (pData == NULL) {
            printf("Allocate memory failed.\n");
            break;
        }

        // 2、超时模式采集图像(超时1000ms即丢弃)
        nRet = MV_CC_GetOneFrameTimeout(handle, pData, g_nPayloadSize, &stImageInfo, 1000);
        if (nRet == MV_OK) {
            // printf("Get One Frame: Width[%d], Height[%d], nFrameNum[%d]\n",
            //        stImageInfo.nWidth, stImageInfo.nHeight, stImageInfo.nFrameNum);
        } else {
            printf("No data，Don't GetOneFrame [0x%x]\n", nRet);
            StopImageStream(handle); // 关闭设备
            free(pData);
            pData = NULL;
            throw 1; //故意使程序异常进入程序重启脚本
            break;
        }

        bool bConvertRet = false;
        bConvertRet = Convert2Mat(&stImageInfo, pData);  // 转换数据流格式为Mat类型
        free(pData); // 释放内存
        pData = NULL;
    } while (0);

}