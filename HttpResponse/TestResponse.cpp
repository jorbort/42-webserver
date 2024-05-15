#include <string>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include "Response.hpp"

int main(int argc, char **argv) {
	int			fd;
	std::string	method;

	if (argc < 2)
	{
		std::cout << "arg 1: METHOD, arg 2: URL(GET, POST)" << std::endl;
		return (0);
	}
	method = argv[1];
	if (method.compare("GET") == 0)
	{
		fd = open(argv[2], O_RDONLY);
		Response response(Response::GET, 200, fd);
		std::cout << response.createResponse() << std::endl;
	}
	close(fd);
}