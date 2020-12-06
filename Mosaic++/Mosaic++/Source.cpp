#include <iostream>
#include "BasePictures.h"
#include "PictureTools.h"
#include "StopWatch.h"
#include "Mosaic.h"


int main()
{
	//Make mosaic 
	BasePictures test(1000);
	test.CreatePictures();

	Mat input = imread("..//test.jpg", IMREAD_COLOR);

	input = PictureTools::resize(input, 1200, 650);

	Mat input2 = Mosaic::makeMosaic(test.GetMediumColor(), input);
	imshow("Original", input);
	imshow("Returned", input2);
	waitKey(0);

	return 0;
}