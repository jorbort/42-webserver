#include <string>
#include <unordered_map>

class Response {
	public:
		Response();
		~Response();

		std::string	createResponse(int fd);
	private:
		enum ContentType {
			HTML, JSON, PLAIN_TEXT, XML,
			JPEG, PNG, GIF,
			JAVASCRIPT, CSS,
			PDF, ZIP
		};
		size_t	maxBodySize;
		int		statusCode;

		std::string	addStatusLine(void);
		std::string	addDateHeader(void);
		std::string	addContentTypeHeader(ContentType type);
		std::string	addContentLengthHeader(int contentLength);
		std::string	addLastModified(void);
		std::string	addBody(int fd);
		std::string	replaceNewlines(const std::string& input);
		std::string	getStatusCodeDescription(int status);
		std::string	getContentType(ContentType type);
};