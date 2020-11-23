#include "PictureTools.h"
#include "BasePictures.h"
#include <iostream>

Mat PictureTools::crop(const Mat& image, Point topL, Point botR)
{
	uint64_t height = botR.first - topL.first;
	uint64_t width = botR.second - topL.second;
	Mat result(height, width, CV_8UC3);
	//std::cout << result.rows << " " << result.cols;
	for (int index_rows = 0; index_rows < height; index_rows++)
		for (int index_cols = 0; index_cols < width; index_cols++)
		{
			result.at<Vec3b>(index_rows, index_cols)[0] = image.at<Vec3b>(topL.first + index_rows, topL.second + index_cols)[0];
			result.at<Vec3b>(index_rows, index_cols)[1] = image.at<Vec3b>(topL.first + index_rows, topL.second + index_cols)[1];
			result.at<Vec3b>(index_rows, index_cols)[2] = image.at<Vec3b>(topL.first + index_rows, topL.second + index_cols)[2];
		}
	return result;
}

Mat PictureTools::resize(const Mat& image, const uint16_t& width, const uint16_t& height)
{
	assert(!image.empty());
	Mat newimage(width, height, CV_8UC3);

	int x_ratio = (int)((image.cols << 16) / width) + 1;
	int y_ratio = (int)((image.rows << 16) / height) + 1;

	int nearX, nearY;
	for (int rows = 0; rows < height; ++rows) {
		for (int cols = 0; cols < width; ++cols) {
			nearX = ((cols * x_ratio) >> 16);
			nearY = ((rows * y_ratio) >> 16);

			newimage.at<cv::Vec3b>(rows, cols)[0] = image.at<cv::Vec3b>(nearY, nearX)[0];
			newimage.at<cv::Vec3b>(rows, cols)[1] = image.at<cv::Vec3b>(nearY, nearX)[1];
			newimage.at<cv::Vec3b>(rows, cols)[2] = image.at<cv::Vec3b>(nearY, nearX)[2];
		}
	}
	return newimage;
}

Scalar PictureTools::averageColor(const Mat& image)
{
	assert(!image.empty());
	double blue, green, red;
	blue = green = red = 0;
	int size = (image.rows) * (image.cols);
	for (int rows = 0; rows < image.rows; ++rows)
	{
		for (int cols = 0; cols < image.cols; ++cols)
		{
			blue += (int)image.at<cv::Vec3b>(rows, cols)[0] / (double)100;
			green += (int)image.at<cv::Vec3b>(rows, cols)[1] / (double)100;
			red += (int)image.at<cv::Vec3b>(rows, cols)[2] / (double)100;

		}
	}
	red = red / size;
	green = green / size;
	blue = blue / size;
	red = red * 100;
	green = green * 100;
	blue = blue * 100;

	return Scalar(blue, green, red);
}

Mat PictureTools::makeMosaic(const std::vector<Scalar>& dataPictures, Mat& image, const uint8_t& partitionSize)
{
	bool v1 = image.cols % partitionSize == 0;
	bool v2 = image.rows % partitionSize == 0;
	bool v3 = v1 || v2;
	assert(v3);
	//if any pixels would remain after partitioning process, call abort;


	Mat result(image.rows, image.cols, CV_8UC3);

	for (auto x = 0; x < image.cols; x += partitionSize)
		for (auto y = 0; y < image.rows; y += partitionSize)
		{
			Mat partition = crop(image, { y,x }, { y + partitionSize, x + partitionSize });

			Scalar partitionAverage = averageColor(partition);

			// compare partitionAverage with mapped images's average;*first implementation using STL vector
			Scalar medColor = PictureTools::averageColor(partition);
			int sumMedColor = medColor[0] + medColor[1] + medColor[2];
			int bestMatch = std::abs((sumMedColor)-
				(dataPictures[0][0] + dataPictures[0][1] + dataPictures[0][2]));
			unsigned int bestIndex = 0;
			for (unsigned int index = 1; index < dataPictures.size(); ++index)
			{
				int curr = std::abs((sumMedColor)
					-(dataPictures[index][0] + dataPictures[index][1] + dataPictures[index][2]));
				if (bestMatch > curr)
				{
					bestIndex = index;
					bestMatch = curr;
				}
			}
			//std::cout << bestMatch << std::endl;

			// partition = closestMappedImage;

			for (auto i = x; i < x + partitionSize; ++i)
			{
				for (auto j = y; j < y + partitionSize; ++j)
				{
					if (i < result.cols && j < result.rows) {
						result.at<Vec3b>({ i,j })[0] = partition.at<Vec3b>({ i - x, j - y })[0];
						result.at<Vec3b>({ i,j })[1] = partition.at<Vec3b>({ i - x, j - y })[1];
						result.at<Vec3b>({ i,j })[2] = partition.at<Vec3b>({ i - x, j - y })[2];
					}
					else
					{
						//nimic
					}
				}
			}
		}

	return result;
}

