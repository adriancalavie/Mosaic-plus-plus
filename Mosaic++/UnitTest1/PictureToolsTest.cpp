#include "pch.h"
#include "CppUnitTest.h"
#include "../Mosaic++/PictureTools.h"
#include "../Mosaic++/PictureTools.cpp"
#include "../Mosaic++/StopWatch.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <tuple>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PictureToolsTest
{
	TEST_CLASS(PictureToolsTest)
	{
	public:

		TEST_METHOD(TestSpeed)
		{			
			//PictureTools::resize vs cv::resize execution time

			cv::Mat input = cv::imread("..//test.jpg", IMREAD_COLOR);
			cv::Mat input2 = cv::imread("..//test.jpg", IMREAD_COLOR);
			
			stopwatch swOriginal;
			stopwatch swMine;

			/*
			swOriginal.tick();

			//input = PictureTools::resize(input, 800, 800);
			//cv::resize(input, input, { 800,800 });

			swOriginal.tock();
			*/

			swMine.tick();
			
			input2 = PictureTools::resize(input2, 800, 800);
			//cv::resize(input2, input2, { 800,800 });

			swMine.tock();

			//Assert::AreEqual(swOriginal, swMine);


			//PictureTools::crop vs cv::Rect crop execution time

			/*
			cv::Mat input = cv::imread("..//test.jpg", IMREAD_COLOR);
			cv::Mat input2 = cv::imread("..//test.jpg", IMREAD_COLOR);

			stopwatch swPTCrop;
			stopwatch swCVCrop;
			
			swPTCrop.tick();
			cv::Mat result = PictureTools::cropSquare(input, { 0,0 }, { 20,20 });
			swPTCrop.tock();

			swCVCrop.tick();
			cv::Rect myROI(0, 0, 20, 20);
			cv::Mat result2 = input2(myROI);
			swCVCrop.tock();
			*/

			std::cout << swMine.report_ms;
			//Assert::AreEqual(swPTCrop, swCVCrop);

		}

		TEST_METHOD(Resize)
		{
			Mat testImage = imread("..//test.jpg", IMREAD_COLOR);

			testImage = PictureTools::resize(testImage, 600, 600);

			bool testingSize = testImage.rows == 600 && testImage.cols == 600;

			Assert::IsTrue(testingSize);
			Assert::IsTrue(!testImage.empty());
		}

		TEST_METHOD(Crop)
		{
			Mat testImage = imread("..//test.jpg", IMREAD_COLOR);

			Mat cropedImage = PictureTools::crop(testImage, { 0,0 }, { 20,20 });

			bool isEqual = true;

			for (int index_x = 0; index_x < 20; ++index_x)
				for (int index_y = 0; index_y < 20; ++index_y)
					if (testImage.at<Vec3b>(index_x, index_y) != cropedImage.at<Vec3b>(index_x, index_y))
					{
						isEqual = false;
						break;
					}

			Assert::IsTrue(isEqual);
		}
		TEST_METHOD(HueShiftImage)
		{
			Mat testImage = imread("..//test.jpg", IMREAD_COLOR);
			Mat hueShiftedImage = PictureTools::hueShiftImage(testImage, 45);
			std::tuple <uint8_t, uint8_t, uint8_t> testColor;

			bool isEqual = true;
			for (int index_x = 0; index_x < testImage.rows; ++index_x)
				for (int index_y = 0; index_y < testImage.cols; ++index_y)
				{
					testColor = PictureTools::hueShiftPixel(testImage.at<cv::Vec3b>(index_x, index_y)[2], testImage.at<cv::Vec3b>(index_x, index_y)[1], testImage.at<cv::Vec3b>(index_x, index_y)[0], 45);
					if (testColor != std::tuple <uint8_t, uint8_t, uint8_t>(hueShiftedImage.at<cv::Vec3b>(index_x, index_y)[2], hueShiftedImage.at<cv::Vec3b>(index_x, index_y)[1], hueShiftedImage.at<cv::Vec3b>(index_x, index_y)[0]))
					{
						isEqual = false;
						break;
					}
				}
			Assert::IsTrue(isEqual);
		}
	};
}