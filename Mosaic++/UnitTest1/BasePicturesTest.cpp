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

		TEST_METHOD(TestConstructor)
		{
			BasePictures pictures(1000);
			Assert::IsTrue(pictures.GetPictureCount() == 1000);
			//Assert::IsTrue(pictures.getExtension() == ".jpg");
		}

		TEST_METHOD(TestReadPhoto)
		{
			cv::Mat image=BasePictures::ReadPhoto("0.jpg");
			Assert::IsTrue(!image.empty());
		}

		TEST_METHOD(TestAddPicturesInUnordered_Map)
		{
			BasePictures test(10);
			test.CreatePictures();
			Assert::IsTrue(test.GetMediumColor().size() > 0);
		}


		TEST_METHOD(TestAddPicturesMosaicTest)
		{
			std::ifstream in("data_base.txt");
			Assert::IsTrue(!in.eof());
		}
	};
}