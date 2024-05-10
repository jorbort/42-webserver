/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPRequestParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juan-anm < juan-anm@student.42barcelona    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 18:37:54 by juan-anm          #+#    #+#             */
/*   Updated: 2024/05/10 19:02:22 by juan-anm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPRequestParser.hpp"

HttpRequest::HttpRequest(const std::string& request){
	ParseRequest(request);
}

HttpRequest::~HttpRequest(){}

void HttpRequest::ParseRequest(const std::string &request){
	
}