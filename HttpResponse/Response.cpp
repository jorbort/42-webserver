#include "Response.hpp"
#include <string>

Response::Response() {
	inicializeStatusCodeMap();
}

Response::~Response() {}

std::string	Response::createResponse() {
	std::string response;
	int			tmpStatus;

	tmpStatus = 200;
	response += addStatusLine(tmpStatus);
	return response;
}

std::string	Response::addStatusLine(int status) {
	std::string statusLine;

	statusLine += "HTTP/1.1";
	statusLine += " ";
	statusLine += std::to_string(status);
	statusLine += " ";
	statusLine += getStatusDescription(status);
	statusLine += "\r\n";
	return statusLine;
}

std::string	Response::getStatusDescription(int status) {
	if (this->status_code_map.find(status) == this->status_code_map.end())
		return "STATUS_CODE_ERROR";
	else
		return this->status_code_map[status];
}

void	Response::inicializeStatusCodeMap(void) {
	//1xx: Information
	this->status_code_map[100] = "Continue";
	this->status_code_map[101] = "Switching Protocols";
	//2xx: Successful
	this->status_code_map[200] = "OK";
	this->status_code_map[201] = "Created";
	this->status_code_map[202] = "Accepted";
	this->status_code_map[203] = "Non-authoritative Information";
	this->status_code_map[204] = "No Content";
	this->status_code_map[205] = "Reset Content";
	this->status_code_map[206] = "Partial Content";
	//3xx: Redirection
	this->status_code_map[300] = "Multiple Choices";
	this->status_code_map[301] = "Moved Permanently";
	this->status_code_map[302] = "Found";
	this->status_code_map[303] = "See Other";
	this->status_code_map[304] = "Not Modified";
	this->status_code_map[305] = "Use Proxy";
	this->status_code_map[306] = "Unused";
	this->status_code_map[307] = "Temporary Redirect";
	//4xx: Client Error
	this->status_code_map[400] = "Bad Request";
	this->status_code_map[401] = "Unauthorized";
	this->status_code_map[402] = "Payment Required";
	this->status_code_map[403] = "Forbidden";
	this->status_code_map[404] = "Not Found";
	this->status_code_map[405] = "Method Not Allowed";
	this->status_code_map[406] = "Not Acceptable";
	this->status_code_map[407] = "Proxy Authentication Required";
	this->status_code_map[408] = "Request Timeout";
	this->status_code_map[409] = "Conflict";
	this->status_code_map[410] = "Gone";
	this->status_code_map[411] = "Length Required";
	this->status_code_map[412] = "Precondition Failed";
	this->status_code_map[413] = "Request Entity Too Large";
	this->status_code_map[414] = "Request-url Too Long";
	this->status_code_map[415] = "Unsupported Media Type";
	this->status_code_map[416] = "Requested Range Not Satisfiable";
	this->status_code_map[417] = "Expectation Failed";
	//5xx: Server Error
	this->status_code_map[500] = "Internal Server Error";
	this->status_code_map[501] = "Not Implemented";
	this->status_code_map[502] = "Bad Gateway";
	this->status_code_map[503] = "Service Unavailable";
	this->status_code_map[504] = "Gateway Timeout";
	this->status_code_map[505] = "HTTP Version Not Supported";
}