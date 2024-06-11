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
bool Location::isAutoindex(void)
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