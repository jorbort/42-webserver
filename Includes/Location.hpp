#pragma once

#include <string>
#include <vector>

class Location
{
	protected:
		std::string address;
		std::string root;
		std::vector<std::string> allowed_methods;
		bool autoindex;
		std::string index;
		std::vector<std::string> cgiPath;
		std::string path;
	public:
		Location();
		~Location();
};
