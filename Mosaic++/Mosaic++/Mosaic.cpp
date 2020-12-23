#include "Mosaic.h"
#include "BasePictures.h"
#include "PictureTools.h"
#include <algorithm>
#include <random>
#include <time.h> 

uint32_t euclideanDistance(const cv::Scalar& firstColor, const cv::Scalar& secondColor)
{
	int8_t blueD, greenD, redD;
	blueD = firstColor[0] - secondColor[0];
	greenD = firstColor[1] - secondColor[1];
	redD = firstColor[2] - secondColor[2];
	return (blueD * blueD + greenD * greenD + redD * redD);
}

uint32_t RiemersmaDistance(const cv::Scalar& firstColor, const cv::Scalar& secondColor)
{
	uint32_t rmean = ((long)firstColor[0] + (long)secondColor[0]) / 2;
	uint32_t r = (long)firstColor[0] - (long)secondColor[0];
	uint32_t g = (long)firstColor[1] - (long)secondColor[1];
	uint32_t b = (long)firstColor[2] - (long)secondColor[2];

	return (((512 + rmean) * r * r) >> 8) + 4 * g * g + (((767 - rmean) * b * b) >> 8);
}

void Mosaic::alphaBlending(cv::Mat& image, const cv::Scalar& color)
{
	for (auto x = 0; x < image.rows; ++x)
	{
		for (auto y = 0; y < image.cols; ++y)
		{
			image.at<cv::Vec3b>(x, y)[0] = (image.at<cv::Vec3b>(x, y)[0] + color[0]) / 2;
			image.at<cv::Vec3b>(x, y)[1] = (image.at<cv::Vec3b>(x, y)[1] + color[1]) / 2;
			image.at<cv::Vec3b>(x, y)[2] = (image.at<cv::Vec3b>(x, y)[2] + color[2]) / 2;
		}
	}
}

cv::Mat Mosaic::makeMosaic(const cv::Mat& image, const BasePictures& basePictures, const Method& method, const Type& type, const uint8_t& partitionSize, bool blending)
{
	//TODO : change partitionSize to width and height pair
	assert(!image.empty());

	std::cout << "entered make mosaic\n";

	cv::Mat copyOriginalImage;

	int v1 = image.cols / partitionSize * partitionSize;
	int v2 = image.rows / partitionSize * partitionSize;
	switch (method)
	{
	case Method::CROPPING:
		copyOriginalImage = std::move(PictureTools::cropSquare(image, { 0,0 }, { v2,v1 }));
	case Method::RESIZING:
		copyOriginalImage = std::move(PictureTools::resize(image, v1, v2, PictureTools::Algorithm::bilinearInterpolation));
	}
	switch (type)
	{
	case Type::SQUARE:
		return makeRectangle(basePictures.GetMediumColor(), copyOriginalImage, blending, partitionSize);
	case Type::TRIANGLE:
		return makeTriangle(basePictures.GetMediumColor(), copyOriginalImage, blending, partitionSize);
	case Type::DIAMOND:
		return makeDiamond(basePictures.GetMediumColor(), copyOriginalImage, blending, partitionSize);
	case Type::RECTANGLE:
		return makeRectangle(basePictures.GetMediumColor(), copyOriginalImage, blending, partitionSize);
	default:
		break;
	}
}

int cmmdc(int firstNumber, int secondNumber)
{
	int res;
	while (firstNumber % secondNumber)
	{
		res = firstNumber % secondNumber;
		firstNumber = secondNumber;
		secondNumber = res;
	}

	return secondNumber;
}



cv::Mat Mosaic::makeRectangle(const std::unordered_map<cv::Scalar, std::string>& dataPictures, const cv::Mat& image, bool blending, const uint8_t& partitionSize)
{
	//can be used to create squares as well
	//removed redundant makeSquare function

	int v1 = image.cols % partitionSize;
	int v2 = image.rows % partitionSize;

	cv::Mat result(image.rows, image.cols, CV_8UC3);

	for (auto x = 0; x < image.cols - 1; x += partitionSize)
		for (auto y = 0; y < image.rows - 1; y += partitionSize)
		{
			cv::Scalar mediumColor = PictureTools::averageColorSquare(image, { y,x }, { partitionSize,partitionSize });
			std::string pictureName;
			int closestDistance = INT_MAX;
			cv::Scalar closestColor = mediumColor;
			for (auto itr : dataPictures)
			{
				int currDistance = RiemersmaDistance(mediumColor, itr.first);

				if (currDistance < closestDistance)
				{
					closestColor = itr.first;
					closestDistance = currDistance;
					pictureName = itr.second;
				}
			}

			cv::Mat testPhoto = std::move(BasePictures::readPhoto(pictureName));
			testPhoto = PictureTools::resize(testPhoto, partitionSize, partitionSize, PictureTools::Algorithm::bilinearInterpolation);
			if (blending)
			{
				alphaBlending(testPhoto, closestColor);
			}

			Mosaic::replaceCellRectangle(result, testPhoto, std::make_pair(y, x));
		}

	return result;
}

cv::Mat Mosaic::makeTriangle(const std::unordered_map<cv::Scalar, std::string>& dataPictures, const cv::Mat& image, bool blending, const uint8_t& partitionSize)
{
	int v1 = image.cols % partitionSize;
	int v2 = image.rows % partitionSize;
	bool v3 = v1 || v2;
	assert(!v3);

	cv::Mat result(image.rows, image.cols, CV_8UC3);

	for (auto x = 0; x < image.cols - 1; x += partitionSize)
		for (auto y = 0; y < image.rows - 1; y += partitionSize)
		{
			//random type triangle 1|2 or 3|4
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_int_distribution<> dis(0, 1);
			uint8_t type = dis(gen);

			std::string pictureName;
			int closestDistance = INT_MAX;
			cv::Scalar medColor;

			//first triangle

			if (type)
				medColor = PictureTools::averageColorTriangle(image, { y,x }, { partitionSize, partitionSize }, 1);
			else
				medColor = PictureTools::averageColorTriangle(image, { y,x }, { partitionSize, partitionSize }, 3);


			for (auto itr : dataPictures)
			{
				int currDistance = RiemersmaDistance(medColor, itr.first);

				if (currDistance < closestDistance)
				{
					closestDistance = currDistance;
					pictureName = itr.second;
				}
			}

			cv::Mat testPhoto = std::move(BasePictures::readPhoto(pictureName));
			testPhoto = PictureTools::resize(testPhoto, partitionSize, partitionSize, PictureTools::Algorithm::bilinearInterpolation);
			if (blending)
			{
				alphaBlending(testPhoto, PictureTools::averageColor(testPhoto));
			}
			if (type)
				Mosaic::replaceCellTriangle(result, testPhoto, std::make_pair(y, x), 1, { 0,0 }, { partitionSize, partitionSize });
			else
				Mosaic::replaceCellTriangle(result, testPhoto, std::make_pair(y, x), 3, { 0,0 }, { partitionSize, partitionSize });

			//second triangle
			closestDistance = INT_MAX;
			if (type)
				medColor = PictureTools::averageColorTriangle(image, { y,x }, { partitionSize, partitionSize }, 2);
			else
				medColor = PictureTools::averageColorTriangle(image, { y,x }, { partitionSize, partitionSize }, 4);
			for (auto itr : dataPictures)
			{
				int currDistance = euclideanDistance(medColor, itr.first);

				if (currDistance < closestDistance)
				{
					closestDistance = currDistance;
					pictureName = itr.second;

				}
			}
			testPhoto = std::move(BasePictures::readPhoto(pictureName));
			testPhoto = std::move(PictureTools::resize(testPhoto, partitionSize, partitionSize));

			if (blending)
			{
				alphaBlending(testPhoto, PictureTools::averageColor(testPhoto));
			}

			if (type)
				Mosaic::replaceCellTriangle(result, testPhoto, std::make_pair(y, x), 2, { 0,0 }, { partitionSize, partitionSize });
			else
				Mosaic::replaceCellTriangle(result, testPhoto, std::make_pair(y, x), 4, { 0,0 }, { partitionSize, partitionSize });
		}

	return result;
}

cv::Mat Mosaic::makeDiamond(const std::unordered_map<cv::Scalar, std::string>& dataPictures, const cv::Mat& image, bool blending, const uint8_t& partitionSize)
{
	int v1 = image.cols % partitionSize;
	int v2 = image.rows % partitionSize;
	bool v3 = v1 || v2;
	assert(!v3);
	//if any pixels would remain after partitioning process, call abort;


	cv::Mat result(image.rows, image.cols, CV_8UC3);

	for (auto x = 0; x < image.cols - 1; x += partitionSize)
		for (auto y = 0; y < image.rows - 1; y += partitionSize)
		{
			// compare partitionAverage with mapped images's average;
			cv::Scalar medColor = PictureTools::averageColorSquare(image, { y,x }, { partitionSize,partitionSize });

			std::string pictureName;
			int closestDistance = INT_MAX;
			cv::Scalar closestColor = medColor;
			for (auto itr : dataPictures)
			{
				int currDistance = euclideanDistance(medColor, itr.first);

				if (currDistance < closestDistance)
				{
					closestColor = itr.first;
					closestDistance = currDistance;
					pictureName = itr.second;
				}
			}

			cv::Mat testPhoto = std::move(BasePictures::readPhoto(pictureName));
			testPhoto = std::move(PictureTools::resize(testPhoto, partitionSize, partitionSize, PictureTools::Algorithm::bilinearInterpolation));
			if (blending)
			{
				alphaBlending(testPhoto, closestColor);
			}

			//Main cell
			Mosaic::replaceCellDiamond(result, testPhoto, std::make_pair(y, x + partitionSize / 2));

			unsigned int xDiamond = x + partitionSize / 2;
			unsigned int yDiamond = y + partitionSize / 2;

			//Through diamonds
			if (x < image.cols - partitionSize - 1 && y < image.rows - partitionSize - 1)
				Mosaic::replaceCellDiamond(result, testPhoto, std::make_pair(yDiamond, x + partitionSize));

			//LEFT MARGIN
			if (x == 0)
			{
				replaceCellTriangle(result, testPhoto, { y,x }, 1, { 0, 0 }, { partitionSize , partitionSize / 2});
				replaceCellTriangle(result, testPhoto, { y,x }, 4, { 0, 0 }, { partitionSize , partitionSize / 2});
			}
			else
				//RIGHT
				if (x == image.cols - partitionSize)
				{
					replaceCellTriangle(result, testPhoto, { y,xDiamond }, 3, { 0, 0 }, { partitionSize, partitionSize / 2});
					replaceCellTriangle(result, testPhoto, { y + partitionSize / 2 , xDiamond }, 2, { 0, 0 }, { partitionSize / 2, partitionSize / 2 });
				}
					
			//TOP
			if (y == 0)
			{
				replaceCellTriangle(result, testPhoto, { y,xDiamond }, 3, { 0, 0 }, { partitionSize , partitionSize / 2 });
				replaceCellTriangle(result, testPhoto, { y,x }, 1, { 0, 0 }, { partitionSize , partitionSize / 2 });
			}
			else
				//BOTTOM
				if (y == image.rows - partitionSize)
				{
					replaceCellTriangle(result, testPhoto, { y + partitionSize / 2,x }, 4, { 0, 0 }, { partitionSize / 2  , partitionSize / 2 });
					replaceCellTriangle(result, testPhoto, { y + partitionSize / 2,xDiamond }, 2, { 0, 0 }, { partitionSize / 2 , partitionSize / 2 });
				}
			
		}


	//TO DO: MAKE IT WORK FOR ALL DIMENSIONS

	return result;
}

void Mosaic::replaceCellRectangle(cv::Mat& originalPicture, cv::Mat& mosaicPhoto, const Point& topL)
{
	assert(!originalPicture.empty());
	for (int index_rows = 0; index_rows < mosaicPhoto.rows; index_rows++)
		for (int index_cols = 0; index_cols < mosaicPhoto.cols; index_cols++)
		{
			originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[0] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[0]);
			originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[1] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[1]);
			originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[2] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[2]);
		}
}

void Mosaic::replaceCellTriangle(cv::Mat& originalPicture, cv::Mat& mosaicPhoto, const Point& topL, const uint8_t& type, const Point& start, const Point& end)
{
	/*
	^| 1 /|		^|\ 3 |      ^ -> topL
	 |  / |		 | \  |
	 | /  |		 |  \ |
	 |/ 2 |		 | 4 \|
	*/
	assert(!originalPicture.empty());
	switch (type)
	{
	case 1:
		for (int index_rows = start.first; index_rows < end.first; index_rows++)
			for (int index_cols = start.second; index_cols <= end.second + start.second - index_rows - 1; index_cols++)
			{
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[0] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[0]);
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[1] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[1]);
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[2] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[2]);
			}
		break;
	case 2:
		for (int index_rows = start.first; index_rows < end.first; index_rows++)
			for (int index_cols = start.second + end.second - index_rows - 1; index_cols < end.second; index_cols++)
			{
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[0] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[0]);
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[1] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[1]);
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[2] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[2]);
			}
		break;
	case 3:
		for (int index_rows = start.first; index_rows < end.first; index_rows++)
			for (int index_cols = index_rows - start.first; index_cols < end.second; index_cols++)
			{
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[0] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[0]);
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[1] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[1]);
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[2] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[2]);
			}
		break;
	case 4:
		for (int index_rows = start.first; index_rows < end.first; index_rows++)
			for (int index_cols = start.second; index_cols < end.second - end.first + index_rows; index_cols++)
			{
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[0] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[0]);
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[1] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[1]);
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[2] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[2]);
			}
		break;
	default:
		break;
	}

}

void Mosaic::replaceCellDiamond(cv::Mat& originalPicture, cv::Mat& mosaicPhoto, const Point& top)
{
	/*
		  ^
	 |   /|\   |      ^ -> topL
	 |  / | \  |      For diamond use replaceCellTriangle
	 | /  |  \ |
	 |/ 2 | 4 \|
	 -----------
	 |\ 3 |	1 /|
	 | \  |  / |
	 |  \ | /  |
	 |   \|/   |
	*/
	Point topD, start, end;

	//1
	topD.first = top.first;
	topD.second = top.second - mosaicPhoto.cols / 2;
	start.first = mosaicPhoto.rows / 2;
	start.second = mosaicPhoto.cols / 2;
	end.first = mosaicPhoto.rows;
	end.second = mosaicPhoto.cols;
	replaceCellTriangle(originalPicture, mosaicPhoto, topD, 1, start, end);

	//2
	topD.first = top.first;
	topD.second = top.second - mosaicPhoto.cols / 2;
	start.first = 0;
	start.second = 0;
	end.first = mosaicPhoto.rows / 2;
	end.second = mosaicPhoto.cols / 2;
	replaceCellTriangle(originalPicture, mosaicPhoto, topD, 2, start, end);

	//3
	topD.first = top.first;
	topD.second = top.second - mosaicPhoto.cols / 2;
	start.first = mosaicPhoto.rows / 2;
	start.second = 0;
	end.first = mosaicPhoto.rows;
	end.second = mosaicPhoto.cols / 2;
	replaceCellTriangle(originalPicture, mosaicPhoto, topD, 3, start, end);

	//4
	topD = top;
	topD.second = top.second - mosaicPhoto.cols / 2;
	start.first = 0;
	start.second = mosaicPhoto.cols / 2;
	end.first = mosaicPhoto.rows / 2;
	end.second = mosaicPhoto.cols;
	replaceCellTriangle(originalPicture, mosaicPhoto, topD, 4, start, end);

}
