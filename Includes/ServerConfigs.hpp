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
		// ServerConfigs( ServerConfigs const & src );
		~ServerConfigs();
		void initErrorPages(void);
		// ServerConfigs &		operator=( ServerConfigs const & rhs );

};

#endif 