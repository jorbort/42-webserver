#pragma once

#include <string>
#include <map>
#include "ResponseHeader.hpp"
#include "CGIHandler.hpp"
#include "../HttpRequest/HTTPRequest.hpp"

class Response : private ResponseHeader{
	public:
		Response(HttpRequest &request);
		~Response();
		std::string createResponse();

		enum Method {
			GET, POST, DELETE, UNKNOWN
		};

		Method	method;
		char *	uri;
		char *	extension;

	private:
		int							_statusCode;
		std::map<int, std::string>	_errorPageMap;
		std::string					_body;
		int							_contentLength;
		bool						_isCGI;
		CGIHandler *				_CGIhandler;
		int							_CGIfd;
		std::string					_defaultErrorBody;

		Method		getMethod(std::string method);
		char *		getExtension(char *uri);
		void		setDefaultErrorBody();
		void		initErrorPageMap();
		std::string	getDirName(const std::string &path);
		bool		isURIAcceptable(const char *uri);
		bool		isCGI(const char *extension);
		bool		isProcessableCGI(const char *extension);
		std::string	errorResponse();
		std::string	getErrorPage();
		std::string readBody(const char *path);
        std::string	readBody(const int fd);
};