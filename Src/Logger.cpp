#include "Logger.hpp"

void Logger::print(std::string level, std::string message)
{
	std::string compare[3] = {"Error", "Warning", "Ok"};
	int i = 0;
	for(; i < 3 ; i++)
	{
		if (level == compare[i])
		{
			break;
		}
	}
	switch (i)
	{
		case 0 :
		{
			std::cerr << RED << message << std::endl;
			std::cerr << RESET;
			break;
		}
		case 1 :
		{
			std::cerr << YELLOW << message << std::endl;
			std::cerr << RESET;
			break;
		}
		case 2 :
		{
			std::cout << GREEN << message <<std::endl;
			std::cout << RESET;
			break;
		}
		default :
		{
			std::cerr << CYAN << "invalid logger level" << std::endl;
			std::cerr << RESET;
		}
	}
}

void Logger::printTrain() {
	std::cout << YELLOW << "Server goes Choo!!! choo!!!" << RESET <<std::endl;
    std::cout << GREEN <<"                           #########\n";                      
std::cout << "                        ######\n";                            
std::cout << "                   ___###___________\n";                      
std::cout << "            _,--_________________--._\n";               
std::cout << "          /',--'\\## __ //'--.'\\   ,  /\\    _\n";
std::cout << "-._  /\\  //'   ##\\\\       ,-_,-'  \\_///\\,-'`\\\\-' \\/  \\,-'\n"; 
std::cout << "   \\/  \\//    _  _\\\\ ,-._/         //'       \\\\\n";           
std::cout << "       //'--._M_|H|\\\\___   ___   _//   ___   _\\\\   ___\n";    
std::cout << "      | |    (|   | \\  | |   | |// | |   | | | | |   |\n";   
std::cout << "   ___| |   /ooo=oo-o\\\\oo-oo=oo-//=oo-oo=oo-oo| |=oo=oo___\n";
std::cout << "        -------------------------------------------------|\n";           
std::cout << "         |    \\                           .   /\n";           
std::cout << "   .      \\   /\\            .  -     '    .  /\n";            
std::cout << "           `-.  '-..     ' '          ,  ,,-'     `\n";       
std::cout << "              \\     '- '            ,/,-'/\n";                
std::cout << "    \\          \\_- ' '             --',-'\n";                 
std::cout << "              -'                    \\/            |\n";       
std::cout << "     |  _  -                          - _        .\n";       
std::cout << "                                                /\n" << RESET;
}