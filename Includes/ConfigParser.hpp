#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

#include <cstddef>
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
		size_t nOfServers;
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
		void splitServers(const std::vector<std::string> &configfile);
		void createServer(std::string &conf, ServerConfigs &server);
		size_t serverBegin(size_t start,const std::vector<std::string> &configfile);
		size_t serverEnd(size_t start,const std::vector<std::string> &configfile);
		void compareServers();
		
    std::vector<std::string> splitConfigLines(const std::string &conf);
    void parseLocation (std::vector<std::string>::iterator &it, std::vector<std::string>::iterator &end, ServerConfigs &server);
	void findLocationEnd(std::vector<std::string>::iterator &locationEnd, std::vector<std::string> &vect);
};


#endif
