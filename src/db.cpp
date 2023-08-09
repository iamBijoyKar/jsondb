#include <fstream>
#include <iostream>
#include <string>
#include<vector>
#include<unordered_map>
#include<filesystem>
#include<userenv.h>
#include "include/color.hpp"
#include "include/json.hpp"
#include "table-print.cpp"
#include "utils.cpp"
using json = nlohmann::json;

namespace db {
// todo: add a formatter for terminal output
    void viewTable(json database,std::string tableName) {
        for(auto it:database["tables"]) {
            if(it["name"]==tableName) {
                json columns = it["columns"];
                json rows = it["rows"];
                tablePrint::printTable(columns,rows); // formatted output of table data
                return;
            }
        }
        std::cout << dye::red("Table not found!") << std::endl;
    }

    void viewJson(json database,std::string tableName) {
        for(auto it:database["tables"]) {
            if(it["name"]==tableName) {
                std::cout<< std::setw(4) << dye::white_on_aqua(it) << std::endl;
                return;
            }
        }
        std::cout << dye::red("Table not found!") << std::endl;
    }

    void createTable(json &database, std::string tableName, std::string columnsData,std::ofstream &file) {
    
        for(auto it:database["tables"]) {
            if(it["name"] == tableName) {
                std::cout << dye::red("Table already exists! Try to use different name ...") << std::endl;
                return;
            }
        }
        if(columnsData[0] != '{' || columnsData.back() != '}') {
            std::cout << dye::red("Invalid columns data!") << std::endl;
            return;
        }

        utils::string::replaceStr(columnsData, '{', ' ');
        utils::string::replaceStr(columnsData, '}', ' ');

        std::vector<std::string> columns = utils::string::splitString(columnsData, ",");

        std::unordered_map<std::string, std::string> columnsMap;
        for(auto it:columns){
            std::vector<std::string> column = utils::string::splitString(it, ":");
            columnsMap[utils::string::trim(column[0])] = utils::string::trim(column[1]); // trim column name and type
        }

        json table = json::object();
        table["name"] = tableName;
        for(auto it:columnsMap) {
            json obj = json::object();
            obj["name"] = it.first;
            obj["type"] = it.second;
            table["columns"].push_back(obj);
            table["rows"] = json::array();
        }
        database["tables"].push_back(table);

        // file << std::setw(4) << database << std::endl;

        std::cout << dye::green(tableName) << " Table created " << " " << std::endl;

    }

    void insertData(json &database, std::string tableName, std::string data,std::ofstream &file) {
        if(data[0] != '{' || data.back() != '}') {
            std::cout << dye::red("Invalid data!") << std::endl;
            return;
        }

        utils::string::replaceStr(data, '{', ' ');
        utils::string::replaceStr(data, '}', ' ');

        std::vector<std::string> dataVector = utils::string::splitString(data, ",");
        std::unordered_map<std::string, std::string> dataMap;

        for(auto it:dataVector) {
            std::vector<std::string> data = utils::string::splitString(it, ":");
            dataMap[utils::string::trim(data[0])] = utils::string::trim(data[1]); // trim column name and type 
        }

        for(auto &it:database["tables"]) {
            if(it["name"] == tableName) {
                json obj = json::object();
                for(auto mapIt:dataMap) {
                    obj[mapIt.first] = mapIt.second;
                }
                it["rows"].push_back(obj);
                // success message and data view
                std::cout << dye::green("Data inserted successfully!") << std::endl;
                std::cout << "| ";
                for(auto it:obj){
                    std::cout << std::setw(10) << it << " |";
                }
                std::cout << std::endl;
                return;
            }
        }
        std::cout << dye::red("Table not found!") << std::endl;
    }

    void parseQuery(std::string query,json &databse,std::ofstream &file) {
        std::vector<std::string> queryVector = utils::string::splitString(query, " ");
        if(queryVector[0] == "CREATE") {
            if(queryVector[1] == "TABLE") {
                createTable(databse, queryVector[2], queryVector[3],file);
            }
            else{
                std::cout << dye::red("Invalid query!") << std::endl;
            }
        }
        else if(queryVector[0] == "INSERT") {
            if(queryVector[1] == "INTO") {
                insertData(databse, queryVector[2], queryVector[3],file);
            }
            else{
                std::cout << dye::red("Invalid query!") << std::endl;
            }
        }
        else if(queryVector[0] == "SELECT") {
            if(queryVector[1] == "*") {
                std::cout << "Data selected" << std::endl;
            }
        }
        else if(queryVector[0] == "UPDATE") {
            std::cout << "Data updated" << std::endl;
        }
        else if(queryVector[0] == "DELETE") {
            std::cout << "Data deleted" << std::endl;
        }
        else if(queryVector[0] == "DESCRIBE") {
            std::cout << "Table description" << std::endl;
        }
        else if(queryVector[0] == "VIEW"){
            if(queryVector[1] == "JSON"){
                if(queryVector.size() > 2){
                    viewJson(databse,queryVector[2]);
                }
                else{
                    std::cout << dye::white_on_aqua(databse.dump(4)) << std::endl;
                }
            }
            else if(queryVector[1] == "TABLE"){
                if(queryVector.size() > 2){
                    viewTable(databse,queryVector[2]);
                }
                else{
                    std::cout << dye::red("Invalid query!") << std::endl;
                }
            }
            else{
                std::cout << dye::red("Invalid query!") << std::endl;
            }   

        }
        else{
            std::cout << dye::red("Invalid query!") << std::endl;
        }
    }

    void createDb(std::string dbName, std::string dbPath) {
        std::string filePath = dbPath + "\\" + dbName + ".jsonDb.json";
        std::ofstream f(filePath);
        nlohmann::json data;
        data["name"] = dbName;
        data["tables"] = nlohmann::json::array();

        f << std::setw(4) << data << std::endl;
        f.close();
    }

    void useDb(std::string dbName, std::string dbPath) {
        std::string filePath = dbPath + "\\" + dbName + ".jsonDb.json";
        std::ifstream f(filePath);

        bool isDbCreated = f.good();
        if(!isDbCreated) {
            std::cout << dye::red("Database not found !") << std::endl;
            return;
        }

        json database = json::parse(f);
        json databaseCopy = database;
        bool isExit = false;
        bool isFirstCommandExecuted = false;
        f.close();

        std::ofstream o(filePath);

        while(!isExit){
            std::string command;
            std::cout << dye::green("db : ")<< dye::green(dbName+" > ") << std::flush;
            std::getline(std::cin, command);
            if(command == "exit") {
                isExit = true;
                std::cout << dye::blue("Database " + dbName + " closed with code 0\nBye!\n") << std::endl;
                continue;
            }
            else if(command == ""){
                continue;
            }

            try{
                parseQuery(command,database,o);
            }
            catch(const std::exception& e){
                std::cout << dye::red("Error! ") << std::endl;
                throw e;
                database = databaseCopy;
                std::cout << dye::red("The command is exited. Database rolled back to previous state!") << std::endl;
            }

            if(!isFirstCommandExecuted){
                isFirstCommandExecuted = true;
            }
            databaseCopy = database;
        }
        if(isExit && !isFirstCommandExecuted){
            o << std::setw(4) << database << std::endl;
            o.close();
            return;
        }
        o << std::setw(4) << database << std::endl;
        o.close();
    
    }

    int viewAllDb(std::string dbPath) {
        std::vector<std::filesystem::path> dbList;

        for (const auto & entry : std::filesystem::directory_iterator(dbPath)) {
            std::filesystem::path path = entry.path();
            std::string fileName = path.filename().generic_string();
            if(fileName.find(".jsonDb.json") != std::string::npos) {
                dbList.push_back(path.filename().replace_extension(""));
            }
        }
        
        if(dbList.size() == 0) {
            std::cout << dye::red("No database found!") << std::endl;
            return 1;
        }
        std::cout << dye::purple("Databases : ") << std::endl;
        for(auto it:dbList) {
            std::cout << dye::green(it) << std::endl;
        }
        return 0;
    }
}

namespace dbPtr{
    void (*createDbPtr)(std::string , std::string ) = &db::createDb;
    void (*useDbPtr)(std::string , std::string ) = &db::useDb;
    int (*viewAllDbPtr)(std::string ) = &db::viewAllDb;
}
