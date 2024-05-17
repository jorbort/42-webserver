#include "Response.hpp"
#include <string>
#include <ctime>
#include <fcntl.h>
#include <unistd.h>

Response::~Response() {}

std::string Response::createGETresponse(int fd) {
	int			statusCode;
	size_t		maxBodySize;
	std::string	body;
	size_t		contentLength;
	{
		statusCode = 200;
		maxBodySize = SIZE_MAX;
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
		maxBodySize = SIZE_MAX;
		setBody(body, contentLength, fd, maxBodySize);
		close(fd);
	}

	std::string	response;

	response = addStatusLine(404);
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
	while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0 &&
			totalBytesRead < maxBodySize) {
		body += buffer;
		std::memset(buffer, 0, sizeof(buffer));
		totalBytesRead += bytesRead;
	}
	contentLength = totalBytesRead;
}
