/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juan-anm < juan-anm@student.42barcelona    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:38:04 by juan-anm          #+#    #+#             */
/*   Updated: 2024/05/13 00:13:14 by juan-anm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fstream>
#include "HTTPRequestParser.hpp"

int main(void)
{
	std::string content;
	std::string line;
	std::ifstream file("Request.txt");

	if (!file.is_open()) {
		std::cerr << "Failed to open the file." << std::endl;
		return 1;
	}
	while (std::getline(file, line)) {
		content += line + '\n';
	}
	file.close();
	std::cout << "File content:\n" << content << std::endl;

	HttpRequest ParseReq(content);
	
	return 0;
}

/*POST /data HTTP/1.1
Host: api.example.com
Content-Type: application/json
Authorization: Bearer YOUR_ACCESS_TOKEN

{"key1": "value1", "key2": "value2"}*/