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

	//cv::imshow("Original", input);

	stopwatch swMine;
	swMine.tick();
	//PictureTools::resize(input, 1200, 800, PictureTools::Algorithm::nearestNeighbour);
	//cv::resize(input, input, { 1200, 800 }, 0);
	swMine.tock();

	std::cout << swMine.report_ms();

	/*
	cv::imshow("Original2", input);

	cv::Mat input2 = Mosaic::makeMosaic(test.GetMediumColor(), input);
	cv::imwrite("..//Resulting pictures\\" + std::to_string(test.getNumberPictures()) + test.getExtension(), input2);
	cv::imshow("Returned", input2);
	cv::waitKey(0);
	*/

	return 0;
}