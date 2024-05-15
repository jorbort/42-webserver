#include <string>
#include <unordered_map>

class Response {
	public:
		Response();
		~Response();

		std::string	createResponse();
	private:
		std::unordered_map<int, std::string> status_code_map;

		void		inicializeStatusCodeMap(void);
		std::string	addStatusLine(int status);
		std::string getStatusDescription(int status);
};