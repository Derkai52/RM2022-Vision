#include <sstream>    // 建数据集新加的头文件
#include <string.h>   // 建数据集新加的头文件

// 保存有效的装甲板图作为标签(需要去others/src/options.cpp 修改设置开启)
        if (save_labelled_boxes) {
            for (const auto &one_box : armor_boxes) {
                char filename[100];
               if (id2name[one_box.id].data() != (string)"NO"){
                    
                    //filename = "./armor_box_photo/1.jpg";
                    //sprintf(filename, PROJECT_DIR"/armor_box_photo/%s_%d.jpg", id2name[one_box.id].data(),
                    //    time(nullptr) + clock());
                    //string s;
                    //sprintf(s, "/armor_box_photo/%s_%d.jpg", id2name[one_box.id].data(), time(nullptr) + clock()); 
                    
                    //cv::imwrite((const string)str, box_roi);
                //    cout << str<< endl;
                ostringstream oss;  //创建一个流

	            oss << "/root/桌面/CIDP-RM-CV-2021/armor_box_photo/"<< id2name[one_box.id].data() <<"/" << time(nullptr) <<clock()<<".jpg";  //把值传递入流中  %s_%d
	            string result = oss.str();  //获取转换后的字符并将其写入result  

                auto box_roi = src(one_box.rect);
                cv::resize(box_roi, box_roi, cv::Size(48, 36));
                cv::imwrite(result, box_roi);  // 将装甲板ROI图片存储
                }
            }
        }
