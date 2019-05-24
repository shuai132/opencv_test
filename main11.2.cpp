#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"

#include <iostream>
#include <cvaux.h>

using namespace cv;

int main() {
    Mat srcImage1 = imread("1.png", 1);
    Mat srcImage2 = imread("2.png", 1);

    if (!srcImage1.data || !srcImage2.data) {
        printf("img error!");
        return -1;
    }

//    imshow("in1", srcImage1);
//    imshow("in2", srcImage2);

    int minHessian = 1000;
    SurfFeatureDetector detector(minHessian);
    std::vector<KeyPoint> kp1, kp2;

    detector.detect(srcImage1, kp1);
    detector.detect(srcImage2, kp2);

    SurfDescriptorExtractor extractor;
    Mat descriptors1, descriptors2;
    extractor.compute(srcImage1, kp1, descriptors1);
    extractor.compute(srcImage2, kp2, descriptors2);

    BruteForceMatcher<L2<float>> matcher;
    std::vector<DMatch> matches;
    matcher.match(descriptors1, descriptors2, matches);

    Mat imgMatches;
    drawMatches(srcImage1, kp1, srcImage2, kp2, matches, imgMatches);

    imshow("output", imgMatches);

    waitKey(0);
    return 0;
}