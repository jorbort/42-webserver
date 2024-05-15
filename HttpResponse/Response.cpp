#include "Response.hpp"
#include <string>
#include <ctime>
#include <unistd.h>

Response::Response(MethodType type, int statusCode, int fd) {
	this->method = type;
	this->statusCode = statusCode;
	this->fd = fd;
	this->maxBodySize = SIZE_MAX;
}

Response::~Response() {}

std::string	Response::createResponse(void) {
	if (this->method == GET)
		return createGETresponse();
	else if (this->method == POST)
		return createPOSTresponse();
	else if (this->method == DELETE)
		return createDELETEresponse();
	else
		return "Unknown Method\n";
}

std::string	Response::createGETresponse(void) {
	std::string response;
	int			tmpContentLength;

	tmpContentLength = 88;
	this->maxBodySize = tmpContentLength;
	response = addStatusLine();
	response += addDateHeader();
	response += addContentTypeHeader(HTML);
	response += addContentLengthHeader(tmpContentLength);
	response += addLastModified();
	response += "\r\n";
	response += addBody();
	return response;
}

std::string	Response::createPOSTresponse(void) {
	return "POST Response Not Implemented";
}

std::string	Response::createDELETEresponse(void) {
	return "DELETE Response Not Implemented";
}

std::string	Response::addStatusLine(void) {
	std::string statusLine;

	statusLine += "HTTP/1.1";
	statusLine += ' ';
	statusLine += std::to_string(this->statusCode);
	statusLine += ' ';
	statusLine += getStatusCodeDescription(this->statusCode);
	statusLine += "\r\n";
	return statusLine;
}

std::string	Response::addDateHeader(void) {
	std::string	date;
   	time_t		now;
	tm			*gmtm;

	date = "Date:";
	date += " ";
	{
		now = time(0);
		gmtm = gmtime(&now);
	}
	date += asctime(gmtm);
	date.replace(date.rfind('\n'), 1, "\r\n");
	return date;
}

std::string	Response::addContentTypeHeader(ContentType type) {
	std::string	contentType;

	contentType = "Content-Type:";
	contentType += ' ';
	contentType += getContentType(type);
	contentType += "\r\n";
	return contentType;
}

std::string	Response::addContentLengthHeader(int length) {
	std::string contentLength;

	contentLength = "Content-Length:";
	contentLength += " ";
	contentLength += std::to_string(length);
	contentLength += "\r\n";;
	return contentLength;
}

std::string	Response::addLastModified(void) {
	std::string lastModified;

	lastModified = "Last-Modified:";
	lastModified += ' ';
	lastModified += "NOT IMPLEMENTED";
	lastModified += "\r\n";
	return lastModified;
}

std::string	Response::addBody(void) {
	std::string	body;
	char		buffer[1024];
	ssize_t		bytesRead;
	size_t		totalBytesRead;

	totalBytesRead = 0;
	while ((bytesRead = read(this->fd, buffer, sizeof(buffer))) > 0 &&
			totalBytesRead < this->maxBodySize) {
		body += buffer;
		std::memset(buffer, 0, sizeof(buffer));
		totalBytesRead += bytesRead;
	}
	return (replaceNewlines(body));
}

std::string Response::replaceNewlines(const std::string& input) {
    std::string result;
    
    for (std::string::const_iterator it = input.begin(); it != input.end(); ++it) {
        if (*it == '\n')
            result += "\r\n";
        else
            result += *it;
    }
    return result;
}

std::string	Response::getStatusCodeDescription(int statusCode) {
	switch (statusCode) {
	//1xx: Information
		case 100: return "Continue";
		case 101: return "Switching Protocols";
	//2xx: Successful
		case 200: return "OK";
		case 201: return "Created";
		case 202: return "Accepted";
		case 203: return "authoritative Information";
		case 204: return "No Content";
		case 205: return "Reset Content";
		case 206: return "Partial Content";
	//3xx: Redirection
		case 300: return "Multiple Choices";
		case 301: return "Moved Permanently";
		case 302: return "Found";
		case 303: return "See Other";
		case 304: return "Not Modified";
		case 305: return "Use Proxy";
		case 306: return "Unused";
		case 307: return "Temporary Redirect";
	//4xx: Client Error
		case 400: return "Bad Request";
		case 401: return "Unauthorized";
		case 402: return "Payment Required";
		case 403: return "Forbidden";
		case 404: return "Not Found";
		case 405: return "Method Not Allowed";
		case 406: return "Not Acceptable";
		case 407: return "Proxy Authentication Required";
		case 408: return "Request Timeout";
		case 409: return "Conflict";
		case 410: return "Gone";
		case 411: return "Length Required";
		case 412: return "Precondition Failed";
		case 413: return "Request Entity Too Large";
		case 414: return "Request-url Too Long";
		case 415: return "Unsupported Media Type";
		case 416: return "Requested Range Not Satisfiable";
		case 417: return "Expectation Failed";
	//5xx: Server Error
		case 500: return "Internal Server Error";
		case 501: return "Not Implemented";
		case 502: return "Bad Gateway";
		case 503: return "Service Unavailable";
		case 504: return "Gateway Timeout";
		case 505: return "HTTP Version Not Supported";
		default: return "Unknown Status Code";
	}
}

std::string	Response::getContentType(ContentType type) {
	switch (type) {
		case HTML:          return "text/html; charset=UTF-8";
		case JSON:          return "application/json";
		case PLAIN_TEXT:    return "text/plain; charset=UTF-8";
		case XML:           return "application/xml";
		case JPEG:          return "image/jpeg";
		case PNG:           return "image/png";
		case GIF:           return "image/gif";
		case JAVASCRIPT:    return "application/javascript";
		case CSS:           return "text/css";
		case PDF:           return "application/pdf";
		case ZIP:           return "application/zip";
		default:            return "Unsupported Content Type";
	}
}