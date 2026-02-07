#ifndef LIGHT_BAR_H
#define LIGHT_BAR_H

#include <opencv2/opencv.hpp>

class LightBar {
public:
    cv::RotatedRect rect;  // 旋转矩形
    cv::Point2f center;    // 中心点
    float width;           // 宽度
    float height;          // 高度
    float angle;           // 角度
    float area;            // 面积
    
    LightBar();
    LightBar(const cv::RotatedRect& lightRect);
    
    // 判断是否为有效灯条
    bool isValid() const;
    
    // 获取边界点
    std::vector<cv::Point2f> getPoints() const;
    
    // 绘制灯条
    void draw(cv::Mat& image, const cv::Scalar& color = cv::Scalar(0, 255, 0)) const;
};

#endif // LIGHT_BAR_H