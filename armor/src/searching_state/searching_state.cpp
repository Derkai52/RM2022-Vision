// 神经网络感知模式
#include "inference.h"

extern ArmorFinder armor_finder;

bool ArmorFinder::stateSearching(cv::Mat &src) {
    state = SEARCHING_STATE;
    vector<ArmorObject> objects;  // 创建装甲板目标属性容器
    cv::Mat input = src;
    uint8_t color;
    ArmorObject cur_object;
    vector<ArmorObject> temp_objects;
    color = mcu_data.enemy_color;
    if (recv_close){
        color = 1; // TODO: 应当使用主控板数据
    }
    if (armor_finder.detect(input, objects)) { // 前向推理获得目标装甲板
        for (auto armor_object : objects) {
            // 过滤2号工程机器人并进行优先级排序
            if(armor_object.color == color && armor_object.cls != 2) {
                if(temp_objects.size() == 0)
                    temp_objects.push_back(armor_object);

                // 按面积大小排序 TODO: 需要更好的多目标决策方法
                else {
                    if(temp_objects.back().area < armor_object.area)
                        temp_objects.push_back(armor_object);
                }
            }
            else {
                target_box.rect = cv::Rect2f();
            }
        }

        if(temp_objects.size()>0){
            // 生成当前目标信息
            target_box = temp_objects.back();
            target_box.id = (temp_objects.back().color)*8 + temp_objects.back().cls;

            target_network_solving(); // 解算目标并发送数据
            DebugT(1, 17, "当前区域存在装甲板数: "<< objects.size());
        }
    }
    else {  // 当前帧无装甲板
        target_box.rect = cv::Rect2f();
        objects.resize(0);
        objects.clear();
        DebugT(1, 17, "当前区域无装甲板: ");
    }

    return true;
}

