#include <iostream>
#include <opencv2/opencv.hpp>
#include "ArmorDetector.h"
#include "LightBar.h"

using namespace std;
using namespace cv;

int main() {
    // 创建装甲板检测器
    ArmorDetector detector;
    detector.setColor(ArmorDetector::Color::RED);
    
    // 创建视频捕获对象
    VideoCapture cap;
    
    // 尝试打开测试视频
    cap.open("test_video.mp4");
    
    // 如果视频打不开，尝试使用摄像头
    if (!cap.isOpened()) {
        cout << "无法打开视频文件，尝试使用摄像头..." << endl;
        cap.open(0);
    }
    
    if (!cap.isOpened()) {
        cerr << "错误：无法打开任何视频源！" << endl;
        return -1;
    }
    
    cout << "视频源打开成功！" << endl;
    cout << "按ESC键退出程序" << endl;
    
    // 创建显示窗口
    namedWindow("原始视频", WINDOW_AUTOSIZE);
    namedWindow("二值化图像", WINDOW_AUTOSIZE);
    namedWindow("装甲板检测结果", WINDOW_AUTOSIZE);
    
    while (true) {
        Mat frame;
        cap >> frame;
        
        if (frame.empty()) {
            cout << "视频播放结束！" << endl;
            break;
        }
        
        // 缩放图像以加快处理速度
        resize(frame, frame, Size(640, 480));
        
        // 装甲板检测
        vector<LightBar> light_bars = detector.detect(frame);
        
        // 创建结果图像
        Mat result = frame.clone();
        Mat binary = detector.preprocess(frame);
        
        // 绘制检测到的灯条
        for (const auto& light_bar : light_bars) {
            light_bar.draw(result, Scalar(0, 255, 0));
        }
        
        // 显示检测到的灯条数量
        string count_text = "Light Bars: " + to_string(light_bars.size());
        putText(result, count_text, Point(10, 30), 
                FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 255), 2);
        
        // 显示图像
        imshow("原始视频", frame);
        imshow("二值化图像", binary);
        imshow("装甲板检测结果", result);
        
        // 按键处理
        int key = waitKey(30);
        if (key == 27) break;  // ESC退出
    }
    
    cap.release();
    destroyAllWindows();
    
    cout << "程序结束！" << endl;
    return 0;
}
