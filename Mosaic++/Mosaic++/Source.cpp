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

	input = PictureTools::resize(input, 1200, 700);
	cv::Mat input3 = Mosaic::makeMosaic(input, test, Type::square, 10);
	cv::imwrite("..//Resulting pictures\\Original" + std::to_string(test.getNumberPictures()) + test.getExtension(), input3);
	cv::imshow("Original", input3);
	cv::waitKey(0);

	return 0;
}