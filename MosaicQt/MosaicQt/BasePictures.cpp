#include "BasePictures.h"

BasePictures::BasePictures() :
	m_processedPictures(Data::Defaults::PATH_PICTURES_FOR_MOSAIC),
	m_dataBase(Data::Defaults::PATH_DATA_BASE_FILE)
{
	// EMPTY
}

[[nodiscard]] const BasePictures::map& BasePictures::GetMediumColor() const noexcept
{
	return m_mediumColor;
}

void BasePictures::CreatePictures(const std::string& dirName)
{
	std::ofstream out;
	out.open(m_dataBase, std::ios_base::app);
	for (const auto& entry : std::filesystem::directory_iterator(dirName))
	{
		if (cv::Mat img = cv::imread(entry.path().string(), cv::IMREAD_COLOR); !img.empty()) {
			img = PictureTools::Resize(img, 100, 100);
			cv::Scalar aux = PictureTools::AverageColorRectangle(img, { 0, 0 }, { img.rows, img.cols });
			out << aux[0] << " "
				<< aux[1] << " "
				<< aux[2] << " "
				<< entry.path().string().substr(dirName.size() + 1) << std::endl;
			
			cv::imwrite(m_processedPictures + entry.path().string().substr(dirName.size()), img);

			m_mediumColor.insert({ aux, { std::move(img), entry.path().string().substr(dirName.size() + 1)} });
		}
	}
	out.close();
}

cv::Mat BasePictures::ReadPhoto(const std::string& pictureName, const std::string& fileName)
{
	cv::Mat img = std::move(cv::imread(fileName + pictureName, cv::IMREAD_COLOR));
	assert(!img.empty());
	return img;
}

[[nodiscard]] uint16_t BasePictures::GetNumberPictures() const
{
	return m_mediumColor.size();
}

[[nodiscard]] const std::string& BasePictures::GetFileDestination() const
{
	return m_processedPictures;
}

void BasePictures::SetFileDestination(const std::string& source)
{
	m_processedPictures = source;
}

void BasePictures::AddBasePicturesMosaic()
{
	for (std::ifstream in(m_dataBase); !in.eof();)
	{
		std::string line;
		std::getline(in, line);
		if (std::regex_match(line, std::regex(R"(\d*.\d* \d*.\d* \d*.\d* .*)"))) {
			std::stringstream ss(line);
			std::string item;
			cv::Scalar aux;

			std::getline(ss, item, ' ');
			aux[0] = std::stod(item);
			std::getline(ss, item, ' ');
			aux[1] = std::stod(item);
			std::getline(ss, item, ' ');
			aux[2] = std::stod(item);
			std::getline(ss, item);
			cv::Mat validation;
			validation = cv::imread(m_processedPictures + item, cv::IMREAD_COLOR);
			if (!validation.empty())
				m_mediumColor.insert({ aux,{validation, std::move(item)} });
		}
	}
}