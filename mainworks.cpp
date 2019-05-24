#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

using namespace cv;

// 图片的宽高
#define IMG_WIDTH       700
#define IMG_HEIGHT      481
// 两个边缘之间的任意一点
#define MID_POINT_X     250
#define MID_POINT_Y     250
//// 为加速处理 只取上述点的附近图像即可(此处取点和上下10个像素的条状区域)
//#define MORE_POINT      10

int main() {
    Mat srcImage = imread("test2.png");
    // 黑色区域的一个子集
    Mat roiImage = srcImage;//srcImage(Rect(0, MID_POINT_Y - MORE_POINT, IMG_WIDTH, MORE_POINT * 2 + 1));

    imshow("1. srcImage", srcImage);
    Mat midImage;

    // 转为灰度图并进行图像平滑
    cvtColor(roiImage, midImage, COLOR_BGR2GRAY);
    GaussianBlur(roiImage, roiImage, Size(9, 9), 2, 2);
    imshow("2. roiImage", roiImage);

    // 边缘检测
    Canny(roiImage, midImage, 100, 100, 3);
    // 霍夫线变换
    std::vector<Vec4i> lines;
    HoughLinesP(midImage, lines, 1, CV_PI, 10, 10, 50);

    //// 找离参照点左右分别最近的竖线
    // 1. x轴位置排序
    std::sort(lines.begin(), lines.end(), [](Vec4i v1, Vec4i v2) {
        return v1[0] < v2[0];
    });

    // 检测线段是否满足要求
    auto isUsefulLine = [](Vec4i l) {
        auto y1 = l[3];
        auto y2 = l[1];

        return y1 < MID_POINT_Y && y2 > MID_POINT_Y;
    };
    // 2. 找出左右相邻
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


    // 在图中绘制出每条线段
    for (auto l : lines) {
        printf("%s: x: %d, y: %d x2: %d, y2: %d\n", isUsefulLine(l) ? "Good:" : "Bad:", l[0], l[1], l[2], l[3]);

        Scalar greenColor = Scalar(0,255,0);
        Scalar redColor = Scalar(0,0,255);
        line(roiImage, Point(l[0], l[1]), Point(l[2], l[3]), isUsefulLine(l) ? greenColor : redColor, 1, LINE_AA);
        //此句代码的OpenCV2版为：
        //line( dstImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(186,88,255), 1, CV_AA);
    }
    imshow("3. midImage", midImage);
    imshow("4. lineImage", roiImage);


    waitKey(0);
    return 0;
}