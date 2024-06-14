#include "../Includes/Logger.hpp"
#include "../Includes/WebServ.hpp"
#include "../Includes/ConfigParser.hpp"
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
		std::string arg = argv[1];
		try
		{
			webServer = new Server(arg);
			webServer->initCluster();
			webServer->RunServer();
			delete webServer;
		}
		catch(std::exception &e)
		{
			(void)e;
			// if (webServer)
			// 	delete webServer;
			return (1);
		}
	}
	else if (argc == 1)
	{
	   std::string arg = "config/default.conf";
;
		try
		{
			webServer = new Server(arg);
			//webServer->initCluster();
			webServer->RunServer();
		}
		catch (std::exception &e)
		{
			(void)e;
			return (1);
		}
	}
	

return (0);
}
