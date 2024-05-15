/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestParser.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juan-anm < juan-anm@student.42barcelona    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 19:24:05 by juan-anm          #+#    #+#             */
/*   Updated: 2024/05/15 19:42:48 by juan-anm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUESTPARSER_HPP
#define HTTPREQUESTPARSER_HPP

#include <iostream>
#include <string>
#include <map>
#include "HTTPRequest.hpp"

class HTTPRequestParser
{
	private:
		HTTPRequestParser(const HTTPRequestParser &other);
		HTTPRequestParser& operator=(const HTTPRequestParser &other);
	public:
		HTTPRequestParser();
		~HTTPRequestParser();
		
		void	parseRequest(const HttpRequest &request, const char *begin, const char *end);
};
