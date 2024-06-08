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
	std::cout << name <<std::endl;
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
			end = str.find(";");
			line = str.substr(begin, end - begin);
			if (line != "GET" && line != "POST" && line != "DELETE" )
				throw std::invalid_argument("Invalid http method");
			else
				this->allowed_methods.push_back(line);
			break;
		}
		line = str.substr(begin, end - begin);
		if (line != "GET" && line != "POST" && line != "DELETE")
			throw std::invalid_argument("Invalid http method");
		else
			this->allowed_methods.push_back(line);
		begin = end + 1;
		if (begin == std::string::npos)
			break;
	}
}

void Location::addCgiPath(const std::string &cgi)
{
    this->cgiPath.push_back(cgi);
}

void Location::setCgiExtension(const std::string &ext)
{
    if(ext == ".py")
        this->cgiExtensions.push_back(".py");
    if(ext == ".sh")
        this->cgiExtensions.push_back(".sh");
}

void Location::setIndex(const std::string &index)
{
    this->index = index;
}

void Location::setUploadPath(const std::string &upload)
{
    this->uploadPath = upload;
}
