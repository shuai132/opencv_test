#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"

#include <iostream>
#include <vector>

using namespace cv;

// 标定点
#define MID_POINT_X     245
#define MID_POINT_Y     250
// 为加速处理 只取上述点的附近图像即可(此处取点和上下10个像素的条状区域)
#define MORE_POINT      10

int main() {
    Mat srcImage = imread("test.png");
    imshow("1. srcImage", srcImage);

    // 只取标定点区域图像处理即可
    Mat roiImage = srcImage(Rect(0, MID_POINT_Y - MORE_POINT, srcImage.rows, MORE_POINT * 2 + 1));

    // 转为灰度图并进行图像平滑
    Mat grayImage;
    cvtColor(roiImage, grayImage, COLOR_BGR2GRAY);
    GaussianBlur(grayImage, grayImage, Size(9, 9), 2, 2);
    imshow("2. grayImage", grayImage);

    // 边缘检测
    Mat cannyImage;
    Canny(grayImage, cannyImage, 100, 50, 3);   // 此算法很依赖边缘检测的完整性 需要妥善选择值
    imshow("3. cannyImage", cannyImage);

    //// 认为边缘检测是封闭的图 算平均距离 计算每列中距离标定点最近的左右两个点的距离并取平均
    auto getDistanceOfRow = [](Mat image, int row) {
        int left = MID_POINT_X;
        int right = MID_POINT_X;

        auto* data = image.ptr<uchar>(row);
        for(auto i = MID_POINT_X; i >= 0; i--) {
            auto v = data[i];
            if (v > 0) {
                left = i;
                break;
            }
        }
        if (left == MID_POINT_X) goto NOT_FOUND;

        for(auto i = MID_POINT_X; i < image.cols; i++) {
            auto v = data[i];
            if (v > 0) {
                right = i;
                break;
            }
        }
        if (right == MID_POINT_X) goto NOT_FOUND;

        return right - left;

        NOT_FOUND:
        return 0;
    };

    float sum = 0;
    int num = 0;
    for (auto i = 0; i < roiImage.rows; i++) {
        auto row = i;
        auto distance = getDistanceOfRow(cannyImage, row);
        if (distance == 0) {
            printf("not found at row:%d\n", row);
            continue;
        }
        sum += distance;
        num++;
        printf("row: %03d, distance: %d\n", row, distance);
    }
    auto distance = sum / num;
    printf("avg distance: %f\n", distance);

    waitKey(0);
    return 0;
}