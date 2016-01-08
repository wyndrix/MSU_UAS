// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Unmanned_Systems_Object_Recognition Version 2.0.0
// Engineer: Zachary Taylor
// Date: January 5, 2016

// Purpose: The Purpose of this code is to detect and count various shapes based off the number of vertices they contain and draw lines around them in the original image.
// Previous Changes: Dialating the threshold image to try and reduce shapes seen. Changed the Data window so it looks a bit more professional. 
// Notes: For the section relating to identifying shapes, it could be made, potentially, substantially smaller. See lines 77 and 78 for notes. Will experiment later... 
// ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/imgproc/imgproc_c.h"
#include <iostream>
#include <stdio.h>
//#include <tesseract\baseapi.h>
//#include <leptonica\allheaders.h>

using namespace cv;
using namespace std;

Mat img;
Mat img_gray;
RNG rng(12345);

void contour_finder(int, void*);

int main(int argc, char *argv[])
{
	img = imread("C:/temp/realpic2.jpg", CV_LOAD_IMAGE_UNCHANGED);
	contour_finder(0, 0);
	waitKey();
}

void contour_finder(int, void*)
{
	// Finding the contours of different shapes
	int triangle = 0, square = 0, rectangle = 0, pentagon = 0, hexagon = 0, star = 0, plus_sign = 0;
	double distance[4];
	double distavg;
	int j = 0;
	Mat img_edge;
	Mat img_final;
	Mat img_blur;
	Scalar color(255, 0, 0);
	cvtColor(img, img_gray, CV_RGB2GRAY);
	imwrite("C:/temp/Gray.png", img_gray);
	blur(img_gray, img_blur, Size(3, 3));
	//Canny(img_blur, img_edge, 200, 250, 5, true);
	threshold(img_blur, img_final, 125, 255, 1);
	//dilate(img_edge, img_final, Mat(), Point(-1, -1), 3, 1, 1);
	//erode(img_edge, img_final, Mat(), Point(-1, -1), 3);
	imshow("Edge Image", img_edge);
	imshow("Gray Blur", img_gray);
	imshow("Final Image", img_final);

	vector<vector<Point> > contours;
	vector<Point> result;

	int rows = img_final.rows;
	int cols = img_final.cols;
	int size = rows*cols;

	// Filter out noise based on area of pixels it contains
	findContours(img_final, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	for (size_t i = 0; i < contours.size(); i++){
		approxPolyDP(Mat(contours[i]), result, arcLength(Mat(contours[i]), true)*0.02, true);

		if ((contourArea(result) > 300) && (contourArea(result) < size))
		{
			// Finding Triangles
			if (result.size() == 3)
			{
				triangle = triangle + 1;
				Point pt[3];

				for (j = 0; j < 3; j++)
				{
					pt[j] = result[j];
				}

				line(img, pt[0], pt[1], color, 2, 8, 0);
				line(img, pt[1], pt[2], color, 2, 8, 0);
				line(img, pt[2], pt[0], color, 2, 8, 0);

			}

			// Finding Quadrilaterals
			else if (result.size() == 4)
			{
				Point pt[4];

				for (j = 0; j < 4; j++)
				{
					pt[j] = result[j];
				}

				line(img, pt[0], pt[1], color, 2, 8, 0);
				line(img, pt[1], pt[2], color, 2, 8, 0);
				line(img, pt[2], pt[3], color, 2, 8, 0);
				line(img, pt[3], pt[0], color, 2, 8, 0);


				distance[0] = norm(pt[0] - pt[1]);
				distance[1] = norm(pt[1] - pt[2]);
				distance[2] = norm(pt[2] - pt[3]);
				distance[3] = norm(pt[3] - pt[0]);

				distavg = ((distance[0] + distance[1] + distance[2] + distance[3]) / 4);

				if ((distance[0] < (distavg*1.05)) && (distance[0] > (distavg*0.95)))
				{
					square++;
				}

				else
				{
					rectangle++;
				}

			}

			// Finding Pentagons
			else if (result.size() == 5)
			{
				pentagon = pentagon + 1;
				Point pt[5];

				for (j = 0; j < 5; j++)
				{
					pt[j] = result[j];
				}

				line(img, pt[0], pt[1], color, 2, 8, 0);
				line(img, pt[1], pt[2], color, 2, 8, 0);
				line(img, pt[2], pt[3], color, 2, 8, 0);
				line(img, pt[3], pt[4], color, 2, 8, 0);
				line(img, pt[4], pt[0], color, 2, 8, 0);

			}

			// Finding Hexagons
			else if (result.size() == 6)
			{
				hexagon = hexagon + 1;
				Point pt[6];

				for (j = 0; j < 6; j++)
				{
					pt[j] = result[j];
				}

				line(img, pt[0], pt[1], color, 2, 8, 0);
				line(img, pt[1], pt[2], color, 2, 8, 0);
				line(img, pt[2], pt[3], color, 2, 8, 0);
				line(img, pt[3], pt[4], color, 2, 8, 0);
				line(img, pt[4], pt[5], color, 2, 8, 0);
				line(img, pt[5], pt[0], color, 2, 8, 0);

			}

			// Finding Stars
			else if (result.size() == 10)
			{
				star = star + 1;
				Point pt[10];

				for (j = 0; j < 6; j++)
				{
					pt[j] = result[j];
				}

				line(img, pt[0], pt[1], color, 2, 8, 0);
				line(img, pt[1], pt[2], color, 2, 8, 0);
				line(img, pt[2], pt[3], color, 2, 8, 0);
				line(img, pt[3], pt[4], color, 2, 8, 0);
				line(img, pt[4], pt[5], color, 2, 8, 0);
				line(img, pt[5], pt[6], color, 2, 8, 0);
				line(img, pt[6], pt[7], color, 2, 8, 0);
				line(img, pt[7], pt[8], color, 2, 8, 0);
				line(img, pt[8], pt[9], color, 2, 8, 0);
				line(img, pt[9], pt[0], color, 2, 8, 0);
			}

			// Finding Plus-Symbols
			else if (result.size() == 12)
			{
				plus_sign = plus_sign + 1;
				Point pt[10];

				for (j = 0; j < 6; j++)
				{
					pt[j] = result[j];
				}

				line(img, pt[0], pt[1], color, 2, 8, 0);
				line(img, pt[1], pt[2], color, 2, 8, 0);
				line(img, pt[2], pt[3], color, 2, 8, 0);
				line(img, pt[3], pt[4], color, 2, 8, 0);
				line(img, pt[4], pt[5], color, 2, 8, 0);
				line(img, pt[5], pt[6], color, 2, 8, 0);
				line(img, pt[6], pt[7], color, 2, 8, 0);
				line(img, pt[7], pt[8], color, 2, 8, 0);
				line(img, pt[8], pt[9], color, 2, 8, 0);
				line(img, pt[9], pt[10], color, 2, 8, 0);
				line(img, pt[10], pt[11], color, 2, 8, 0);
				line(img, pt[11], pt[0], color, 2, 8, 0);

			}
		}
	}

	cout << "-------------------------------------------------------------------------------" << endl;
	cout << "Unmanned Systems Image Recognition Program Version 2.0.0" << endl;
	cout << "-------------------------------------------------------------------------------" << endl << endl;
	cout << "Number of Objects Located:" << endl << endl << "Triangles: " << triangle << endl << "Squares: " << square << endl << "Rectangles: " << rectangle << endl << "Pentagons: " << pentagon << endl << "Hexagons: " << hexagon << endl << "Stars: " << star << endl << "Plus-Signs: " << plus_sign << endl << endl;
	cout << distance[0] << endl << distance[1] << endl << distance[2] << endl << distance[3] << endl << distavg << endl;
	imshow("Original Image", img);
	system("tesseract.exe C:/temp/Gray.png out");

}