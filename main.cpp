#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

using namespace cv;

// 标定点
#define MID_POINT_X     245
#define MID_POINT_Y     250

int main() {
    Mat srcImage = imread("test.png");
    imshow("1. srcImage", srcImage);

    // 转为灰度图并进行图像平滑
    Mat grayImage;
    cvtColor(srcImage, grayImage, COLOR_BGR2GRAY);
    GaussianBlur(grayImage, grayImage, Size(9, 9), 2, 2);
    imshow("2. grayImage", grayImage);

    // 边缘检测
    Mat cannyImage;
    Canny(grayImage, cannyImage, 100, 100, 3);
    imshow("3. cannyImage", cannyImage);

    // 霍夫线变换
    std::vector<Vec4i> lines;
    HoughLinesP(cannyImage, lines, 1, CV_PI, 10, 10, 50);

    //// 找离参照点左右分别最近的竖线
    // x轴位置排序
    std::sort(lines.begin(), lines.end(), [](Vec4i v1, Vec4i v2) {
        return v1[0] < v2[0];
    });

    // 检测线段是否满足要求
    auto isUsefulLine = [](Vec4i l) {
        auto y1 = l[3];
        auto y2 = l[1];

        return y1 < MID_POINT_Y && y2 > MID_POINT_Y;
    };

    // 找出左右相邻
    {
        int left = 0;
        int right = 0;
        for (auto l : lines) {
            // 过滤不符合要求的线段
            if (!isUsefulLine(l)) continue;

            auto x = l[0];
            if (x < MID_POINT_X) left = x;
            if (x > MID_POINT_X) {
                right = x;
                break;
            }
        }
        if (left == 0 || right == 0) {
            printf("find error!\n");
        } else {
            printf("find: x-left: %d, x-right: %d, distance: %d\n", left, right, right - left);
        }
    }

    //// 可视化效果
    Mat outImage = srcImage.clone();
    // 绘制预设点
    circle(outImage, Point(MID_POINT_X, MID_POINT_Y), 5, Scalar(255,255,255), -1);
    // 在图中绘制出每条线段
    for (auto l : lines) {
        //printf("%s: x: %d, y: %d x2: %d, y2: %d\n", isUsefulLine(l) ? "Good:" : "Bad:", l[0], l[1], l[2], l[3]);

        Scalar greenColor = Scalar(0,255,0);
        Scalar redColor = Scalar(0,0,255);
        line(outImage, Point(l[0], l[1]), Point(l[2], l[3]), isUsefulLine(l) ? greenColor : redColor, 1, LINE_AA);
    }
    imshow("4. lineImage", outImage);

    waitKey(0);
    return 0;
}