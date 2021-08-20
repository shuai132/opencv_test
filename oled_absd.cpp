#include "stdio.h"
#include "iostream"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

#define MAX_NUMBER  INT_MAX

int main() {
    VideoCapture cap;
//    cap.open("1629390633805677.mp4");
//    cap.open("1.gif");
    cap.open("a.gif");
//    cap.open(0);
    if(!cap.isOpened()) {
        cout<<"film open failed!\n";
        return 0;
    }

    system("mkdir out");

    Mat img;
    int frames = MAX_NUMBER;
    while(frames--)
    {
        cap.read(img);
        if (img.empty()) break;
        static int frame_ct = 0;
        static int picture_ct = frames+1;
        int axis_x,axis_y;
        cvtColor(img, img, COLOR_RGB2GRAY);
        resize(img,img,Size(128,64),0,0,INTER_LINEAR);
//            resize(img,img,Size(1280,640),0,0,INTER_LINEAR);
//            threshold(img, img, 0, 255,  THRESH_TRIANGLE);
//            adaptiveThreshold(img, img,255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY,251,10);
        adaptiveThreshold(img, img, 255, ADAPTIVE_THRESH_GAUSSIAN_C,THRESH_BINARY,25,10);
//            resize(img,img,Size(1280,640),0,0,INTER_LINEAR);
//            for(axis_x=0;axis_x<1280;axis_x++)
//                for(axis_y=0;axis_y<640;axis_y++)
//                    {
//                        gray = (img.at<Vec3b>(Point(axis_x,axis_y))[0] + img.at<Vec3b>(Point(axis_x,axis_y))[1] + img.at<Vec3b>(Point(axis_x,axis_y))[2])/3;
//                        gray = (gray>127?255:0);
//                        img.at<Vec3b>(Point(axis_x,axis_y))[0] = gray;
//					    img.at<Vec3b>(Point(axis_x,axis_y))[1] = gray;
//					    img.at<Vec3b>(Point(axis_x,axis_y))[2] = gray;
//                    }
        imshow("gif",img);
//            sprintf(gif,"%s%d%s","./out/",frame_ct,"_gif.bmp");
//            resize(img,img,Size(128,64),0,0,INTER_LINEAR);
//            Acquire_pot(frames,picture_ct,img);
//            imwrite(gif,img);
        waitKey(1000/cap.get(CAP_PROP_FPS));
        frame_ct++;
    }
    cap.release();
    destroyWindow("gif");
    cout<<"finish!\n";
    return 0;
}
