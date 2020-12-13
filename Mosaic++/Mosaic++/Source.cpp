#include <iostream>
#include "BasePictures.h"
#include "PictureTools.h"
#include "StopWatch.h"
#include "Mosaic.h"

int main()
{
	//Make mosaic 
	/*
	BasePictures test(1001);
	test.addPicturesMosaic(false);
	*/


	cv::Mat input = cv::imread("..//test.jpg", cv::IMREAD_COLOR);



	input = PictureTools::resize(input, 1200, 700);
	Mosaic::alphaBlending(input, cv::Scalar(178, 61, 194));
	/*cv::Mat input2 = Mosaic::makeMosaic(test.GetMediumColor(), input);*/
	/*cv::imwrite("..//Resulting pictures\\" + std::to_string(test.getNumberPictures()) + test.getExtension(), input2);*/
	cv::imshow("Original", input);
	/*cv::imshow("Returned", input2);*/
	cv::waitKey(0);
	*/

	return 0;
}