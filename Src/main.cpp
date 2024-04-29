#include "../Includes/Logger.hpp"
#include "../Includes/WebServ.hpp"

int main(void)
{
	Logger::print("Error", "hay un error");
	Logger::print("Warning", "una advertencia");
	Logger::print("Ok", "todo ok");
	Logger::print("otro", "una opcion que no existe");
}
