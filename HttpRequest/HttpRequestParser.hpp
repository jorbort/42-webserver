/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequestParser.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juan-anm < juan-anm@student.42barcelona    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 00:20:15 by juan-anm          #+#    #+#             */
/*   Updated: 2024/05/31 00:27:11 by juan-anm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTPARSER_HPP
#define HTTPREQUESTPARSER_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
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
		// void	parseRequest(HttpRequest &request, const std::string &req);
		// void	parseBody(HttpRequest &request, const char *begin, const char *end);
		void	parseRequest(HttpRequest &request_class, char *original_str, size_t len);
		void	parseBody(HttpRequest &request, char *begin, size_t contentlength);
		bool	check_request_str(const char *str);
		bool	invalid_CRLF(const std::string &str);
		bool	check_method(HttpRequest &request);
		void	parseFirstLine(HttpRequest &request, const std::string &str);
		void	parseHeaders(HttpRequest &request, const std::vector<std::string>	&lines);
				
		bool	isChar(int c);
		bool	isControl(int c);
		bool	isSpecial(int c);
		bool	isDigit(int c);
		
};

#endif
