#include "Response.hpp"
#include <string>
#include <ctime>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

Response::~Response() {}

std::string Response::createResponse(void) {
	return "Not Implemented";
}

std::string Response::createGETresponse(int fd) {
	int			statusCode;
	size_t		maxBodySize;
	std::string	body;
	size_t		contentLength;
	{
		statusCode = 200;
		maxBodySize = sizeof(size_t);
		setBody(body, contentLength, fd, maxBodySize);
	}

	std::string response;

	response = addStatusLine(statusCode);
	response += addDateHeader();
	response += addServerHeader();
	response += addContentTypeHeader(HTML);
	response += addContentLengthHeader(contentLength);
	response += addLastModified();
	response += "\r\n";
	response += body;
	return response;
}

std::string Response::create404NotFoundResponse(void) {
	int			statusCode;
	size_t		maxBodySize;
	std::string	body;
	size_t		contentLength;
	{
		int	fd = open("../docs/web/error_pages/404.html", O_RDONLY);
		statusCode = 404;
		maxBodySize = sizeof(size_t);
		setBody(body, contentLength, fd, maxBodySize);
		close(fd);
	}

	std::string	response;

	response = addStatusLine(statusCode);
	response += addDateHeader();
	response += addContentTypeHeader(HTML);
	response += addContentLengthHeader(contentLength);
	response += "\r\n";
	response += body;
	return response;
}

void	Response::setBody(std::string &body, size_t &contentLength, int fd, size_t maxBodySize) {
	char		buffer[1024];
	ssize_t		bytesRead;
	size_t		totalBytesRead;

	totalBytesRead = 0;
	while ((bytesRead = read(fd, buffer, sizeof(buffer) - 1)) > 0 &&
			totalBytesRead < maxBodySize) {
		buffer[bytesRead] = '\0';
		body.append(buffer);
		buffer[0] = '\0';
		totalBytesRead += bytesRead;
	}
	contentLength = totalBytesRead;
}
