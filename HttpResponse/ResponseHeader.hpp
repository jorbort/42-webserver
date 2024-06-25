#pragma once

#include <string>

class ResponseHeader {
	public:
		~ResponseHeader();
	protected:
		enum ContentType {
			HTML, JSON, PLAIN_TEXT, XML,
			JPEG, PNG, GIF,
			JAVASCRIPT, CSS,
			PDF, ZIP
		};
		//HEADER
		static std::string	addStatusLine(int statusCode);
		static std::string	addDateHeader(void);
		static std::string	addServerHeader(void);
		static std::string	addContentTypeHeader(ContentType type);
		static std::string	addContentLengthHeader(int contentLength);
		static std::string	addLastModified(void);
        static void			setBody(std::string &body, size_t &contentLength, int fd, size_t maxBodySize);
		//utils
		static std::string	replaceNewlines(const std::string& input);
		static std::string	getStatusCodeDescription(int status);
		static std::string	getContentType(ContentType type);
		static std::string	toString(int nb);
};