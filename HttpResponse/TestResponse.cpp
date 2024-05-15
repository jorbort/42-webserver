#include <string>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include "Response.hpp"

int main(void) {
	Response response;
	int		fd;

	fd = open("test.html", O_RDONLY);
	std::cout << response.createResponse(fd) << std::endl;
	close(fd);
}