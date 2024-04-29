#include "../Includes/Logger.hpp"
#include "../Includes/WebServ.hpp"

int main(int argc, char **argv)
{
	if (argc > 2)
	{
		Logger::print("Error", "Error : invalid amount of arguments");
		return (1);
	}
	Server webServer;
	if (argc == 2)
	{
		std::string arg = argv[1];
		webServer.ParseConfig(arg);
	}
	Logger::print("Warning", "una advertencia");
	Logger::print("Ok", "todo ok");
	Logger::print("otro", "una opcion que no existe");
}
