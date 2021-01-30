#include "Mosaic.h"

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

cv::Mat Mosaic::MakeMosaic(const cv::Mat& image, const bp& basePictures, const Method& method, const Type& type, const uint8_t& partitionSize, const Algorithm& algorithm, const bool& blending)
{
	assert(!image.empty());

	cv::Mat copyOriginalImage = image;

	if (image.cols % partitionSize != 0 || image.rows % partitionSize != 0)
	{
		int v1 = image.cols / partitionSize * partitionSize;
		int v2 = image.rows / partitionSize * partitionSize;
		switch (method)
		{
		case Method::CROPPING:
			copyOriginalImage = std::move(pt::CropSquare(image, { 0,0 }, { v2,v1 }));
			break;
		case Method::RESIZING:
			copyOriginalImage = std::move(pt::Resize(image, v1, v2, pt::Algorithm::BILINEAR_INTERPOLATION));
			break;
		default:
			break;
		}
	}

	switch (type)
	{
	case Type::SQUARE:
		return MakeRectangle(basePictures.GetMediumColor(), copyOriginalImage, algorithm, blending, partitionSize);
	case Type::TRIANGLE:
		return MakeTriangle(basePictures.GetMediumColor(), copyOriginalImage, algorithm, blending, partitionSize);
	case Type::DIAMOND:
		return MakeDiamond(basePictures.GetMediumColor(), copyOriginalImage, algorithm, blending, partitionSize);
	default:
		break;
	}
}

cv::Mat Mosaic::MakeRectangle(const bp::map& dataPictures, const cv::Mat& image, const Algorithm& algorithm, const bool& blending, const uint8_t& partitionSize)
{
	cv::Mat result(image.rows, image.cols, CV_8UC3);

	auto test = [&](Point start, Point end) {

		for (int x = start.first; x < end.first - 1; x += partitionSize)
		{
			for (int y = start.second; y < end.second - 1; y += partitionSize)
			{
				cv::Scalar mediumColor = pt::AverageColorRectangle(image, { x,y }, { partitionSize,partitionSize });
				std::string pictureName;

				cv::Mat cell = std::move(FindPictureWithColorMed(dataPictures, mediumColor, pictureName, algorithm));
				cell = pt::Resize(cell, partitionSize, partitionSize, pt::Algorithm::BILINEAR_INTERPOLATION);

				if (blending)
				{
					pt::AlphaBlending(cell, mediumColor);
				}
				Mosaic::ReplaceCellRectangle(result, std::move(cell), std::make_pair(x, y));


			}
		}
	};
	Point start1, start2, start3, start4;
	Point end1, end2, end3, end4;

	int partitii_rand = image.rows / partitionSize;
	int partitii_coloane = image.cols / partitionSize;

	int randuri = partitii_rand / 2 * partitionSize;
	int coloane = partitii_coloane / 2 * partitionSize;

	start1 = { 0,0 };
	end1 = { randuri, coloane };

	start2 = { 0,coloane };
	end2 = { randuri, image.cols };

	start3 = { randuri,0 };
	end3 = { image.rows, coloane };

	start4 = { randuri,coloane };
	end4 = { image.rows, image.cols };

	std::thread t1{ test, std::ref(start1), std::ref(end1) };
	std::thread t2{ test, std::ref(start2), std::ref(end2) };
	std::thread t3{ test, std::ref(start3), std::ref(end3) };
	std::thread t4{ test, std::ref(start4), std::ref(end4) };


	t1.join();
	t2.join();
	t3.join();
	t4.join();

	//test({ 0,0 }, { image.rows, image.cols });

	return result;
}

cv::Mat Mosaic::MakeTriangle(const bp::map& dataPictures, const cv::Mat& image, const Algorithm& algorithm, const bool& blending, const uint8_t& partitionSize)
{
	cv::Mat result(image.rows, image.cols, CV_8UC3);
	auto test = [&](Point start, Point end) {

		for (int x = start.first; x < end.first - 1; x += partitionSize)
		{
			for (int y = start.second; y < end.second - 1; y += partitionSize)
			{
				//random type triangle 1|2 or 3|4
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<> dis(0, 1);
				uint8_t type = dis(gen);

				cv::Scalar medColor;

				//first triangle

				if (type)
					medColor = pt::AverageColorTriangle(image, { x,y }, { partitionSize, partitionSize }, 1);
				else
					medColor = pt::AverageColorTriangle(image, { x,y }, { partitionSize, partitionSize }, 3);

				std::string pictureName = "";

				cv::Mat cell = std::move(FindPictureWithColorMed(dataPictures, medColor, pictureName, algorithm));
				cell = pt::Resize(cell, partitionSize, partitionSize, pt::Algorithm::BILINEAR_INTERPOLATION);
				if (blending)
				{
					pt::AlphaBlending(cell, medColor);
				}
				if (type)
					Mosaic::ReplaceCellTriangle(result, std::move(cell), std::make_pair(x, y), 1, { 0,0 }, { partitionSize, partitionSize });
				else
					Mosaic::ReplaceCellTriangle(result, std::move(cell), std::make_pair(x, y), 3, { 0,0 }, { partitionSize, partitionSize });

				//second triangle
				if (type)
					medColor = pt::AverageColorTriangle(image, { x,y }, { partitionSize, partitionSize }, 2);
				else
					medColor = pt::AverageColorTriangle(image, { x,y }, { partitionSize, partitionSize }, 4);

				cell = std::move(FindPictureWithColorMed(dataPictures, medColor, pictureName, algorithm));
				cell = std::move(pt::Resize(cell, partitionSize, partitionSize));

				if (blending)
				{
					pt::AlphaBlending(cell, medColor);
				}

				if (type)
					Mosaic::ReplaceCellTriangle(result, std::move(cell), std::make_pair(x, y), 2, { 0,0 }, { partitionSize, partitionSize });
				else
					Mosaic::ReplaceCellTriangle(result, std::move(cell), std::make_pair(x, y), 4, { 0,0 }, { partitionSize, partitionSize });
			}
		}
	};


	Point start1, start2, start3, start4;
	Point end1, end2, end3, end4;

	int partitii_rand = image.rows / partitionSize;
	int partitii_coloane = image.cols / partitionSize;

	int randuri = partitii_rand / 2 * partitionSize;
	int coloane = partitii_coloane / 2 * partitionSize;

	start1 = { 0,0 };
	end1 = { randuri, coloane };

	start2 = { 0,coloane };
	end2 = { randuri, image.cols };

	start3 = { randuri,0 };
	end3 = { image.rows, coloane };

	start4 = { randuri,coloane };
	end4 = { image.rows, image.cols };

	std::thread t1{ test, std::ref(start1), std::ref(end1) };
	std::thread t2{ test, std::ref(start2), std::ref(end2) };
	std::thread t3{ test, std::ref(start3), std::ref(end3) };
	std::thread t4{ test, std::ref(start4), std::ref(end4) };


	t1.join();
	t2.join();
	t3.join();
	t4.join();
	return result;
}

cv::Mat Mosaic::MakeDiamond(const bp::map& dataPictures, const cv::Mat& image, const Algorithm& algorithm, const bool& blending, const uint8_t& partitionSize)
{
	cv::Mat result(image.rows, image.cols, CV_8UC3);
	auto test = [&](Point start, Point end) {

		for (int x = start.first; x < end.first - 1; x += partitionSize)
		{
			for (int y = start.second; y < end.second - 1; y += partitionSize)
			{
				// compare partitionAverage with mapped images's average;
				cv::Scalar medColor = pt::AverageColorRectangle(image, { x,y }, { partitionSize, partitionSize });

				std::string namePhoto = "";
				cv::Mat cell = std::move(FindPictureWithColorMed(dataPictures, medColor, namePhoto, algorithm));
				cell = std::move(pt::Resize(cell, partitionSize, partitionSize, pt::Algorithm::BILINEAR_INTERPOLATION));

				if (blending)
				{
					pt::AlphaBlending(cell, medColor);
				}
				Mosaic::ReplaceCellDiamond(result, std::move(cell), std::make_pair(x, y + (partitionSize) / 2));
			}
		}
		for (int x = start.first+ (partitionSize + 1) / 2; x < end.first  -partitionSize - 1; x += partitionSize)
		{
			for (int y = start.second+partitionSize/2; y < end.second - partitionSize - 1; y += partitionSize)
			{
				// compare partitionAverage with mapped images's average;
				cv::Scalar medColor = pt::AverageColorRectangle(image, { x,y }, { partitionSize, partitionSize });

				std::string namePhoto = "";
				cv::Mat cell = std::move(FindPictureWithColorMed(dataPictures, medColor, namePhoto, algorithm));
				cell = std::move(pt::Resize(cell, partitionSize, partitionSize, pt::Algorithm::BILINEAR_INTERPOLATION));

				if (blending)
				{
					pt::AlphaBlending(cell, medColor);
				}
				Mosaic::ReplaceCellDiamond(result, std::move(cell), std::make_pair(x, y + (partitionSize) / 2));
			}
		}
	};
	Point start1, start2, start3, start4;
	Point end1, end2, end3, end4;

	int partitii_rand = image.rows / partitionSize;
	int partitii_coloane = image.cols / partitionSize;

	int randuri = partitii_rand / 2 * partitionSize;
	int coloane = partitii_coloane / 2 * partitionSize;

	start1 = { 0,0 };
	end1 = { randuri+partitionSize, coloane+partitionSize };

	start2 = { 0,coloane };
	end2 = { randuri, image.cols };

	start3 = { randuri,0 };
	end3 = { image.rows, coloane };

	start4 = { randuri-partitionSize,coloane-partitionSize };
	end4 = { image.rows, image.cols };

	std::thread t1{ test, std::ref(start1), std::ref(end1) };
	std::thread t2{ test, std::ref(start2), std::ref(end2) };
	std::thread t3{ test, std::ref(start3), std::ref(end3) };
	std::thread t4{ test, std::ref(start4), std::ref(end4) };
	std::thread t5{ MakeMargins , std::ref(result), std::ref(dataPictures), std::ref(image), std::ref(algorithm), std::ref(blending), std::ref(partitionSize) };

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();

	return result;
}

void Mosaic::MakeMargins(cv::Mat& result, const bp::map& dataPictures, const cv::Mat& image, const Algorithm& algorithm, const bool& blending, const uint8_t& partitionSize)
{

	unsigned int x_left = 0;
	unsigned int x_right = image.cols - partitionSize;


	for (int y = 0; y < image.rows - 1; y += partitionSize)
	{
		cv::Scalar medColorLeft = pt::AverageColorRectangle(image, { y,x_left }, { partitionSize,partitionSize });
		cv::Scalar medColorRight = pt::AverageColorRectangle(image, { y,x_right }, { partitionSize,partitionSize });

		std::string namePhotoLeft = "";
		cv::Mat cellLeft = std::move(FindPictureWithColorMed(dataPictures, medColorLeft, namePhotoLeft, algorithm));
		cellLeft = std::move(pt::Resize(cellLeft, partitionSize, partitionSize, pt::Algorithm::BILINEAR_INTERPOLATION));

		std::string namePhotoRight = "";
		cv::Mat cellRight = std::move(FindPictureWithColorMed(dataPictures, medColorRight, namePhotoRight, algorithm));
		cellRight = std::move(pt::Resize(cellRight, partitionSize, partitionSize, pt::Algorithm::BILINEAR_INTERPOLATION));

		if (blending)
		{
			pt::AlphaBlending(cellLeft, medColorLeft);
			pt::AlphaBlending(cellRight, medColorRight);
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

	for (int x = 0; x < image.cols - 1; x += partitionSize)
	{
		cv::Scalar medColorTop = pt::AverageColorRectangle(image, { y_top,x }, { partitionSize,partitionSize });
		cv::Scalar medColorBottom = pt::AverageColorRectangle(image, { y_bottom,x }, { partitionSize,partitionSize });

		std::string namePhotoTop = "";
		cv::Mat cellTop = std::move(FindPictureWithColorMed(dataPictures, medColorTop, namePhotoTop, algorithm));
		cellTop = std::move(pt::Resize(cellTop, partitionSize, partitionSize, pt::Algorithm::BILINEAR_INTERPOLATION));

		std::string namePhotoBottom = "";
		cv::Mat cellBottom = std::move(FindPictureWithColorMed(dataPictures, medColorBottom, namePhotoBottom, algorithm));
		cellBottom = std::move(pt::Resize(cellBottom, partitionSize, partitionSize, pt::Algorithm::BILINEAR_INTERPOLATION));

		if (blending)
		{
			pt::AlphaBlending(cellTop, medColorTop);
			pt::AlphaBlending(cellBottom, medColorBottom);
		}

		unsigned int yDiamond = y_bottom + (partitionSize + 1) / 2;
		unsigned int xDiamond = x + (partitionSize + 1) / 2;

		ReplaceCellTriangle(result, std::move(cellTop), { y_top, xDiamond }, 3, { y_top, y_top }, { partitionSize , partitionSize / 2 });
		ReplaceCellTriangle(result, std::move(cellTop), { y_top, x }, 1, { y_top, y_top }, { partitionSize , partitionSize / 2 });

		ReplaceCellTriangle(result, std::move(cellBottom), { yDiamond, x }, 4, { y_top, y_top }, { partitionSize / 2  , (partitionSize + 1) / 2 });
		ReplaceCellTriangle(result, std::move(cellBottom), { yDiamond, xDiamond }, 2, { y_top, y_top }, { partitionSize / 2 , partitionSize / 2 });

	}
}


cv::Mat Mosaic::FindPictureWithColorMed(const bp::map& dataPictures, const cv::Scalar& mediumColor, std::string& pictureDifferent, const Algorithm& algorithm)
{
	std::string pictureName;
	uint32_t closestDistance = INT_MAX;
	for (auto itr : dataPictures)
	{
		uint32_t currDistance;
		if (algorithm == Algorithm::RIEMERSMA)
			currDistance = RiemersmaDistance(mediumColor, itr.first);
		else
			currDistance = EuclideanDistance(mediumColor, itr.first);

		if (currDistance < closestDistance && itr.second != pictureDifferent)
		{
			closestDistance = currDistance;
			pictureName = itr.second;
		}

	}

	pictureDifferent = pictureName;
	return bp::ReadPhoto(pictureName);
}

void Mosaic::ReplaceCellRectangle(cv::Mat& originalPicture, cv::Mat&& mosaicPhoto, const Point& topL)
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
