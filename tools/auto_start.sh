#!/bin/bash
#开机自启动视觉自瞄代码并检测，可调整sleep后的数字来调整监测周期(单位：秒)
#(请根据实际路径自行替换)
while true
do
        ps -ef|grep -v grep|grep "/home/nuc/Desktop/CIDP-RM-CV-2021/build/run --match-red" || /home/nuc/Desktop/CIDP-RM-CV-2021/build/run --match-red
        sleep 2
done
