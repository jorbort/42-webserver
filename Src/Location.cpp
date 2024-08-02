#include "../Includes/WebServ.hpp"
#include "../Includes/Location.hpp"
#include <cstddef>
#include <stdexcept>
#include <string>
#include <sstream>
#include <algorithm>

Location::Location()
{
	this->autoindex = -1;
}

Location::~Location()
{

}

void Location::setName(const std::string &name)
{
	this->name = name;
}

void Location::setRoot(const std::string &root)
{
	this->root = root;
}

void Location::setPath(void)
{
    if (this->name[0] == '/'){
        this->name = this->name.substr(1);
    }
	this->path = this->getRoot()+ this->name;
}

void Location::toggleAutoIndex(const std::string &status)
{
	if (this->autoindex != -1)
		throw std::invalid_argument("repeated parameter autoindex");
	else
	{
		if (status == "on")
			this->autoindex = 1;
		else if (status == "off")
			this->autoindex = 0;
		else
			throw std::invalid_argument("invalid autoindex argument");
	}
}

void Location::addMethods(const std::string &str)
{
	size_t begin = 0;
	std::string line = "";
	while(42)
	{
		size_t end = str.find(" ",begin);
		if(end == std::string::npos)
		{
			end = str.find(";");
			line = str.substr(begin, end - begin);
			if (line != "GET" && line != "POST" && line != "DELETE" )
				throw std::invalid_argument("Invalid http method 1");
			else
				this->allowed_methods.push_back(line);
			break;
		}
		line = str.substr(begin, end - begin);
		if (line != "GET" && line != "POST" && line != "DELETE")
			throw std::invalid_argument("Invalid http method 2");
		else
			this->allowed_methods.push_back(line);
		begin = end + 1;
		if (begin == std::string::npos)
			break;
	}
}

void Location::addCgiPath(const std::string &cgi)
{
	std::string invalidChars = "\0 \t\n\r*?[]()&|;<>''\"`\\$!' '";
    std::istringstream iss(cgi);
	std::string value;
	while (iss >> value)
	{
		if (value.find_first_of(invalidChars) != std::string::npos) {
        	throw std::invalid_argument("Invalid character found in string");
    	}
		this->cgiPath.push_back(value);
	}
}

void Location::setCgiExtension(const std::string &ext)
{
	std::istringstream iss(ext);
    std::string value;
	std::vector<std::string> cgiExtensions ;
	cgiExtensions.push_back(".cgi");
    cgiExtensions.push_back(".pl");
    cgiExtensions.push_back(".py");
    cgiExtensions.push_back(".sh");
    cgiExtensions.push_back(".exe");
    cgiExtensions.push_back(".php");
    cgiExtensions.push_back(".rb");
    cgiExtensions.push_back(".tcl");
	cgiExtensions.push_back(".js");
    while (iss >> value)
    {
        if(std::find(cgiExtensions.begin(), cgiExtensions.end(),value) == cgiExtensions.end())
            throw std::invalid_argument("Invalid cgi extension");
        this->cgiExtensions.push_back(value);
    }
}

void Location::setIndex(const std::string &index)
{
	if (!getIndex().empty())
		throw std::invalid_argument("more than one index in location ");
	this->index = index;
}

void Location::setUploadPath(const std::string &upload)
{
	if (!getUPloadPath().empty())
		throw std::invalid_argument("more than one upload path for the location ");
    this->uploadPath = upload;
}

const std::string &Location::getName(void) const
{
	return (this->name);
}

const std::string &Location::getRoot(void) const
{
	return (this->root);
}
std::vector<std::string>::iterator Location::getMethods(void)
{
	const std::vector<std::string>::iterator it = this->allowed_methods.begin();
	return (it);
}

std::vector<std::string>::iterator Location::getCgiExtensions(void)
{
	const std::vector<std::string>::iterator it = this->cgiExtensions.begin();
	return (it);
}

int Location::isAutoindex(void)
{
	return (this->autoindex);
}
std::vector<std::string>::iterator Location::getCgiPath(void)
{
	std::vector<std::string>::iterator it = this->cgiPath.begin();
	return (it);
}
const std::string &Location::getPath(void) const
{
	return(this->path);
}
const std::string &Location::getUPloadPath(void) const
{
	return (this->uploadPath);
}
const std::string &Location::getIndex(void) const
{
	return (this->index);
}

std::ostream & operator<<(std::ostream &stream,  Location const &loc)
{
	stream << loc.getName() << " ";
	stream << loc.getPath() << " ";
	stream << loc.getRoot() << " ";
	stream << loc.getIndex() << " ";
	stream << loc.getUPloadPath() << std::endl;
	return (stream);
}

bool Location::checkLocation(void)
{
	this->setPath();
	if (this->getName().empty())
	{
		return (false);
	}
	else if (this->getRoot().empty())
	{
		return (false);
	}
	else if (this->getPath().empty())
	{
		return (false);
	}
	else if (this->getPath() == "cgi-bin" && this->cgiPath.empty())
	{
		this->cgiPath.push_back("/bin/bash");
		this->cgiPath.push_back("/usr/bin/python3");
		this->cgiExtensions.push_back(".sh");
		this->cgiExtensions.push_back(".py");
	}
	else if (this->isAutoindex() == -1)
	{
		this->toggleAutoIndex("off");
	}

	return (true);
}
