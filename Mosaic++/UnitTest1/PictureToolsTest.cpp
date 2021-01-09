#include "pch.h"
#include "CppUnitTest.h"
#include "../Picture Tools/PictureTools.h"
#include "../Mosaic++/StopWatch.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <tuple>
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PictureToolsTest
{
	TEST_CLASS(PictureToolsTest)
	{
	public:

		TEST_METHOD(TestSpeed)
		{			
			//PictureTools::resize vs cv::resize execution time

			cv::Mat input = cv::imread("..//test.jpg", cv::IMREAD_COLOR);
			cv::Mat input2 = cv::imread("..//test.jpg", cv::IMREAD_COLOR);
			
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

			std::cout << swMine.report_ms();
			//Assert::AreEqual(swPTCrop, swCVCrop);

		}

		TEST_METHOD(TestResizeNeighbour)
		{
			cv::Mat testImage = cv::imread("..//test.jpg", cv::IMREAD_COLOR);

			testImage = PictureTools::resize(testImage, 600, 600,PictureTools::Algorithm::nearestNeighbour);

			bool testingSize = testImage.rows == 600 && testImage.cols == 600;

			//Assert::IsTrue(testingSize);
			Assert::IsTrue(!testImage.empty());
		}

		TEST_METHOD(TestResizeBilinearInterpolation)
		{
			cv::Mat testImage = cv::imread("..//test.jpg", cv::IMREAD_COLOR);

			testImage = PictureTools::resize(testImage, 1200, 780,PictureTools::Algorithm::bilinearInterpolation);

			bool testingSize = testImage.rows == 1200 && testImage.cols == 780;

			//Assert::IsTrue(testingSize);
			Assert::IsTrue(!testImage.empty());
		}

		TEST_METHOD(TestCrop)
		{
			cv::Mat testImage = cv::imread("..//test.jpg", cv::IMREAD_COLOR);

			cv::Mat cropedImage = PictureTools::cropSquare(testImage, { 0,0 }, { 20,20 });

			bool isEqual = true;

			for (int index_x = 0; index_x < 20; ++index_x)
				for (int index_y = 0; index_y < 20; ++index_y)
					if (testImage.at<cv::Vec3b>(index_x, index_y) != cropedImage.at<cv::Vec3b>(index_x, index_y))
					{
						isEqual = false;
						break;
					}

			Assert::IsTrue(isEqual);
		}
		TEST_METHOD(TestHueShiftImage)
		{
			cv::Mat testImage = cv::imread("..//test.jpg", cv::IMREAD_COLOR);
			cv::Mat hueShiftedImage = PictureTools::hueShiftImage(testImage, 45);
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