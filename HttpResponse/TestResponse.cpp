#include <string>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include "Response.hpp"

void	test_GET_response(void)
{
	int	fd;

	fd = open("test.html", O_RDONLY);
	std::cout << Response::createGETresponse(fd) << std::endl;
	close(fd);
}

int main(void) {
	test_GET_response();
}