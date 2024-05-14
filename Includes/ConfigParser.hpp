#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <iostream>
# include <string>
# include <map>
#include <vector>
#include "Location.hpp"

class ConfigParser
{
	private:
		std::map<std::string, std::string> _ConfOptions;
		std::vector<std::string> _ConfFile;
		std::string configPath;
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
		//void defaultConf(void);
		Location locations[100];
};


#endif
