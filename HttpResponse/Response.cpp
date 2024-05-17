#include "Response.hpp"
#include <string>
#include <ctime>
#include <unistd.h>

Response::~Response() {}

std::string Response::createGETresponse(int fd) {
	int			statusCode;
	size_t		maxBodySize;
	std::string	body;
	size_t		contentLength;
	std::string response;
	{
		statusCode = 200;
		maxBodySize = SIZE_MAX;
		setBody(body, contentLength, fd, maxBodySize);
	}

	response = addStatusLine(statusCode);
	response += addDateHeader();
	response += addContentTypeHeader(HTML);
	response += addContentLengthHeader(contentLength);
	response += addLastModified();
	response += "\r\n";
	response += body;
	return response;
}

void	Response::setBody(std::string &body, size_t &contentLength, int fd, size_t maxBodySize) {
	char		buffer[1024];
	ssize_t		bytesRead;
	size_t		totalBytesRead;

	totalBytesRead = 0;
	while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0 &&
			totalBytesRead < maxBodySize) {
		body += buffer;
		std::memset(buffer, 0, sizeof(buffer));
		totalBytesRead += bytesRead;
	}
	contentLength = totalBytesRead;
}
