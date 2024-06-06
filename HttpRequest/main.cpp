/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juan-anm  <juan-anm@student.42barcel>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:38:04 by juan-anm          #+#    #+#             */
/*   Updated: 2024/06/06 19:57:15 by juan-anm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include <iostream>
#include "HttpRequestParser.hpp"
#include "HttpRequest.hpp"

// GET / HTTP/1.1
// User-Agent: -
// Host: hello.com
// Accept: */*
// Referer: http://base64.guru/tools/http-request-online
// Content-Type: application/json
// Authorization: Bearer YOUR_ACCESS_TOKEN
// Content-Length: 36

int main(void)
{
	// const char* Request = "GET / HTTP/1.1\r\nHost: www.google.com\r\n\r\n";
	
	char Request2[30000] = 	
						{"\r\n"
						"POST api.example.com:8080/docs/web/error_pages HTTP/1.1\r\n"
						"Host: 		api.example.com\r\n"
						"Content-Type: application/json				\r\n"
						"Authorization: Bearer YOUR_ACCESS_TOKEN\r\n"
						"Content-Length: 36\r\n"
						"Transfer-Encoding: chunked\r\n"
						"\r\n"
						"{\"key1\"\0: \"value1\", \"key2\": \"value2\"}"};

	HttpRequest request;
	HttpRequestParser Request_parser;
	
	// Request_parser.parseRequest(request, Request2.c_str(), Request2.c_str() + Request2.size());
	Request_parser.parseRequest(request, Request2, sizeof(Request2) / sizeof(char));
	
	std::cout << request << std::endl;
	if (request._ErrorCode != 0)
		std::cout << "ERROR: BAD REQUEST" << std::endl;
	std::cout << request._ContentLength << std::endl;
	std::cout << std::boolalpha << request._chunked << std::endl;

	return 0;
}
// #include <iostream>
// #include <sys/stat.h>

// bool directoryExists(const char* path) {
//     struct stat info;
//     if (stat(path, &info) != 0)
//         return false;
//     else if (info.st_mode & S_IFDIR)
//         return true;
//     else
//         return false;
// }

// int main() {
//     const char* directoryPath = "/path/to/your/directory";

//     if (directoryExists(directoryPath)) {
//         std::cout << "Directory exists!" << std::endl;
//     } else {
//         std::cout << "Directory does not exist!" << std::endl;
//     }

//     return 0;
// }
