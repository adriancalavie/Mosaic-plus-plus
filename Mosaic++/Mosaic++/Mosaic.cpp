#include "Mosaic.h"
#include "BasePictures.h"
#include "PictureTools.h"

int euclideanDistance(const Scalar& firstColor, const Scalar& secondColor)
{
	int blueD, greenD, redD;
	blueD = firstColor[0] - secondColor[0];
	greenD = firstColor[1] - secondColor[1];
	redD = firstColor[2] - secondColor[2];
	return (blueD * blueD + greenD * greenD + redD * redD);
}
Mat Mosaic::makeMosaic(const std::unordered_map<cv::Scalar, std::string>& dataPictures, Mat& image, const uint8_t& partitionSize)
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
			// compare partitionAverage with mapped images's average;*first implementation using STL vector
			Scalar medColor = PictureTools::averageColor(image, { y,x }, { partitionSize,partitionSize });

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

			Mat testPhoto;
			testPhoto = BasePictures::readPhoto(pictureName);

			for (auto i = x; i < x + partitionSize; ++i)
			{
				for (auto j = y; j < y + partitionSize; ++j)
				{
					if (i < result.cols && j < result.rows) {
						result.at<Vec3b>({ i,j })[0] = testPhoto.at<Vec3b>({ i - x, j - y })[0];
						result.at<Vec3b>({ i,j })[1] = testPhoto.at<Vec3b>({ i - x, j - y })[1];
						result.at<Vec3b>({ i,j })[2] = testPhoto.at<Vec3b>({ i - x, j - y })[2];
					}
					else
					{
						//nimic
					}
				}
			}
			//PictureTools::replaceCell(result, testPhoto, std::make_pair(x, y));
		}

	return result;
}

void Mosaic::replaceCell(Mat& originalPicture, const Mat& mosaicPhoto, const std::pair<int, int>& topL)
{
	assert(originalPicture.empty());
	for (int index_rows = 0; index_rows < mosaicPhoto.rows; index_rows++)
		for (int index_cols = 0; index_cols < mosaicPhoto.cols; index_cols++)
		{
			originalPicture.at<Vec3b>(index_rows + topL.first, index_cols + topL.second)[0] = mosaicPhoto.at<Vec3b>(index_rows, index_cols)[0];
			originalPicture.at<Vec3b>(index_rows + topL.first, index_cols + topL.second)[1] = mosaicPhoto.at<Vec3b>(index_rows, index_cols)[1];
			originalPicture.at<Vec3b>(index_rows + topL.first, index_cols + topL.second)[2] = mosaicPhoto.at<Vec3b>(index_rows, index_cols)[2];
		}
}
