#include <string>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "Response.hpp"
#include "HTTPRequest.hpp"

#define YELLOW "\033[1;33m"
#define COLOR_OFF "\033[0m"

void	test_GET_response(void)
{
	Response response;
	HttpRequest request;

	request.setMethod("GET");
	request.setVersion("HTTP/1.1");
	request.setURI("./test.sh");
	std::cout << YELLOW << "Test GET Response" << COLOR_OFF << "\n\n";
	std::cout << response.createResponse(request) << std::endl;
	std::cout << std::endl;
}

void	test_404NotFound_response(void)
{
	Response response;

	std::cout << YELLOW << "Test 404NotFound Response" << COLOR_OFF << "\n\n";
	std::cout << response.create404NotFoundResponse() << std::endl;
	std::cout << std::endl;
}

int main(void) {
	test_GET_response();
	test_404NotFound_response();
}