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
	if (argc == 2)
	{
		try
		{
			Server webServer(argv[1]);
			webServer.RunServer();
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
			Server webServer;
			webServer.RunServer();
		}
		catch (std::exception &e)
		{
			(void)e;
			return (1);
		}
	}
	

return (0);
}
