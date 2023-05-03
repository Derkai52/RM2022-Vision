#include <show_images.h>
#include <opencv2/highgui.hpp>
#include <options.h>
#include <log.h>

using namespace cv;

/**************************
 * 显示单个装甲板区域及其类别 *
 **************************/
void showArmorBox(std::string windows_name, const cv::Mat &src, const ArmorObject &box) {
    static Mat image2show;
    image2show = src.clone();

    // 绘制十字参考线
    cv::line(image2show, cv::Point(720,0),cv::Point(720,1080),(180,255,100),2);
    cv::line(image2show, cv::Point(0,540),cv::Point(1440,540),(180,255,100),2);

    // 显示当前视觉模式【预测、 非预测】
    if(armor_predictor){
        putText(image2show, "Predict ON", Point(20,30), cv::FONT_HERSHEY_DUPLEX, 1, Scalar(0, 255, 0));
    }else{
        putText(image2show, "Predict OFF", Point(20,30), cv::FONT_HERSHEY_DUPLEX, 1, Scalar(0, 0, 255));
    }

    if(box.rect == cv::Rect2f()){
        imshow(windows_name, image2show);
        return;
    }


    // 绘制文字
    char prob[10];
    sprintf(prob, "%.1f", box.prob);
    if (box.id == -1)
        putText(image2show, id2name[box.id]+" "+prob, Point(box.rect.x + 2, box.rect.y - 4), cv::FONT_HERSHEY_DUPLEX, 1,
                Scalar(0, 255, 0));
    else if (0 <= box.id && box.id < 8)
        putText(image2show, id2name[box.id]+" "+prob, Point(box.rect.x + 2, box.rect.y - 4), cv::FONT_HERSHEY_DUPLEX, 1,
                Scalar(255, 0, 0));
    else if (8 <= box.id && box.id < 15)
        putText(image2show, id2name[box.id]+" "+prob, Point(box.rect.x + 2, box.rect.y - 4), cv::FONT_HERSHEY_DUPLEX, 1,
                Scalar(0, 0, 255));
    else LOGE_INFO("Invalid box id:%d!", box.id);

    // 绘制装甲板四点矩形
    for (int i = 0; i < 4; i++) {
        line(image2show, box.pts[i], box.pts[(i + 1) % 4], Scalar(200, 200, 200), 2);
    }
    imshow(windows_name, image2show);

}