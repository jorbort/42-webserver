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
		unsigned int hostIp;
		std::vector<Location> locations;
		std::vector<std::string> serverName;
		long ClientMaxBodySize;
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
		void setBodySize(const std::string &number);
		void setIndex(const std::string &path);
		void setRoot(const std::string &path);
		void setHost(const std::string &host);
		void setSocket(int fd);
		void toggleAutoindex();
		void addErrorPage(const std::string &errors);

};

#endif
