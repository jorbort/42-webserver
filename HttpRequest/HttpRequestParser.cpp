/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juan-anm < juan-anm@student.42barcelona    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 19:23:57 by juan-anm          #+#    #+#             */
/*   Updated: 2024/05/21 01:04:33 by juan-anm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequestParser.hpp"

HttpRequestParser::HttpRequestParser() : State(PushMethod) {
}

HttpRequestParser::~HttpRequestParser(){}

// void	HttpRequestParser::parseRequest(HttpRequest &request, const char *begin, const char *end){
void	HttpRequestParser::parseRequest(HttpRequest &request, const std::string &req){
	std::vector<std::string>	lines;
	std::istringstream			Req(req);
	std::string					line;
	size_t 						bytes_read = 0;
	unsigned int 				i = 0;
	
	if (req.empty()){
		request._RequestState = ERROR;
		return;
	}
	while (std::getline(Req, line)){
		bytes_read += line.length() + 1;
		if (!line.empty() && line.at(line.length() - 1) == '\r')
			line.erase(line.length() - 1);
		if (line.empty() && lines.size() != 0)
			break;
		if (line.empty() && lines.size() == 0 && bytes_read > 1){
		request._RequestState = ERROR;
		return;
		}
		if (!line.empty())
			lines.push_back(line);
	}
	if (lines.size() == 0){
		request._RequestState = ERROR;
		return;
	}
	if (bytes_read < req.length())
		//fill vector of remaining bytes
	request._method = lines[0].substr(0, lines[0].find(' '));
	if (request._method == "GET")
		request._RequestMethod = GET;
	else if (request._method == "POST")
		request._RequestMethod = POST;
	else if (request._method == "DELETE")
		request._RequestMethod = DELETE;
	else{
		request._RequestMethod = NONE;
		request._RequestState = ERROR;
		return;
	}
		request._URI = lines[i].substr(lines[0].find(request._method) + request._method.size() + 1, lines[0].find(' '));
		request._version = lines[i].substr(lines[0].find(request._URI) + request._URI.size() + 1, lines[0].size());
	i++;		
	for (; i  < lines.size(); i++){
		if (lines[i] == "\r")
			break;
		std::string key = lines[i].substr(0, lines[i].find(':'));
		std::string value = lines[i].substr(lines[i].find(' '));
		request._headers[key] = value;
	}
}
//check for strange characters in line
// check for double RCLF in lines that are not seprating body
// check for body headers and control
// improve headers mapping

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
