#ifndef SERVERCONFIGS_HPP
# define SERVERCONFIGS_HPP

# include <iostream>
# include <string>
# include <vector>
# include "Location.hpp"
# include <map>
//# include "WebServ.hpp"

class ServerConfigs
{
	private:
		int listen;
		std::vector<Location> locations;
		std::string serverName;
		unsigned long ClientMaxBodySize;
		std::string index;
		std::string root;
		struct sockaddr_in *_serverAddress;
		int _fd;
		bool autoindex;
		std::map<int, std::string> _errorPages;

	public:

		ServerConfigs();
		~ServerConfigs();
		void initErrorPages(void);

		void setListen(const std::string &port);
		void addLocation(const Location &location);
		void setName(const std::string &name);
		void setBodySize(unsigned long number);
		void setIndex(std::string &path);
		void setRoot(std::string &path);
		void setSocket(int fd);
		void toggleAutoindex();
		void addErrorPage(int key, std::string &value);

};

#endif
