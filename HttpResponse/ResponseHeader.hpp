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
		std::string	addStatusLine(int statusCode);
		std::string	addDateHeader(void);
		std::string	addServerHeader(void);
		std::string	addContentTypeHeader(ContentType type);
		std::string	addContentLengthHeader(int contentLength);
		std::string	addLastModified(void);
		//utils
		std::string	replaceNewlines(const std::string& input);
		std::string	getStatusCodeDescription(int status);
		std::string	getContentType(ContentType type);
		std::string	toString(int nb);
};