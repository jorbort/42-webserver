/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juan-anm < juan-anm@student.42barcelona    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 19:23:57 by juan-anm          #+#    #+#             */
/*   Updated: 2024/05/16 00:49:25 by juan-anm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequestParser.hpp"

HttpRequestParser::HttpRequestParser(){}

HttpRequestParser::~HttpRequestParser(){}

void	HttpRequestParser::parseRequest(HttpRequest &request, const char *begin, const char *end){
	while(begin != end)
	{
		


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
