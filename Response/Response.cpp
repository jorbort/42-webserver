#include "Response.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

Response::Response(HttpRequest &request) {
	this->_method = request._method;
	this->_uri = request._URI_path;
	this->_statusCode = 200;
	this->_envp = NULL;
	initErrorPageMap();	
}

Response::~Response() {

}

std::string	Response::createResponse() {
	if (!isURIAcceptable(this->_uri))
		return errorResponse();
	if (isCGI(this->_uri)) {
		if (!isProcessableCGI(this->_uri))
			return errorResponse();
		handleCGI(this->_uri);
	}
	return "Not Implemented";
}

void	Response::initErrorPageMap() {
	this->_errorPageMap[400] = "docs/web/error_pages/400.html";
	this->_errorPageMap[403] = "docs/web/error_pages/403.html";
	this->_errorPageMap[404] = "docs/web/error_pages/404.html";
	this->_errorPageMap[500] = "docs/web/error_pages/500.html";
	this->_errorPageMap[502] = "docs/web/error_pages/502.html";
}

bool	Response::isURIAcceptable(const std::string uri) {
	struct stat info;
	if (stat(uri.c_str(), &info) != 0) {
		this->_statusCode = 404;
		return false;
	} else if (info.st_mode & S_IFDIR) {
		//Given uri points to directory.
		if (_method.compare("POST") == 0 || \
			_method.compare("DELETE") == 0) {
			this->_statusCode = 405;
			return false;
		}
		else if (_method.compare("GET") == 0) {
			//TODO it depends to server's configuration.
			this->_statusCode = 403;
			return false;
		}
		else {
			this->_statusCode = 405;
			return false;
		}
	}
	return true;
}

std::string	Response::errorResponse() {
	this->_body = readBody(getErrorPage());
	this->_contentLength = _body.size();

	std::string	response;

	response = addStatusLine(this->_statusCode);
	response += addDateHeader();
	response += addContentTypeHeader(HTML);
	response += addContentLengthHeader(this->_contentLength);
	response += "\r\n";
	response += this->_body;
	return response;
}

std::string Response::getErrorPage() {
	return this->_errorPageMap[this->_statusCode];
}

std::string Response::readBody(const std::string path) {
	int	fd;

	fd = open(path.c_str(), O_RDONLY);
	return this->readBody(fd);
}

std::string	Response::readBody(const int fd) {
	std::string body;
	char		buffer[1024];
	ssize_t		bytesRead;

	while ((bytesRead = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
		buffer[bytesRead] = '\0';
		body.append(buffer);
		buffer[0] = '\0';
	}
}