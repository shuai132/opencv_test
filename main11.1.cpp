#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"

#include <iostream>

using namespace cv;

int main() {
    Mat srcImage1 = imread("1.png", 1);
    Mat srcImage2 = imread("2.png", 1);

    if (!srcImage1.data || !srcImage2.data) {
        printf("img error!");
        return -1;
    }

    imshow("原始图1", srcImage1);
    imshow("原始图2", srcImage2);

    int minHessian = 1000;
    SurfFeatureDetector detector(minHessian);
    std::vector<KeyPoint> kp1, kp2;

    detector.detect(srcImage1, kp1);
    detector.detect(srcImage2, kp2);

    Mat imgKp1, imgKp2;
    drawKeypoints(srcImage1, kp1, imgKp1, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    drawKeypoints(srcImage2, kp2, imgKp2, Scalar::all(-1), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

    imshow("特征效果1:", imgKp1);
    imshow("特征效果2:", imgKp2);

    waitKey(0);
    return 0;
}