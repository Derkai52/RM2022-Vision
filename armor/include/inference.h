#ifndef YOLOXARMOR_INFERENCE_H
#define YOLOXARMOR_INFERENCE_H

#include <iterator>
#include <memory>
#include <string>
#include <vector>
#include <iostream>

#include <inference_engine.hpp>
#include <opencv2/opencv.hpp>
#include <Eigen/Core>
#include <serial.h>
#include <additions.h>
#include <options.h>
#include <systime.h>
#include "general.h"

using namespace std;
using namespace cv;
using namespace InferenceEngine;

struct ArmorObject
{
    int id;                       // 类型编号
    Point2f apex[4];              // 灯条四点坐标
    cv::Rect_<float> rect;        // 灯条四点矩形
    int cls;                      // 目标类别 (0:哨兵 1:英雄 2：工程 3、4、5：步兵 6：前哨站 7：基地)
    int color;                    // 目标颜色分类 (0:蓝色 1:红色 2:灰色)
    int area;                     // 矩形面积大小
    float prob;                   // 分类置信度
    std::vector<cv::Point2f> pts; // 灯条四点坐标
};


class ArmorFinder
{
public:
    ArmorFinder(uint8_t &color, Serial &u);
    ~ArmorFinder();

    void run(cv::Mat &src);                             // 自瞄主函数
    bool detect(Mat &src,vector<ArmorObject>& objects);
    bool initModel(string path);

private:
    typedef enum{
        SEARCHING_STATE, TRACKING_STATE, STANDBY_STATE
    } State;                                            // 自瞄状态枚举定义

    Core ie;
    CNNNetwork network;                                 // 网络
    ExecutableNetwork executable_network;               // 可执行网络
    InferRequest infer_request;                         // 推理请求
    MemoryBlob::CPtr moutput;
    string input_name;
    string output_name;

    const uint8_t &enemy_color;                         // 敌方颜色，引用外部变量，自动变化
    systime frame_time;                                 // 当前帧对应时间
    State state;                                        // 自瞄状态对象实例
    ArmorObject target_box, last_box;                   // 目标装甲板

    Eigen::Matrix<float,3,3> transfrom_matrix;
    Serial &serial;                                     // 串口对象，引用外部变量，用于和能量机关共享同一个变量

    bool stateSearching(cv::Mat &src);                  // 神经网络感知模式
    bool stateStandBy(cv::Mat &src);                    // 备用模式(待开发)
    bool target_network_solving();                      // 神经网络模式目标解算

};

#endif //YOLOXARMOR_INFERENCE_H
