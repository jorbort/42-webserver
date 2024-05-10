/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestParser.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juan-anm < juan-anm@student.42barcelona    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:37:57 by juan-anm          #+#    #+#             */
/*   Updated: 2024/05/10 18:59:37 by juan-anm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <map>

enum Method{
	GET,
	POST,
	DELETE,
	NONE
};

class HttpRequest
{
	private:
		HttpRequest(const HttpRequest &other);
		HttpRequest& operator=(const HttpRequest &other);
		HttpRequest();
		
		Method 								_RequestMethod;
		std::string							_URI;
		std::string							_version;
		std::map<std::string, std::string>	_headers;
		std::map<std::string, std::string>	_body;

		void	ParseRequest(const std::string &request);

	public:
		HttpRequest(const std::string& request);
		~HttpRequest();

};