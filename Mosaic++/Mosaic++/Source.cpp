#include <iostream>
#include "BasePictures.h"
#include "PictureTools.h"
#include "StopWatch.h"
#include <vector>
#include "Mosaic.h"


int main()
{
	//Make mosaic 
	BasePictures test(1000, ".jpg");
	test.CreatingPicturesForMosaics();
	Mat input = imread("..//test.jpg", IMREAD_COLOR);
	input = PictureTools::resize(input, 600, 600);
	Mat input2 = Mosaic::makeMosaic(test.GetMediumColor(), input);
	imshow("Original", input);
	imshow("Returned", input2);
	waitKey(0);

	return 0;
}