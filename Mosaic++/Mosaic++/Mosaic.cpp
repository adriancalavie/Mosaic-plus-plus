#include "Mosaic.h"
#include "BasePictures.h"
#include "PictureTools.h"
#include <algorithm>


uint32_t euclideanDistance(const cv::Scalar& firstColor, const cv::Scalar& secondColor)
{
	int8_t blueD, greenD, redD;
	blueD = firstColor[0] - secondColor[0];
	greenD = firstColor[1] - secondColor[1];
	redD = firstColor[2] - secondColor[2];
	return (blueD * blueD + greenD * greenD + redD * redD);
}

void Mosaic::alphaBlending(cv::Mat& image, const cv::Scalar& color)
{
	for (auto x = 0; x < image.rows; ++x)
	{
		for (auto y = 0; y < image.cols; ++y)
		{
			image.at<cv::Vec3b>(x, y)[0] = (image.at<cv::Vec3b>(x, y)[0] + color[0])/2;
			image.at<cv::Vec3b>(x, y)[1] = (image.at<cv::Vec3b>(x, y)[1] + color[1])/2;
			image.at<cv::Vec3b>(x, y)[2] = (image.at<cv::Vec3b>(x, y)[2] + color[2])/2;
		}
	}
}

cv::Mat Mosaic::makeMosaic(const std::unordered_map<cv::Scalar, std::string>& dataPictures, const cv::Mat& image, const uint8_t& partitionSize, const uint8_t& shape, bool blending)
{
	assert(!image.empty());

	switch (shape)
	{
	case 1:
		return makeSquare(dataPictures, image, blending, partitionSize);
	case 2:
		return makeTriangle(dataPictures, image, blending, partitionSize);
	default:
		break;
	}
}

int cmmdc(int firstNumber,int secondNumber)
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

cv::Mat Mosaic::makeSquare(const std::unordered_map<cv::Scalar, std::string>& dataPictures, const cv::Mat& image, bool blending, const uint8_t& partitionSize)
{
	int v1 = image.cols % partitionSize;
	int v2 = image.rows % partitionSize;
	bool v3 = v1 || v2;
	assert(v3);

	//if any pixels would remain after partitioning process, call abort;


	cv::Mat result(image.rows, image.cols, CV_8UC3);
	//int counter = 1;

	for (auto x = 0; x < image.cols; x += partitionSize)
		for (auto y = 0; y < image.rows; y += partitionSize)
		{
			// compare partitionAverage with mapped images's average;*first implementation using STL vector
			cv::Scalar medColor = PictureTools::averageColor(image, { y,x }, { partitionSize,partitionSize });

			std::string pictureName;
			int closestDistance = INT_MAX;
			for (auto itr : dataPictures)
			{
				int currDistance = euclideanDistance(medColor, itr.first);

				if (currDistance < closestDistance)
				{
					closestDistance = currDistance;
					pictureName = itr.second;
				}
			}

			cv::Mat testPhoto = std::move(BasePictures::readPhoto(pictureName));

			
			if (blending)
			{				
				alphaBlending(testPhoto, PictureTools::averageColor(testPhoto));
			}
			

			for (auto i = x; i < x + partitionSize; ++i)
			{
				for (auto j = y; j < y + partitionSize; ++j)
				{
					if (i < result.cols && j < result.rows) {
						result.at<cv::Vec3b>({ i,j })[0] = std::move(testPhoto.at<cv::Vec3b>({ i - x, j - y })[0]);
						result.at<cv::Vec3b>({ i,j })[1] = std::move(testPhoto.at<cv::Vec3b>({ i - x, j - y })[1]);
						result.at<cv::Vec3b>({ i,j })[2] = std::move(testPhoto.at<cv::Vec3b>({ i - x, j - y })[2]);
					}
					else
					{
						//nimic
					}
				}
			}
		}

   //TO DO: MAKE IT WORK FOR ALL DIMENSIONS

	return result;
}

cv::Mat Mosaic::makeTriangle(const std::unordered_map<cv::Scalar, std::string>& dataPictures, const cv::Mat& image, bool blending, const uint8_t& partitionSize)
{
	return cv::Mat();
}

void Mosaic::replaceCellRectangle(cv::Mat& originalPicture, cv::Mat& mosaicPhoto, const Point& topL)
{
	assert(originalPicture.empty());
	for (int index_rows = 0; index_rows < mosaicPhoto.rows; index_rows++)
		for (int index_cols = 0; index_cols < mosaicPhoto.cols; index_cols++)
		{
			originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[0] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[0]);
			originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[1] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[1]);
			originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[2] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[2]);
		}
}

void Mosaic::replaceCellTriangle(cv::Mat& originalPicture, cv::Mat& mosaicPhoto, const Point& topL, const uint8_t& type)
{
	/*
	| 1 /|		|\ 3 |
	|  / |		| \  |
	| /  |		|  \ |
	|/ 2 |		| 4 \|
	*/
	assert(originalPicture.empty());
	switch (type)
	{
	case 1:
		for (int index_rows = 0; index_rows < mosaicPhoto.rows; index_rows++)
			for (int index_cols = 0; index_cols < mosaicPhoto.cols - index_rows - 1; index_cols++)
			{
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[0] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[0]);
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[1] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[1]);
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[2] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[2]);
			}
		break;
	case 2:
		for (int index_rows = 0; index_rows < mosaicPhoto.rows; index_rows++)
			for (int index_cols = mosaicPhoto.cols - index_rows - 1; index_cols < mosaicPhoto.cols; index_cols++)
			{
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[0] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[0]);
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[1] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[1]);
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[2] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[2]);
			}
		break;
	case 3:
		for (int index_rows = 0; index_rows < mosaicPhoto.rows; index_rows++)
			for (int index_cols = index_rows; index_cols < mosaicPhoto.cols; index_cols++)
			{
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[0] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[0]);
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[1] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[1]);
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[2] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[2]);
			}
		break;
	case 4:
		for (int index_rows = 0; index_rows < mosaicPhoto.rows; index_rows++)
			for (int index_cols = 0; index_cols < index_rows + 1; index_cols++)
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

void Mosaic::replaceCellDiamond(cv::Mat& originalPicture, cv::Mat& mosaicPhoto, const Point& top, const Point& low, const Point& left, const Point& right)
{
	//TOO DO
}
