#include "Logger.hpp"
#include "WebServ.hpp"
#include "ConfigParser.hpp"
#include <iostream>

int main(int argc, char **argv)
{
	if (argc > 2)
	{
		Logger::print("Error", "Error : invalid amount of arguments");
		return (1);
	}
	Server *webServer;
	if (argc == 2)
	{
		try
		{
			webServer = new Server(argv[1]);
			webServer->RunServer();
			delete webServer;
		}
		catch(std::exception &e)
		{
			(void)e;
			return (1);
		}
	}
	else if (argc == 1)
	{
		try
		{
			webServer = new Server();
			webServer->RunServer();
			delete webServer;
		}
		catch (std::exception &e)
		{
			(void)e;
			return (1);
		}
	}
	

return (0);
}
