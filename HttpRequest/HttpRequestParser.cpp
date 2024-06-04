/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juan-anm  <juan-anm@student.42barcel>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 19:23:57 by juan-anm          #+#    #+#             */
/*   Updated: 2024/06/04 16:22:44 by juan-anm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequestParser.hpp"

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

// void	HttpRequestParser::parseRequest(HttpRequest &request_class, const char *begin, const char *end){
void	HttpRequestParser::parseRequest(HttpRequest &request_class, char *original_str, size_t len){
	std::string					line;
	std::string 				req_str = original_str;
	std::istringstream			Req(req_str);
	std::vector<std::string>	lines;
	size_t 						bytes_read = 0;
		
	if (req_str.empty() || check_request_str(req_str.c_str()) || invalid_CRLF(req_str)){
		request_class._ErrorCode = 400;
		std::cout << "1" << std::endl;
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
		std::cout << "2" << std::endl;
		return;
	}
	if (parseFirstLine(request_class, lines[0]) || parseURI(request_class) || parseHeaders(request_class, lines))
		return;
		
	// if (bytes_read < req_str.length() && (request_class._headers.find("Transfer-Encoding") != request_class._headers.end()))
	std::cout << bytes_read << "bytes read " << req_str.size() << "req_str.size" << " " << len << " len"<< std::endl;
		parseBody(request_class, original_str + bytes_read, len - bytes_read);
}
// Need to implement Folding?
// check for bodys null characters how to implement = full request size needed from server;

bool	HttpRequestParser::parseFirstLine(HttpRequest &request_class, const std::string &str){
	request_class._method = str.substr(0, str.find(' '));
	if (check_method(request_class)){
		request_class._ErrorCode = 405;
		std::cout << "3" << std::endl;
		return 1;
	}
	request_class._URI = str.substr(str.find(request_class._method) + request_class._method.size() + 1, str.find(' '));
	request_class._version = str.substr(str.find(request_class._URI) + request_class._URI.size() + 1, str.size());
	if (request_class._version.compare("HTTP/1.1")){
		request_class._ErrorCode = 505;
		std::cout << "6" << std::endl;
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
		if ((std::find(_RealHeaders.begin(), _RealHeaders.end(), key) == _RealHeaders.end())
			|| (request_class._headers.find(key) != request_class._headers.end()) || value.empty()
			|| containsMoreThanOne(lines[i], ':') || isValidHeaderValue(lines[i])){
				request_class._ErrorCode = 400;
				std::cout << "helloo" << std::endl;
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
if (count > 1)
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

	while(i < contentlength){
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
