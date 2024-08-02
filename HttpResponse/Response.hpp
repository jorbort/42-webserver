#pragma once

#include <string>
#include <map>
#include "ResponseHeader.hpp"
#include "CGIHandler.hpp"
#include "../HttpRequest/HTTPRequest.hpp"
#include "../Includes/ServerConfigs.hpp"
#include <dirent.h>

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
		std::string	location;
		char *	extension;
		ServerConfigs *server;
		std::map<std::string, std::string> _headers;

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
		bool		isProcessableCGI(const char *extension, std::vector<std::string> allowed_methods);
		std::string	errorResponse();
		std::string	getStatusPage();
		std::string readBody(const char *path);
        std::string	readBody(const int &fd);
		std::string	writeContent(const char *path);
		std::string deleteContent(const char *path);
		bool isUriInServer(const char *uri);
		bool isMethodAllowed(std::string method, std::string location);
		bool isAutoIndex(std::string location);
		std::string generateAutoIndex(std::string  filePath);
		void getFormUri(std::string filename);
		char * getContentDisposition(char *reqBody, std::string &filename);
};
