#include "../Includes/ConfigParser.hpp"
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>


/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

ConfigParser::ConfigParser()
{
	this->nOfServers = 0;
}

ConfigParser::ConfigParser( const ConfigParser & src )
{
	*this = src;
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

ConfigParser::~ConfigParser()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

ConfigParser &ConfigParser::operator=( ConfigParser const & rhs )
{
	this->_ConfFile = rhs._ConfFile;
	this->configPath = rhs.configPath;

	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
// */

// void ConfigParser::findConfigs()
// {
// 	std::string key;
// 	std::string value;
// 	std::string line;
// 	std::vector<std::string>::iterator vBegin = _ConfFile.begin() , vEnd = _ConfFile.end();
// 	bool bracket = false;
// 	int bracketCount = 0;

// 	for (; vBegin != vEnd; vBegin++)
// 	{
// 		line = *vBegin;
// 		if (line[line.length()] == '{')
// 		{
// 			bracket = true;
// 			bracketCount++;
// 		}
// 		if (!line.compare(0,6, "listen"))
// 		{
// 			_ConfOptions["listen"] = line.substr(line.find(" ") + 1, (line.length()));
// 		}
// 	}
// 	std::cout << _ConfOptions["listen"] << std::endl;
//}

void ConfigParser::splitServers(std::string &configfile)
{
	size_t start = 0;
	size_t end = 1;

	if (configfile.find("server",0) == std::string::npos)
		throw std::invalid_argument("no server found in the file");
	while (start != end && start < configfile.length())
	{
		start = serverBegin(start, configfile);
		end = serverEnd(start, configfile);
		if (start == end)
			throw std::invalid_argument("invalid server scope");
		this->_ConfFile.push_back(configfile.substr(start, end - start + 1));
		this->nOfServers++;
		start = end + 1;
	}
}

size_t ConfigParser::serverBegin(size_t start, std::string &configfile)
{
	size_t i;
	for (i = start ; configfile[i];i++)
	{
		if (configfile[i] == 's')
			break;
	}
	if (!configfile[i])
		return (start);
	if (configfile.compare(i , 6, "server") != 0)
		throw std::invalid_argument("wrong character in server scope {");
	i += 6;
	while (configfile[i] && isspace(configfile[i]))
		i++;
	if (configfile[i] == '{')
		return (i);
	else
		throw std::invalid_argument("wrong character in server scope {");
}

size_t ConfigParser::serverEnd(size_t start, std::string &configfile)
{
	size_t i;
	size_t scopes = 0;

	for (i = start + 1; configfile[i]; i++)
	{
		if (configfile[i] == '{')
			scopes++;
		if (configfile[i] == '}')
		{
			if (!scopes)
				return (i);
			scopes--;
		}
	}
	return (start);
}

void ConfigParser::ParseConfig()
{
	std::ifstream file;
	std::string line = "";
	std::string configfile = "";

	file.open(this->getpath().c_str(), std::ios::in );
	if (file.is_open() == false)
		throw std::invalid_argument("");
	while (!file.eof())
	{
		getline(file, line);
		if (line[0] != '\n' && !line.empty() && line[0] != '#')
		{
			if (line[line.length() - 1] == '{')
				line += "\n";
			epurString(line);
			configfile.append(line);
		}
	}
	file.close();
	splitServers(configfile);
	for (size_t i = 0; i < this->nOfServers ; i++)
	{

		ServerConfigs server;
		createServer(this->_ConfFile[i], server);
		// this->_servers.push_back(server);
	}
	//if (this->nOfServers > 1)
	//	compareServers(); -->to-do
}

void ConfigParser::createServer(std::string &conf, ServerConfigs &server)
{
	(void)server;
	std::string::size_type begin = 0;
	std::string::size_type end;
	std::string line = "";
	std::vector<std::string> vect;

	while (1)
	{
		end = conf.find_first_of(';' ,begin);
		if (end  == std::string::npos)
		 	break;
		line = conf.substr(begin, end - begin);
		vect.push_back(line);
		std::cout << line << std::endl;
		begin = conf.find_first_not_of(';',end);
		if (begin == std::string::npos)
			break;
	}
	std::vector<std::string>::iterator it = vect.begin();
	for(; it != vect.end(); it++)
	{
		std::string tmp  = *it;
		//std::cout << tmp << std::endl;
		if (tmp == "{")
			continue;
		std::cout << tmp.substr( tmp.find(" ") + 1, tmp.length() -1) << std::endl;
		if (!tmp.compare(0,6,"listen"))
		{
			std::cout << tmp.substr( 8, tmp.length() -1) << std::endl;
			server.setListen(tmp.substr( 6, tmp.length() -1));
		}
	}
}

void ConfigParser::epurString(std::string &str)
{
	std::string res = "";
	int i = 0;
	bool flag = false;

	while (str[i] == ' ' || str[i] == '\t')
		i++;
	while (str[i])
	{
		if (str[i] == ' ' || str[i] == '\t')
			flag = true;
		if (str[i] != ' ' && str[i] != '\t')
		{
			if (flag)
				res.push_back(32);
			flag = 0;
			res.push_back(str[i]);
		}
		i++;
	}
	str = res;
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

void ConfigParser::setConfPath(std::string &path)
{
	this->configPath = path;
}

std::string &ConfigParser::getpath(void)
{
	return (this->configPath);
}


/* ************************************************************************** */
