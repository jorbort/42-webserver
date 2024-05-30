#include <string>
#include <unordered_map>
#include "ResponseHeader.hpp"

class Response : public ResponseHeader {
	public:
		~Response();

		//Response for each method
		static std::string createGETresponse(int fd);
		//Response for errors
		static std::string create404NotFoundResponse(void);
	private:
		//body for GET
		static void	setBody(std::string &body, size_t &contentLength, int fd, size_t maxBodySize);
};