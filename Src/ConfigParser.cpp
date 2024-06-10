#include "../Includes/ConfigParser.hpp"
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstdlib>

std::string removeNewline(std::string str) {
    std::string newStr;
    for (std::string::iterator it = str.begin(); it != str.end(); ++it) {
        if (*it != '\n') {
            newStr += *it;
        }
    }
    return newStr;
}

std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(" \t");
    size_t last = str.find_last_not_of(" \t");
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


void ConfigParser::splitServers(const std::vector<std::string>& configLines) {
    nOfServers = 0;
    size_t start = 0;
    size_t end = 1;
    std::string configfile;

    for (size_t i = 0; i < configLines.size(); ++i) {
        configfile += configLines[i] ;
    }

    if (configfile.find("server", 0) == std::string::npos)
        throw std::invalid_argument("no server found in the file");

    while (start != std::string::npos) {
        start = serverBegin(start, configLines);
		if (start == std::string::npos)
			break;
        end = serverEnd(start, configLines);
        if (end == std::string::npos)
            throw std::invalid_argument("invalid server scope");
        _ConfFile.push_back(configfile.substr(start, end - start + 1));
        ++nOfServers;
        start = end + 1;
    }
}

size_t ConfigParser::serverBegin(size_t start, const std::vector<std::string>& configLines) {
    size_t i;
    std::string configfile;

    for (size_t j = 0; j < configLines.size(); ++j) {
        configfile += configLines[j] + "\n";
    }

    for (i = start; i < configfile.size(); ++i) {
        if (configfile[i] == 's')
            break;
    }

    if (i >= configfile.size())
        return std::string::npos;

    if (configfile.compare(i, 6, "server") != 0)
        throw std::invalid_argument("wrong character in server scope {");

    i += 6;
    while (i < configfile.size() && isspace(configfile[i]))
        ++i;

    if (configfile[i] == '{')
        return i;
    else
        throw std::invalid_argument("wrong character in server scope {");
}

size_t ConfigParser::serverEnd(size_t start, const std::vector<std::string>& configLines) {
    std::string configfile;

    for (size_t j = 0; j < configLines.size(); ++j) {
        configfile += configLines[j] + "\n";
    }

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
	std::vector<std::string> configfile ;

	file.open(this->getpath().c_str(), std::ios::in );
	if (file.is_open() == false)
		throw std::invalid_argument("");
	while (std::getline(file,line))
	{
		while (std::getline(file, line))
		{
        	line = trim(line);
        	if (line.empty() || line[0] == '#') {
            	continue;
        }
        configfile.push_back(line + "\n");
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

std::vector<std::string> ConfigParser::splitConfigLines(const std::string &conf)
{
  std::vector<std::string> lines;
  std::string::size_type begin = 0;
  std::string::size_type end;
  std::string line;

  while(42)
  {
    end = conf.find_first_of('\n',begin);
    if (end == std::string::npos)
      break;
    line = conf.substr(begin, end - begin);
    lines.push_back(line);
    begin = conf.find_first_not_of('\n',end);
    if (begin == std::string::npos)
      break;
  }
  return (lines);
}

void ConfigParser::parseLocation(std::vector<std::string>::iterator &it,std::vector<std::string>::iterator &end, ServerConfigs & server)
{
  Location location;
  std::string tmp = *it;
  std::string::size_type start = tmp.find(" " ) + 1;
  std::string::size_type pathEnd = tmp.find_first_of("{");
  if (pathEnd == std::string::npos)
    throw std::invalid_argument("invalid scope in location");
  location.setName(tmp.substr(start, pathEnd - start));
  ++it;
  while (it != end)
  {
	 ++it;

        std::string line = *it;
        line = trim(line);
        if (line.empty()) {
            continue;
        }
        std::string key = line.substr(0, line.find(" "));
        std::string value = line.substr(line.find(" ") + 1);
        if (key == "root") {
            location.setRoot(value);
        } else if (key == "allow_methods") {
            location.addMethods(value);
         }// else if (key == "autoindex") {
        //     location.setAutoindex(value == "on" ? true : false);
        // } else if (key == "index") {
        //     location.setIndex(value);
        // } else if (key == "upload_store") {
        //     location.setUploadStore(value);
        // } else if (key == "cgi_path") {
        //     location.setCgiPath(value);
        // } else if (key == "cgi_ext") {
        //     location.setCgiExt(value);
        // }
  }

    // if (!location.checkLocation())
    //     throw std::invalid_argument("conflicting information in  location cannot set up server ");;   ---falta implementar checkLocation
    server.addLocation(location);
}


void ConfigParser::createServer(std::string &conf, ServerConfigs &server)
{
	std::vector<std::string> vect = splitConfigLines(conf);

	std::vector<std::string>::iterator it = vect.begin();
	while (it != vect.end())
	{
		std::string tmp  = removeNewline(*it);

		epurString(tmp);

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
			size_t start = tmp.find(" ") + 1;
			size_t end = tmp.find(";", start);
			server.setRoot(tmp.substr(start,end - start));
		}
		if (tmp.substr(0,5) == "index")
		{
			server.setIndex(tmp.substr(tmp.find(" ") + 1));
		}
		if (tmp.substr(0,9) == "autoindex")
		{
			server.toggleAutoindex();
		}
		if (tmp.substr(0,tmp.find(" ") ) == "location")
		{

      		std::vector<std::string>::iterator locationEnd = it;

			findLocationEnd(locationEnd, vect);
      		parseLocation(it, locationEnd, server);
		}
    	++it;
	}
	}

void ConfigParser::findLocationEnd(std::vector<std::string>::iterator &locationEnd, std::vector<std::string> &vect)
{
    int braceCount = 0;
    while (locationEnd != vect.end()) {
        for (size_t j = 0; j < (*locationEnd).size(); ++j) {
            if ((*locationEnd)[j] == '{') {
                ++braceCount;
            } else if ((*locationEnd)[j] == '}') {
                --braceCount;
                if (braceCount == 0) {
                    return;
                }
            }
        }
        ++locationEnd;
    }
    if (braceCount != 0) {
        throw std::invalid_argument("invalid scope in locations");
    }
}

void ConfigParser::epurString(std::string &str)
{
	std::string res = "";
	int i = 0;
	bool flag = false;

	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
		i++;
	while (str[i])
	{
		if (str[i] == ' ' || str[i] == '\t')
			flag = true;
		if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
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
