#include "pch.h"
#include "CppUnitTest.h"
#include "../Mosaic++/PictureTools.h"
#include "../Mosaic++/PictureTools.cpp"
#include "../Mosaic++/StopWatch.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PictureToolsTest
{
	TEST_CLASS(PictureToolsTest)
	{
	public:

		TEST_METHOD(TestSpeed)
		{			
			
			cv::Mat input = imread("..//test.jpg", IMREAD_COLOR);
			cv::Mat input2 = imread("..//test.jpg", IMREAD_COLOR);
			
			stopwatch swOriginal;
			stopwatch swMine;
			swOriginal.tick();

			//input = PictureTools::resize(input, 800, 800);
			cv::resize(input, input, { 800,800 });

			swOriginal.tock();

			swMine.tick();

			input2 = PictureTools::resize(input2, 800, 800);
			//cv::resize(input2, input2, { 800,800 });

			swMine.tock();

			//Assert::AreEqual(swOriginal, swMine);
		}

		
	};
}