#include "pch.h"
#include "CppUnitTest.h"
#define private public
#include "../QtExample/Mosaic.h"
#include "../QtExample/Mosaic.cpp"
#include "../QtExample/BasePictures.h"
#include "../QtExample/BasePictures.cpp"
#include "../QtExample/Data.h"
#include "../QtExample/Data.cpp"
#include "../QtExample/StopWatch.h"
#include "../QtExample/StopWatch.cpp"
#undef private
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MosaicTests
{
	TEST_CLASS(MosaicTests)
	{
	public:

		TEST_METHOD(TestEuclideanDistance)
		{
			cv::Scalar color1(0, 0, 1);
			cv::Scalar color2(255, 255, 255);

			uint8_t blueD = color1[0] - color2[0];
			uint8_t greenD = color1[1] - color2[1];
			uint8_t redD = color1[2] - color2[2];

			uint32_t euclid = blueD * blueD + greenD * greenD + redD * redD;
			uint32_t euclid_function = EuclideanDistance(color1, color2);

			std::cout << euclid << " " << euclid_function;
			Assert::IsTrue(euclid_function == euclid);
		}

		TEST_METHOD(TestBasePicturesConstructor)
		{
			BasePictures pictures;

			Assert::IsTrue(pictures.GetFileDestination() == Data::Defaults::PATH_PICTURES_FOR_MOSAIC);

		}

		TEST_METHOD(TestReadPhoto)
		{
			cv::Mat image = BasePictures::ReadPhoto("0.jpg", "..//" + Data::Defaults::PATH_PICTURES_FOR_MOSAIC);
			Assert::IsTrue(!image.empty());
		}

		TEST_METHOD(TestAddPicturesInUnordered_Map)
		{
			BasePictures test;
			test.CreatePictures();
			Assert::IsTrue(test.GetMediumColor().size() > 0);
		}

		TEST_METHOD(TestAddPicturesMosaicTest)
		{
			std::ifstream in("data_base.txt");
			Assert::IsTrue(!in.eof());
		}

		TEST_METHOD(TestBlending)
		{
			cv::Scalar color2(255, 255, 255);
			cv::Mat testImage = cv::imread(Data::Defaults::PATH_TEST_IMAGE, cv::IMREAD_COLOR);
			cv::Mat copyTestImage = testImage;
			PictureTools::AlphaBlending(testImage, color2);

			Assert::IsTrue(!testImage.empty());
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

			first = EuclideanDistance(color1, color3);
			second = EuclideanDistance(color2, color3);

			indexEuclid = first < second ? true : false;

			first = RiemersmaDistance(color1, color3);
			second = RiemersmaDistance(color2, color3);

			indexRiemersma = first < second ? true : false;

			Assert::AreNotEqual(indexEuclid, indexRiemersma);
		}

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

			input2 = PictureTools::Resize(input2, 800, 800);
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
			cv::Mat testImage = cv::imread(Data::Defaults::PATH_TEST_IMAGE, cv::IMREAD_COLOR);

			testImage = PictureTools::Resize(testImage, 600, 600, PictureTools::Algorithm::NEAREST_NEIGHBOUR);

			bool testingSize = testImage.rows == 600 && testImage.cols == 600;

			Assert::IsTrue(!(testImage.empty() && testingSize));
		}

		TEST_METHOD(TestResizeBilinearInterpolation)
		{
			cv::Mat testImage = cv::imread(Data::Defaults::PATH_TEST_IMAGE, cv::IMREAD_COLOR);

			testImage = PictureTools::Resize(testImage, 1200, 780, PictureTools::Algorithm::BILINEAR_INTERPOLATION);

			bool testingSize = testImage.rows == 1200 && testImage.cols == 780;

			Assert::IsTrue(!(testImage.empty() && testingSize));

		}

		TEST_METHOD(TestCrop)
		{
			cv::Mat testImage = cv::imread("..//test.jpg", cv::IMREAD_COLOR);

			cv::Mat cropedImage = PictureTools::CropSquare(testImage, { 0,0 }, { 20,20 });

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

		TEST_METHOD(TestMakeMosaicSquare)
		{
			cv::Mat testImage = cv::imread(Data::Defaults::PATH_TEST_IMAGE, cv::IMREAD_COLOR);
			BasePictures pictures;
			pictures.AddBasePicturesMosaic();
			cv::Mat output = Mosaic::MakeMosaic(testImage, pictures, Method::RESIZING, Type::SQUARE, testImage.rows / 10, Algorithm::RIEMERSMA, false);
			Assert::IsTrue(testImage.empty() && output.empty());
		}

		TEST_METHOD(TestMakeMosaicTriangle)
		{
			cv::Mat testImage = cv::imread(Data::Defaults::PATH_TEST_IMAGE, cv::IMREAD_COLOR);
			BasePictures pictures;
			pictures.AddBasePicturesMosaic();
			cv::Mat output = Mosaic::MakeMosaic(testImage, pictures, Method::RESIZING, Type::TRIANGLE, testImage.rows / 10, Algorithm::RIEMERSMA, false);
			Assert::IsTrue(testImage.empty() && output.empty());
		}

		TEST_METHOD(TestMakeMosaicDiamond)
		{
			cv::Mat testImage = cv::imread(Data::Defaults::PATH_TEST_IMAGE, cv::IMREAD_COLOR);
			BasePictures pictures;
			pictures.AddBasePicturesMosaic();
			cv::Mat output = Mosaic::MakeMosaic(testImage, pictures, Method::RESIZING, Type::DIAMOND, testImage.rows / 10, Algorithm::RIEMERSMA, false);
			Assert::IsTrue(testImage.empty() && output.empty());
		}

		TEST_METHOD(TestMediumColorSquare)
		{
			cv::Scalar color(-1, -1, -1);
			cv::Mat testImage = cv::imread(Data::Defaults::PATH_TEST_IMAGE, cv::IMREAD_COLOR);
			color = PictureTools::AverageColorRectangle(testImage, { 0,0 }, { testImage.rows, testImage.cols });
			if (color[0] == -1 || color[1] == -1 || color[2] == -1)
				Assert::IsTrue(true);
		}

		TEST_METHOD(TestMediumColorTriangle)
		{
			cv::Scalar color(-1, -1, -1);
			cv::Mat testImage = cv::imread(Data::Defaults::PATH_TEST_IMAGE, cv::IMREAD_COLOR);

			auto test = [&](uint8_t type)
			{
				color = PictureTools::AverageColorTriangle(testImage, { 0,0 }, { testImage.rows, testImage.cols }, 1);
				if (color[0] == -1 || color[1] == -1 || color[2] == -1)
					Assert::IsTrue(true);
				color[0] == -1;
				color[1] == -1;
				color[2] == -1;
			};
			test(1);
			test(2);
			test(3);
			test(4);
		}

		TEST_METHOD(TestReplaceCellSquare)
		{
			cv::Mat testImage = cv::imread(Data::Defaults::PATH_TEST_IMAGE, cv::IMREAD_COLOR);
			cv::Mat output(testImage.rows, testImage.cols, CV_8UC3);
			output = cv::Scalar(0, 0, 0);

			Mosaic::ReplaceCellRectangle(output, std::move(testImage), { 0,0 });
			cv::Scalar medColor = PictureTools::AverageColorRectangle(output, { 0,0 }, { output.rows, output.cols });
			if (medColor[0] == 0 && medColor[1] == 0 && medColor[2] == 0)
				Assert::IsTrue(true);
		}

		TEST_METHOD(TestReplaceCellDiamond)
		{
			cv::Mat testImage = cv::imread(Data::Defaults::PATH_TEST_IMAGE, cv::IMREAD_COLOR);
			cv::Mat output(testImage.rows, testImage.cols, CV_8UC3);
			output = cv::Scalar(0, 0, 0);
			Mosaic::ReplaceCellDiamond(output, std::move(testImage), { 0,testImage.cols / 2 });

			cv::Scalar medColor = PictureTools::AverageColorRectangle(output, { 0,0 }, { output.rows, output.cols });
			if (medColor[0] == 0 && medColor[1] == 0 && medColor[2] == 0)
				Assert::IsTrue(true);
		}

		TEST_METHOD(TestReplaceCellTriangle)
		{
			cv::Mat testImage = cv::imread(Data::Defaults::PATH_TEST_IMAGE, cv::IMREAD_COLOR);
			cv::Mat output(testImage.rows, testImage.cols, CV_8UC3);
			output = cv::Scalar(0, 0, 0);

			auto test = [&](uint8_t type)
			{
				Mosaic::ReplaceCellTriangle(output, std::move(testImage), { 0,0 }, type, { 0,0 }, { testImage.rows, testImage.cols });
				cv::Scalar medColor = PictureTools::AverageColorRectangle(output, { 0,0 }, { output.rows, output.cols });
				if (medColor[0] == 0 && medColor[1] == 0 && medColor[2] == 0)
					Assert::IsTrue(true);
				output = cv::Scalar(0, 0, 0);
			};
			test(1);
			test(2);
			test(3);
			test(4);
		}

		TEST_METHOD(TestFindMediumColorPicture)
		{
			cv::Mat testImage = cv::imread(Data::Defaults::PATH_TEST_IMAGE, cv::IMREAD_COLOR);
			BasePictures basePicture;
			basePicture.AddBasePicturesMosaic();
			cv::Scalar medColor = PictureTools::AverageColorRectangle(testImage, { 0,0 }, { testImage.rows, testImage.cols });
			std::string namePicture;
			testImage = Mosaic::FindPictureWithColorMed(basePicture.GetMediumColor(), medColor, namePicture, Algorithm::RIEMERSMA);
			Assert::IsTrue(!testImage.empty());
		}

	};
}
