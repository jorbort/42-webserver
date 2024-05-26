#include "../Includes/WebServ.hpp"
#include "../Includes/Location.hpp"
#include <stdexcept>


Location::Location()
{
	this->autoindex = false;
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
	this->path = this->root + this->name;
}

void Location::toggleAutoIndex(void)
{
	if (this->autoindex )
		throw std::invalid_argument("repeated parameter autoindex");
	else
		this->autoindex = true;
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
			line = str.substr(begin, end - begin);
			if (line != "GET" && line != "POST" && line != "DELETE" && line != "HEAD")
				throw std::invalid_argument("Invalid http method");
			else
				this->allowed_methods.push_back(line);
			break;
		}
		line = str.substr(begin, end - begin);
		if (line != "GET" && line != "POST" && line != "DELETE" && line != "HEAD")
			throw std::invalid_argument("Invalid http method");
		else
			this->allowed_methods.push_back(line);
		begin = end + 1;
		if (begin == std::string::npos)
			break;	
	}
	std::vector<std::string>::iterator itb = this->allowed_methods.begin();
	for (;itb != this->allowed_methods.end(); itb++)
		std::cout << *itb << std::endl;
}