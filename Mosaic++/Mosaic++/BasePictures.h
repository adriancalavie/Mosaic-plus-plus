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
#include "../Picture Tools/PictureTools.h"

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

class BasePictures
{
public:
	using map = std::unordered_map<cv::Scalar, std::string>;

private:
	std::uint16_t m_numberPictures;
	map m_mediumColor;


	std::string m_source;
	std::string m_extension;
	std::string m_processedPictures;
	std::string m_dataBase;

public:
	BasePictures(const uint16_t & = 0);

	const map& GetMediumColor() const noexcept;
	const void CreatePictures();

	void SetFolder(const std::string&);
	void SetPicturesNumber(const std::uint16_t&);

	const uint16_t& GetPictureCount() const;
	static cv::Mat ReadPhoto(const std::string & = "\n", const std::string & = Data::Defaults::PATH_PICTURES_FOR_MOSAIC);

public:
	const std::string& GetFileSource() const;
	void SetFileSource(const std::string&);

	const std::string& GetFileDestination() const;
	void SetFileDestination(const std::string&);

	const std::string& GetExtension() const;
	void SetExtension(const std::string&);

	const uint16_t& GetNumberPictures() const;
	void SetNumberPictures(const uint16_t&);

	void SetDataBase(const std::string&);

	void AddPicturesMosaic(const bool&);

};

