/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juan-anm < juan-anm@student.42barcelona    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:37:54 by juan-anm          #+#    #+#             */
/*   Updated: 2024/05/13 00:16:01 by juan-anm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequestParser.hpp"

HttpRequest::HttpRequest(const std::string& request){
	ParseRequest(request);
}

HttpRequest::~HttpRequest(){}

void HttpRequest::ParseRequest(const std::string &request){
	_method = strtok((char *)request.c_str(), " ");
	if (_method == "GET")
		_RequestMethod = GET;
	else if (_method == "POST")
		_RequestMethod = POST;
	else if (_method == "DELETE")
		_RequestMethod = DELETE;
	else
		_RequestMethod = NONE;
	_URI = strtok(NULL, " ");
	_version = strtok(NULL, "\n");
	
	std::string line;
	std::istringstream req(request);
	while (std::getline(req, line)){
		if (line == "\n")
			break;
		std::string key = strtok((char *)line.c_str(), ":");
		std::string value = strtok(NULL, "\n");
		_headers[key] = value;
	}
	while (std::getline(req, line)){
		std::string key = strtok((char *)line.c_str(), ":");
		std::string value = strtok(NULL, "\n");
		_body[key] = value;
	}
}

