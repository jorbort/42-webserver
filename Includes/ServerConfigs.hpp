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
		long clientMaxBodySize;
		std::vector<Location> locations;
		std::vector<std::string> serverName;
		std::string index;
		std::string root;
		struct sockaddr_in *_serverAddress;
		int _fd;
		int autoindex;
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
		void toggleAutoindex(const std::string &status);
		void addErrorPage(const std::string &errors);

		int getListen() const;
		unsigned int getHostIp() const;
		std::vector<Location> getLocations() const;
		std::vector<std::string> getServerName() const;
		long getClientMaxBodySize() const;
		std::string getIndex() const;
		std::string getRoot() const;
		struct sockaddr_in *getServerAddress() const;
		int getSocket() const;
		int getAutoindex() const;
		const std::string &getErrorPage(int key) const;

};

#endif
