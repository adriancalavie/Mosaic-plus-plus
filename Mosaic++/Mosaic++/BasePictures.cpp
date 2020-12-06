#include "BasePictures.h"
#include "PictureTools.h"

BasePictures::BasePictures(const uint16_t& numberPictures)
{
	this->m_numberPictures = numberPictures;
	
	m_extension = ".jpg";
	m_source = "..//Base Pictures\\";
	m_processedPictures = "..//Pictures for mosaics\\";
}

const std::unordered_map<cv::Scalar, std::string>& BasePictures::GetMediumColor() const
{
	return m_mediumColor;
}

const void BasePictures::CreatePictures() 
{
	uint16_t count = 0;
	std::string image_path = m_source;
	image_path += std::to_string(count) + m_extension;

	while (count < this->m_numberPictures)
	{
		cv::Mat img = cv::imread(image_path, cv::IMREAD_COLOR);
		img=PictureTools::resize(img, 10, 10);
		this->m_mediumColor.insert(std::make_pair(PictureTools::averageColor(img), std::to_string(count) + m_extension));

		cv::imwrite(m_processedPictures+std::to_string(count)+m_extension, img);
		assert(!img.empty());
			
		++count;
		image_path = m_source + std::to_string(count) + m_extension;

	}
}


cv::Mat BasePictures::readPhoto(const std::string& pictureName, const std::string& fileName)
{
	cv::Mat img = cv::imread(fileName+ pictureName, cv::IMREAD_COLOR);
	assert(!img.empty());
	return img;
}

void BasePictures::setExtension(const std::string& extensionName)
{
	m_extension = extensionName;
}

void BasePictures::setPicturesNumber(const std::uint16_t& number)
{
	this->m_numberPictures = number;
}

const uint16_t& BasePictures::getPictureCount() const
{
	return this->m_numberPictures;
}

const std::string& BasePictures::getExtension() const
{
	return m_extension;
}

const std::string& BasePictures::getFileSource() const
{
	return m_source;
}

void BasePictures::setFileSource(const std::string& source)
{
	m_source = source;
}

const std::string& BasePictures::getFileDestination() const
{
	return m_processedPictures;
}

void BasePictures::setFileDestination(const std::string& source)
{
	m_processedPictures = source;
}

size_t BasePictures::HashKey::operator()(const cv::Scalar& toHash) const
{
		size_t res = 17;
		res = res * 31 + std::hash<double>()(toHash.val[0]);
		res = res * 31 + std::hash<double>()(toHash.val[1]);
		res = res * 31 + std::hash<double>()(toHash.val[2]);
		return res;
}
