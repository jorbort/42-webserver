#include "../Includes/Logger.hpp"
#include "../Includes/WebServ.hpp"
#include "../Includes/ConfigParser.hpp"

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
		try 
		{
				webServer = Server(arg);
		}
		catch(std::exception &e)
		{
			Logger::print("Error", e.what());
			return (1);
		}
	}
	else if (argc == 1)
	{
		try 
		{
			webServer = Server();
		}
		catch (std::exception &e)
		{
			Logger::print("Error", e.what());
			return (1);
		}
	}
	try
	{
		webServer.RunServer();

	}
	catch(const std::exception& e)
	{
		Logger::print("Error", e.what());
		return (1);
	}
	

}
