#include "camera/video_wrapper.h"
#include <opencv2/opencv.hpp>

using namespace cv;


VideoWrapper::VideoWrapper(const std::string &filename) {

    video.open(0);
    video.set(CAP_PROP_FOURCC, VideoWriter::fourcc('M', 'J', 'P', 'G'));
}

VideoWrapper::~VideoWrapper() = default;


bool VideoWrapper::init() {
    return video.isOpened();
}

bool VideoWrapper::read(cv::Mat &src) {
    return video.read(src);
}

