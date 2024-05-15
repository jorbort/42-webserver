/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juan-anm < juan-anm@student.42barcelona    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:38:04 by juan-anm          #+#    #+#             */
/*   Updated: 2024/05/16 00:42:34 by juan-anm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <iostream>
#include "HttpRequestParser.hpp"
#include "HttpRequest.hpp"

int main(void)
{
	// const char* Request = "GET / HTTP/1.1\r\nHost: www.google.com\r\n\r\n";
	
	std::string Request2 = 	"POST /data HTTP/1.1\r\n"
						"Host: api.example.com\r\n"
						"Content-Type: application/json\r\n"
						"Authorization: Bearer YOUR_ACCESS_TOKEN\r\n"
						"\r\n"
						"{\"key1\": \"value1\", \"key2\": \"value2\"}";

	HttpRequest request;
	HttpRequestParser Request_parser;
	
	Request_parser.parseRequest(request, Request2.c_str(), Request2.c_str() + Request2.size());
	
	std::cout << request << std::endl;

	return 0;
}

