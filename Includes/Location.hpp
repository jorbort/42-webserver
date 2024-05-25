#pragma once

#include <string>
#include <vector>

class Location
{
	protected:
		std::string name;
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

		void setAddress(const std::string &address);
		void setRoot(const std::string &root);
		void addMethods(const std::string &methods);
		void toggleAutoIndex();
		void addCgiPath(const std::string &cgi);
		void setPath(const std::string &path);
		void setName(const std::string &name);

		std::string &getAdress(void);
		std::string &getRoot(void);
		std::string::iterator &getMethods(void);
		bool getAutoindex();
		std::string &getCgiPath(std::string &ext);
		std::string &getPath(void);
};
