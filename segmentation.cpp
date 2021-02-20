////********************************************************************************************////
////             Complete image processing chain                  				   	            ////
////                                                                                            ////
////																							////
////             Author: COULIBALY Adama									                    ////
////                                           													////
////																							////
////             Compilation: make      												 		////
////						  																 	////
////																							////
////			 Description: This program makes it possible to apply all the treatments	    ////
////                               of a complete image processing chain.        	            ////
////													 										////
////	Sources: http://docs.opencv.org/doc/tutorials/											////
////********************************************************************************************////

// Importing the necessary libraries.

#include <stdio.h>
#include <stdlib.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {

	//----- Beginning of the Segmentation phase -----

	// Reading the input color image.
	Mat image0 = imread(argv[1]);
	if (!image0.data) {
		cout << "Could not load image.\n" << endl;
		return -1;
	}

	Mat image1;
	// Convert color image to grayscale image.
	cvtColor(image0, image1, COLOR_RGB2GRAY);

	// Application of the OTSU algorithm.
	threshold(image1, image1, 0, 255, CV_THRESH_BINARY_INV | CV_THRESH_OTSU);

	//----- End of the Segmentation phase -----

	//----- Beginning of the post-segmentation phase -----

	// Definition of structuring elements
	Mat element = getStructuringElement(MORPH_RECT, Size(13, 13));
	Mat element1 = getStructuringElement(MORPH_RECT, Size(7, 7));
	Mat image2;

	// dilation and erosion
	dilate(image1, image2, element);
	erode(image2, image2, element1);

	// Declaration of a vector to contain the contours of the detected regions
	vector<vector<Point> > contours;

	// Search for outlines of detected regions.
	findContours(image2, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	// Labeling of regions
	Mat markers = Mat::zeros(image2.size(), CV_32SC1);
	for (size_t i = 0; i < contours.size(); i++)
		drawContours(markers, contours, static_cast<int>(i),
				Scalar::all(static_cast<int>(i) + 1), -1);

	// Random color generation
	vector<Vec3b> colors;
	for (size_t i = 0; i < contours.size(); i++) {
		int blue = theRNG().uniform(0, 255);
		int green = theRNG().uniform(0, 255);
		int red = theRNG().uniform(0, 255);
		colors.push_back(Vec3b((uchar) blue, (uchar) green, (uchar) red));
	}

	// Coloring the regions with different colors.
	Mat image3 = Mat::zeros(markers.size(), CV_8UC3);
	for (int i = 0; i < markers.rows; i++) {
		for (int j = 0; j < markers.cols; j++) {
			int index = markers.at<int>(i, j);
			if (index > 0 && index <= static_cast<int>(contours.size()))
				image3.at<Vec3b>(i, j) = colors[index - 1];
			else
				image3.at<Vec3b>(i, j) = Vec3b(0, 0, 0);
		}
	}

	// And logic between the input image and the mask obtained after post-segmentation
	Mat image4;
	image0.copyTo(image4, image3);

	//----- END of the post-segmentation phase -----

	// Presentation of the results
	imshow("input_image", image0);
	imshow("otsu_image_segmentation", image1);
	if (!imwrite("otsu_image_segmentation.png", image1))
		cout << "Error while saving" << endl;
	imshow("post-segmentation_image", image3);
	if (!imwrite("post-segmentation_image.png", image3))
		cout << "Error while saving" << endl;
	imshow("segmented_image", image4);
	if (!imwrite("segmented_image.png", image4))
		cout << "Error while saving" << endl;

	waitKey(0);
	return 0;
}

