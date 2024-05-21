/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juan-anm < juan-anm@student.42barcelona    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:37:57 by juan-anm          #+#    #+#             */
/*   Updated: 2024/05/22 00:39:03 by juan-anm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPREQUEST_HPP
#define HTTPREQUEST_HPP

#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <sstream>
#include <vector>

enum Method{
	GET,
	POST,
	DELETE,
	NONE,
};

enum ParsingState{
	ERROR,
	INCOMPLETE,
	SUCCESS
};

class HttpRequest
{
	private:
		HttpRequest(const HttpRequest &other);
		HttpRequest& operator=(const HttpRequest &other);
		
	public:
		ParsingState						_RequestState;
		Method 								_RequestMethod;
		std::string							_method;
		std::string							_URI;
		std::string							_version;
		std::map<std::string, std::string>	_headers;
		std::vector<uint8_t>				_body;

		HttpRequest();
		~HttpRequest();
		
		/*GETTERS*/
		std::string							getMethod() const;
		std::string							getURI() const;
		std::string							getVersion() const;
		std::map<std::string, std::string>	getHeaders() const;
		std::vector<uint8_t>				getBody() const;
		/*SETTERS*/
		void								setMethod(const std::string &method);
		void								setURI(const std::string &URI);
		void								setVersion(const std::string &version);
		void								setHeaders(const std::map<std::string, std::string> &headers);
		void								setBody(const std::vector<uint8_t> &body);
};

std::ostream& operator<<(std::ostream &out, const HttpRequest &request);

#endif

