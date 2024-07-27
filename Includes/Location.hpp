#pragma once

#include <string>
#include <vector>

class Location
{
	protected:
		std::string name;
		std::string root;
		int autoindex;
		std::string index;
		std::vector<std::string> cgiPath;
		std::string path;
		std::vector<std::string> cgiExtensions;
		std::string uploadPath;
	public:
		Location();
		~Location();
		std::vector<std::string> allowed_methods;

		void setRoot(const std::string &root);
		void addMethods(const std::string &methods);
		void toggleAutoIndex(const std::string &status);
		void addCgiPath(const std::string &cgi);
		void setPath(void);
		void setName(const std::string &name);
		void setCgiExtension(const std::string &ext);
		void setIndex(const std::string &index);
		void setUploadPath(const std::string &uploadPath);

		bool checkLocation(void);

		const std::string &getName(void) const;
		const std::string &getRoot(void) const ;
		std::vector<std::string>::iterator getMethods(void);
		int isAutoindex(void);
		std::vector<std::string>::iterator getCgiPath(void);
		const std::string &getPath(void) const;
		const std::string &getUPloadPath(void) const;
		const std::string &getIndex(void) const;
};

std::ostream & operator<<(std::ostream &stream, const Location &loc);
