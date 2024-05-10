/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juan-anm < juan-anm@student.42barcelona    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:38:04 by juan-anm          #+#    #+#             */
/*   Updated: 2024/05/10 19:08:30 by juan-anm         ###   ########.fr       */
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
		content += line;
	}
	file.close();
	std::cout << "File content:\n" << content << std::endl;

	HttpRequest ParseReq(content);
	
	return 0;
}