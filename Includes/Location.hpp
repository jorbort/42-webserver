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

		void setAddress(std::string &address);
		void setRoot(std::string &root);
		void addMethod(std::string &method);
		void toggleAutoIndex();
		void addCgiPath(std::string &cgi);
		void setPath(std::string);

		std::string &getAdress(void);
		std::string &getRoot(void);
		std::string::iterator &getMethods(void);
		bool getAutoindex();
		std::string &getCgiPath(std::string &ext);
		std::string &getPath(void);
};
