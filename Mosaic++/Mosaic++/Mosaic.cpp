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
			image.at<cv::Vec3b>(x, y)[0] = (image.at<cv::Vec3b>(x, y)[0] + color[0]) / 2;
			image.at<cv::Vec3b>(x, y)[1] = (image.at<cv::Vec3b>(x, y)[1] + color[1]) / 2;
			image.at<cv::Vec3b>(x, y)[2] = (image.at<cv::Vec3b>(x, y)[2] + color[2]) / 2;
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

cv::Mat Mosaic::makeSquare(const std::unordered_map<cv::Scalar, std::string>& dataPictures, const cv::Mat& image, bool blending, const uint8_t& partitionSize)
{
	int v1 = image.cols % partitionSize;
	int v2 = image.rows % partitionSize;
	bool v3 = v1 || v2;
	assert(!v3);

	//if any pixels would remain after partitioning process, call abort;


	cv::Mat result(image.rows, image.cols, CV_8UC3);
	
	for (auto x = 0; x < image.cols-1; x += partitionSize)
		for (auto y = 0; y < image.rows-1; y += partitionSize)
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

			Mosaic::replaceCellRectangle(result, testPhoto, std::make_pair(y, x));
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
		for (int index_rows = start.first; index_rows <= end.first; index_rows++)
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
			for (int index_cols = index_rows-start.first; index_cols < end.second; index_cols++)
			{
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[0] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[0]);
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[1] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[1]);
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[2] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[2]);
			}
		break;
	case 4:
		for (int index_rows = start.first; index_rows < end.first; index_rows++)
			for (int index_cols = start.second; index_cols <end.second-end.first+index_rows; index_cols++)
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
	topD.second = top.second-mosaicPhoto.cols/2;
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
	topD.first = top.first ;
	topD.second = top.second - mosaicPhoto.cols / 2;
	start.first = mosaicPhoto.rows/2;
	start.second = 0;
	end.first = mosaicPhoto.rows;
	end.second = mosaicPhoto.cols/2;
	replaceCellTriangle(originalPicture, mosaicPhoto, topD, 3, start, end);

	//4
	topD = top;
	topD.second = top.second - mosaicPhoto.cols / 2;
	start.first = 0;
	start.second = mosaicPhoto.cols/2;
	end.first = mosaicPhoto.rows/2;
	end.second = mosaicPhoto.cols;
	replaceCellTriangle(originalPicture, mosaicPhoto, topD, 4, start, end);

}
