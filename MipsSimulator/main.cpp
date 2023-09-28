#include <iostream>
#include "Mips.h"

int main()
{
	
	MipsSimulator s;
	s.loadSource("./example.txt");
	
	for(auto x : s.getTokens())
		std::cout<<x<<"\n";
	
	for(auto x : s.getInstructions())
		std::cout<<x<<"\n";
	
	std::cout<<"\n\n\n\nPress ENTER to start the program:\n";
	std::cin.get();
	
	s.runProgram();
    return 0;
}