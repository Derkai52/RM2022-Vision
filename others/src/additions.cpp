#include <cstring>
#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio/videoio_c.h>
#include <additions.h>
#include <inference.h>
#include <log.h>

#define RECEIVE_LOG_LEVEL LOG_MSG

using namespace std;
using namespace cv;

extern Serial serial;
extern uint8_t last_state;

extern ArmorFinder armor_finder;
// extern Energy energy;


void uartReceive(Serial *pSerial) {
    char buffer[40];
    LOGM(STR_CTR(WORD_LIGHT_WHITE, "data receive start!"));
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        pSerial->ReadData((uint8_t *) buffer, sizeof(mcu_data)+1);
       // cout << "数据长度："<< (sizeof(mcu_data)+1) << endl;
        if (buffer[sizeof(mcu_data)] == '\n') {
            memcpy(&mcu_data, buffer, sizeof(mcu_data));
//            printf("buffer[0] = %d\n",(int)buffer[0]);
//            printf("buffer[1] = %d\n",(int)buffer[1]);
//            printf("buffer[2] = %d\n",(int)buffer[2]);
//            printf("buffer[3] = %d\n",(int)buffer[3]);
//            printf("buffer[4] = %d\n",(int)buffer[4]);
//            printf("buffer[5] = %d\n",(int)buffer[5]);
//            printf("buffer[6] = %d\n",(int)buffer[6]);
//            printf("buffer[7] = %d\n",(int)buffer[7]);
//            printf("Get yaw: %f, pitch: %f!", mcu_data.curr_yaw, mcu_data.curr_pitch);
//            printf("Get Mode%d", mcu_data.enemy_color);
//            LOGM("Get, state:%c, mark:%d!", mcu_data.state, (int) mcu_data.mark);
//            LOGM("Get yaw: %f, pitch: %f!", mcu_data.curr_yaw, mcu_data.curr_pitch);
//            LOGM("Get delta x: %d, delta y: %d!", mcu_data.delta_x, mcu_data.delta_y);
//            static int t = time(nullptr);
//            static int cnt = 0;
//            if(time(nullptr) > t){
//                t = time(nullptr);
//                LOGM("fps:%d", cnt);
//                cnt = 0;
//            }else{
//                cnt++;
//            }
        }else{
//            LOGW("corrupt data!");
        }
    }
}

void showOrigin(const cv::Mat &src) {
    if (!src.empty()) {
        imshow("origin", src);
        cv::waitKey(1);
    } else return;
}

\
void extract(cv::Mat &src) {//图像预处理，将视频切成640×480的大小
    if (src.empty()) return;
    float length = static_cast<float>(src.cols);
    float width = static_cast<float>(src.rows);
    if (length / width > 640.0 / 480.0) {
        length *= 480.0 / width;
        resize(src, src, cv::Size(length, 480));
        src = src(Rect((length - 640) / 2, 0, 640, 480));
    } else {
        width *= 640.0 / length;
        resize(src, src, cv::Size(640, width));
        src = src(Rect(0, (width - 480) / 2, 640, 480));
    }
}

double getPointLength(const cv::Point2f &p) {
    return sqrt(p.x * p.x + p.y * p.y);
}
