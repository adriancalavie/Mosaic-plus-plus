#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <unordered_map>
#include <filesystem>
#include "Data.h"
#include "../../Mosaic++/Picture Tools/PictureTools.h"
#include <regex>

#ifndef BASEPICTURES_EXPORTS
#define BASEPICTURES_API __declspec(dllexport)
#else
#define BASEPICTURES_API __declspec(dllimport)
#endif // !BASEPICTURES_EXPORTS

template <>
struct std::hash<cv::Scalar>
{
	size_t operator()(const cv::Scalar& k) const
	{
		size_t res = 17;
		res = res * 31 + std::hash<double>()(k.val[0]);
		res = res * 31 + std::hash<double>()(k.val[1]);
		res = res * 31 + std::hash<double>()(k.val[2]);
		return res;
	}
};

class BASEPICTURES_API BasePictures
{
public:
	using map = std::unordered_map<cv::Scalar, std::string>;

private:
	map m_mediumColor;
	std::string m_processedPictures;
	std::string m_dataBase;

public:
	BasePictures(const uint16_t & = 0);

	const map& GetMediumColor() const noexcept;
	const void CreatePictures(const std::string& ="");

	static cv::Mat ReadPhoto(const std::string & = "\n", const std::string & = Data::Defaults::PATH_PICTURES_FOR_MOSAIC);

	const std::string& GetFileDestination() const;
	void SetFileDestination(const std::string&);
	const uint16_t& GetNumberPictures() const;
	
	void AddBasePicturesMosaic();
};

