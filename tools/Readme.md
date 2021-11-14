## 1.测试距离:
将代码复制至 armor/src/armor_finder/send_target/send_target.cpp 大约位于10行.注意调用时的参数

## 2.建立数据集:
将 armor_box_photo 存储数据集文件夹放至工程根目录下.将 save_labelled 文件复制至armor/src/armor/finder/find/find_armor_box.cpp 大约位于162行.注意同时将头文件导入,注意修改代码内的文件存放路径;如需长期开启,请修改 others/src/options.cpp 对应的默认设置为 true

## 3.开机自启动:
将 auto_start.sh 文件添加至 ubuntu程序自启动中.注意修改代码内的文件存放路径!开机自启动建议以终端启动
命令示例(注意以实际路径为准)：
> /usr/bin/gnome-terminal -x /home/sixuanvision/Desktop/auto_start.sh