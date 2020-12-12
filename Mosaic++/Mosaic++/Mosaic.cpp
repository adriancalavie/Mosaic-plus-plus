#include "Mosaic.h"
#include "BasePictures.h"
#include "PictureTools.h"



uint32_t euclideanDistance(const cv::Scalar& firstColor, const cv::Scalar& secondColor)
{
	int8_t blueD, greenD, redD;
	blueD = firstColor[0] - secondColor[0];
	greenD = firstColor[1] - secondColor[1];
	redD = firstColor[2] - secondColor[2];
	return (blueD * blueD + greenD * greenD + redD * redD);
}

void Mosaic::alphaBlending(const std::unordered_map<cv::Scalar, std::string>& dataPictures)
{
	//trying to blend photo mosaic better
	cv::Mat foreground = cv::imread("");
	cv::Mat background = cv::imread("");
	cv::Mat alpha = cv::imread("");

	// Convert Mat to float data type
	foreground.convertTo(foreground, CV_32FC3);
	background.convertTo(background, CV_32FC3);

	// Normalize the alpha mask to keep intensity between 0 and 1
	alpha.convertTo(alpha, CV_32FC3, 1.0 / 255); // 

	// Storage for output image
	cv::Mat ouImage = cv::Mat::zeros(foreground.size(), foreground.type());

	multiply(alpha, foreground, foreground);

	multiply(cv::Scalar::all(1.0) - alpha, background, background);

	add(foreground, background, ouImage);

	imshow("alpha blended image", ouImage / 255);
	cv::waitKey(0);


}

cv::Mat Mosaic::makeMosaic(const std::unordered_map<cv::Scalar, std::string>& dataPictures, const cv::Mat& image, const uint8_t& partitionSize, const uint8_t& shape)
{
	assert(!image.empty());

	switch (shape)
	{
	case 1:
		return makeSquare(dataPictures, image, partitionSize);
	default:
		break;
	}
}

cv::Mat Mosaic::makeSquare(const std::unordered_map<cv::Scalar, std::string>& dataPictures, const cv::Mat& image, const uint8_t& partitionSize)
{
	bool v1 = image.cols % partitionSize == 0;
	bool v2 = image.rows % partitionSize == 0;
	bool v3 = v1 || v2;
	assert(v3);

	//if any pixels would remain after partitioning process, call abort;


	cv::Mat result(image.rows, image.cols, CV_8UC3);

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

	return result;
}

void Mosaic::replaceCell(cv::Mat& originalPicture, cv::Mat& mosaicPhoto, const Point& topL)
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
