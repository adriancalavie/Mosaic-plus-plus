#include "pch.h"
#include "CppUnitTest.h"
#include "../Mosaic++/BasePictures.h"
#include "../Mosaic++/BasePictures.cpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BasePicturesTest
{
	TEST_CLASS(BasePicturesTest)
	{
	public:

		TEST_METHOD(Constructor)
		{
			BasePictures pictures(1000, ".jpg");
			Assert::IsTrue(pictures.getNumberPicture() == 1000);
			Assert::IsTrue(pictures.getPictureExtension() == ".jpg");
		}

		TEST_METHOD(ReadPhoto)
		{
			cv::Mat image=BasePictures::readPhoto("0.jpg");
			Assert::IsTrue(!image.empty());
		}

		TEST_METHOD(AddPicturesInUnordered_Map)
		{
			BasePictures test(10);
			test.CreatingPicturesForMosaics();
			Assert::IsTrue(test.GetMediumColor().size() > 0);
		}
	};
}