/*******************************************************************************************/
/*     ______    ____    ____     ____          ______                                     */
/*    / ____/   /  _/   / __ \   / __ \        /_  __/ ___    ____ _   ____ ___            */
/*   / /        / /    / / / /  / /_/ / ______  / /   / _ \  / __ `/  / __ `__ \           */
/*  / /___    _/ /    / /_/ /  / ____/ /_____/ / /   /  __/ / /_/ /  / / / / / /           */
/*  \____/   /___/   /_____/  /_/             /_/    \___/  \__,_/  /_/ /_/ /_/            */
/*                                                                                         */
/*          ____            __              __  ___                  __                    */
/*         / __ \  ____    / /_   ____     /  |/  / ____ _   _____  / /_  ___    _____     */
/*        / /_/ / / __ \  / __ \ / __ \   / /|_/ / / __ `/  / ___/ / __/ / _ \  / ___/     */
/*       / _, _/ / /_/ / / /_/ // /_/ /  / /  / / / /_/ /  (__  ) / /_  /  __/ / /         */
/*      /_/ |_|  \____/ /_.___/ \____/  /_/  /_/  \__,_/  /____/  \__/  \___/ /_/          */
/*                                                                                         */
/*********************<< Designed by SX-CV-RobotTeam in 2021 >>*****************************/

// 本程序使用 640 * 480 作为输入图像尺寸
// 默认为【红色】装甲板为目标，更改目标装甲板颜色指令请查阅 options.cpp 功能列表

#include <iostream>
#include <thread>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <serial.h>  // 串口
#include <camera/camera_wrapper.h>  // 迈德相机设备操作
#include <config/setconfig.h>  // 迈德相机参数设置

#include <camera/video_wrapper.h> // 从文件读取视频的包装器
#include <camera/wrapper_head.h>
#include "MvCameraControl.h" // 海康相机控制库
#include "others/include/camera/Hikcamera_wrapper.h" // 海康相机设备操作
#include "others/include/camera/Hikcamera_debug.h" // 海康相机参数调试
#include <energy/energy.h> // 【Warning】能量机关部分【已弃用，将在未来版本更新】
#include <armor_finder/armor_finder.h>  // 装甲板识别
#include <options.h> // 调试指令交互
#include <additions.h> // 其他组件添加
#define DO_NOT_CNT_TIME // 模块记时器(调试用)
//#define DEBUG // 注释此行即可自动执行所在方颜色选择(前提是能从裁判系统读取机器人颜色方)
#include <log.h> // 日志

using namespace cv;
using namespace std;

McuData mcu_data = {    // 单片机端回传结构体
        0,              // 当前云台yaw角
        0,              // 当前云台pitch角
        ARMOR_STATE,    // 当前状态，自瞄-大符-小符
        0,                 // 云台角度标记位
        0,              // 是否为反陀螺模式
        ENEMY_BLUE,      // 敌方颜色
        0,              // 能量机关x轴补偿量
        0,              // 能量机关y轴补偿量
};
WrapperHead *video = nullptr;    // 云台摄像头视频源

Serial serial(115200);    // 串口对象
// 自瞄主程序对象
ArmorFinder armor_finder(mcu_data.enemy_color, serial, PROJECT_DIR"/tools/para/", mcu_data.anti_top);
// ArmorFinder armor_finder(mcu_data.enemy_color, serial, PROJECT_DIR"/tools/para/", mcu_data.anti_top); //如果可以实现双向通信再使用这个
uint8_t last_state = ARMOR_STATE;     // 上次状态，用于初始化

cv::VideoWriter *videos = NULL;// 保存视频类(如需使用首先确保工程根目录下存在video文件夹)
// IplImage* frame;//同上

extern void *handle; //【Warning】: 并不是一个可读性高的写法。可能引起混乱和冲突。

// 主函数
int main(int argc, char *argv[]) {
    StopImageStream(handle); //  防止未销毁的设备导致内存报错  //【Warning】: 大多数情况下是不必要的操作。可能引发错误
    processOptions(argc, argv);             // 处理命令行参数
    thread receive(uartReceive, &serial);   // 开启串口接收线程
    if (color == false)  // 如果从裁判系统读取不了颜色则手动设置目标颜色
        mcu_data.enemy_color = ENEMY_RED;
    else
        mcu_data.enemy_color = ENEMY_BLUE;

    CamInit(0); // 相机初始化，默认选择使用设备0
    while (true) {
        GetOneImage(); // 获取一帧图像并处理
    }
    cout << "Warning: AutoAiming is Over!!!" << endl;
    return 0;
}
