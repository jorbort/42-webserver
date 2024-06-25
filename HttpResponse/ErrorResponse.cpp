#include <string>
#include <fcntl.h>
#include <unistd.h>
#include "ErrorResponse.hpp"

std::string ErrorResponse::create404NotFoundResponse(void) {
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