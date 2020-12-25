//#include <iostream>
//#include "BasePictures.h"
//#include "PictureTools.h"
//#include "StopWatch.h"
//#include "Mosaic.h"
//
//int main()
//{
//	//Make mosaic;
//	BasePictures test;
//	test.setNumberPictures(5997);
//	test.setFileSource("D:\\Info Unitbv 2020-2021\\Semestrul I\\Modern C++\\Mozaic\\Mosaic++\\Pictures for mosaics");
//	test.setDataBase("D:\\Info Unitbv 2020-2021\\Semestrul I\\Modern C++\\Mozaic\\Mosaic++\\Mosaic++\\data_base.txt");
//	test.setFileDestination("D:\\Info Unitbv 2020-2021\\Semestrul I\\Modern C++\\Mozaic\\Mosaic++\\Pictures for mosaics\\");
//	test.addPicturesMosaic(false);
//
//
//	cv::Mat input = cv::imread("D:\\Info Unitbv 2020-2021\\Semestrul I\\Modern C++\\Mozaic\\Mosaic++\\test.jpg", cv::IMREAD_COLOR);
//
//	cv::Mat input3 = Mosaic::makeMosaic(input, test, Method::RESIZING, Type::TRIANGLE, 5, false);
//	cv::imwrite("D:\\Info Unitbv 2020-2021\\Semestrul I\\Modern C++\\Mozaic\\Mosaic++\\Resulting pictures\\Tringle.jpg", input3);
//	cv::imshow("Original", input3);
//	cv::waitKey(0);
//
//	return 0;
//}
//
