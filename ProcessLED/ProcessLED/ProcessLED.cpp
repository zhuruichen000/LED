// ProcessLED.cpp : 定义控制台应用程序的入口点。
//
#include"stdafx.h"
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/types_c.h> 
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/imgproc/imgproc_c.h>
#include <iostream>
using namespace cv;
using namespace std;

void findcontours()
{
	vector<vector<Point> > g_vContours;
	vector<Vec4i> g_vHierarchy;
	Mat srcimg = imread("D:\\a2.jpg");
	//Rect roi1(424, 315, 2000, 1400);
	//srcimg = srcimg(roi1);
	Mat grayImage;
	cvtColor(srcimg, grayImage, COLOR_BGR2GRAY);
	Mat BlurImage;
	blur(grayImage, BlurImage, Size(60,60));
	Mat ThresholdImage;

	threshold(BlurImage, ThresholdImage, 30, 200, CV_THRESH_BINARY_INV);
	//Canny(srcimg, srcimg, 100, 255, 3);
	findContours(ThresholdImage, g_vContours, g_vHierarchy, RETR_TREE, CHAIN_APPROX_NONE);
	double maxVal = 0;
	int contourIdx;
	int maxValIdx;
	
	for (contourIdx = 0; contourIdx < g_vContours.size(); contourIdx++)
	{
		double contourspace = contourArea(g_vContours[contourIdx]);
		if (maxVal < contourspace)
		{
			maxVal = contourspace;
			maxValIdx= contourIdx;
		}
	}

	Rect position = boundingRect(g_vContours[maxValIdx+1]);
	Rect roiArea = Rect(position.tl().x, position.tl().y, position.width, position.height);
	Mat srcROI = Mat(srcimg, roiArea);
	imshow("截取后图像", srcROI);
	Mat grayImage1;
	cvtColor(srcROI, grayImage1, COLOR_BGR2GRAY);
	Mat binary_image;
	adaptiveThreshold(grayImage1, binary_image, 255, CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY_INV, 25, 10);
	imshow("自适应二值", binary_image);
	Mat de_noise = binary_image.clone(); 
	imshow("去噪", de_noise);
	medianBlur(binary_image, de_noise, 5); //中值滤波
	imshow("中值", de_noise);

	
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(de_noise, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);//CV_RETR_EXTERNAL只检测外部轮廓，可根据自身需求进行调整

	Mat contoursImage(de_noise.rows, de_noise.cols, CV_8U, Scalar(255));
	int index = 0;
	double resx[20], resy[20];
	int count = 0;
	int space=0;
	for (; index >= 0; index = hierarchy[index][0]) {
		cv::Scalar color(rand() & 255, rand() & 255, rand() & 255);
		// for opencv 2
		// cv::drawContours(dstImage, contours, index, color,  CV_FILLED, 8, hierarchy);//CV_FILLED所在位置表示轮廓线条粗细度，如果为负值（如thickness==cv_filled），绘制在轮廓内部
		// for opencv 3
		//cv::drawContours(contoursImage, contours, index, color, cv::FILLED, 8, hierarchy);

		cv::drawContours(contoursImage, contours, index, Scalar(0), 1, 8, hierarchy);//描绘字符的外轮廓

		Rect rect = boundingRect(contours[index]);//检测外轮廓
		if (arcLength(contours[index], false) > 50)
		{
			rectangle(contoursImage, rect, Scalar(0, 0, 255), 3);//对外轮廓加矩形框
			resx[space] = rect.x + rect.width / 2;
			resy[space] = rect.y + rect.height / 2;
			printf("%d", space);
			space++;
		}
	  }

      imshow( "ttt",contoursImage);

	


	/*findContours(srcROI,contour, g_vHierarchy,RETR_LIST, CHAIN_APPROX_NONE);
	double x0[10] = {0};
	double y0[10] = {0};
	for (int k = 1; k < contour.size(); k++)
	{
		if (contour > 1000)
		{
			RotatedRect[]box;
			box = new RotatedRect[contour.size()];
			box[k] = minAreaRect();
			x0[k] = box[k].center.x;
			y0[k] = box[k].center.y;
	}
	}
	double resultX = average(x0);
	double resultY = average(y0);
	double width;
	if (resultX > resultY)
	{
		width = resultX;
	}
	else
		width = resultY;*/



}

void main()
{
	findcontours();

	waitKey(0);
	system("pause");

}





