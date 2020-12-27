#include "CommandLineInterface.h"


const std::unordered_map<std::string, CommandLineInterface::Parameter> CommandLineInterface::FLAGS = {
	{"--shape",				Parameter::TYPE},
	{"-s",					Parameter::TYPE},

	{"--partiton",			Parameter::SIZE},
	{"-p",					Parameter::SIZE},

	{"--method",			Parameter::METHOD},
	{"-m",					Parameter::METHOD},

	{"--extension",			Parameter::EXTENSION},
	{"-e",					Parameter::EXTENSION},

	{"--directory",			Parameter::PATH},
	{"-d",					Parameter::PATH},

	{"--help",				Parameter::HELP},
	{"-h",					Parameter::HELP},

	{"--version",			Parameter::VERSION},
	{"-v",					Parameter::VERSION}
};

std::unordered_map<CommandLineInterface::PathType, std::string> CommandLineInterface::PATHS = {
	{PathType::SOURCE,						    Data::Defaults::PARENT_PATH},

	{PathType::PROCESSED,						Data::Defaults::PARENT_PATH},

	{PathType::DATABASE,						Data::Defaults::PARENT_PATH},

	{PathType::NUMBER_PHOTOS,					"10"}

};

const std::unordered_set<std::string> CommandLineInterface::COMMANDS = { "make", "set_img_pool_dir" };


const std::unordered_set<std::string> CommandLineInterface::KNOWN_EXTENSIONS = { "jpg", "png" };

CommandLineInterface::CommandLineInterface(int argc, char* args[])
{
	if (argc < 2)
	{
		std::cerr << Data::Errors::PARAMETER_COUNT;
	}
	else
		//if we have enough arguments
	{
		auto command = COMMANDS.find(std::string(args[1]));
		if (command != COMMANDS.end())
			//if we have a valid command as a first argument
		{

			bool hasParams = false;
			std::unordered_map<std::string, std::string> flagsParams;
			std::vector<std::string> commandParams;

			for (int i = 2; i < argc; ++i)
				//we search for either flags or parameters
			{
				std::string argument(args[i]);

				if (FLAGS.find(argument) != FLAGS.end())
					//if we found a flag
				{

					if (FLAGS.at(argument) == Parameter::HELP)//if -h
					{
						commandController(*command);
						return;
					}
					else
					{
						if (hasParams)
							//if the flag requires parameters
						{
							if (i < argc)
								//if we have enough arguments
								if (inCheck(argument, args[++i]))
									//if the flag parameter is valid
									flagsParams.insert({ argument, args[i] });
						}
						else
						{
							std::cerr << Data::Errors::WRONG_ARGUMENT_ORDER;
						}
					}

				}
				else/* if (COMMANDS.find(std::string(args[i])) != COMMANDS.end())*/
				{
					//else, if we find a command parameter
					if (isPath(argument) && isFile(argument))//TODO: optimise for set_img_pool_dir
						//if we have an image as an input
					{
						commandParams.push_back(argument);
						hasParams = true;
					}
					else
					{
						std::cerr << Data::Errors::WRONG_INPUT;
						return;
					}
				}
			}

			if (flagsParams.size() > 0)
				commandController(*command, commandParams, flagsParams);
			else
				commandController(*command, commandParams);
		}
		else
		{
			if (FLAGS.find(std::string(args[1])) != FLAGS.end())
				//if instead of a command, we find a help or version request
			{
				if (FLAGS.at(std::string(args[1])) == Parameter::HELP)
				{
					std::cout << Data::Info::HELP_LEVEL.at(Data::HelpTypes::GENERAL_HELP);
				}
				else if (FLAGS.at(std::string(args[1])) == Parameter::VERSION)
				{
					std::cout << Data::Info::VERSION;
				}
			}
			else
			{
				std::cerr << Data::Errors::UNKNOWN_COMMAND;
			}
		}
	}

}

void CommandLineInterface::make(const std::vector<std::string>& params)
{

	BasePictures test;
	test.setPicturesNumber(1000);
	test.setFileSource("D:\\Mosaic++\\Mosaic++\\Base pictures\\");
	test.setDataBase("D:\\Mosaic++\\Mosaic++\\Mosaic++\\data_base.txt");
	test.addPicturesMosaic(false);


	for (const auto& image : params)
	{
		cv::Mat input = cv::imread(image, cv::IMREAD_COLOR);

		/*cv::imshow("Original", input);*/
		cv::waitKey(0);

		cv::Mat output = Mosaic::makeMosaic(input, test, Method::RESIZING, Type::SQUARE, 10);

		cv::imwrite("D:\\Mosaic++\\Mosaic++\\Resulting pictures\\" + std::to_string(test.getNumberPictures()) + test.getExtension(), output);

		cv::imshow("Made with Mosaic++", output);
		cv::waitKey(0);
	}
}

void CommandLineInterface::make(const std::vector<std::string>& params, const std::unordered_map<std::string, std::string>& flags)
{
	/*std::cout << "entered on custom make";*/

	std::string extension = "jpg";
	Type shape = Type::RECTANGLE;
	Method methodMosaication = Method::RESIZING;
	int inputSize;
	uint8_t partitionSize = Mosaic::getDefaultSize();
	std::string directory = "D:\\Mosaic++\\Mosaic++\\Resulting pictures\\";

	for (auto& flag : flags)
	{
		switch (FLAGS.find(flag.first)->second)
		{
		case Parameter::TYPE:
			if (flag.second == "rectangle")
				shape = Type::RECTANGLE;
			else if (flag.second == "diamond")
				shape = Type::DIAMOND;
			else if (flag.second == "triangle")
				shape = Type::TRIANGLE;
			else
				std::cerr << Data::Errors::WRONG_ARGUMENT;
			break;

		case Parameter::SIZE:
			inputSize = std::move(std::stoi(flag.second));
			if (inputSize <= UINT8_MAX)
				partitionSize = inputSize;
			else
				std::cerr << Data::Errors::OUT_OF_BOUNDS;
			break;

		case Parameter::METHOD:
			if (flag.second == "cropp")
				methodMosaication = Method::CROPPING;
			else if (flag.second == "resize")
				methodMosaication = Method::RESIZING;
			else
				std::cerr << Data::Errors::WRONG_ARGUMENT;
			break;

		case Parameter::EXTENSION:
			if (KNOWN_EXTENSIONS.find(flag.second) != KNOWN_EXTENSIONS.end())
			{
				extension = flag.second;
			}
			else
				std::cerr << Data::Errors::WRONG_ARGUMENT;
			break;

		case Parameter::PATH:

			if (isPath(flag.second))
			{
				directory = flag.second;
			}
			else
				std::cerr << Data::Errors::WRONG_ARGUMENT;
			break;
		default:
			std::cerr << "\nHow on earth did you end up in here?\n";
			break;
		}


	}

	BasePictures imagesPool;
	imagesPool.setPicturesNumber(std::move(std::stoi(CommandLineInterface::PATHS.at(CommandLineInterface::PathType::NUMBER_PHOTOS))));
	imagesPool.setFileSource(CommandLineInterface::PATHS.at(CommandLineInterface::PathType::SOURCE));//D:\\Mosaic++\\Mosaic++\\Base pictures\\;
	imagesPool.setDataBase(CommandLineInterface::PATHS.at(CommandLineInterface::PathType::DATABASE));//D:\\Mosaic++\\Mosaic++\\Mosaic++\\data_base.txt;
	imagesPool.addPicturesMosaic(false);

	for (const auto& image : params)
	{
		cv::Mat input = cv::imread(image, cv::IMREAD_COLOR);

		/*cv::imshow("Original", input);*/
		cv::waitKey(0);

		input = PictureTools::resize(input, 1200, 700);
		cv::Mat output = Mosaic::makeMosaic(input, imagesPool, methodMosaication, shape, partitionSize);

		cv::imwrite(directory + "Output." + extension, output);

		cv::imshow("Made with Mosaic++", output);
		cv::waitKey(0);
	}
}

void CommandLineInterface::make()
{
	std::cout << Data::Info::HELP_LEVEL.at(Data::HelpTypes::MAKE_HELP);
}

void CommandLineInterface::commandController(const std::string& command, const std::vector<std::string>& params)
{
	if (command == "make")
	{
		make(params);
		return;
	}

	if (command == "set_img_pool_dir")
	{
		setImgPoolDir(params);
		return;
	}

	std::cout << "\nHow did you end up in here?";
}

void CommandLineInterface::commandController(const std::string& command, const std::vector<std::string>& params, const std::unordered_map<std::string, std::string>& flags)
{
	if (command == "make")
	{
		make(params, flags);
		return;
	}

	if (command == "set_img_pool_dir")
	{
		setImgPoolDir(params, flags);
		return;
	}

	std::cout << "\nHow did you end up in here?";
}

void CommandLineInterface::commandController(const std::string& command)
{
	if (command == "make")
	{
		make();
		return;
	}

	if (command == "set_img_pool_dir")
	{
		setImgPoolDir();
		return;
	}

	std::cout << "\nHow did you end up in here?";
}

void CommandLineInterface::setImgPoolDir(const std::vector<std::string>& params)
{
	if (params.size() < 3)
	{
		std::cerr << Data::Errors::PARAMETER_COUNT;
		return;
	}

	std::string source;
	std::string databaseFile;
	std::string number;

	//
	source = params[0];
	std::cout << source << std::endl;
	number = params[1];
	std::cout << number << std::endl;
	databaseFile = params[2];
	std::cout << databaseFile << std::endl;

	if (isFile(databaseFile))
	{
		this->PATHS.at(CommandLineInterface::PathType::SOURCE) = source;
		this->PATHS.at(CommandLineInterface::PathType::NUMBER_PHOTOS) = number;
		this->PATHS.at(CommandLineInterface::PathType::DATABASE) = databaseFile;
	}
	else
		std::cerr << Data::Errors::WRONG_ARGUMENT;
}

void CommandLineInterface::setImgPoolDir(const std::vector<std::string>& params, const std::unordered_map<std::string, std::string>& flags)
{
	if (params.size() < 3)
	{
		std::cerr << Data::Errors::PARAMETER_COUNT;
		return;
	}

	std::string source;
	std::string databaseFile;
	std::string number;

	//
	source = params[0];
	std::cout << source << std::endl;
	number = params[1];
	std::cout << number << std::endl;
	databaseFile = params[2];
	std::cout << databaseFile << std::endl;

	if (isFile(databaseFile))
	{
		this->PATHS.at(CommandLineInterface::PathType::SOURCE) = source;
		this->PATHS.at(CommandLineInterface::PathType::NUMBER_PHOTOS) = number;
		this->PATHS.at(CommandLineInterface::PathType::DATABASE) = databaseFile;
	}
	else
		std::cerr << Data::Errors::WRONG_ARGUMENT;
}

void CommandLineInterface::setImgPoolDir()
{
	std::cout << Data::Info::HELP_LEVEL.at(Data::HelpTypes::SET_POOL_HELP);
}

inline bool CommandLineInterface::isExtension(const std::string& parameter)
{
	return KNOWN_EXTENSIONS.find(parameter) != KNOWN_EXTENSIONS.end();
}

inline bool CommandLineInterface::isDir(const std::string& parameter)
{
	if (_access(parameter.c_str(), 0) == 0)
	{
		struct stat status;

		stat(parameter.c_str(), &status);

		if (status.st_mode & S_IFDIR)
		{
			return true;
		}

		else
		{
			//this is a file path, not a directory path

			return false;
		}
	}
	else
	{
		//The path doesn't exist

		return false;
	}

}

inline bool CommandLineInterface::isType(const std::string& parameter)
{
	std::regex poolType("diamond|rectangle|square|triangle", std::regex_constants::icase);

	return std::regex_match(parameter, poolType);
}

inline bool CommandLineInterface::isSize(const std::string& parameter)
{
	std::regex isNumber("[1-9]+[0-9]*");

	return std::regex_match(parameter, isNumber);
}

inline bool CommandLineInterface::isMethod(const std::string& parameter)
{
	std::regex poolMethod("cropp|resize", std::regex_constants::icase);

	return std::regex_match(parameter, poolMethod);
}

inline bool CommandLineInterface::inCheck(const std::string& flag, const std::string& parameter)
{
	switch (FLAGS.at(flag))
	{
	case Parameter::EXTENSION:
		return isExtension(parameter);
		break;

	case Parameter::PATH:
		return isDir(parameter);
		break;

	case Parameter::TYPE:
		return isType(parameter);
		break;

	case Parameter::SIZE:
		return isSize(parameter);
		break;

	case Parameter::METHOD:
		return isMethod(parameter);
		break;

	case Parameter::HELP:
	case Parameter::VERSION:
	default:
		std::cerr << "No idea how you got here";
		break;
	}

	return false;
}

inline bool CommandLineInterface::isFile(const std::string& parameter)
{
	std::string matcher = ".*.(";

	for (auto extension : KNOWN_EXTENSIONS)
	{
		matcher += extension + '|';
	}

	matcher[matcher.size() - 1] = ')';
	return std::regex_match(parameter, std::regex(matcher));
}

inline bool CommandLineInterface::isPath(const std::string& parameter)
{
	struct stat buffer;
	return (stat(parameter.c_str(), &buffer) == 0);
}
