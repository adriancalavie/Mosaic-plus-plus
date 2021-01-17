//
//#include "CommandLineInterface.h"
//
//
//const std::unordered_map<std::string, Flag::Parameter> CommandLineInterface::FLAGS = {
//	{"--shape",				Flag::Parameter::TYPE},
//	{"-s",					Flag::Parameter::TYPE},
//
//	{"--partiton",			Flag::Parameter::SIZE},
//	{"-p",					Flag::Parameter::SIZE},
//
//	{"--method",			Flag::Parameter::METHOD},
//	{"-m",					Flag::Parameter::METHOD},
//
//	{"--extension",			Flag::Parameter::EXTENSION},
//	{"-e",					Flag::Parameter::EXTENSION},
//
//	{"--directory",			Flag::Parameter::PATH},
//	{"-d",					Flag::Parameter::PATH},
//
//	{"--help",				Flag::Parameter::HELP},
//	{"-h",					Flag::Parameter::HELP},
//
//	{"--version",			Flag::Parameter::VERSION},
//	{"-v",					Flag::Parameter::VERSION}
//};
//
//std::unordered_map<CommandLineInterface::PathType, std::string> CommandLineInterface::PATHS = {
//	{PathType::SOURCE,						    Data::Defaults::PARENT_PATH},
//
//	{PathType::PROCESSED,						Data::Defaults::PATH_PICTURES_FOR_MOSAIC},
//
//	{PathType::DATABASE,						Data::Defaults::PATH_DATA_BASE_FILE},
//
//	{PathType::NUMBER_PHOTOS,					"10"}
//
//};
//
//const std::unordered_map<std::string, std::unordered_set<std::string>> CommandLineInterface::COMMANDS = {
//	{
//		"make",											{
//														 "-s", "--shape",
//														 "-p","--partiton",
//														 "--method","-m",
//														 "--help","-h"
//														}		
//	},
//
//	{
//		"set_img_pool_dir",								{
//														 "-d","--directory", 
//														 "-h","--help"
//														}
//	}
//};
//
//CommandLineInterface::CommandLineInterface(int argc, char* args[])
//{
//	if (argc < 2)
//	{
//		std::cerr << Data::Errors::PARAMETER_COUNT;
//	}
//	else
//		//daca aem destule argumente
//	{
//		auto command = COMMANDS.find(std::string(args[1]));
//		if (command != COMMANDS.end())
//			//daca primul argument este o comanda
//		{
//			bool hasParams = true;
//			std::unordered_map<std::string, std::string> flagsParams;
//			std::vector<std::string> imagePaths;
//
//			for (int index = 2; index < argc; ++index) 
//				//parcurgem restul argumentelor
//			{
//
//				std::string argument(args[index]);
//
//				if (command->second.find(argument) != command->second.end())
//					//daca argumnetul este un flag ce corespunde comenzii
//				{
//					if (FLAGS.at(argument) == Flag::Parameter::HELP)
//						//daca e -h || --help
//					{
//						commandController(argument);
//						return;
//					}
//					else
//					{
//						if (index + 1 < argc)
//							//daca avem destule argumente in continuare
//							if (inCheck(argument, args[++index]))
//								//daca parametrul este valid
//								flagsParams.insert({ argument, args[index] });
//							else
//							{
//								std::cerr << Data::Errors::WRONG_PARAMETER;
//							}
//
//					}
//				}
//				else
//				//if we find a path;
//				{
//
//					if (isPath(argument) && isFile(argument))
//						//if we have an image as an input
//					{
//						imagePaths.push_back(argument);
//						hasParams = true;
//					}
//					else
//					{
//						std::cerr << Data::Errors::WRONG_INPUT;
//						return;
//					}
//				}
//			}
//
//			if (flagsParams.size() > 0)
//				commandController(command->first, imagePaths, flagsParams);
//			else
//				commandController(command->first, imagePaths);
//		}
//		else
//		{
//			if (FLAGS.find(std::string(args[1])) != FLAGS.end())
//				//daca in loc de o comanda, gasim o cerere de 'help' sau 'version'
//			{
//				if (FLAGS.at(std::string(args[1])) == Flag::Parameter::HELP)
//				{
//					std::cout << Data::Info::HELP_LEVEL.at(Data::HelpTypes::GENERAL_HELP);
//				}
//				else if (FLAGS.at(std::string(args[1])) == Flag::Parameter::VERSION)
//				{
//					std::cout << Data::Info::VERSION;
//				}
//			}
//			else
//				//altfel comanda nu exista
//			{
//				std::cerr << Data::Errors::UNKNOWN_COMMAND;
//			}
//		}
//	}
//	//FlagNoExpect help({ "-h", "--help" }, Flag::Parameter::HELP, "make");
//	//FlagNoExpect help({ "-v", "--version" }, Flag::Parameter::VERSION, std::nullopt);
//
//}
//
///*
//CommandLineInterface::CommandLineInterface(int argc, char* args[])
//{
//	if (argc < 2)
//	{
//		std::cerr << Data::Errors::PARAMETER_COUNT;
//	}
//	else
//		//if we have enough arguments
//	{
//		auto command = COMMANDS.find(std::string(args[1]));
//		if (command != COMMANDS.end())
//			//if we have a valid command as a first argument
//		{
//
//			bool hasParams = false;
//			std::unordered_map<std::string, std::string> flagsParams;
//			std::vector<std::string> commandParams;
//
//			for (int i = 2; i < argc; ++i)
//				//we search for either flags or parameters
//			{
//				std::string argument(args[i]);
//
//				if (FLAGS.find(argument) != FLAGS.end())
//					//if we found a flag
//				{
//
//					if (FLAGS.at(argument) == Parameter::HELP)//if -h
//					{
//						commandController(*command);
//						return;
//					}
//					else
//					{
//						if (hasParams)
//							//if the flag requires parameters
//						{
//							if (i < argc)
//								//if we have enough arguments
//								if (inCheck(argument, args[++i]))
//									//if the flag parameter is valid
//									flagsParams.insert({ argument, args[i] });
//						}
//						else
//						{
//							std::cerr << Data::Errors::WRONG_ARGUMENT_ORDER;
//						}
//					}
//
//				}
//				else// if (COMMANDS.find(std::string(args[i])) != COMMANDS.end())
//				{
//					//else, if we find a command parameter
//					if (isPath(argument) && isFile(argument))//TODO: optimise for set_img_pool_dir
//						//if we have an image as an input
//					{
//						commandParams.push_back(argument);
//						hasParams = true;
//					}
//					else
//					{
//						std::cerr << Data::Errors::WRONG_INPUT;
//						return;
//					}
//				}
//			}
//
//			if (flagsParams.size() > 0)
//				commandController(*command, commandParams, flagsParams);
//			else
//				commandController(*command, commandParams);
//		}
//		else
//		{
//			if (FLAGS.find(std::string(args[1])) != FLAGS.end())
//				//if instead of a command, we find a help or version request
//			{
//				if (FLAGS.at(std::string(args[1])) == Parameter::HELP)
//				{
//					std::cout << Data::Info::HELP_LEVEL.at(Data::HelpTypes::GENERAL_HELP);
//				}
//				else if (FLAGS.at(std::string(args[1])) == Parameter::VERSION)
//				{
//					std::cout << Data::Info::VERSION;
//				}
//			}
//			else
//			{
//				std::cerr << Data::Errors::UNKNOWN_COMMAND;
//			}
//		}
//	}
//
//}
//*/
//
//void CommandLineInterface::commandController(const std::string& command, const std::vector<std::string>& params)
//{
//	if (command == "make")
//	{
//		make(params);
//		return;
//	}
//
//	if (command == "set_img_pool_dir")
//	{
//		setImgPoolDir(params);
//		return;
//	}
//
//	std::cout << "\nHow did you end up in here?";
//}
//
//void CommandLineInterface::commandController(const std::string& command, const std::vector<std::string>& params, const std::unordered_map<std::string, std::string>& flags)
//{
//	if (command == "make")
//	{
//		make(params, flags);
//		return;
//	}
//
//	if (command == "set_img_pool_dir")
//	{
//		setImgPoolDir(params, flags);
//		return;
//	}
//
//	std::cout << "\nHow did you end up in here?";
//}
//
//void CommandLineInterface::commandController(const std::string& command)
//{
//	if (command == "make")
//	{
//		make();
//		return;
//	}
//
//	if (command == "set_img_pool_dir")
//	{
//		setImgPoolDir();
//		return;
//	}
//
//	std::cout << "\nHow did you end up in here?";
//}
//
//void CommandLineInterface::make(const std::vector<std::string>& params)
//{
//	const std::unordered_map<std::string, std::string>& emptyFlags = {};
//
//	make(params, emptyFlags);
//}
//
//void CommandLineInterface::make(const std::vector<std::string>& params, const std::unordered_map<std::string, std::string>& flags)
//{
//	/*std::cout << "entered on custom make";*/
//
//	std::string extension = "jpg";
//	Type shape = Type::RECTANGLE;
//	Method methodMosaication = Method::RESIZING;
//	int inputSize;
//	uint8_t partitionSize = Mosaic::getDefaultSize();
//	std::string directory = Data::Defaults::PATH_RESULT_IMAGE;
//
//	for (auto& flag : flags)
//	{
//		switch (FLAGS.find(flag.first)->second)
//		{
//		case Flag::Parameter::TYPE:
//			if (flag.second == "rectangle")
//				shape = Type::RECTANGLE;
//			else if (flag.second == "diamond")
//				shape = Type::DIAMOND;
//			else if (flag.second == "triangle")
//				shape = Type::TRIANGLE;
//			else
//				std::cerr << Data::Errors::WRONG_ARGUMENT;
//			break;
//
//		case Flag::Parameter::SIZE:
//			inputSize = std::move(std::stoi(flag.second));
//			if (inputSize <= UINT8_MAX)
//				partitionSize = inputSize;
//			else
//				std::cerr << Data::Errors::OUT_OF_BOUNDS;
//			break;
//
//		case Flag::Parameter::METHOD:
//			if (flag.second == "cropp")
//				methodMosaication = Method::CROPPING;
//			else if (flag.second == "resize")
//				methodMosaication = Method::RESIZING;
//			else
//				std::cerr << Data::Errors::WRONG_ARGUMENT;
//			break;
//
//		case Flag::Parameter::EXTENSION:
//			if (true)//Data::Info::KNOWN_EXTENSIONS.find(flag.second) != Data::Info::KNOWN_EXTENSIONS.end())
//			{
//				extension = flag.second;
//			}
//			else
//				std::cerr << Data::Errors::WRONG_ARGUMENT;
//			break;
//
//		case Flag::Parameter::PATH:
//
//			if (isPath(flag.second))
//			{
//				directory = flag.second;
//			}
//			else
//				std::cerr << Data::Errors::WRONG_ARGUMENT;
//			break;
//		default:
//			std::cerr << "\nHow on earth did you end up in here?\n";
//			break;
//		}
//
//
//	}
//
//	BasePictures imagesPool;
//
//	/*
//	imagesPool.setPicturesNumber(std::move(std::stoi(CommandLineInterface::PATHS.at(CommandLineInterface::PathType::NUMBER_PHOTOS))));
//	imagesPool.setFileSource(CommandLineInterface::PATHS.at(CommandLineInterface::PathType::SOURCE));//D:\\Mosaic++\\Mosaic++\\Base pictures\\;
//	imagesPool.setDataBase(CommandLineInterface::PATHS.at(CommandLineInterface::PathType::DATABASE));//D:\\Mosaic++\\Mosaic++\\Mosaic++\\data_base.txt;
//	imagesPool.setFileDestination
//	*/
//
//	imagesPool.addPicturesMosaic(false);
//
//	for (const auto& image : params)
//	{
//		cv::Mat input = cv::imread(image, cv::IMREAD_COLOR);
//
//		cv::Mat output = Mosaic::makeMosaic(input, imagesPool, methodMosaication, shape, partitionSize);
//
//		cv::imwrite(directory + "Output." + extension, output);
//
//		cv::imshow("Made with Mosaic++", output);
//		cv::waitKey(0);
//	}
//}
//
//void CommandLineInterface::make()
//{
//	std::cout << Data::Info::HELP_LEVEL.at(Data::HelpTypes::MAKE_HELP);
//}
//
//void CommandLineInterface::setImgPoolDir(const std::vector<std::string>& params)
//{
//	if (params.size() < 3)
//	{
//		std::cerr << Data::Errors::PARAMETER_COUNT;
//		return;
//	}
//
//	std::string source;
//	std::string databaseFile;
//	std::string number;
//
//	//
//	source = params[0];
//	std::cout << source << std::endl;
//	number = params[1];
//	std::cout << number << std::endl;
//	databaseFile = params[2];
//	std::cout << databaseFile << std::endl;
//
//	if (isFile(databaseFile))
//	{
//		this->PATHS.at(CommandLineInterface::PathType::SOURCE) = source;
//		this->PATHS.at(CommandLineInterface::PathType::NUMBER_PHOTOS) = number;
//		this->PATHS.at(CommandLineInterface::PathType::DATABASE) = databaseFile;
//	}
//	else
//		std::cerr << Data::Errors::WRONG_ARGUMENT;
//}
//
//void CommandLineInterface::setImgPoolDir(const std::vector<std::string>& params, const std::unordered_map<std::string, std::string>& flags)
//{
//	if (params.size() < 3)
//	{
//		std::cerr << Data::Errors::PARAMETER_COUNT;
//		return;
//	}
//
//	std::string source;
//	std::string databaseFile;
//	std::string number;
//
//	//
//	source = params[0];
//	std::cout << source << std::endl;
//	number = params[1];
//	std::cout << number << std::endl;
//	databaseFile = params[2];
//	std::cout << databaseFile << std::endl;
//
//	if (isFile(databaseFile))
//	{
//		this->PATHS.at(CommandLineInterface::PathType::SOURCE) = source;
//		this->PATHS.at(CommandLineInterface::PathType::NUMBER_PHOTOS) = number;
//		this->PATHS.at(CommandLineInterface::PathType::DATABASE) = databaseFile;
//	}
//	else
//		std::cerr << Data::Errors::WRONG_ARGUMENT;
//}
//
//void CommandLineInterface::setImgPoolDir()
//{
//	std::cout << Data::Info::HELP_LEVEL.at(Data::HelpTypes::SET_POOL_HELP);
//}
//
//inline bool CommandLineInterface::isExtension(const std::string& parameter)
//{
//	return true;//Data::Info::KNOWN_EXTENSIONS.find(parameter) != Data::Info::KNOWN_EXTENSIONS.end();
//}
//
//inline bool CommandLineInterface::isDir(const std::string& parameter)
//{
//	if (_access(parameter.c_str(), 0) == 0)
//	{
//		struct stat status;
//
//		stat(parameter.c_str(), &status);
//
//		if (status.st_mode & S_IFDIR)
//		{
//			return true;
//		}
//
//		else
//		{
//			//this is a file path, not a directory path
//
//			return false;
//		}
//	}
//	else
//	{
//		//The path doesn't exist
//
//		return false;
//	}
//
//}
//
//inline bool CommandLineInterface::isType(const std::string& parameter)
//{
//	std::regex poolType("diamond|rectangle|square|triangle", std::regex_constants::icase);
//
//	return std::regex_match(parameter, poolType);
//}
//
//inline bool CommandLineInterface::isSize(const std::string& parameter)
//{
//	std::regex isNumber("[1-9]+[0-9]*");
//
//	return std::regex_match(parameter, isNumber);
//}
//
//inline bool CommandLineInterface::isMethod(const std::string& parameter)
//{
//	std::regex poolMethod("cropp|resize", std::regex_constants::icase);
//
//	return std::regex_match(parameter, poolMethod);
//}
//
//inline bool CommandLineInterface::isFile(const std::string& parameter)
//{
//	std::string matcher = ".*.(";
//
//	//for (auto extension : Data::Info::KNOWN_EXTENSIONS)
//	//{
//		//matcher += extension + '|';
//	//}
//
//	matcher[matcher.size() - 1] = ')';
//	return std::regex_match(parameter, std::regex(matcher));
//}
//
//inline bool CommandLineInterface::isPath(const std::string& parameter)
//{
//	struct stat buffer;
//	return (stat(parameter.c_str(), &buffer) == 0);
//}
//
//inline bool CommandLineInterface::inCheck(const std::string& flag, const std::string& parameter)
//{
//	switch (FLAGS.at(flag))
//	{
//	case Flag::Parameter::EXTENSION:
//		return isExtension(parameter);
//		break;
//
//	case Flag::Parameter::PATH:
//		return isDir(parameter);
//		break;
//
//	case Flag::Parameter::TYPE:
//		return isType(parameter);
//		break;
//
//	case Flag::Parameter::SIZE:
//		return isSize(parameter);
//		break;
//
//	case Flag::Parameter::METHOD:
//		return isMethod(parameter);
//		break;
//
//	case Flag::Parameter::HELP:
//
//	case Flag::Parameter::VERSION:
//
//	default:
//		std::cerr << "No idea how you got here";
//		break;
//	}
//
//	return false;
//}
