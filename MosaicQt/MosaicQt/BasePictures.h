#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <unordered_map>
#include <filesystem>
#include <regex>

#include "Data.h"
#include "../Picture Tools/PictureTools.h"


template <>
struct std::hash<cv::Scalar>
{
	size_t operator()(const cv::Scalar& k) const noexcept
	{
		size_t res = 17;
		res = res * 31 + std::hash<double>()(k.val[0]);
		res = res * 31 + std::hash<double>()(k.val[1]);
		res = res * 31 + std::hash<double>()(k.val[2]);
		return res;
	}
};

class BasePictures
{

public:
	using value = std::pair<cv::Mat, std::string>;
	using map = std::unordered_map<cv::Scalar, value>;

private:
	map m_mediumColor;
	std::string m_processedPictures;
	std::string m_dataBase;

public:
	BasePictures();

	[[nodiscard]] const map& GetMediumColor() const noexcept;
	void CreatePictures(const std::string& dirName = "");

	static cv::Mat ReadPhoto(const std::string & = "\n", const std::string & = Data::Defaults::PATH_PICTURES_FOR_MOSAIC);

	[[nodiscard]] const std::string& GetFileDestination() const;
	void SetFileDestination(const std::string&);
	[[nodiscard]] uint16_t GetNumberPictures() const;

	void AddBasePicturesMosaic();
};


