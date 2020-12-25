#include "Data.h"

//INFOS:

const std::string Data::Info::VERSION = "0.0.1a";
const std::string Data::Info::MAKE_HELP = "make <image1, ...> [flags] [flag_parameters]\n";
const std::string Data::Info::SET_POOL_HELP = "WIP";
//INFOS END


//ERRORS:
const std::string Data::Errors::PARAMETER_COUNT = "Wrong number of parameters";
const std::string Data::Errors::UNKNOWN_COMMAND = "Unknown command used";
const std::string Data::Errors::WRONG_ARGUMENT_ORDER = "Wrong agrument order";
const std::string Data::Errors::WRONG_INPUT = "The argument may be an unsupported file extension or an invalid path(check for file existence!)";
const std::string Data::Errors::WRONG_ARGUMENT = "One or more arguments are invalid";
//ERRORS END