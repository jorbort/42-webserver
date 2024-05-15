/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juan-anm < juan-anm@student.42barcelona    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:38:04 by juan-anm          #+#    #+#             */
/*   Updated: 2024/05/15 19:46:40 by juan-anm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <iostream>
#include "HTTPRequestParser.hpp"
#include "HTTPRequest.hpp"

int main(void)
{
	// const char* Request = "GET / HTTP/1.1\r\nHost: www.google.com\r\n\r\n";
	
	std::string Request2 = 	"POST /data HTTP/1.1\r\n"
						"Host: api.example.com\r\n"
						"Content-Type: application/json\r\n"
						"Authorization: Bearer YOUR_ACCESS_TOKEN\r\n"
						"\r\n"
						"{\"key1\": \"value1\", \"key2\": \"value2\"}";

	std::cout << Request2 << std::endl;
	HttpRequest request;
	HTTPRequestParser Request_parser;
	
	Request_parser.parseRequest(request, Request2.c_str(), Request2.c_str() + Request2.size());
	


	return 0;
}
