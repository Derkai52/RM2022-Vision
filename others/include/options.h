#ifndef _OPTIONS_H_
#define _OPTIONS_H_

// #ifdef PATH
//     #define PROJECT_DIR PATH
// #else
//     #define PROJECT_DIR ""
// #endif

extern bool show_armor_box;
extern bool show_origin;
extern bool run_with_camera;
extern bool debug_camera;
extern bool wait_uart;
extern bool show_info;
extern bool run_by_frame;
extern bool recv_close;      // 无下位机控制，仅用于测试
extern bool armor_predictor; // 预测器

void processOptions(int argc, char **argv);

#endif /* _OPTIONS_H_ */
