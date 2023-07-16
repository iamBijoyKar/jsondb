#include <fstream>
#include <iostream>
#include <string>

#include "../include/color.hpp"
#include "../include/json.hpp"

namespace db {
    void createDb(std::string dbName, std::string dbPath) {
        std::string filePath = dbPath + "\\" + dbName + ".json";
        std::ofstream f(filePath);
        nlohmann::json data;
        data["name"] = dbName;
        f << data;
        f.close();
    }
    void useDb(std::string dbName, std::string dbPath) {
        std::string filePath = dbPath + "\\" + dbName + ".json";
        std::ifstream f(filePath);

        bool isDbCreated = f.good();
        if(!isDbCreated) {
            std::cout << dye::red("Database not found !") << std::endl;
            return;
        }

        bool isExit = false;

        while(!isExit){
            std::string command;
            std::cout << dye::green("db : ")<< dye::green(dbName+" > ") << std::flush;
            std::getline(std::cin, command);
            if(command == "exit") {
                isExit = true;
                std::cout << dye::blue("Database "+dbName+" closed with code 0\nBye!\n") << std::endl;
                continue;
            }
        }
        
        f.close();
    }
}

namespace dbPtr{
    void (*createDbPtr)(std::string , std::string ) = &db::createDb;
    void (*useDbPtr)(std::string , std::string ) = &db::useDb;
}
