#include "ArmorDetector.h"
#include <iostream>

ArmorDetector::ArmorDetector() : target_color(Color::RED) {
    // 创建形态学操作核（3x3矩形）
    kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
}

void ArmorDetector::setColor(Color color) {
    target_color = color;
}

cv::Mat ArmorDetector::preprocess(const cv::Mat& frame) {
    cv::Mat hsv, color_mask, binary;
    
    // 转换为HSV颜色空间
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
    
    // 获取颜色掩膜
    getColorMask(hsv, color_mask);
    
    // 形态学操作：开运算去除小噪点，闭运算连接小区域
    cv::Mat processed;
    cv::morphologyEx(color_mask, processed, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(processed, binary, cv::MORPH_CLOSE, kernel);
    
    return binary;
}

void ArmorDetector::getColorMask(const cv::Mat& hsv, cv::Mat& mask) {
    if (target_color == Color::RED) {
        cv::Mat mask1, mask2;
        cv::inRange(hsv, red_lower1, red_upper1, mask1);
        cv::inRange(hsv, red_lower2, red_upper2, mask2);
        mask = mask1 | mask2;
    } else if (target_color == Color::BLUE) {
        cv::inRange(hsv, blue_lower, blue_upper, mask);
    } else {
        mask = cv::Mat::zeros(hsv.size(), CV_8UC1);
    }
}

std::vector<LightBar> ArmorDetector::findLightBars(const cv::Mat& binary) {
    std::vector<LightBar> light_bars;
    
    // 查找轮廓
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binary.clone(), contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    // 筛选轮廓
    for (const auto& contour : contours) {
        // 轮廓面积
        double area = cv::contourArea(contour);
        if (area < 50) continue;
        
        // 获取最小外接旋转矩形
        cv::RotatedRect min_rect = cv::minAreaRect(contour);
        
        // 创建LightBar对象
        LightBar light_bar(min_rect);
        
        // 有效性检查
        if (light_bar.isValid()) {
            light_bars.push_back(light_bar);
        }
    }
    
    return light_bars;
}

std::vector<LightBar> ArmorDetector::detect(const cv::Mat& frame) {
    // 预处理（颜色分割）
    cv::Mat binary = preprocess(frame);
    
    // 查找灯条
    std::vector<LightBar> light_bars = findLightBars(binary);
    
    return light_bars;
}