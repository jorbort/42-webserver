#include "../Includes/ConfigParser.hpp"
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstdlib>



std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    return (first == std::string::npos || last == std::string::npos) ? "" : str.substr(first, last - first + 1);
}

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

void ConfigParser::splitServers(std::string &configfile)
{
	size_t start = 0;
	size_t end = 1;

	if (configfile.find("server",0) == std::string::npos)
		throw std::invalid_argument("no server found in the file");
	while (start != end && start < configfile.length() -1)
	{
		start = serverBegin(start, configfile);
		end = serverEnd(start, configfile);
		if (end == std::string::npos)
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
    if (start >= configfile.size() || configfile[start] != '{') {
        return std::string::npos;
    }

    int braceCount = 1;

    for (size_t i = start + 1; i < configfile.size(); ++i) {
        if (configfile[i] == '{') {
            ++braceCount;
        } else if (configfile[i] == '}') {
            --braceCount;
            if (braceCount == 0) {
                return i;
            }
        }
    }
    return std::string::npos;
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
			epurString(line);
			if (line[line.length()-1] == '{' || line[0] == '}')
			{
				configfile.append( line + "\n");
			}
			else
			{
				configfile.append(line);
			}
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
		begin = conf.find_first_not_of(';',end);
		if (begin == std::string::npos)
			break;
	}
	std::vector<std::string>::iterator it = vect.begin();
	for(; it != vect.end(); it++)
	{
		std::string tmp  = *it;
		
		
		if (tmp.substr(0,6) == "listen")
		{
			server.setListen(tmp.substr(7));
		}
		if (tmp.substr(0,4) == "host")
		{
			server.setHost(tmp.substr(tmp.find(" ")));
		}
		if (tmp.substr(0,11) == "server_name")
		{
			server.setName(tmp.substr(tmp.find(" ")));
		}
		if (tmp.substr(0,10) == "error_page")
		{
			server.addErrorPage(tmp.substr(tmp.find(" ") + 1));
		}
		if (tmp.substr(0,20) == "client_max_body_size")
		{
			server.setBodySize(tmp.substr(tmp.find(" ") + 1));
		}
		if (tmp.substr(0,4) == "root")
		{
			server.setRoot(tmp.substr(tmp.find(" ") + 1));
		}
		if (tmp.substr(0,5) == "index")
		{
			server.setIndex(tmp.substr(tmp.find(" ") + 1));
		}
		if (tmp.substr(0,9) == "autoindex")
		{
			server.toggleAutoindex();
		}
		if (tmp.substr(0,8) == "location")
		{
			Location location;
			size_t start = tmp.find(" ") + 1;
			size_t end = tmp.find_first_of("{");
			if( end == std::string::npos)
				throw std::invalid_argument("invalid scope in location");
			//location.setName(tmp.substr(start, tmp.find_first_of("{") - start));
			start = end + 2;
			if (tmp.substr(start,4) == "root")
				location.setRoot(tmp.substr(start + 5));
			
			while(42)
			{
				it++;
				tmp = *it;
				if (tmp.substr(0,13) == "allow_methods")
				{
					location.addMethods(tmp.substr(tmp.find(" ") + 1));
				}

			}
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
