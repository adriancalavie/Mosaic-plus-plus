#include "CommandLineInterface.h"
#include "Data.h"


const std::unordered_map<std::string, CommandLineInterface::Parameter> CommandLineInterface::FLAGS = {
	{"--shape",		Parameter::TYPE},
	{"-s",			Parameter::TYPE},

	{"--extension",	Parameter::EXTENSION},
	{"-e",			Parameter::EXTENSION},

	{"--directory",	Parameter::PATH},
	{"-d",			Parameter::PATH},

	{"--help",		Parameter::NONE},
	{"-h",			Parameter::NONE},

	{"--version",	Parameter::NONE},
	{"-v",			Parameter::NONE}
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
		if (COMMANDS.find(std::string(args[1])) != COMMANDS.end())
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

					if (FLAGS.at(argument) == Parameter::NONE)
					{
						make(argument);
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
				else
					//else, if we find a command parameter
				{
					if (isPath(argument) && isFile(argument))//TODO: optimise for set_img_pool_dir
						//if we have an image as an input
					{
						commandParams.push_back(argument);
						hasParams = true;
					}
					else
					{
						std::cerr << Data::Errors::WRONG_INPUT;
					}
				}
			}


			make(commandParams);
		}
		else
		{
			if (FLAGS.find(std::string(args[1])) != FLAGS.end() && FLAGS.at(std::string(args[1])) == Parameter::NONE)
				//if instead of a command, we find a help or version request
			{
				//TODO: show all documentation on all the commands
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

	BasePictures test(5900);
	test.addPicturesMosaic(false);


	for (auto image : params)
	{
		cv::Mat input = cv::imread(image, cv::IMREAD_COLOR);
		input = PictureTools::resize(input, 1200, 700);
		cv::Mat input3 = Mosaic::makeMosaic(input, test, Type::square, 10);

		cv::imwrite("D:\\Mosaic++\\Mosaic++\\Resulting pictures\\" + std::to_string(test.getNumberPictures()) + test.getExtension(), input3);

		cv::imshow("CommandTest_1.2_", input3);
		cv::waitKey(0);
	}
}

void CommandLineInterface::make(const std::vector<const std::string&>& params, const std::unordered_map<std::string, std::string>& flags)
{
	//TODO implementation of this
}

void CommandLineInterface::make(const std::string& flag)
{
	if (flag == "-v" || flag == "--version")
	{
		std::cout << Data::Info::VERSION;
		return;
	}


	if (flag == "-h" || flag == "--help")
	{
		std::cout << Data::Info::MAKE_HELP;
	}
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
	return (parameter == "diamond" || parameter == "rectangle" || parameter == "square" || parameter == "triangle");
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

	case Parameter::NONE:
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
