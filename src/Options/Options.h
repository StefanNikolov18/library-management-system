#pragma once
#include <string>

namespace option{

    //namespace for System class configs
    namespace libraryConfig{
        ///@brief Loads only MAXSTORING number of library items into the system
        constexpr size_t MAXSTORING = 50;
        ///@brief Displays only PAGES number of library items on the screen
        constexpr size_t PAGES = 10;

    }

    //namespace for files to be read
    namespace fileNames{
        /// @brief name of the file for reading books in system
        constexpr const char* BOOKSFILENAME = "Books1.txt";
        /// @brief name of the file for reading users in system
        constexpr const char* USERSFILENAME = "Users1.txt";
    }

}
