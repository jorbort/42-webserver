#pragma once

#include <string>
#include <map>
#include "ResponseHeader.hpp"
#include "../HttpRequest/HTTPRequest.hpp"

class Response : private ResponseHeader{
	public:
		Response(HttpRequest &request);
		~Response();
		std::string createResponse();

	private:
		std::string					_method;
		std::string 				_uri;
		int							_statusCode;
		char **						_envp;
		std::map<int, std::string>	_errorPageMap;
		std::string					_body;
		int							_contentLength;
		bool						_isCGI;
		int							_CGIfd;

		void		initErrorPageMap();
		bool		isURIAcceptable(const std::string uri);
		bool		isCGI(const std::string uri);
		bool		isProcessableCGI(const std::string uri);
		void		handleCGI(const std::string uri);
		std::string	errorResponse();
		std::string	getErrorPage();
		std::string readBody(const std::string path);
        std::string	readBody(const int fd);
};