#include <string>
#include <unordered_map>

class Response {
	public:
		enum MethodType { GET, POST, DELETE };

		Response(MethodType type, int statusCode, int fd);
		~Response();

		std::string	createResponse(void);
	private:
		enum ContentType {
			HTML, JSON, PLAIN_TEXT, XML,
			JPEG, PNG, GIF,
			JAVASCRIPT, CSS,
			PDF, ZIP
		};
		MethodType	method;
		int			statusCode;
		int			fd;
		size_t		maxBodySize;

		//Response for each method
		std::string	createGETresponse(void);
		std::string	createPOSTresponse(void);
		std::string	createDELETEresponse(void);
		//HEADER
		std::string	addStatusLine(void);
		std::string	addDateHeader(void);
		std::string	addContentTypeHeader(ContentType type);
		std::string	addContentLengthHeader(int contentLength);
		std::string	addLastModified(void);
		//body for GET
		std::string	addBody(void);
		//utils
		std::string	replaceNewlines(const std::string& input);
		std::string	getStatusCodeDescription(int status);
		std::string	getContentType(ContentType type);
};