#include "../Includes/ConfigParser.hpp"
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstdlib>
#include <stack>
#include "../Includes/Logger.hpp"



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
    for (size_t i = 0; i < _servers.size(); ++i) {
        delete [] _servers[i];
    }
    _servers.clear();
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

void ConfigParser::splitServers(const std::vector<std::string>& configLines) {
    std::string configfile;

    for (size_t i = 0; i < configLines.size(); ++i) {
        configfile += configLines[i] ;
    }
    if (configfile.find("server", 0) == std::string::npos)
        throw std::invalid_argument("no server found in the file");

    std::stack<size_t> serverBlockStack;
    size_t start = 0;
    while (start != std::string::npos) {
        start = configfile.find("server", start);
        if (start == std::string::npos)
            break;

        size_t blockStart = configfile.find("{", start);
        if (blockStart == std::string::npos)
            throw std::invalid_argument("invalid server scope");

        serverBlockStack.push(blockStart);

        size_t end = blockStart;
        while (!serverBlockStack.empty()) {
            size_t nextEnd = configfile.find_first_of("{}", end + 1);
   			if (nextEnd == std::string::npos || nextEnd == end) {
        		throw std::invalid_argument("Unmatched braces in server configuration");
    		}
    		end = nextEnd;
    		if (configfile[end] == '{') {
        		serverBlockStack.push(end);
    		} else {
        		serverBlockStack.pop();
    		}
        }
        _ConfFile.push_back(configfile.substr(start, end - start + 1));
        this->nOfServers++;
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
		throw std::invalid_argument("failed to open file");
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
	this->splitServers(configfile);
	for (size_t i = 0; i < this->nOfServers ; i++)
	{
		ServerConfigs *server = new ServerConfigs;
		createServer(this->_ConfFile[i], *server);
		server->checkServer(*server);
		this->_servers.push_back(server);
	}
    if (this->nOfServers > 1)
		compareServers();
    for (size_t i = 0 ; i < this->nOfServers; i++){
        this->_servers[i]->initSocket();
    }
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
	while (it != end)
	{
		++it;
        std::string line = *it;
        line = trim(line);
        if (line.empty()) {
            continue;
        }
		epurString(line);
        std::string::size_type spacePos = line.find(" ");
		std::string key = spacePos != std::string::npos ? line.substr(0, spacePos) : line;
		std::string value = spacePos != std::string::npos ? line.substr(spacePos + 1) : "";
		if (key != "}" && (value.empty() || value.find_first_not_of(' ') == std::string::npos)) {
    		throw std::invalid_argument("Invalid value");
		}
		if (key == "root") {
            location.setRoot(value);
        } else if (key == "allow_methods") {
            location.addMethods(value);
         } else if (key == "autoindex") {
            location.toggleAutoIndex(value);
        } else if (key == "index") {
            location.setIndex(value);
        } else if (key == "upload_store") {
            location.setUploadPath(value);
        } else if (key == "cgi_path") { 
            location.addCgiPath(value);
        }else if (key == "cgi_ext") {
            location.setCgiExtension(value);
        }
	}
    if (!location.checkLocation())
        throw std::invalid_argument("conflicting information in  location cannot set up server ");
    server.addLocation(location);
}


void ConfigParser::createServer(std::string &conf, ServerConfigs &server)
{
	std::vector<std::string> vect = splitConfigLines(conf);

	std::vector<std::string>::iterator it = vect.begin();
	for(; it != vect.end(); ++it)
	{
		std::string tmp  = removeNewline(*it);
		epurString(tmp);

		std::string::size_type spacePos = tmp.find(" ");
        std::string key = spacePos != std::string::npos ? tmp.substr(0, spacePos) : tmp;
        std::string value = spacePos != std::string::npos ? tmp.substr(spacePos + 1) : "";
		
		if ((key[key.length() -1] != '}' && key[key.length() -1] != '{') && (value.empty() || value.find_first_not_of(' ') == std::string::npos)) {
			throw std::invalid_argument("Invalid value");
		}
		 if (key == "listen")
        {
			if (server.getListen() != 0)
				throw std::invalid_argument("Error repeated argument, listen");
            server.setListen(value);
        }
        else if (key == "host")
        {
            if (server.getHostIp() != 0)
				throw std::invalid_argument("Error repeated argument, host");
			server.setHost(value);
        }
        else if (key == "server_name")
        {
            if (!server.getServerName().empty())
				throw std::invalid_argument("Error repeated argument, server_name");
			server.setName(value);
        }
        else if (key == "error_page")
        {
            server.addErrorPage(value);
        }
        else if (key == "client_max_body_size")
        {
            server.setBodySize(value);
        }
        else if (key == "root")
        {
			if (!server.getRoot().empty())
				throw std::invalid_argument("Error repeated argument, root");
            size_t end = value.find(";");
            server.setRoot(value.substr(0, end));
        }
        else if (key == "index")
        {
			if (!server.getIndex().empty())
				throw std::invalid_argument("Error repeated argument, index");
            server.setIndex(value);
        }
        else if (key == "autoindex")
        {
            size_t end = value.find(";");
            server.toggleAutoindex(value.substr(0, end));
        }
		if (key == "location")
		{
      		std::vector<std::string>::iterator locationEnd = it;

			findLocationEnd(locationEnd, vect);
      		parseLocation(it, locationEnd, server);
		}
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
		if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n' && !(str[i] == ';' && str[i+1] == '\0'))
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

void ConfigParser::setConfPath(std::string &path)
{
	this->configPath = path;
}

std::string &ConfigParser::getpath(void)
{
	return (this->configPath);
}

void ConfigParser::compareServers(void)
{
	size_t i = 0;

	while (i < this->nOfServers -1)
    {
	    size_t j = i +1;
        while (j < this->nOfServers)
        {
            if (this->_servers[i]->getHostIp() == this->_servers[j]->getHostIp())
                throw std::invalid_argument("there cannot be two servers with the same ip");
            if (this->_servers[i]->getListen() == this->_servers[j]->getListen())
                throw std::invalid_argument("there cannot be to servers listening to the same port");
            if (this->_servers[i]->getServerName() == this->_servers[j]->getServerName())
                throw std::invalid_argument("there cannot be two servers with the same name");
            j++;
        }
        i++;    
    }
}