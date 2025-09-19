/**
 * @file main.cpp
 * @brief Main entry file to start the library system.
 */

#include <iostream>
#include <fstream>
#include <string>
#include "System/System.h"
#include "Commands/Command.h"
#include "Options/Options.h"

 /**
  * @brief Main function to start the application.
  *
  * Initializes files, loads data into the system, reads commands from the user,
  * and executes corresponding actions via the Command class. On "exit" command, saves data.
  *
  */

int main() {

	try {
		// Name of the files
		 const std::string fileNameBooks = option::fileNames::BOOKSFILENAME;
		 const std::string fileNameUsers = option::fileNames::USERSFILENAME;

		// Loading files
		 std::ifstream fileBooks(fileNameBooks, std::ios::in);
		 std::ifstream fileUsers(fileNameUsers, std::ios::in);

		// checking is open
		if (!fileBooks.is_open()) {
			std::cerr << "Failed to open Books.txt\n";
			return 1;  
		}
		if (!fileUsers.is_open()) {
			std::cerr << "Failed to open Users.txt\n";
			return 1;
		}

		//Loading Library from files
		System Library(fileBooks, fileUsers, fileNameBooks, fileNameUsers);

		std::string command;
		std::cout << "Hello in Library!\n";
		for (;;) { 

			std::cout << "> ";
			std::getline(std::cin, command);

			//if exit break from woop
			if (command == "exit") {
				
				try {
					Library.saveToFiles();  //saving all to files
					std::cout << "System saved. Exiting..." << std::endl;
				}
				catch (const std::exception& e) { 
					std::cerr << "Error saving system: " << e.what() << std::endl;
					throw;
				}
				break; 
			}

			//Open command
			Command cmd(Library, command);
			//executing the one was typed
			cmd.execute(); 

			
			fileBooks.clear();
			fileBooks.seekg(0, std::ios::beg);
		}
	}
	catch (const std::bad_alloc& ba) { // memory leak somewhere
		std::cerr << "Error: " << ba.what() << std::endl;
		return 2; 
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 3;
	}
	catch (...) { 
		std::cerr << "Unknown error occurred!" << std::endl;
		return 4;
	}
	
	

	return 0;
}