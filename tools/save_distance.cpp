int testdata(int maxline, double z=-1.0){  
    /*Input：maxline：设置数据足够的量, z:待收集变量
      Notes：测距实验函数。使用时请先将装甲板提前置于固定位置再开启灯，运行时自动采集距离数据并保存至data.txt文档中。当数据量达到设定值后停止收集。 */
    char flag;
    int counts;
    //1:建立与文件的流
    FILE *fp=fopen("/root/桌面/CIDP-RM-CV-2021/data.txt","a+"); // 若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾后，即文件原先的内容会被保留
    //2:检测文件是否打开成功；
    if(!fp){
       printf("打开失败！\n");
       return -1; //返回异常
    }

    while (!feof(fp)){      // 从文件流中读取数据判断是否到达EOF,注意文件流尾部最后一行没有换行符
            flag = fgetc(fp);
            if (flag=='\n') counts++;
    }

    if (counts >= maxline){ // 当检测到数据量达到足够多时
                cout << "数据已录入足够多" << endl;
                fclose(fp);  // 关闭文件流
                return -1;
            }

    if(!z){  // 此处可加入样本筛选策略
        cout << "测距失败" << endl;
    }

    // 最后还写入换行符（控制字符）
    else{
        cout << z << endl;
        fprintf(fp,"%lf\n", z); // 数据文件保存格式
        
        cout << "结果已保存" << endl;
        fclose(fp);
    }
    return 0;
    
    //将数据格式化输出到指定文件流,int fprintf( FILE *stream, const char *format, [ argument ]...) 
    //注：此函数，是将format字符串写入到指定输出流中，format包括空格字符，非空格字符，说明符之中的一个或多个。如:fprintf(fp," "); 就是将空格输入到流中。
    //可理解为进程借助流将数据打印（fprintf）到了文件中;
}
