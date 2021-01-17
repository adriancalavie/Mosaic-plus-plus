#include <iostream>
#include "BasePictures.h"
#include "StopWatch.h"
#include "Mosaic.h"


int main()
{
	BasePictures test(5997);

	test.AddPicturesMosaic(false);

	cv::Mat input = cv::imread(Data::Defaults::PATH_TEST_IMAGE, cv::IMREAD_COLOR);

	std::cout << input.rows << " " << input.cols << std::endl;
	cv::Mat input3 = Mosaic::MakeMosaic(input, test, Method::RESIZING, Type::SQUARE, 30, false);
	std::string str = Data::Defaults::PATH_RESULT_IMAGE + "Diamond.jpg";
	cv::imwrite(str, input3);
	cv::imshow("Processed Image", input3);
	cv::waitKey(0);

	return 0;
}

