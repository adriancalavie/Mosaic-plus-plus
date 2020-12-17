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
	//Mosaic::alphaBlending(input, cv::Scalar(178, 0, 194));
	//cv::Mat input2 = Mosaic::makeMosaic(test.GetMediumColor(), input, 10, 1 , true);
	cv::Mat input3 = Mosaic::makeMosaic(test.GetMediumColor(), input, 10, 1 , false);
	cv::imwrite("..//Resulting pictures\\Original" + std::to_string(test.getNumberPictures()) + test.getExtension(), input3);
	//cv::imwrite("..//Resulting pictures\\Blend" + std::to_string(test.getNumberPictures()) + test.getExtension(), input2);
	//cv::imshow("OriginalBlend", input2);
	cv::imshow("Original", input3);
	cv::waitKey(0);

	return 0;
}