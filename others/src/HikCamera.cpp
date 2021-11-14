// 本文件用于备份海康相机SDK，请勿删除
// #include"HikCamera.h"


// void *handle = NULL;
// unsigned int g_nPayloadSize = 0;
//     int nRet = MV_OK;
// int RGB2BGR(unsigned char *pRgbData, unsigned int nWidth, unsigned int nHeight)
// {
//     if (NULL == pRgbData)
//     {
//         return MV_E_PARAMETER;
//     }

//     for (unsigned int j = 0; j < nHeight; j++)
//     {
//         for (unsigned int i = 0; i < nWidth; i++)
//         {
//             unsigned char red = pRgbData[j * (nWidth * 3) + i * 3];
//             pRgbData[j * (nWidth * 3) + i * 3] = pRgbData[j * (nWidth * 3) + i * 3 + 2];
//             pRgbData[j * (nWidth * 3) + i * 3 + 2] = red;
//         }
//     }

//     return MV_OK;
// }

// // convert data stream in Mat format ת��Mat��ʽ��������
// bool Convert2Mat(MV_FRAME_OUT_INFO_EX *pstImageInfo, unsigned char *pData)
// {
//     cv::Mat srcImage;
//     if (pstImageInfo->enPixelType == PixelType_Gvsp_Mono8)
//     {
//         srcImage = cv::Mat(pstImageInfo->nHeight, pstImageInfo->nWidth, CV_8UC1, pData);
//     }
//     else if (pstImageInfo->enPixelType == PixelType_Gvsp_RGB8_Packed)
//     {
//         RGB2BGR(pData, pstImageInfo->nWidth, pstImageInfo->nHeight);
//         srcImage = cv::Mat(pstImageInfo->nHeight, pstImageInfo->nWidth, CV_8UC3, pData);
//     }
//     else
//     {
//         printf("unsupported pixel format\n");
//         return false;
//     }
//     imshow("1",srcImage);
//     if (NULL == srcImage.data)
//     {
//         return false;
//     }

//     return true;
// }

// bool PrintDeviceInfo(MV_CC_DEVICE_INFO *pstMVDevInfo)
// {
//     if (NULL == pstMVDevInfo)
//     {
//         printf("The Pointer of pstMVDevInfo is NULL!\n");
//         return false;
//     }
//     if (pstMVDevInfo->nTLayerType == MV_GIGE_DEVICE)
//     {
//         int nIp1 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0xff000000) >> 24);
//         int nIp2 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x00ff0000) >> 16);
//         int nIp3 = ((pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x0000ff00) >> 8);
//         int nIp4 = (pstMVDevInfo->SpecialInfo.stGigEInfo.nCurrentIp & 0x000000ff);

//         // ch:打印当前相机ip和用户自定义名字 | en:print current ip and user defined name
//         printf("Device Model Name: %s\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chModelName);
//         printf("CurrentIp: %d.%d.%d.%d\n", nIp1, nIp2, nIp3, nIp4);
//         printf("UserDefinedName: %s\n\n", pstMVDevInfo->SpecialInfo.stGigEInfo.chUserDefinedName);
//     }
//     else if (pstMVDevInfo->nTLayerType == MV_USB_DEVICE)
//     {
//         printf("Device Model Name: %s\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chModelName);
//         printf("UserDefinedName: %s\n\n", pstMVDevInfo->SpecialInfo.stUsb3VInfo.chUserDefinedName);
//     }
//     else
//     {
//         printf("Not support.\n");
//     }

//     return true;
// }

// bool read(){
//      int nRet = MV_OK;
//     do
//     {
//         MV_CC_DEVICE_INFO_LIST stDeviceList;
//         memset(&stDeviceList, 0, sizeof(MV_CC_DEVICE_INFO_LIST));

//         // 枚举设备
//         // enum device
//         nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &stDeviceList);
//         if (MV_OK != nRet)
//         {
//             printf("MV_CC_EnumDevices fail! nRet [%x]\n", nRet);
//             break;
//         }

//         if (stDeviceList.nDeviceNum > 0)
//         {
//             for (int i = 0; i < stDeviceList.nDeviceNum; i++)
//             {
//                 printf("[device %d]:\n", i);
//                 MV_CC_DEVICE_INFO *pDeviceInfo = stDeviceList.pDeviceInfo[i];
//                 if (NULL == pDeviceInfo)
//                 {
//                     break;
//                 }
//                 PrintDeviceInfo(pDeviceInfo);
//             }
//         }
//         else
//         {
//             printf("Find No Devices!\n");
//             break;
//         }

//         // printf("Please Intput camera index: ");
//         unsigned int nIndex = 0;
//         // scanf("%d", &nIndex);

//         if (nIndex >= stDeviceList.nDeviceNum)
//         {
//             printf("Intput error!\n");
//             break;
//         }

//         // 选择设备并创建句柄
//         // select device and create handle
//         nRet = MV_CC_CreateHandle(&handle, stDeviceList.pDeviceInfo[nIndex]);
//         if (MV_OK != nRet)
//         {
//             printf("MV_CC_CreateHandle fail! nRet [%x]\n", nRet);
//             break;
//         }

//         // 打开设备
//         // open device
//         nRet = MV_CC_OpenDevice(handle);
//         if (MV_OK != nRet)
//         {
//             printf("MV_CC_OpenDevice fail! nRet [%x]\n", nRet);
//             break;
//         }

//         // ch:探测网络最佳包大小(只对GigE相机有效) | en:Detection network optimal package size(It only works for the GigE camera)
//         if (stDeviceList.pDeviceInfo[nIndex]->nTLayerType == MV_GIGE_DEVICE)
//         {
//             int nPacketSize = MV_CC_GetOptimalPacketSize(handle);
//             if (nPacketSize > 0)
//             {
//                 nRet = MV_CC_SetIntValue(handle, "GevSCPSPacketSize", nPacketSize);
//                 if (nRet != MV_OK)
//                 {
//                     printf("Warning: Set Packet Size fail nRet [0x%x]!\n", nRet);
//                 }
//             }
//             else
//             {
//                 printf("Warning: Get Packet Size fail nRet [0x%x]!\n", nPacketSize);
//             }
//         }

//         /*************************************************/ //属性设置
//         // 设置触发模式为off
//         // set trigger mode as off
//         nRet = MV_CC_SetEnumValue(handle, "TriggerMode", 0);
//         if (MV_OK != nRet)
//         {
//             printf("MV_CC_SetTriggerMode fail! nRet [%x]\n", nRet);
//         }
//         //设置像素格式为RGB
//         nRet = MV_CC_SetEnumValue(handle, "PixelFormat", PixelType_Gvsp_RGB8_Packed);
//         if (MV_OK != nRet)
//         {
//             printf("Set Pixel Format fail! nRet [0x%x]\n", nRet);
//         }

//         //设置亮度为200
//         MV_CC_SetBrightness(handle, 200);
//         //设置帧率为100（理论上100，能否实际跑到100未知）
//         MV_CC_SetFrameRate(handle, 100);
//         //设置曝光模式
//         nRet = MV_CC_SetExposureAutoMode(handle, 2);
//         if (MV_OK != nRet)
//         {
//             printf("Set Exposure Auto Mode fail! nRet [0x%x]\n", nRet);
//         }

//         MV_CC_SetGainMode(handle, 2);

//         /**********************************************/

//         //获取有效载荷大小
//         // Get payload size
//         MVCC_INTVALUE stParam;
//         memset(&stParam, 0, sizeof(MVCC_INTVALUE));
//         nRet = MV_CC_GetIntValue(handle, "PayloadSize", &stParam);
//         if (MV_OK != nRet)
//         {
//             printf("Get PayloadSize fail! nRet [0x%x]\n", nRet);
//         }
//         g_nPayloadSize = stParam.nCurValue;

//         // 开始取流
//         // start grab image
//         nRet = MV_CC_StartGrabbing(handle);
//         if (MV_OK != nRet)
//         {
//             printf("MV_CC_StartGrabbing fail! nRet [%x]\n", nRet);
//             break;
//         }
//     } while (0);

//      do
//         {

//             MV_FRAME_OUT_INFO_EX stImageInfo = {0};
//             memset(&stImageInfo, 0, sizeof(MV_FRAME_OUT_INFO_EX));
//             unsigned char *pData = (unsigned char *)malloc(sizeof(unsigned char) * (g_nPayloadSize));
//             if (pData == NULL)
//             {
//                 printf("Allocate memory failed.\n");
//                 break;
//             }

//             // get one frame from camera with timeout=1000ms �������ȡһ����ʱ=1000ms��֡
//             nRet = MV_CC_GetOneFrameTimeout(handle, pData, g_nPayloadSize, &stImageInfo, 1000);
//             if (nRet == MV_OK)
//             {
//                 printf("Get One Frame: Width[%d], Height[%d], nFrameNum[%d]\n",
//                        stImageInfo.nWidth, stImageInfo.nHeight, stImageInfo.nFrameNum);
//             }
//             else
//             {
//                 printf("No data[0x%x]\n", nRet);
//                 free(pData);
//                 pData = NULL;
//                 break;
//             }

//             bool bConvertRet = false;
        
//             bConvertRet = Convert2Mat(&stImageInfo, pData);            
//             // cv::waitKey(10);

//                 free(pData);
//                 pData = NULL;
         

          
//         } while (0);

//         int nRet = MV_OK;
//        do
//     {
//           // 停止取流
//             // stop grab image
//             nRet = MV_CC_StopGrabbing(handle);
//             if (MV_OK != nRet)
//             {
//                 printf("MV_CC_StopGrabbing fail! nRet [%x]\n", nRet);
//                 break;
//             }
//          // 关闭设备
//     // close device
//     nRet = MV_CC_CloseDevice(handle);
//     if (MV_OK != nRet)
//     {
//         printf("MV_CC_CloseDevice fail! nRet [%x]\n", nRet);
//         break;
//     }

//     // 销毁句柄
//     // destroy handle
//     nRet = MV_CC_DestroyHandle(handle);
//     if (MV_OK != nRet)
//     {
//         printf("MV_CC_DestroyHandle fail! nRet [%x]\n", nRet);
//         break;
//     }

//     if (nRet != MV_OK)
//     {
//         if (handle != NULL)
//         {
//             MV_CC_DestroyHandle(handle);
//             handle = NULL;
//         }
//     }
//     }while(0);
// }