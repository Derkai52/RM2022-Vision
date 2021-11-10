#include <armor_finder/armor_finder.h>
#include <config/setconfig.h>
#include<options.h>
#include <log.h>

#include<math.h>

double seek_Yp(double y)//y=xt-gxxtt/2vv-gxx/2vv
{
	double x = 7.0;
	double v = 30.0;
	double g = 9.8;
	double a, b, c;
	a = x * x * g / v / v / 2 * -1;
	b = x;
	c = -1 * g * x * x / v / v / 2 - y;
	return ((-1 * b) - sqrt(b * b - 4 * a * c)) / 2*a;
}




int testdata(double z=-1.0){
    
    //1:建立与文件的流
    FILE *fp=fopen(PROJECT_DIR"/data.txt","a+"); //若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾后，即文件原先的内容会被保留
    
    //2:检测文件是否打开成功；
    if(!fp){
       printf("打开失败！\n");
       return -1; //返回异常
    }
    else if(!z){  //
        cout << "测距失败" << endl;
    }
    //将数据格式化输出到指定文件流,int fprintf( FILE *stream, const char *format, [ argument ]...) 
    //注：此函数，是将format字符串写入到指定输出流中，format包括空格字符，非空格字符，说明符之中的一个或多个。如:fprintf(fp," "); 就是将空格输入到流中。
    //可理解为进程借助流将数据打印（fprintf）到了文件中;

    //将个人信息，写入指定流中，数据间以一个空格分隔，最后还写入换行符（控制字符）。
    else{
        cout << z << endl;
        fprintf(fp,"%lf\n", z);
        
        cout << "结果已保存" << endl;
        fclose(fp);
    }
    return 0;
}

static bool sendTarget(Serial &serial, double x, double y, double z, uint16_t shoot_delay) {
    static short x_tmp, y_tmp, z_tmp;
    uint8_t buff[14];

#ifdef WITH_COUNT_FPS
    static time_t last_time = time(nullptr);
    static int fps;
    time_t t = time(nullptr);
    if (last_time != t) {
        last_time = t;
        cout << "识别到装甲板: 帧率:" << fps << ", ( Yaw轴偏移量：" << x << ", Pitch轴偏移量：" << y << ", 距离：" << z << " )" << endl;
        //testdata(z);   // 开启时收集距离数据
        fps = 0;
    }
    fps += 1;
#endif

    x_tmp = static_cast<int>(x * 1000);
    y_tmp = static_cast<int>(y * 1000);
    z_tmp = static_cast<int>(z);

    buff[0] = 's';
    // Yaw轴
    buff[1] = static_cast<char>((x_tmp >> 24) & 0xFF);
    buff[2] = static_cast<char>((x_tmp >> 16) & 0xFF);
    buff[3] = static_cast<char>((x_tmp >> 8) & 0xFF);
    buff[4] = static_cast<char>((x_tmp >> 0) & 0xFF);
    
    // Pitch轴
    buff[5] = static_cast<char>((y_tmp >> 24) & 0xFF);
    buff[6] = static_cast<char>((y_tmp >> 16) & 0xFF);
    buff[7] = static_cast<char>((y_tmp >> 8) & 0xFF);
    buff[8] = static_cast<char>((y_tmp >> 0) & 0xFF);
    
    // 距离量
    buff[9] = static_cast<char>((z_tmp >> 24) & 0xFF);
    buff[10] = static_cast<char>((z_tmp >> 16) & 0xFF);
    buff[11] = static_cast<char>((z_tmp >> 8) & 0xFF);
    buff[12] = static_cast<char>((z_tmp >> 0) & 0xFF); 
/*    buff[1] = static_cast<char>(0x12);
    buff[2] = static_cast<char>(0x23);
    buff[3] = static_cast<char>(0x34);
    buff[4] = static_cast<char>(0x45);
    buff[5] = static_cast<char>(0x56);
    buff[6] = static_cast<char>(0x67);
    buff[7] = static_cast<char>(0x78);
    buff[8] = static_cast<char>(0x89);
    */
    buff[13] = 'e';
//    if(buff[7]<<8 | buff[8])
//        cout << (buff[7]<<8 | buff[8]) << endl;
    return serial.WriteData(buff, sizeof(buff));
}

bool ArmorFinder::sendBoxPosition(uint16_t shoot_delay) {
    //cout <<target_box.rect <<endl;
    if (target_box.rect == cv::Rect2d()) return false;

    // if(enemy_color == BOX_BLUE)
    //     if(target_box.box_color != enemy_color) return false;
    // else if(target_box.box_color == enemy_color) return false;
    if (shoot_delay) {
        LOGM(STR_CTR(WORD_BLUE, "next box %dms"), shoot_delay);
    }
    auto rect = target_box.rect;
    double dx = rect.x + rect.width / 2 - IMAGE_CENTER_X;
    double dy = rect.y + rect.height / 2 - IMAGE_CENTER_Y;
    double yaw = atan(dx / FOCUS_PIXAL) * 180 / PI;
    //double yaw1 = yaw-0.3;  //消除摩擦轮不同步导致的弹道偏移
    double pitch = atan(dy / FOCUS_PIXAL) * 180 / PI;
    //double ahaha = atan(400000/DISTANCE_HEIGHT)-1.3;
    //cout << ahaha<< endl;
    pitch = pitch - atan(400000/DISTANCE_HEIGHT)-2;  // 消除相机与枪口的误差,相机在枪口线上0.04m,数值对应0.04*10000  == 40000  //1.4
    //cout<<"转换前的pitch:"<<pitch << "转换后的pitch:"<< pitch1 << endl;

	// double y=2.0;
	// y=seek_Yp(y);
	// y = atan(y)*180.0/PI;
	// printf("%f\n",y);



    double dist = DISTANCE_HEIGHT / rect.height;
    return sendTarget(serial, yaw, -pitch, dist, shoot_delay);
}
