#include <string>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "Response.hpp"
#include "../HttpRequest/HTTPRequest.hpp"

#define YELLOW "\033[1;33m"
#define COLOR_OFF "\033[0m"

void	test_GET_response(void) {
	HttpRequest request;

	request.setMethod("GET");
	request.setVersion("HTTP/1.1");
	request._URI_path = "./test.html";

	Response response(request);
	std::cout << YELLOW << "Test GET Response" << COLOR_OFF << "\n\n";
	std::cout << response.createResponse() << std::endl;
	std::cout << std::endl;
}

void	test_GET_CGI_response(void) {
	HttpRequest request;

	request.setMethod("GET");
	request.setVersion("HTTP/1.1");
	request._URI_path = "./test.sh";

	Response response(request);
	std::cout << YELLOW << "Test GET CGI Response" << COLOR_OFF << "\n\n";
	std::cout << response.createResponse() << std::endl;
	std::cout << std::endl;
}

void	test_POST_response(void) {
	HttpRequest request;

	request.setMethod("POST");
	request.setVersion("HTTP/1.1");
	request._URI_path = "./post_test.html";
	std::string body;
	body = "<http>\n";
	body += "  <body>\n";
	body += "    <h1>this is post test</h1>\n";
	body += "  </body>\n";
	body += "</http>\n";
	for (size_t i = 0; i < body.size(); i++)
		request._body.push_back(body[i]);
	request._ContentLength = body.size();

	Response response(request);
	std::cout << YELLOW << "Test POST Response" << COLOR_OFF << "\n\n";
	std::cout << response.createResponse() << std::endl;
	std::cout << std::endl;
}

void	test_POST_CGI_response(void) {
	HttpRequest request;

	request.setMethod("POST");
	request.setVersion("HTTP/1.1");
	request._URI_path = "./test.py";
	std::string body;
	body = "<http>\n";
	body += "  <body>\n";
	body += "    <h1>this is post test</h1>\n";
	body += "  </body>\n";
	body += "</http>\n";
	for (size_t i = 0; i < body.size(); i++)
		request._body.push_back(body[i]);
	request._ContentLength = body.size();

	Response response(request);
	std::cout << YELLOW << "Test POST Response" << COLOR_OFF << "\n\n";
	std::cout << response.createResponse() << std::endl;
	std::cout << std::endl;
}

void	test_DELETE_response(void) {
	HttpRequest request;
	{
		request.setMethod("POST");
		request.setVersion("HTTP/1.1");
		request._URI_path = "./tmp.html";
		std::string body;
		body = "<http>\n";
		body += "  <body>\n";
		body += "    <h1>this is temporal file to delete</h1>\n";
		body += "  </body>\n";
		body += "</http>\n";
		for (size_t i = 0; i < body.size(); i++)
			request._body.push_back(body[i]);
		request._ContentLength = body.size();

		Response response(request);
		response.createResponse();
	}
	sleep(2);
	request.setMethod("DELETE");
	request.setVersion("HTTP/1.1");
	request._URI_path = "./tmp.html";

	Response response(request);
	std::cout << YELLOW << "Test DELETE Response" << COLOR_OFF << "\n\n";
	std::cout << response.createResponse() << std::endl;
	std::cout << std::endl;
}

void	test_404NotFound_response(void) {
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
	test_POST_response();
	test_POST_CGI_response();
	test_DELETE_response();
	test_404NotFound_response();
}