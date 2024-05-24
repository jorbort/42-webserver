/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juan-anm < juan-anm@student.42barcelona    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 19:23:57 by juan-anm          #+#    #+#             */
/*   Updated: 2024/05/24 18:01:18 by juan-anm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequestParser.hpp"

HttpRequestParser::HttpRequestParser() : State(PushMethod){
}

HttpRequestParser::~HttpRequestParser(){}

// void	HttpRequestParser::parseRequest(HttpRequest &request_class, const char *begin, const char *end){
void	HttpRequestParser::parseRequest(HttpRequest &request_class, const std::string &req_str){
	std::vector<std::string>	lines;
	std::istringstream			Req(req_str);
	std::string					line;
	size_t 						bytes_read = 0;
	unsigned int 				i = 0;
	
	if (req_str.empty() || check_request_str(req_str.c_str()) || invalid_CRLF(req_str)){
		request_class._RequestState = ERROR;
		std::cout << "1" << std::endl;
		return;
	}
	while (std::getline(Req, line)){
		bytes_read += line.length() + 1;
		if (!line.empty() && line.at(line.length() - 1) == '\r')
			line.erase(line.length() - 1);
		if (line.empty() && lines.size() != 0)
			break;
		if (line.empty() && lines.size() == 0 && bytes_read > 1){
			request_class._RequestState = ERROR;
			return;
		}
		if (!line.empty())
			lines.push_back(line);
	}
	if (lines.size() == 0){
		request_class._RequestState = ERROR;
		std::cout << "2" << std::endl;
		return;
	}
	request_class._method = lines[0].substr(0, lines[0].find(' '));
	if (check_method(request_class)){
		request_class._RequestState = ERROR;
		std::cout << "3" << std::endl;
		return;
	}
	request_class._URI = lines[i].substr(lines[0].find(request_class._method) + request_class._method.size() + 1, lines[0].find(' '));
	request_class._version = lines[i].substr(lines[0].find(request_class._URI) + request_class._URI.size() + 1, lines[0].size());
	i++;		
	for (; i  < lines.size(); i++){
		if (lines[i] == "\r")
			break;
		std::string key = lines[i].substr(0, lines[i].find(':'));
		std::string value = lines[i].substr(lines[i].find(' ') + 1);
		request_class._headers[key] = value;
	}
	// if (bytes_read < req_str.length() && (request_class._headers.find("Transfer-Encoding") != request_class._headers.end()))
		parseBody(request_class, req_str.c_str() + bytes_read, req_str.c_str() + req_str.size());
}
// improve headers mapping
// body parsing ...
// check for correct sintaxis in header
// check 

bool	HttpRequestParser::check_request_str(const char *str){
	int i = 0;
	
	while(str[i])
	{
		if (!isChar(str[i]))
			return 1;
		i++;
	}
	return 0;
}

bool	HttpRequestParser::invalid_CRLF(const std::string &str){
int count = 0;
size_t pos = 0;

while ((pos = str.find("\r\n\r\n", pos)) != std::string::npos) {
	count++;
	pos += 2; // Move ahead by 2 characters to avoid finding the same sequence again
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

void	HttpRequestParser::parseBody(HttpRequest &request_class, const char *begin, const char *end){
	while(begin != end){
		request_class._body.push_back(*begin++);
	}
}

// Check if a byte is an HTTP character.
bool HttpRequestParser::isChar(int c)
{
return c >= 0 && c <= 127;
}

// Check if a byte is an HTTP control character.
bool HttpRequestParser::isControl(int c)
{
return (c >= 0 && c <= 31) || (c == 127);
}

// Check if a byte is defined as an HTTP special character.
bool HttpRequestParser::isSpecial(int c)
{
switch (c)
{
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
bool HttpRequestParser::isDigit(int c)
{
return c >= '0' && c <= '9';
}
