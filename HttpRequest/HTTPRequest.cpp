/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juan-anm < juan-anm@student.42barcelona    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 00:28:57 by juan-anm          #+#    #+#             */
/*   Updated: 2024/05/27 09:56:58 by juan-anm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::HttpRequest() : _RequestState(INCOMPLETE), _RequestMethod(NONE){}

HttpRequest::~HttpRequest(){}

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
	std::vector<uint8_t> body = request.getBody();

	out << "Method: " << request.getMethod() << std::endl;
	out << "URI: " << request.getURI() << std::endl;
	out << "Version: " << request.getVersion() << std::endl;
	out << "Headers: " << std::endl;
	for (std::map<std::string, std::string>::reverse_iterator it = headers.rbegin(); it != headers.rend(); it++){
		out << it->first << ": " << it->second << std::endl;
	}
	out << "Body: " << std::endl;
	for (std::vector<uint8_t>::iterator it = body.begin(); it != body.end(); it++){
		out << *it;
	}

	return (out);
}

