#include <iostream>
#include <opencv2/opencv.hpp>
#include "ArmorDetector.h"
#include "LightBar.h"

using namespace std;
using namespace cv;

int main() {
    // 创建装甲板检测器
    ArmorDetector detector;
    // 修改了preprocess函数，setColor不再影响颜色分割，
    // 但为了保持类接口一致，可以保留。或者稍后可以从ArmorDetector类中移除相关代码。
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
    
    cout << "程序开始运行，自动识别红蓝双色装甲板灯条..." << endl;
    cout << "按 ESC 键可退出程序。" << endl;
    
    // 创建显示窗口
    namedWindow("原始视频", WINDOW_AUTOSIZE);
    namedWindow("二值化图像", WINDOW_AUTOSIZE);
    namedWindow("装甲板检测结果", WINDOW_AUTOSIZE);
    
    Mat frame;
    
    while (true) {
        cap >> frame;
        if (frame.empty()) {
            cout << "视频处理结束。" << endl;
            break;
        }
        
        resize(frame, frame, Size(640, 480));
        
        // --- 核心：执行检测 ---
        vector<LightBar> light_bars = detector.detect(frame);
        Mat result = frame.clone();
        Mat binary = detector.preprocess(frame);
        // --- 检测结束 ---
        
        // --- 绘制结果：此处可以根据后续需求区分颜色，目前统一用绿色 ---
        for (const auto& light_bar : light_bars) {
            light_bar.draw(result, Scalar(0, 255, 0)); // 所有灯条先用绿色框绘制
        }
        
        // 显示检测到的灯条数量
        string count_text = "Light Bars: " + to_string(light_bars.size());
        putText(result, count_text, Point(10, 30), 
                FONT_HERSHEY_SIMPLEX, 1.0, Scalar(0, 0, 255), 2);
        
        // 显示图像
        imshow("原始视频", frame);
        imshow("二值化图像", binary);
        imshow("装甲板检测结果", result);
        
        // 唯一的控制：ESC键退出。延迟时间可控制播放速度。
        int key = waitKey(30);
        if (key == 27) { // ESC
            cout << "用户中断退出。" << endl;
            break;
        }
    }
    
    cap.release();
    destroyAllWindows();
    
    cout << "程序结束！" << endl;
    return 0;
}