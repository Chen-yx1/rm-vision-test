#include "LightBar.h"
#include <iostream>
#include <cmath>

LightBar::LightBar() : width(0), height(0), angle(0), area(0) {}

LightBar::LightBar(const cv::RotatedRect& lightRect) 
    : rect(lightRect) {
    center = rect.center;
    width = rect.size.width;
    height = rect.size.height;
    angle = rect.angle;
    area = width * height;
}

bool LightBar::isValid() const {
    // 1. 面积过滤 - 去除太小的噪点
    if (area < 50) return false;
    
    // 2. 长宽比过滤 - 灯条通常较长
    float aspect_ratio;
    if (height > width) {
        aspect_ratio = height / width;
    } else {
        aspect_ratio = width / height;
    }
    
    // 灯条的长宽比通常在2:1到8:1之间
    if (aspect_ratio < 1.5 || aspect_ratio > 8.0) return false;
    
    // 3. 角度过滤 - 灯条通常接近垂直
    float abs_angle = std::abs(angle);
    
    // 允许一定的倾斜角度（±60度以内）
    if (abs_angle > 60) return false;
    
    return true;
}

std::vector<cv::Point2f> LightBar::getPoints() const {
    cv::Point2f pts[4];
    rect.points(pts);
    return std::vector<cv::Point2f>(pts, pts + 4);
}

void LightBar::draw(cv::Mat& image, const cv::Scalar& color) const {
    std::vector<cv::Point2f> points = getPoints();
    
    // 绘制旋转矩形的四条边
    for (int i = 0; i < 4; i++) {
        cv::line(image, points[i], points[(i+1)%4], color, 2);
    }
    
    // 绘制中心点
    cv::circle(image, center, 3, cv::Scalar(0, 0, 255), -1);
    
    // 显示角度信息
    std::string angle_text = "A:" + std::to_string(int(angle));
    cv::putText(image, angle_text, center + cv::Point2f(10, 0), 
                cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
}
