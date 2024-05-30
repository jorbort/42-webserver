#include <string>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "Response.hpp"

#define YELLOW "\033[1;33m"
#define COLOR_OFF "\033[0m"

void	test_GET_response(void)
{
	int	fd;

	std::cout << YELLOW << "Test GET Response" << COLOR_OFF << "\n\n";
	fd = open("test.html", O_RDONLY);
	std::cout << Response::createGETresponse(fd) << std::endl;
	close(fd);
	std::cout << std::endl;
}

void	test_404NotFound_response(void)
{
	std::cout << YELLOW << "Test 404NotFound Response" << COLOR_OFF << "\n\n";
	std::cout << Response::create404NotFoundResponse() << std::endl;
	std::cout << std::endl;
}

int main(void) {
	test_GET_response();
	test_404NotFound_response();
}