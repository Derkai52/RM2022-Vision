#include <inference.h>
#include <options.h>
#include <log.h>
#include "predictor_kalman.h"
#include "kalman.h"
#include <chrono>

extern PredictorKalman predictor;
extern cv::Mat ori_src;

bool ArmorFinder::target_network_solving() {
    // TODO: 这里使用了一种 Trick 的过滤方法
    if (target_box.rect == cv::Rect2f()) return false;

    struct timeval tv;
    gettimeofday(&tv,NULL);
    long long int sys_time_stamp = tv.tv_sec * 1000 + tv.tv_usec / 1000; // 获取当前系统时间戳

    cv::Mat im2show = ori_src;
    if(armor_predictor){
        bool ok = predictor.predict(target_box.apex, target_box.cls, sys_time_stamp, im2show);
        if(!ok) {
            return false;
        }
    }else{
        bool ok = predictor.none_predict(target_box.apex, target_box.cls, sys_time_stamp, im2show);
    }
    return true;
}
