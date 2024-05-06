#include "../Includes/WebServ.hpp"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <iostream>
#include <unistd.h>

void Server::RunServer(void)
{
	int new_socket;
	std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";
	this->sfd = socket(AF_INET, SOCK_STREAM, 0);  
	if (this->sfd == -1)
		throw SocketException();
	
	sockaddr_in port;
	int portlen = sizeof(port);

	port.sin_family = AF_INET;
	port.sin_port = htons(4242);
	port.sin_addr.s_addr = INADDR_ANY;

	memset(port.sin_zero, '\0', sizeof(port.sin_zero));

	if (bind(this->sfd, (struct sockaddr *) &port, sizeof(port)) == -1)
		throw SocketException();

	if (listen(this->sfd, 10) < 0)
		throw SocketException();
	while (42)
	{
		std::cout << "waiting for conection" << std::endl;
		if (( new_socket = accept(this->sfd,(struct sockaddr *) &port, (socklen_t *)&portlen)) < 0)
			throw SocketException();
		char buffer [3000] = {0};
		if (read(new_socket, buffer, 3000) < 0)
			throw SocketException();
		std::cout << buffer << std::endl;
		write(new_socket, response.c_str(), response.length());
		close(new_socket);
	}

}

const char * Server::SocketException::what() const throw()
{
	return ("error creating or binding the socket");
}