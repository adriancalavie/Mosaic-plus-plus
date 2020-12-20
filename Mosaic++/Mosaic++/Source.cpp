#include <iostream>
#include "BasePictures.h"
#include "PictureTools.h"
#include "StopWatch.h"
#include "Mosaic.h"

int main()
{
	//Make mosaic;

	BasePictures test(5990);
	test.addPicturesMosaic(false);

	cv::Mat input = cv::imread("..//test.jpg", cv::IMREAD_COLOR);

	cv::Mat input3 = Mosaic::makeMosaic(input, test, Type::diamond, 5, false);
	cv::imwrite("..//Resulting pictures\\Original" + std::to_string(test.getNumberPictures()) + "1" + test.getExtension(), input3);
	cv::imshow("Original", input3);
	cv::waitKey(0);

	return 0;
}

