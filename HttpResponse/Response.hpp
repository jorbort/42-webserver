#pragma once

#include <string>
#include <map>
#include "ResponseHeader.hpp"
#include "CGIHandler.hpp"
#include "../HttpRequest/HTTPRequest.hpp"
#include "../Includes/ServerConfigs.hpp"

class Response : private ResponseHeader{
	public:
		Response(HttpRequest &request, ServerConfigs *server);
		~Response();
		std::string createResponse();

		enum Method {
			GET, POST, DELETE, UNKNOWN
		};

		Method	method;
		char *	uri;
		char *	extension;
		ServerConfigs *server;

	private:
		int							_statusCode;
		std::map<int, std::string>	_errorPageMap;
		std::string					_body;
		int							_contentLength;
		char *						_requestContent;
		int							_requestContentLength;
		bool						_isCGI;
		CGIHandler *				_CGIhandler;
		//int							_CGIfd;
		std::string					_defaultErrorBody;

		Method		getMethod(const std::string &method);
		char *		getExtension(const char *uri);
		void		parseRequestBody(const std::vector<char> &rqBody);
		void		setDefaultErrorBody();
		void		initStatusPageMap();
		std::string	getDirName(const std::string &path);
		bool		isURIAcceptable(const char *uri);
		bool		isCGI(const char *extension);
		bool		isProcessableCGI(const char *extension);
		std::string	errorResponse();
		std::string	getStatusPage();
		std::string readBody(const char *path);
        std::string	readBody(const int &fd);
		std::string	writeContent(const char *path);
		std::string deleteContent(const char *path);
};