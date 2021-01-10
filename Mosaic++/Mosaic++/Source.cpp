#include <iostream>
#include "BasePictures.h"
#include "StopWatch.h"
#include "Mosaic.h"


int main()
{
	BasePictures test(5997);

	test.addPicturesMosaic(false);

	cv::Mat input = cv::imread(Data::Defaults::PATH_TEST_IMAGE, cv::IMREAD_COLOR);

	cv::Mat input3 = Mosaic::makeMosaic(input, test, Method::RESIZING, Type::DIAMOND, 30, false);
	cv::imwrite(Data::Defaults::PATH_RESULT_IMAGE + "Diamond.jpg", input3);
	cv::imshow("Original", input3);
	cv::waitKey(0);

	return 0;
}

