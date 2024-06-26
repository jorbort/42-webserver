#include <string>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "Response.hpp"
#include "../HttpRequest/HTTPRequest.hpp"

#define YELLOW "\033[1;33m"
#define COLOR_OFF "\033[0m"

void	test_GET_response(void)
{
	HttpRequest request;

	request.setMethod("GET");
	request.setVersion("HTTP/1.1");
	request._URI_path = "./test.html";

	Response response(request);
	std::cout << YELLOW << "Test GET Response" << COLOR_OFF << "\n\n";
	std::cout << response.createResponse() << std::endl;
	std::cout << std::endl;
}

void	test_GET_CGI_response(void)
{
	HttpRequest request;

	request.setMethod("GET");
	request.setVersion("HTTP/1.1");
	request._URI_path = "./test.sh";

	Response response(request);
	std::cout << YELLOW << "Test GET CGI Response" << COLOR_OFF << "\n\n";
	std::cout << response.createResponse() << std::endl;
	std::cout << std::endl;
}

void	test_404NotFound_response(void)
{
	HttpRequest request;

	request.setMethod("GET");
	request.setVersion("HTTP/1.1");
	request._URI_path = "./noExist.html";

	
	Response response(request);
	std::cout << YELLOW << "Test 404NotFound Response" << COLOR_OFF << "\n\n";
	std::cout << response.createResponse() << std::endl;
	std::cout << std::endl;
}

int main(void) {
	test_GET_response();
	test_GET_CGI_response();
	test_404NotFound_response();
}