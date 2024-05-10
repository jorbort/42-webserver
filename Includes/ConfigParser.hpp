#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <iostream>
# include <string>
# include <map>

class ConfigParser
{
	private:
		std::map<std::string, std::string> _ConfOptions;

	public:

		ConfigParser();
		ConfigParser( ConfigParser const & src );
		~ConfigParser();

		ConfigParser &operator=( ConfigParser const & rhs );
		void ParseConfig(std::string path);
		void ParseConfig(void);
		std::string &getValue(std::string key);

};

std::ostream &operator<<( std::ostream & o, ConfigParser const & i );

#endif 
