#include "Mosaic.h"
#include "BasePictures.h"
#include "../../Mosaic++/Picture Tools/PictureTools.h"

#include <algorithm>
#include <random>
#include <time.h> 

//GETTER
uint8_t Mosaic::GetDefaultSize()
{
	return defaultSize;
}
//END

uint32_t EuclideanDistance(const cv::Scalar& firstColor, const cv::Scalar& secondColor)
{
	int8_t blueDistance, greenDistance, redDistance;
	blueDistance = firstColor[0] - secondColor[0];
	greenDistance = firstColor[1] - secondColor[1];
	redDistance = firstColor[2] - secondColor[2];
	return (blueDistance * blueDistance + greenDistance * greenDistance + redDistance * redDistance);
}

uint32_t RiemersmaDistance(const cv::Scalar& firstColor, const cv::Scalar& secondColor)
{
	uint32_t rmean = ((long)firstColor[0] + (long)secondColor[0]) / 2;
	uint32_t r = (long)firstColor[0] - (long)secondColor[0];
	uint32_t g = (long)firstColor[1] - (long)secondColor[1];
	uint32_t b = (long)firstColor[2] - (long)secondColor[2];

	return (((512 + rmean) * r * r) >> 8) + 4 * g * g + (((767 - rmean) * b * b) >> 8);
}

void Mosaic::AlphaBlending(cv::Mat& image, const cv::Scalar& color)
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

cv::Mat Mosaic::MakeMosaic(const cv::Mat& image, const BasePictures& basePictures, const Method& method, const Type& type, const uint8_t& partitionSize, bool blending)
{
	//TODO : change partitionSize to width and height pair
	assert(!image.empty());

	//std::cout << "entered make mosaic\n";

	cv::Mat copyOriginalImage = image;

	std::cout << "Rows,Cols(before): " << image.rows << " " << image.cols << std::endl;

	if (image.cols % partitionSize != 0 || image.rows % partitionSize != 0)
	{
		int v1 = image.cols / partitionSize * partitionSize;
		int v2 = image.rows / partitionSize * partitionSize;
		switch (method)
		{
		case Method::CROPPING:
			copyOriginalImage = std::move(pt::cropSquare(image, { 0,0 }, { v2,v1 }));
			break;
		case Method::RESIZING:
			copyOriginalImage = std::move(pt::resize(image, v1, v2, pt::Algorithm::BILINEAR_INTERPOLATION));
			break;
		default:
			break;
		}
	}

	std::cout << "Rows,Cols(after): " << image.rows << " " << image.cols << std::endl;

	auto midX = image.cols / 2;
	auto midY = image.rows / 2;

	std::cout << "X,Y(mid): " << midX << " " << midY << std::endl;



	switch (type)
	{
	case Type::SQUARE:
		return MakeRectangle(basePictures.GetMediumColor(), copyOriginalImage, blending, partitionSize);
	case Type::TRIANGLE:
		return MakeTriangle(basePictures.GetMediumColor(), copyOriginalImage, blending, partitionSize);
	case Type::DIAMOND:
		return MakeDiamond(basePictures.GetMediumColor(), copyOriginalImage, blending, partitionSize);
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



cv::Mat Mosaic::MakeRectangle(const std::unordered_map<cv::Scalar, std::string>& dataPictures, const cv::Mat& image, bool blending, const uint8_t& partitionSize)
{
	//can be used to create squares as well
	//removed redundant makeSquare function

	int v1 = image.cols % partitionSize;
	int v2 = image.rows % partitionSize;

	cv::Mat result(image.rows, image.cols, CV_8UC3);

	for (int x = 0; x < image.cols - 1; x += partitionSize)
		for (int y = 0; y < image.rows - 1; y += partitionSize)
		{
			cv::Scalar mediumColor = pt::averageColorRectangle(image, { y,x }, { partitionSize,partitionSize });
			std::string pictureName;

			cv::Mat cell = std::move(FindPictureWithColorMed(dataPictures, mediumColor, pictureName));
			cell = pt::resize(cell, partitionSize, partitionSize, pt::Algorithm::BILINEAR_INTERPOLATION);

			if (blending)
			{
				AlphaBlending(cell, mediumColor);
			}
			Mosaic::ReplaceCellRectangle(result, std::move(cell), std::make_pair(y, x));
		}
	return result;
}

cv::Mat Mosaic::MakeTriangle(const std::unordered_map<cv::Scalar, std::string>& dataPictures, const cv::Mat& image, bool blending, const uint8_t& partitionSize)
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

			cv::Scalar medColor;

			//first triangle

			if (type)
				medColor = pt::averageColorTriangle(image, { y,x }, { partitionSize, partitionSize }, 1);
			else
				medColor = pt::averageColorTriangle(image, { y,x }, { partitionSize, partitionSize }, 3);

			std::string pictureName = "";

			cv::Mat cell = std::move(FindPictureWithColorMed(dataPictures, medColor, pictureName));
			cell = pt::resize(cell, partitionSize, partitionSize, pt::Algorithm::BILINEAR_INTERPOLATION);
			if (blending)
			{
				AlphaBlending(cell, medColor);
			}
			if (type)
				Mosaic::ReplaceCellTriangle(result, std::move(cell), std::make_pair(y, x), 1, { 0,0 }, { partitionSize, partitionSize });
			else
				Mosaic::ReplaceCellTriangle(result, std::move(cell), std::make_pair(y, x), 3, { 0,0 }, { partitionSize, partitionSize });

			//second triangle
			if (type)
				medColor = pt::averageColorTriangle(image, { y,x }, { partitionSize, partitionSize }, 2);
			else
				medColor = pt::averageColorTriangle(image, { y,x }, { partitionSize, partitionSize }, 4);

			cell = std::move(FindPictureWithColorMed(dataPictures, medColor, pictureName));
			cell = std::move(pt::resize(cell, partitionSize, partitionSize));

			if (blending)
			{
				AlphaBlending(cell, medColor);
			}

			if (type)
				Mosaic::ReplaceCellTriangle(result, std::move(cell), std::make_pair(y, x), 2, { 0,0 }, { partitionSize, partitionSize });
			else
				Mosaic::ReplaceCellTriangle(result, std::move(cell), std::make_pair(y, x), 4, { 0,0 }, { partitionSize, partitionSize });
		}
	return result;
}

cv::Mat Mosaic::MakeDiamond(const BasePictures::map& dataPictures, const cv::Mat& image, bool blending, const uint8_t& partitionSize)
{

	cv::Mat result(image.rows, image.cols, CV_8UC3);

	for (auto x = 0; x < image.cols - 1; x += partitionSize)
		for (auto y = 0; y < image.rows - 1; y += partitionSize)
		{
			// compare partitionAverage with mapped images's average;
			cv::Scalar medColor = pt::averageColorRectangle(image, { y,x }, { partitionSize,partitionSize });

			std::string namePhoto = "";
			cv::Mat cell = std::move(FindPictureWithColorMed(dataPictures, medColor, namePhoto));
			cell = std::move(pt::resize(cell, partitionSize, partitionSize, pt::Algorithm::BILINEAR_INTERPOLATION));

			if (blending)
			{
				AlphaBlending(cell, medColor);
			}

			unsigned int xDiamond = x + (partitionSize + 1) / 2;
			unsigned int yDiamond = y + (partitionSize + 1) / 2;

			//Main cell
			Mosaic::ReplaceCellDiamond(result, std::move(cell), std::make_pair(y, x + (partitionSize) / 2));

			//Through diamonds
			if (x < image.cols - partitionSize - 1 && y < image.rows - partitionSize - 1)
				Mosaic::ReplaceCellDiamond(result, std::move(cell), std::make_pair(yDiamond, x + partitionSize));

			/*
			//LEFT MARGIN
			if (x == 0)
			{
				replaceCellTriangle(result, std::move(cell), { y, x }, 1, { 0, 0 }, { partitionSize , partitionSize / 2 });
				replaceCellTriangle(result, std::move(cell), { y, x }, 4, { 0, 0 }, { partitionSize , (partitionSize + 1) / 2 });
			}
			else
				//RIGHT
				if (x == image.cols - partitionSize)
				{
					replaceCellTriangle(result, std::move(cell), { y, xDiamond }, 3, { 0, 0 }, { partitionSize, partitionSize / 2 });
					replaceCellTriangle(result, std::move(cell), { yDiamond , xDiamond }, 2, { 0, 0 }, { partitionSize / 2, partitionSize / 2 });
				}

			//TOP
			if (y == 0)
			{
				replaceCellTriangle(result, std::move(cell), { y, xDiamond }, 3, { 0, 0 }, { partitionSize , partitionSize / 2 });
				replaceCellTriangle(result, std::move(cell), { y, x }, 1, { 0, 0 }, { partitionSize , partitionSize / 2 });
			}
			else
				//BOTTOM
				if (y == image.rows - partitionSize)
				{
					replaceCellTriangle(result, std::move(cell), { yDiamond, x }, 4, { 0, 0 }, { partitionSize / 2  , (partitionSize + 1) / 2 });
					replaceCellTriangle(result, std::move(cell), { yDiamond, xDiamond }, 2, { 0, 0 }, { partitionSize / 2 , partitionSize / 2 });
				}
				*/
		}


	Mosaic::MakeMargins(result, dataPictures, image, blending, partitionSize);

	return result;
}

void Mosaic::MakeMargins(cv::Mat& result, const BasePictures::map& dataPictures, const cv::Mat& image, bool blending, const uint8_t& partitionSize)
{

	unsigned int x_left = 0;
	unsigned int x_right = image.cols - partitionSize;


	for (auto y = 0; y < image.rows - 1; y += partitionSize)
	{
		cv::Scalar medColorLeft = pt::averageColorRectangle(image, { y,x_left }, { partitionSize,partitionSize });
		cv::Scalar medColorRight = pt::averageColorRectangle(image, { y,x_right }, { partitionSize,partitionSize });

		std::string namePhotoLeft = "";
		cv::Mat cellLeft = std::move(FindPictureWithColorMed(dataPictures, medColorLeft, namePhotoLeft));
		cellLeft = std::move(pt::resize(cellLeft, partitionSize, partitionSize, pt::Algorithm::BILINEAR_INTERPOLATION));

		std::string namePhotoRight = "";
		cv::Mat cellRight = std::move(FindPictureWithColorMed(dataPictures, medColorRight, namePhotoRight));
		cellRight = std::move(pt::resize(cellRight, partitionSize, partitionSize, pt::Algorithm::BILINEAR_INTERPOLATION));

		if (blending)
		{
			AlphaBlending(cellLeft, medColorLeft);
			AlphaBlending(cellRight, medColorRight);
		}

		unsigned int yDiamond = y + (partitionSize + 1) / 2;
		unsigned int xDiamond = x_right + (partitionSize + 1) / 2;

		ReplaceCellTriangle(result, std::move(cellLeft), { y, x_left }, 1, { x_left, x_left }, { partitionSize , partitionSize / 2 });
		ReplaceCellTriangle(result, std::move(cellLeft), { y, x_left }, 4, { x_left, x_left }, { partitionSize , (partitionSize + 1) / 2 });

		ReplaceCellTriangle(result, std::move(cellRight), { y, xDiamond }, 3, { x_left, x_left }, { partitionSize , partitionSize / 2 });
		ReplaceCellTriangle(result, std::move(cellRight), { yDiamond, xDiamond }, 2, { x_left, x_left }, { partitionSize / 2 , partitionSize / 2 });

	}

	unsigned int y_top = 0;
	unsigned int y_bottom = image.rows - partitionSize;

	for (auto x = 0; x < image.cols - 1; x += partitionSize)
	{
		cv::Scalar medColorTop = pt::averageColorRectangle(image, { y_top,x }, { partitionSize,partitionSize });
		cv::Scalar medColorBottom = pt::averageColorRectangle(image, { y_bottom,x }, { partitionSize,partitionSize });

		std::string namePhotoTop = "";
		cv::Mat cellTop = std::move(FindPictureWithColorMed(dataPictures, medColorTop, namePhotoTop));
		cellTop = std::move(pt::resize(cellTop, partitionSize, partitionSize, pt::Algorithm::BILINEAR_INTERPOLATION));

		std::string namePhotoBottom = "";
		cv::Mat cellBottom = std::move(FindPictureWithColorMed(dataPictures, medColorBottom, namePhotoBottom));
		cellBottom = std::move(pt::resize(cellBottom, partitionSize, partitionSize, pt::Algorithm::BILINEAR_INTERPOLATION));

		if (blending)
		{
			AlphaBlending(cellTop, medColorTop);
			AlphaBlending(cellBottom, medColorBottom);
		}

		unsigned int yDiamond = y_bottom + (partitionSize + 1) / 2;
		unsigned int xDiamond = x + (partitionSize + 1) / 2;

		ReplaceCellTriangle(result, std::move(cellTop), { y_top, xDiamond }, 3, { y_top, y_top }, { partitionSize , partitionSize / 2 });
		ReplaceCellTriangle(result, std::move(cellTop), { y_top, x }, 1, { y_top, y_top }, { partitionSize , partitionSize / 2 });

		ReplaceCellTriangle(result, std::move(cellBottom), { yDiamond, x }, 4, { y_top, y_top }, { partitionSize / 2  , (partitionSize + 1) / 2 });
		ReplaceCellTriangle(result, std::move(cellBottom), { yDiamond, xDiamond }, 2, { y_top, y_top }, { partitionSize / 2 , partitionSize / 2 });

	}

}


cv::Mat Mosaic::FindPictureWithColorMed(const BasePictures::map& dataPictures, const cv::Scalar& mediumColor, std::string& pictureDifferent)
{
	std::string pictureName;
	uint32_t closestDistance = USHRT_MAX;
	for (auto itr : dataPictures)
	{
		int currDistance = RiemersmaDistance(mediumColor, itr.first);

		if (currDistance < closestDistance && itr.second != pictureDifferent)
		{
			closestDistance = currDistance;
			pictureName = itr.second;
		}
	}
	pictureDifferent = pictureName;
	return BasePictures::ReadPhoto(pictureName);
}

void Mosaic::ReplaceCellRectangle(cv::Mat& originalPicture, cv::Mat&& mosaicPhoto, const Point& topL)
{
	assert(!originalPicture.empty());

	for (auto index_rows = 0; index_rows < mosaicPhoto.rows; index_rows++)
		for (auto index_cols = 0; index_cols < mosaicPhoto.cols; index_cols++)
		{
			originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[0] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[0]);
			originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[1] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[1]);
			originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[2] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[2]);
		}
}

void Mosaic::ReplaceCellTriangle(cv::Mat& originalPicture, cv::Mat&& mosaicPhoto, const Point& topL, const uint8_t& type, const Point& start, const Point& end)
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
		for (auto index_rows = start.first; index_rows < end.first; index_rows++)
			for (auto index_cols = start.second; index_cols <= end.second + start.second - index_rows - 1; index_cols++)
			{
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[0] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[0]);
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[1] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[1]);
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[2] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[2]);
			}
		break;
	case 2:
		for (auto index_rows = start.first; index_rows < end.first; index_rows++)
			for (auto index_cols = start.second + end.second - index_rows - 1; index_cols < end.second; index_cols++)
			{
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[0] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[0]);
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[1] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[1]);
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[2] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[2]);
			}
		break;
	case 3:
		for (auto index_rows = start.first; index_rows < end.first; index_rows++)
			for (auto index_cols = index_rows - start.first; index_cols < end.second; index_cols++)
			{
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[0] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[0]);
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[1] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[1]);
				originalPicture.at<cv::Vec3b>(index_rows + topL.first, index_cols + topL.second)[2] = std::move(mosaicPhoto.at<cv::Vec3b>(index_rows, index_cols)[2]);
			}
		break;
	case 4:
		for (auto index_rows = start.first; index_rows < end.first; index_rows++)
			for (auto index_cols = start.second; index_cols < end.second - end.first + index_rows; index_cols++)
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

void Mosaic::ReplaceCellDiamond(cv::Mat& originalPicture, cv::Mat&& mosaicPhoto, const Point& top)
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
	topD.first = top.first; //0
	topD.second = top.second - mosaicPhoto.cols / 2;
	start.first = mosaicPhoto.rows / 2;
	start.second = mosaicPhoto.cols / 2;
	end.first = mosaicPhoto.rows;
	end.second = mosaicPhoto.cols;
	ReplaceCellTriangle(originalPicture, std::move(mosaicPhoto), topD, 1, start, end);

	//2
	topD.first = top.first;
	topD.second = top.second - mosaicPhoto.cols / 2;
	start.first = 0;
	start.second = 0;
	end.first = mosaicPhoto.rows / 2;
	end.second = mosaicPhoto.cols / 2;
	ReplaceCellTriangle(originalPicture, std::move(mosaicPhoto), topD, 2, start, end);

	//3
	topD.first = top.first;
	topD.second = top.second - mosaicPhoto.cols / 2;
	start.first = mosaicPhoto.rows / 2;
	start.second = 0;
	end.first = mosaicPhoto.rows;
	end.second = mosaicPhoto.cols / 2;
	ReplaceCellTriangle(originalPicture, std::move(mosaicPhoto), topD, 3, start, end);

	//4
	topD = top;
	topD.second = top.second - mosaicPhoto.cols / 2;
	start.first = 0;
	start.second = mosaicPhoto.cols / 2;
	end.first = mosaicPhoto.rows / 2;
	end.second = mosaicPhoto.cols;
	ReplaceCellTriangle(originalPicture, std::move(mosaicPhoto), topD, 4, start, end);

}
