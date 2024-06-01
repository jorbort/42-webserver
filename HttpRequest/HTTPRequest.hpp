/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juan-anm < juan-anm@student.42barcelona    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:37:57 by juan-anm          #+#    #+#             */
/*   Updated: 2024/05/31 17:43:19 by juan-anm         ###   ########.fr       */
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

	// case 400: return "Bad Request";
	// case 401: return "Unauthorized";
	// case 402: return "Payment Required";
	// case 403: return "Forbidden";
	// case 404: return "Not Found";
	// case 405: return "Method Not Allowed";
	// case 406: return "Not Acceptable";
	// case 407: return "Proxy Authentication Required";
	// case 408: return "Request Timeout";
	// case 409: return "Conflict";
	// case 410: return "Gone";
	// case 411: return "Length Required";
	// case 412: return "Precondition Failed";
	// case 413: return "Request Entity Too Large";
	// case 414: return "Request-url Too Long";
	// case 415: return "Unsupported Media Type";
	// case 416: return "Requested Range Not Satisfiable";
	// case 417: return "Expectation Failed";
	// case 505: return "HTTP Version Not Supported";

enum Method{
	GET,
	POST,
	DELETE,
	NONE,
};

class HttpRequest
{
	private:
		HttpRequest(const HttpRequest &other);
		HttpRequest& operator=(const HttpRequest &other);
		
	public:
		int									_ErrorCode;
		Method 								_RequestMethod;
		std::string							_method;
		/*URI PARSING VARIABLES*/
		std::string							_URI;
		std::string							_URI_path;
		std::map<std::string, std::string>	_URI_query;

		std::string							_version;
		std::map<std::string, std::string>	_headers;
		std::vector<uint8_t>				_body;
		bool								_chunked;
		unsigned int						_ContentLength;

		HttpRequest();
		~HttpRequest();
		
		/*GETTERS*/
		int									getErrorCode() const;
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

