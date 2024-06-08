#pragma once

#include <string>
#include <vector>

class Location
{
	protected:
		std::string name;
		std::string root;
		std::vector<std::string> allowed_methods;
		bool autoindex;
		std::string index;
		std::vector<std::string> cgiPath;
		std::string path;
		std::vector<std::string> cgiExtensions;
		std::string uploadPath;
	public:
		Location();
		~Location();

		void setRoot(const std::string &root);
		void addMethods(const std::string &methods);
		void toggleAutoIndex();
		void addCgiPath(const std::string &cgi);
		void setPath(void);
		void setName(const std::string &name);
		void setCgiExtension(const std::string &ext);
		void setIndex(const std::string &index);
		void setUploadPath(const std::string &uploadPath);

		bool checkLocation(void);

		std::string &getAdress(void);
		std::string &getRoot(void);
		std::string::iterator &getMethods(void);
		bool getAutoindex();
		std::string &getCgiPath(std::string &ext);
		std::string &getPath(void);
};
