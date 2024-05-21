/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestParser.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juan-anm < juan-anm@student.42barcelona    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 00:20:15 by juan-anm          #+#    #+#             */
/*   Updated: 2024/05/21 17:45:48 by juan-anm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTPARSER_HPP
#define HTTPREQUESTPARSER_HPP

#include <iostream>
#include <string>
#include <map>
#include "HttpRequest.hpp"

enum State_Parse{
	PushMethod,
	PushURI,
	PushVersion,
	PushHeaders,
	PushBody,
};

class HttpRequestParser
{
	private:
							HttpRequestParser(const HttpRequestParser &other);
		HttpRequestParser&	operator=(const HttpRequestParser &other);
		State_Parse			State;
	
	public:
							HttpRequestParser();
							~HttpRequestParser();
		
		// void	parseRequest(HttpRequest &request, const char *begin, const char *end);
		void	parseRequest(HttpRequest &request, const std::string &req);
		void	parseBody(HttpRequest &request, const char *begin, const char *end);
		bool	isChar(int c);
		bool	isControl(int c);
		bool	isSpecial(int c);
		bool	isDigit(int c);
		
};

#endif
