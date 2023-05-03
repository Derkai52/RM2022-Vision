#ifndef _SHOW_IMAGES_H_
#define _SHOW_IMAGES_H_

#include <opencv2/core.hpp>
#include <inference.h>

void showArmorBox(std::string windows_name, const cv::Mat &src, const ArmorObject &armor_box);

#endif /* _SHOW_IMAGES_H_ */
