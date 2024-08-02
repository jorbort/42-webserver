/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jorge <jorge@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 19:23:57 by juan-anm          #+#    #+#             */
/*   Updated: 2024/08/02 01:46:35 by jorge            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequestParser.hpp"
#include "../Includes/Logger.hpp"

HttpRequestParser::HttpRequestParser(){
	_RealHeaders.push_back("Host");
	_RealHeaders.push_back("User-Agent");
	_RealHeaders.push_back("Accept");
	_RealHeaders.push_back("Accept-Encoding");
	_RealHeaders.push_back("Accept-Language");
	_RealHeaders.push_back("Content-Type");
	_RealHeaders.push_back("Authorization");
	_RealHeaders.push_back("Cache-Control");
	_RealHeaders.push_back("Connection");
	_RealHeaders.push_back("Cookie");
	_RealHeaders.push_back("Referer");
	_RealHeaders.push_back("Range");
	_RealHeaders.push_back("Content-Length");
	_RealHeaders.push_back("Expect");
	_RealHeaders.push_back("Origin");
	_RealHeaders.push_back("Transfer-Encoding");
}

HttpRequestParser::~HttpRequestParser(){}

void	HttpRequestParser::parseRequest(HttpRequest &request_class, char *original_str, size_t len){
	std::string					line;
	std::string 				req_str = original_str;
	std::istringstream			Req(req_str);
	std::vector<std::string>	lines;
	size_t 						bytes_read = 0;

	if (req_str.empty() || check_request_str(req_str.c_str()) || invalid_CRLF(req_str)){ 
		request_class._ErrorCode = 400;
		return;
	}
	while (std::getline(Req, line)){
		bytes_read += line.length() + 1;
		if (!line.empty() && line.at(line.length() - 1) == '\r')
			line.erase(line.length() - 1);
		if (line.empty() && lines.size() != 0)
			break;
		if (line.empty() && lines.size() == 0 && bytes_read > 2){
			request_class._ErrorCode = 400;
			return;
		}
		if (!line.empty())
			lines.push_back(line);
	}
	if (lines.empty()){
		request_class._ErrorCode = 400;
		return;
	}
	if (parseFirstLine(request_class, lines[0]) || parseHeaders(request_class, lines) || parseURI(request_class)){
		return;
	}
	parseBody(request_class, original_str + bytes_read, len - bytes_read);
}
// Need to implement Folding?
// check for bodys null characters how to implement = full request size needed from server;

bool	HttpRequestParser::parseFirstLine(HttpRequest &request_class, const std::string &str){
size_t method_end = str.find(" ");
    if (method_end == std::string::npos) {
        request_class._ErrorCode = 400; // Bad Request
        return 1;
    }
	request_class._method = str.substr(0, method_end);
    if (check_method(request_class)) {
        request_class._ErrorCode = 405; // Method Not Allowed
        return 1;
    }
    size_t uri_start = method_end + 1;
    size_t uri_end = str.find(" ", uri_start);
    if (uri_end == std::string::npos) {
        request_class._ErrorCode = 400; // Bad Request
        return 1;
    }
    request_class._URI = str.substr(uri_start, uri_end - uri_start);
    size_t version_start = uri_end + 1;
    request_class._version = str.substr(version_start);
    if (request_class._version.compare("HTTP/1.1") != 0) {
        request_class._ErrorCode = 505; // HTTP Version Not Supported
        //std::cout << "6" << std::endl;
        return 1;
    }
    return 0;
}

// insert headers into map
// check if header already exists and if its a real header
// clean of white spaces header value string
// check in case of post for body lenght or tranfer encoding chunked
bool	HttpRequestParser::parseHeaders(HttpRequest &request_class, const std::vector<std::string>	&lines){
	std::string key;
	std::string value;

	for (unsigned int i = 1; i  < lines.size(); i++){
			key = lines[i].substr(0, lines[i].find(':'));
			value = cleanWSpaces(lines[i].substr(lines[i].find(':') + 1));
		if ((request_class._headers.find(key) != request_class._headers.end()) || value.empty()
			|| isValidHeaderValue(lines[i])){
				request_class._ErrorCode = 400;
				return 1;
		}
		request_class._headers[key] = value;
	}
	if (request_class._RequestMethod == POST){
		if (request_class._headers.find("Content-Length") != request_class._headers.end())
		{
			request_class._ContentLength = atoi(request_class._headers.find("Content-Length")->second.c_str());
		}
		else if (request_class._headers.find("Transfer-Encoding") != request_class._headers.end()){
			if (request_class._headers.find("Transfer-Encoding")->second == "chunked")
				request_class._chunked = true;
		}
	}
	if (request_class._headers.find("Host") == request_class._headers.end()){
		request_class._ErrorCode = 400;
		return 1;
	}
	return 0;
}

bool HttpRequestParser::parseURI(HttpRequest &request_class){
	HttpRequest &rq = request_class;
	std::string uri = rq.getURI();

	size_t Host_end = uri.find(request_class._headers.find("Host")->second);
	if (Host_end != std::string::npos){
		uri.erase(Host_end, request_class._headers.find("Host")->second.length());
		if (uri[0] == ':'){
			uri.erase(0, 1);
			size_t Port_end = uri.find_first_not_of("0123456789", 1);
			//std::cout << Port_end << std::endl;
			request_class._URI_tcpPort = atol(uri.substr(0, Port_end).c_str());
			uri.erase(0, Port_end);
		}
	}
	if (uri[0] == '/'){
		size_t path_start = uri.find('?');
		request_class._URI_path = uri.substr(1 , path_start);
		uri.erase(0, request_class._URI_path.length());
		if ((path_start = uri.find('?')) != std::string::npos)
		{
			std::string temp;
			std::stringstream ss(uri);
			while (getline(ss, temp, '&'))
			{
				//std::cout << temp << std::endl;
				size_t path_end = temp.find('=');
				if (path_end == std::string::npos || temp.find('=') != temp.rfind('=')){
					request_class._ErrorCode = 400;
					return 1;
				}
				std::string key = temp.substr(path_start + 1, (path_end - path_start) - 1);
				path_start = temp.find('&');
				std::string value = temp.substr(path_end + 1, (path_start - path_end) - 1);
				request_class._URI_query[key] = value;
			}
		}
	}

	return 0;
}

bool	HttpRequestParser::check_request_str(const char *str){
	int i = 0;

	while(str[i]){
		if (!isChar(str[i]))
			return 1;
		i++;
	}
	return 0;
}

// check for more than one double CRLF in the request, if so its an ERROR
bool	HttpRequestParser::invalid_CRLF(const std::string &str){
int count = 0;
size_t pos = 0;
while ((pos = str.find("\r\n\r\n", pos)) != std::string::npos) {
	count++;
	pos += 4; // Move ahead by 4 characters to avoid finding the same sequence again
}
if (count > 2)
	return 1;
else
	return 0;
}
bool	HttpRequestParser::check_method(HttpRequest &request_class){
	if (request_class._method == "GET")
		request_class._RequestMethod = GET;
	else if (request_class._method == "POST")
		request_class._RequestMethod = POST;
	else if (request_class._method == "DELETE")
		request_class._RequestMethod = DELETE;
	else{
		return 1;
	}
	return 0;
}

// void	HttpRequestParser::parseBody(HttpRequest &request_class, const char *begin, const char *end){
void	HttpRequestParser::parseBody(HttpRequest &request_class, char *begin, size_t contentlength){
	size_t i = 0;
	(void)contentlength;
	if (!begin){
		throw std::runtime_error("strin vacio  en el body");
	}
	while(i < request_class._ContentLength){
		request_class._body.push_back(*begin++);
		i++;
	}
}

// check if theres more than one specific char inside the string
bool HttpRequestParser::containsMoreThanOne(const std::string& str, char ch) {
    return std::count(str.begin(), str.end(), ch) > 1;
}

// check for invalid chars inside the header value string
bool	HttpRequestParser::isValidHeaderValue(const std::string& str){
	int i = 0;

	while(str[i]){
		if (!isprint(str[i]) && !isspace(str[i]))
			return 1;
		i++;
	}
	return 0;

}

// clean leading and trailing isspaces from string
// 3.2.  Header Fields RFC7230 OWS
std::string	HttpRequestParser::cleanWSpaces(const std::string& str){
	std::string::size_type start = 0;

	while (start < str.size() && std::isspace(str[start])){
		start++;
	}
	std::string::size_type end = str.size() - 1;
	while (end > 0 && std::isspace(str[end])){
		end--;
	}
	return (str.substr(start, end - start + 1));
}

// Check if a byte is an HTTP character.
bool HttpRequestParser::isChar(int c){
	return c >= 0 && c <= 127;
}

// Check if a byte is an HTTP control character.
bool HttpRequestParser::isControl(int c){
	return (c >= 0 && c <= 31) || (c == 127);
}

// Check if a byte is defined as an HTTP special character.
bool HttpRequestParser::isSpecial(int c){
	switch (c){
		case '(': case ')': case '<': case '>': case '@':
		case ',': case ';': case ':': case '\\': case '"':
		case '/': case '[': case ']': case '?': case '=':
		case '{': case '}': case ' ': case '\t':
			return true;
		default:
			return false;
	}
}

// Check if a byte is a digit.
bool HttpRequestParser::isDigit(int c){
	return c >= '0' && c <= '9';
}


bool HttpRequestParser::isValidPath(const char* path) {
    struct stat info;
    if (stat(path, &info) != 0)
        return false;
    else if (info.st_mode & (S_IFDIR | S_IFREG))
        return true;
    else
        return false;
}
