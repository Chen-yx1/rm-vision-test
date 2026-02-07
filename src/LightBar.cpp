#include "LightBar.h"
#include <iostream>
#include <cmath>  // 添加cmath头文件用于abs函数

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
    // 1. 面积过滤 - 根据实际画面大小调整
    if (area < 100 || area > 5000) return false; 

    // 2. 长宽比过滤 - 灯条是竖长的
    float cur_width = rect.size.width;
    float cur_height = rect.size.height;
    bool is_vertical = (cur_height > cur_width); 
    if (!is_vertical) return false; // 拒绝横摆的矩形

    float aspect_ratio = cur_height / cur_width; // 长边/短边
    if (aspect_ratio < 2.0 || aspect_ratio > 8.0) return false;

    // 3. 【核心修改】放宽倾斜角度至45度
    // RotatedRect的angle范围是[-90, 0)，-90度表示完全竖直。
    // 计算与竖直方向(-90度)的夹角差。
    float angle_from_vertical = std::abs(-90.0f - angle);
    // 允许最多45度的倾斜（覆盖-90度到-45度）
    if (angle_from_vertical > 45.0f) {
        return false; 
    }

    // 4. 轮廓饱满度过滤 - 可选，对“圈”状误检有效
    cv::Rect bounding_rect = rect.boundingRect();
    float fullness = area / (bounding_rect.width * bounding_rect.height);
    if (fullness < 0.5) return false; 

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