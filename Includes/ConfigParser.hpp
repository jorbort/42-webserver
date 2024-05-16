#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <iostream>
# include <string>
# include <map>
#include <vector>
#include "Location.hpp"
#include "ServerConfigs.hpp"

class ConfigParser
{
	private:
		std::vector<std::string> _ConfFile;
		std::string configPath;
		int nOfServers;
		std::vector<ServerConfigs> _servers;
		
	public:

		ConfigParser(void);
		ConfigParser( ConfigParser const & src );
		~ConfigParser(void);

		ConfigParser &operator=( ConfigParser const & rhs );
		void ParseConfig(void);
		void findConfigs(void);
		std::string &getValue(std::string key);
		std::string &getpath(void);
		void setConfPath(std::string &path);
		void epurString(std::string &path);
		void splitServers(void);
		void createServer(std::string &conf, ServerConfigs &server);
		//void defaultConf(void);
};


#endif
