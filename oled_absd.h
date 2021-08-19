/*作者::XNY
**引用::Opencv3.49
**平台::x86_64
**系统::Linux/Windows10
**版本::2.5
**时间::2020.2.12
*/
#ifndef __OLED_ABSD_H
#define __OLED_ABSD_H

/* header files */

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "iostream"
#include "math.h"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"
//#include "opencv2/world.hpp"
#include "unistd.h"

using namespace std;
using namespace cv;

/* buf cache */
char gif[100] = {0};
char avi[100] = "./CV_OUT/1.avi";
/* functions */
int GRAY_DEAL(void);
void Acquire_pot(int img_num,int ct,Mat img);
void Pot_buf_deal(void);
void print_file(void);


/* define */
#define Filliter(A) (A>127)?1:0
#define Numbers_of_img  9980//处理图片张数，自己修改,要小于最大帧数。例如：示例中的gif读取到的帧数为88，那么这个值设置不能超过88！
uchar Point_buf[Numbers_of_img][128*64]={0};
int hex_dig[Numbers_of_img][1024]={0};
unsigned char gray;
/*
int CAP_WIDTH;
int CAP_HEIGHT;
int CAP_FPS;
*/

int GRAY_DEAL(void)
{
    VideoCapture cap;
//    cap.open("1629390633805677.mp4");
//    cap.open("1.gif");
//    cap.open("a.gif");
    cap.open(0);
    if(!cap.isOpened())
    {
        cout<<"film open failed!\n";
        getchar();
        return 0;
    }

    Mat img;
    int frames = Numbers_of_img;
    while(frames--)
    {
        cap.read(img);
        if(!img.empty())
        {
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
            waitKey((1/cap.get(CAP_PROP_FPS))*1000);
            frame_ct++;
        }
    }
    cap.release();
    destroyWindow("gif");
    return 1;
}
/* 取点 */
void Acquire_pot(int img_num,int ct,Mat img)
{
	int xx,yy,counter = 0;
	for(yy=0;yy<128;yy++)	
		for(xx=0;xx<64;xx++)	
		{
			Point_buf[ct-img_num-1][counter] = Filliter( (int) (img.at<uchar>(xx,yy) ) );
			counter++;
		}
}

void Pot_buf_deal(void)
{
	int xx;
	int ct = Numbers_of_img;
	while(ct)
	{
		for(xx=0;xx<128*64;xx++)
		{
			hex_dig[Numbers_of_img-ct][(xx)/8] += ((Point_buf[Numbers_of_img-ct][xx])*( (int)pow( (double)2.0,7 - (xx%8) ) ) );
			if( ( (xx+1)%8 )== 0)
				hex_dig[Numbers_of_img-ct][(xx)/8] = 255 - hex_dig[Numbers_of_img-ct][(xx)/8];//阴码
		}
		ct--;
	}
}

void print_file(void)
{
	FILE *ptr_to_file;
	char pot_txt[Numbers_of_img][1024][10] = {0};
	int ii,jj;
	for(ii=0;ii</*15*/Numbers_of_img;ii++)     //批量输出，格式定义
	{
		for(jj=0;jj<1024;jj++)
		{
		   if(jj == 1023)
				sprintf(pot_txt[ii][jj],"%#x%s",hex_dig[ii][jj],"},\n{");
		   else if( ((jj+1)%16) == 0)
				{
					sprintf(pot_txt[ii][jj],"%#x%s",hex_dig[ii][jj],",\n");
				}
			else
				sprintf(pot_txt[ii][jj],"%c%#x%c%c",' ',hex_dig[ii][jj],',',' ');
		}
	}
	ptr_to_file = fopen("./CV_OUT/pot.txt","w+");
	fwrite(pot_txt,sizeof(char),Numbers_of_img*10*1024,ptr_to_file);
	fclose(ptr_to_file);
	cout<<"write text successful!\n";
}

#endif