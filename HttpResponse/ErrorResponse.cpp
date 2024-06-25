#include <string>
#include <fcntl.h>
#include <unistd.h>
#include "ErrorResponse.hpp"

std::string ErrorResponse::createErrorPage(int statusCode) {
	size_t		maxBodySize;
	std::string	body;
	size_t		contentLength;
	{
		int fd;
		
		switch (statusCode) {
			case 403:
				fd = open("../docs/web/error_pages/403.html", O_RDONLY);
				break ;
			case 404:
				fd = open("../docs/web/error_pages/404.html", O_RDONLY);
				break ;
			case 500:
				fd = open("../docs/web/error_pages/500.html", O_RDONLY);
				break ;
			default:
				fd = open("../docs/web/error_pages/500.html", O_RDONLY);
				break ;
		}
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
