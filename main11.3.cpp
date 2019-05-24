#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"

#include <iostream>
#include <cvaux.h>

using namespace cv;

int main() {
    Mat srcImage1 = imread("1.png", 1);
    Mat srcImage2 = imread("3.png", 1);

    if (!srcImage1.data || !srcImage2.data) {
        printf("img error!");
        return -1;
    }

    int minHessian = 1000;
    SURF detector(minHessian);
    std::vector<KeyPoint> kp1, kp2;

    detector.detect(srcImage1, kp1);
    detector.detect(srcImage2, kp2);

    SURF extractor;
    Mat descriptors1, descriptors2;
    extractor.compute(srcImage1, kp1, descriptors1);
    extractor.compute(srcImage2, kp2, descriptors2);

    FlannBasedMatcher matcher;
    std::vector<DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);

    double maxDist = 0, minDist = 100;
    for (int i = 0; i < descriptors1.rows; i++) {
        auto dist = matches[i].distance;
        if (dist < minDist) minDist = dist;
        if (dist > maxDist) maxDist = dist;
    }
    printf("> max dist: %f\n", maxDist);
    printf("< min dist: %f\n", minDist);

    // 6
    std::vector<DMatch> goodMatches;
    for (auto i = 0; i < descriptors1.rows; i++) {
        if (matches[i].distance <= 5*minDist) {
            goodMatches.push_back(matches[i]);
        }
    }

    // 7
    Mat imgMatches;
    drawMatches(srcImage1, kp1, srcImage2, kp2, goodMatches, imgMatches,
            Scalar::all(-1), Scalar::all(-1),
            vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    // 8
    for (auto m : goodMatches) {
        printf("good match point: %d [%d] [%d]\n", m.queryIdx, m.imgIdx, m.trainIdx);
    }

    imshow("output", imgMatches);

    waitKey(0);
    return 0;
}