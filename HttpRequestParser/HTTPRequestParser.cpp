/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juan-anm < juan-anm@student.42barcelona    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:37:54 by juan-anm          #+#    #+#             */
/*   Updated: 2024/05/13 00:44:37 by juan-anm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequestParser.hpp"

HttpRequest::HttpRequest(const std::string& request){
	ParseRequest(request);
}

HttpRequest::~HttpRequest(){}

void HttpRequest::ParseRequest(const std::string &request){
	std::vector<std::string> lines;
	std::istringstream req(request);
	std::string line;
	unsigned int i = 0;
	
	while (std::getline(req, line)){
		lines.push_back(line);
	}
	if (lines.size() == 0)
		return;
	_method = lines[0].substr(0, lines[0].find(' '));
	if (_method == "GET")
		_RequestMethod = GET;
	else if (_method == "POST")
		_RequestMethod = POST;
	else if (_method == "DELETE")
		_RequestMethod = DELETE;
	else
		_RequestMethod = NONE;
	if (lines.size() > 1)
		_URI = lines[i].substr(lines[0].find(_method) + _method.size() + 1, lines[0].find(' '));
	if (lines.size() > 2)
		_version = lines[i].substr(lines[0].find(_URI) + _URI.size() + 1, lines[0].find(' '));
	i++;		
	for (; i  < lines.size(); i++){
		if (lines[i] == "\r\n")
			break;
		std::string key = lines[i].substr(0, lines[i].find(':'));
		std::string value = lines[i].substr(lines[i].find(' '));
		_headers[key] = value;
	}
	for (; i  < lines.size(); i++){
		if (lines[i] == "\n")
			break;
		std::string key = lines[i].substr(0, lines[i].find(':'));
		std::string value = lines[i].substr(lines[i].find(' '));
		_body[key] = value;
	}

	// _method = strtok((char *)request.c_str(), " ");
	// if (_method == "GET")
	// 	_RequestMethod = GET;
	// else if (_method == "POST")
	// 	_RequestMethod = POST;
	// else if (_method == "DELETE")
	// 	_RequestMethod = DELETE;
	// else
	// 	_RequestMethod = NONE;
	// _URI = strtok(NULL, " ");
	// _version = strtok(NULL, "\n");
	
	// std::string line;
	// std::istringstream req(request);
	// while (std::getline(req, line)){
	// 	if (line == "\n")
	// 		break;
	// 	std::string key = strtok((char *)line.c_str(), ":");
	// 	std::string value = strtok(NULL, "\n");
	// 	_headers[key] = value;
	// }
	// while (std::getline(req, line)){
	// 	std::string key = strtok((char *)line.c_str(), ":");
	// 	std::string value = strtok(NULL, "\n");
	// 	_body[key] = value;
	// }
}

