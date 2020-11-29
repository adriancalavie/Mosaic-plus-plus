#include <iostream>
#include "BasePictures.h"
#include "PictureTools.h"

#include <vector>


int main()
{

	//std::cout << "Test1";

	//BasePictures test(1000, ".jpg");

	BasePictures test(1000, ".jpg");
//>>>>>>> Stashed changes
	test.CreatingPicturesForMosaics();
	waitKey(0);

	/*TESTING RESIZE AND AVERGECOLOR FUNCTIONS
	Mat image = imread("C:\\Users\\Dinu\\Desktop\\poze_de_baza\\16.jpg", IMREAD_COLOR);
	assert(!image.empty());
	PictureTools test2;
	Mat image2 = Mat(200, 200, CV_8UC3, test2.averageColor(image));
	Mat image3 = test2.resize(image, 900, 900);
	imshow("Display Window1", image);
	waitKey(0);
	imshow("Display Window2", image2);
	waitKey(0);
	imshow("Display Window3", image3);
	waitKey(0);
	*/
	
	Mat input = imread("..//test.jpg", IMREAD_COLOR);
	input = PictureTools::resize(input, 800, 800);
	imshow("Original", input);
	waitKey(0);

	Mat output = PictureTools::makeMosaic(test.GetMediumColor(),input);

	/*output=PictureTools::rotateLeft(output);
	output = PictureTools::rotateRight(output);
	output = PictureTools::rotate180(output);*/
	/*Mat testPhoto;
	testPhoto= BasePictures::readPhoto("0.jpg");
	std::pair<int, int> start(0,0);

	PictureTools::replaceCell(output, testPhoto, start);*/
	imshow("Returned", output);
	waitKey(0);
	
	


	return 0;
}