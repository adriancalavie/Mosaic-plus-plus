#include "Command.h"
#include "BasePictures.h"

const std::string Command::mosaicDocs = "[docs]make_mosaic.txt";
const std::string Command::selectDocs = "[docs]select_images_folder.txt";

Command::Command(std::vector<std::string> args)
{

	if (args[0] == "mkmosaic")
	{
		m_type = CommandType::mosaic;

		switch (args.size())
		{
		case 1:
			throw "too few arguments";
			break;
		case 2:
			makeMosaic(args[1], std::nullopt);
			m_help = HelpType::none;

			break;
		case 3:
			if (args[2] == "-help")
				m_help = HelpType::condensed;
			else if (args[2] == "--help")
			{
				m_help = HelpType::expanded;
			}
			else if (std::regex_match(args[2], std::regex("[0-9]*")))
			{
				makeMosaic(args[1], std::stoi(args[2]));
				m_help = HelpType::none;
			}
			else throw "unknown argument";
			break;

		default:
			throw "too many arguments";
			break;
		}
	}
	else if (args[0] == "setimgpool")
	{
		m_type = CommandType::folder;

		if (args.size() > 2) throw "too many arguments";
		else
		{
			selectFolder(args[1]);
		}
	}
	else throw "unknown command";

	if (m_help != HelpType::none)
		help();
}

bool Command::help()
{
	switch (m_type)
	{
	case CommandType::mosaic:
	{
		std::ifstream in(mosaicDocs);
		std::string documentation;

		in >> documentation;
		std::cout << documentation;
	}
	break;

	case CommandType::folder:
	{
		std::ifstream in(selectDocs);
		std::string documentation;

		in >> documentation;
		std::cout << documentation;
	}
	break;

	case CommandType::unknown:
		throw "unkown command";
		break;
	}
}

void Command::makeMosaic(argument path, const std::optional<uint8_t>& partitionSize) const
{
	cv::Mat input = imread(path);
	cv::Mat output;

	BasePictures pool(1000, ".jpg");
	pool.CreatingPicturesForMosaics(picturesPath);

	if (partitionSize.has_value())
	{
		output = Mosaic::makeMosaic(pool.GetMediumColor(), input, partitionSize.value());
	}
	else
	{
		output = Mosaic::makeMosaic(pool.GetMediumColor(), input);
	}

	cv::imshow("Resulted Image", output);
}


void Command::selectFolder(argument path) const
{

}

void Command::selectPicturesExtension(argument path) const
{
	//usint setPictureExtension from BasePictures
}
