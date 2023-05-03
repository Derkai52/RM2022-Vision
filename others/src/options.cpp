#include <options.h>
#include <log.h>
#include <cstring>
#include <map>

bool show_armor_box = false;   // 显示装甲板图
bool show_armor_boxes = false; // 显示所有装甲板图
bool show_origin = false;      // 显示源图
bool run_with_camera = false;  // 图像源仅使用相机
bool save_video = false;       // 录制视频
bool debug_camera = false;     // 调试相机
bool wait_uart = false;        // 串口检测
bool show_info = false;        // 仅终端显示目标信息
bool run_by_frame = false;     // 图像源仅使用视频流
bool recv_close = false;       // 无下位机控制
bool armor_predictor = false;  // 目标预测器


// 使用map保存所有选项及其描述和操作，加快查找速度。
std::map<std::string, std::pair<std::string, void(*)(void)>> options = {
    {"--help",{
        "show the help information.", [](){
            LOG(LOG_MSG, "<HELP>: " STR_CTR(WORD_BLUE, "All options below are for debug use."));
            for(const auto &option : options){
                LOG(LOG_MSG, "<HELP>: " STR_CTR(WORD_GREEN, "%s: %s"), option.first.data(), option.second.first.data());
            }
        }
    }},
    {"--game", {
        "Game mode.", []() {
            debug_camera=true;
            LOGM("Enable Game Mode");
        }
    }},
    {"--show-armor", {
        "show the aim box.", []() {
            show_armor_box = true;
            debug_camera=true;
            LOGM("Enable show armor box");
        }
    }},
    {"--show-armor-predict", {
        "show the aim box with predict.", []() {
            show_armor_box = true;
            armor_predictor = true;
            debug_camera=true;
            LOGM("Enable show armor box with predict");
        }
    }},
    {"--test-show-armor", {
        "show the aim box, recv_close.", []() {
            recv_close = true;
            show_armor_box = true;
            debug_camera=true;
            LOGM("Enable show armor box, recv_close");
        }
    }},
    {"--test-show-armor-predict", {
        "show the aim box with predict, recv close.", []() {
            recv_close = true;
            show_armor_box = true;
            armor_predictor = true;
            debug_camera=true;
            LOGM("Enable show armor box with predict, recv close");
        }
    }},
    {"--show-origin", {
        "show the origin image.", [](){
            show_origin = true;
            LOGM("Enable show origin");
        }
    }},
    {"--run-with-camera", {
        "start the program with camera directly without asking.", []() {
            run_with_camera = true;
            LOGM("Run with camera!");
        }
    }},
    {"--wait-uart", {
        "wait uart until ready before running.", [](){
            wait_uart = true;
            LOGM("Enable wait uart!");
        }
    }}
};

void processOptions(int argc, char **argv) {
    if (argc >= 2) {
        for (int i = 1; i < argc; i++) {
            auto key = options.find(std::string(argv[i])); // 寻找对应选项。
            if(key != options.end()){
                key->second.second();
            }else{
                LOGW("Unknown option: %s. Use --help to see options.", argv[i]);
            }
        }
    }
}