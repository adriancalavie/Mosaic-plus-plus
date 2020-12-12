#include <iostream>
#include "BasePictures.h"
#include "PictureTools.h"
#include "StopWatch.h"
#include "Mosaic.h"


int main()
{
	//Make mosaic 
	BasePictures test(1000);
	test.addPicturesMosaic(false);

	cv::Mat input = cv::imread("..//test.jpg", cv::IMREAD_COLOR);

	input = PictureTools::resize(input, 1200, 650);

	cv::Mat input2 = Mosaic::makeMosaic(test.GetMediumColor(), input);
	cv::imwrite("..//Resulting pictures\\1000.jpg", input2);
	cv::imshow("Original", input);
	cv::imshow("Returned", input2);
	cv::waitKey(0);

	return 0;
}