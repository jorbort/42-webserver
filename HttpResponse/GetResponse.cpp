#include "GetResponse.hpp"
#include <string>
#include <unistd.h>

std::string GetResponse::createGETresponse(int fd) {
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