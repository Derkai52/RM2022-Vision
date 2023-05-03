CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

PROJECT(rm-vision)
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_BUILD_TYPE RELEASE)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPATH=\"\\\"${PROJECT_SOURCE_DIR}\\\"\"")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D${CMAKE_SYSTEM_NAME}")
SET(YAML_CPP_LIBRARIES /usr/local/lib/libyaml-cpp.so)  # 设置yaml-cpp库
SET(HIKVISION_CAMERA_DIR "${PROJECT_SOURCE_DIR}/camera/hikvision") # 设置海康相机SDK工具包目录
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
SET(CMAKE_PREFIX_PATH /opt/intel/openvino_2021/deployment_tools/inference_engine/share) # 设置 OpenVino路径(以实际安装路径为准)
SET(BIN_NAME "run")

add_definitions(-D _GLIBCXX_USE_CXX11_ABI=1)
add_compile_definitions(PROJECT_DIR="${PROJECT_SOURCE_DIR}")

FIND_PACKAGE(InferenceEngine REQUIRED)
FIND_PACKAGE(Eigen3 REQUIRED)
FIND_PACKAGE(OpenCV 4 REQUIRED)
FIND_PACKAGE(Threads)

# 导入海康相机SDK
LINK_DIRECTORIES(${HIKVISION_CAMERA_DIR}/64)
INCLUDE_DIRECTORIES(${HIKVISION_CAMERA_DIR}/include)
INCLUDE_DIRECTORIES(${HIKVISION_CAMERA_DIR}/tool/include)

INCLUDE_DIRECTORIES(${InferenceEngine_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${EIGEN3_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${FFTW3_INCLUDE_DIR})

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/armor/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/others/include)

FILE(GLOB_RECURSE sourcefiles "others/src/*.cpp" "armor/src/*.cpp")
FILE(GLOB_RECURSE Hiksourcefiles "${HIKVISION_CAMERA_DIR}/src/*.cpp" "${HIKVISION_CAMERA_DIR}/tool/src/*.cpp") # 海康相机拓展工具文件
ADD_EXECUTABLE(${BIN_NAME} main.cpp ${sourcefiles} ${Hiksourcefiles})

TARGET_LINK_LIBRARIES(${BIN_NAME} ${CMAKE_THREAD_LIBS_INIT}
                                  ${OpenCV_LIBS}
                                  MvCameraControl
                                  ${Eigen3_LIBRARIES}
                                  ${YAML_CPP_LIBRARIES}
                                  ${InferenceEngine_LIBRARIES}
                                  )

# 设置海康相机配置目录路径
set(HIK_CONFIG_FILE_PATH ${PROJECT_SOURCE_DIR}/camera/hikvision/config)
target_compile_definitions(${BIN_NAME} PRIVATE "HIK_CONFIG_FILE_PATH=\"${HIK_CONFIG_FILE_PATH}\"")

# 设置相机标定相关的配置文件路径
set(HIK_CALI_FILE_PATH ${PROJECT_SOURCE_DIR}/camera/hikvision/config/cameraCaliData)
target_compile_definitions(${BIN_NAME} PRIVATE "HIK_CALI_FILE_PATH=\"${HIK_CALI_FILE_PATH}\"")