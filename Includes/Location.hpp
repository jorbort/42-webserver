#include <string>
#include <vector>

class Location
{
	protected:
		std::string address = "";
		std::string root = "";
		std::vector<std::string> allowed_methods ;
		bool autoindex = false;
		std::string index = "";
	public:
		Location();
		~Location();
};