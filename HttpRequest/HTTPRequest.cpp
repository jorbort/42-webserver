/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juan-anm  <juan-anm@student.42barcel>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 00:28:57 by juan-anm          #+#    #+#             */
/*   Updated: 2024/06/10 19:20:16 by juan-anm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequest.hpp"

HttpRequest::HttpRequest() : _ErrorCode(0), _RequestMethod(NONE), _URI_tcpPort(0), _chunked(0), _ContentLength(0) {}

HttpRequest::~HttpRequest(){}

int HttpRequest::getErrorCode() const{
	return(_ErrorCode);
}

std::string	HttpRequest::getMethod() const{
	return (_method);
}

std::string	HttpRequest::getURI() const{
	return (_URI);
}

std::string	HttpRequest::getVersion() const{
	return (_version);
}

std::map<std::string, std::string>	HttpRequest::getHeaders() const{
	return (_headers);
}

std::vector<uint8_t>	HttpRequest::getBody() const{
	return (_body);
}

void	HttpRequest::setMethod(const std::string &method){
	_method = method;
}

void	HttpRequest::setURI(const std::string &URI){
	_URI = URI;
}

void	HttpRequest::setVersion(const std::string &version){
	_version = version;
}

void	HttpRequest::setHeaders(const std::map<std::string, std::string> &headers){
	for(std::map<std::string, std::string>::const_iterator it = headers.begin(); it != headers.end(); it++){
		_headers[it->first] = it->second;
	}
}

void	HttpRequest::setBody(const std::vector<uint8_t> &body){
	
	for (std::vector<uint8_t>::const_iterator it = body.begin(); it != body.end(); it++){
		_body.push_back(*it);
	}
}

std::ostream& operator<<(std::ostream &out, const HttpRequest &request){
	std::map<std::string, std::string> headers = request.getHeaders();
	std::map<std::string, std::string> Query = request._URI_query;
	std::vector<uint8_t> body = request.getBody();
	
	out << std::endl;
	out << "Method: " << request.getMethod() << std::endl;
	out << std::endl;
	out << "URI: " << request.getURI() << std::endl;
	out << "URI Path: " << request._URI_path << std::endl;
	out << "URI Port: " << request._URI_tcpPort << std::endl;
	for (std::map<std::string, std::string>::iterator it = Query.begin(); it != Query.end(); it++){
		out << it->first << ": " << it->second << std::endl;
	}
	out << std::endl;
	out << "Version: " << request.getVersion() << std::endl;
	out << std::endl;
	out << "Headers: " << std::endl;
	for (std::map<std::string, std::string>::reverse_iterator it = headers.rbegin(); it != headers.rend(); it++){
		out << it->first << ": " << it->second << std::endl;
	}
	out << std::endl;
	out << "Body: " << std::endl;
	for (std::vector<uint8_t>::iterator it = body.begin(); it != body.end(); it++){
		out << *it;
	}
	out << std::endl;
	out << std::endl << "Error Code: " << request.getErrorCode() << std::endl;
	return (out);
}

