#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/objdetect.hpp>
#include<iostream>
#include<math.h>
using namespace std;
using namespace cv;
Mat img;
Mat canvas;
vector<vector<int>> newPoints;
vector<vector<int>> myColors{ {111,68,95,151,128,252},//purple
							   {11,111,168,55,229,255} }; //ORANGE
							   
vector<Scalar> myColorValues{ {238,130,238},//purple
								{0,69,255} };//ORANGE
	                              

Point getContours(Mat imgDil) {
	vector<vector<Point>> contour;
	vector<Vec4i> hierarchy;
	findContours(imgDil, contour, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(img,contours,-1,Scalar(255,0,255,2));
	vector<vector<Point>> conPoly(contour.size());
	vector<Rect> boundRect(contour.size());
	Point myPoint(0, 0);
	for (int i = 0; i < contour.size(); i++)
	{
		int area = contourArea(contour[i]);
		cout << area << endl;
		if (area > 1000)
		{
			float peri = arcLength(contour[i], true);
			approxPolyDP(contour[i], conPoly[i], 0.02 * peri, true);
			cout << conPoly[i].size() << endl;
			boundRect[i] = boundingRect(conPoly[i]);
			myPoint.x = boundRect[i].x + boundRect[i].width/2;
			myPoint.y = boundRect[i].y;
			drawContours(img,contour,i,Scalar(0, 255, 140),2);
		   
		}
	}
	return myPoint;
}

vector<vector<int>> findcolor(Mat img)
{
	Mat imgHSV, mask;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);
	for (int i = 0; i < myColors.size(); i++) {
		Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);
		Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);
		inRange(imgHSV, lower, upper, mask);
		Point myPoint = getContours(mask);
		if (myPoint.x != 0 && myPoint.y != 0) {
			newPoints.push_back({ myPoint.x, myPoint.y, i
				});
		}


	}
	return newPoints;
}
void draw_Canvas(vector<vector<int>> newPoints, vector<Scalar> myColorValues)
{
	double d;
	for (int i = 0; i < newPoints.size(); i++)
	{
		circle(canvas, Point(newPoints[i][0], newPoints[i][1]), 3, myColorValues[newPoints[i][2]], FILLED);
		if (i > 0)
		{
			Point p = Point(newPoints[i - 1][0], newPoints[i - 1][1]);
			Point q = Point(newPoints[i][0], newPoints[i][1]);
			d = sqrt(pow((p.x-q.x), 2) + pow((p.y-q.y), 2));
			if (d < 70)
			{
				line(canvas, p, q, Scalar(myColorValues[newPoints[i][2]]),5,8);
			}
		}
	}
}
void main()
{
	string path = "resources/CANVAS.png";
	canvas = imread(path);
	VideoCapture cap(0);
	while (true)
	{
		cap.read(img);
		flip(img, img, 1);
		newPoints=findcolor(img);
		draw_Canvas(newPoints, myColorValues);
		imshow("Image", img);
		imshow("Canvas", canvas);
		waitKey(4);

	}
	
}