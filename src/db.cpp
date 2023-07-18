#include <fstream>
#include <iostream>
#include <string>
#include<vector>
#include<unordered_map>
#include<filesystem>
#include<userenv.h>
#include "../include/color.hpp"
#include "../include/json.hpp"

using json = nlohmann::json;
namespace parser {

}

void replaceStr(std::string &str, const char from, const char to) {
    std::string::iterator it;
    for(it = str.begin(); it < str.end(); it++) {
        if(*it == from) {
            *it = to;
        }
    }
}


std::vector<std::string> splitString(std::string str, std::string delimiter) {
    std::vector<std::string> splittedString;
    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        splittedString.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    splittedString.push_back(str);
    return splittedString;
}


namespace db {

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

        replaceStr(columnsData, '{', ' ');
        replaceStr(columnsData, '}', ' ');

        std::vector<std::string> columns = splitString(columnsData, ",");

        std::unordered_map<std::string, std::string> columnsMap;
        for(auto it:columns){
            std::vector<std::string> column = splitString(it, ":");
            columnsMap[column[0]] = column[1];
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

        file << std::setw(4) << database << std::endl;

        std::cout << dye::green(tableName) << " Table created " << " " << std::endl;

    }

    void parseQuery(std::string query,json &databse,std::ofstream &file) {
        std::vector<std::string> queryVector = splitString(query, " ");
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
                std::cout << "Data inserted" << std::endl;
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
        bool isExit = false;
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

            parseQuery(command,database,o);

        }
        if(isExit){
            o << std::setw(4) << database << std::endl;
        }
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
