#include "BasePictures.h"
#include "PictureTools.h"


BasePictures::BasePictures(const uint16_t& numberPictures, const std::string& fileBasePictures, const std::string& filePicturesForMosaic, const std::string& extension)
{
	m_numberPictures = numberPictures;
	m_extension = extension;
	m_source = fileBasePictures;
	m_processedPictures = filePicturesForMosaic;
}

const std::unordered_map<cv::Scalar, std::string>& BasePictures::GetMediumColor() const
{
	return m_mediumColor;
}

const void BasePictures::CreatePictures()
{
	std::ofstream out(m_dataBase);
	for (const auto& entry : std::filesystem::directory_iterator(m_source))
	{

		cv::Mat img = cv::imread(entry.path().string(), cv::IMREAD_COLOR);

		cv::Scalar aux = PictureTools::averageColor(img);
		img = PictureTools::resize(img, 10, 10);
		out << aux[0] << " "
			<< aux[1] << " "
			<< aux[2] << " "
			<< entry.path().string().substr(m_source.size() + 1) << std::endl;
		cv::imwrite(m_processedPictures + entry.path().string().substr(m_source.size()), img);
		assert(!img.empty());
	}
	out.close();
}


cv::Mat BasePictures::readPhoto(const std::string& pictureName, const std::string& fileName)
{
	cv::Mat img = std::move(cv::imread(fileName + pictureName, cv::IMREAD_COLOR));
	assert(!img.empty());
	return img;
	
}

void BasePictures::setExtension(const std::string& extensionName)
{
	m_extension = extensionName;
}

const uint16_t& BasePictures::getNumberPictures() const
{
	return m_numberPictures;
}

void BasePictures::setNumberPictures(const uint16_t& number)
{
	m_numberPictures = number;
}

void BasePictures::setDataBase(const std::string& dataBase)
{
	m_dataBase = dataBase;
}

void BasePictures::setFolder(const std::string& filename)
{
	this->m_source = filename;
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


void BasePictures::addPicturesMosaic(const bool& modify)
{
	if (modify)
	{
		CreatePictures();
	}
	std::ifstream in(m_dataBase);
	for (int i = 0; i < m_numberPictures; ++i)
	{
		std::string line;
		std::getline(in, line);
		std::stringstream ss(line);
		std::string item;
		cv::Scalar aux;

		std::getline(ss, item, ' ');
		aux[0] = std::move(std::stod(item));
		std::getline(ss, item, ' ');
		aux[1] = std::move(std::stod(item));
		std::getline(ss, item, ' ');
		aux[2] = std::move(std::stod(item));
		std::getline(ss, item, ' ');
		cv::Mat validation;
		validation = cv::imread(m_processedPictures + item, cv::IMREAD_COLOR);
		if (!validation.empty())
			m_mediumColor.insert(std::make_pair(std::move(aux), std::move(item)));
	}
	in.close();
}