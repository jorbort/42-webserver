#include "ResponseHeader.hpp"
#include <string>
#include <ctime>
#include <unistd.h>
#include <sstream>

ResponseHeader::~ResponseHeader() {}

std::string	ResponseHeader::addStatusLine(int statusCode) {
	std::string statusLine;

	statusLine += "HTTP/1.1 ";
	statusLine += toString(statusCode);
	statusLine += ' ';
	statusLine += getStatusCodeDescription(statusCode);
	statusLine += "\r\n";
	return statusLine;
}

std::string	ResponseHeader::addDateHeader(void) {
	std::string	date;
   	time_t		now;
	tm			*gmtm;

	date = "Date: ";
	{
		now = time(0);
		gmtm = gmtime(&now);
	}
	date += asctime(gmtm);
	date.replace(date.rfind('\n'), 1, "\r\n");
	return date;
}

std::string	ResponseHeader::addServerHeader(void) {
	std::string server;

	server = "Server: ";
	server += "NOT IMPLEMENTED";
	server += "\r\n";
	return server;
}

std::string	ResponseHeader::addContentTypeHeader(ContentType type) {
	std::string	contentType;

	contentType = "Content-Type: ";
	contentType += getContentType(type);
	contentType += "\r\n";
	return contentType;
}

std::string	ResponseHeader::addContentLengthHeader(int length) {
	std::string contentLength;

	contentLength = "Content-Length: ";
	contentLength += toString(length);
	contentLength += "\r\n";;
	return contentLength;
}

std::string	ResponseHeader::addLastModified(void) {
	std::string lastModified;

	lastModified = "Last-Modified: ";
	lastModified += "NOT IMPLEMENTED";
	lastModified += "\r\n";
	return lastModified;
}

void	ResponseHeader::setBody(std::string &body, size_t &contentLength, int fd, size_t maxBodySize) {
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
	//TODO add reading error handling
	contentLength = totalBytesRead;
}

std::string ResponseHeader::replaceNewlines(const std::string& input) {
    std::string result;
    
    for (std::string::const_iterator it = input.begin(); it != input.end(); ++it) {
        if (*it == '\n')
            result += "\r\n";
        else
            result += *it;
    }
    return result;
}

std::string	ResponseHeader::getStatusCodeDescription(int statusCode) {
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

std::string	ResponseHeader::getContentType(ContentType type) {
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

std::string	ResponseHeader::toString(int nb) {
	std::ostringstream temp;
    temp << nb;
    return temp.str();
}