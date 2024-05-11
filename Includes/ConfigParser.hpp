#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <iostream>
# include <string>
# include <map>

class ConfigParser
{
	private:
		std::map<std::string, std::string> _ConfOptions;
		std::string configPath;
	public:

		ConfigParser();
		ConfigParser( ConfigParser const & src );
		~ConfigParser();

		ConfigParser &operator=( ConfigParser const & rhs );
		void ParseConfig(void);
		std::string &getValue(std::string key);
		std::string &getpath();
		void setConfPath(std::string);
		void defaultConf(void);
};


#endif 
