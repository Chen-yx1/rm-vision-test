#ifndef ARMOR_DETECTOR_H
#define ARMOR_DETECTOR_H

#include <opencv2/opencv.hpp>
#include <vector>
#include "LightBar.h"

class ArmorDetector {
public:
    // 装甲板颜色
    enum class Color { RED, BLUE, UNKNOWN };
    
    ArmorDetector();
    
    // 设置检测颜色
    void setColor(Color color);
    
    // 处理单帧图像，返回检测到的灯条
    std::vector<LightBar> detect(const cv::Mat& frame);
    
    // 预处理：颜色分割
    cv::Mat preprocess(const cv::Mat& frame);
    
    // 查找轮廓并筛选灯条
    std::vector<LightBar> findLightBars(const cv::Mat& binary);
    
    // 获取颜色掩膜
    void getColorMask(const cv::Mat& hsv, cv::Mat& mask);
    
private:
    Color target_color;
    
    // 红色HSV范围（两个区间）
    cv::Scalar red_lower1 = cv::Scalar(0, 100, 100);
    cv::Scalar red_upper1 = cv::Scalar(10, 255, 255);
    cv::Scalar red_lower2 = cv::Scalar(160, 100, 100);
    cv::Scalar red_upper2 = cv::Scalar(180, 255, 255);
    
    // 蓝色HSV范围
    cv::Scalar blue_lower = cv::Scalar(100, 100, 100);
    cv::Scalar blue_upper = cv::Scalar(130, 255, 255);
    
    // 形态学操作核
    cv::Mat kernel;
};

#endif // ARMOR_DETECTOR_H