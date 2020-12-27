#include "pch.h"
#include "CppUnitTest.h"
#include "../Mosaic++/Mosaic.h"
#include "../Mosaic++/Mosaic.cpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MosaicTest
{
	TEST_CLASS(MosaicTest)
	{
	public:

		TEST_METHOD(TestEuclideanDistance)
		{
			cv::Scalar color1(0, 0, 1);
			cv::Scalar color2(255, 255, 255);
			int8_t blueD, greenD, redD;
			blueD = color1[0] - color2[0];
			greenD = color1[1] - color2[1];
			redD = color1[2] - color2[2];
			uint16_t euclid = blueD * blueD + greenD * greenD + redD + redD;
			Assert::IsTrue(euclideanDistance(color1, color2) == euclid);

		}

		TEST_METHOD(TestBlending)
		{
			cv::Scalar color2(255, 255, 255);
			cv::Mat testImage = cv::imread("..//test.jpg", cv::IMREAD_COLOR);
			cv::Mat copyTestImage = testImage;
			Mosaic::alphaBlending(testImage, color2);

			Assert::IsTrue(testImage.empty());

		}

		TEST_METHOD(TestRiemersma)
		{
			uint32_t first = INT_MAX;
			uint32_t second = INT_MAX;
			bool indexEuclid;
			bool indexRiemersma;

			cv::Scalar color1(0, 0, 1);
			cv::Scalar color2(255, 255, 255);

			cv::Scalar color3(128, 128, 128);

			first = euclideanDistance(color1, color3);
			second = euclideanDistance(color2, color3);

			indexEuclid = first < second ? true : false;

			first = RiemersmaDistance(color1, color3);
			second = RiemersmaDistance(color2, color3);

			indexRiemersma = first < second ? true : false;

			Assert::AreNotEqual(indexEuclid, indexRiemersma);
		}
	};
}