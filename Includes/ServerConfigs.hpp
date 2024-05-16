#ifndef SERVERCONFIGS_HPP
# define SERVERCONFIGS_HPP

# include <iostream>
# include <string>
# include <vector>
# include "Location.hpp"
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

	public:

		// ServerConfigs();
		// ServerConfigs( ServerConfigs const & src );
		// ~ServerConfigs();

		// ServerConfigs &		operator=( ServerConfigs const & rhs );

};

#endif /* *************************************************** SERVERCONFIGS_H */