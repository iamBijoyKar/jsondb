#include "include/json.hpp"
#include <iostream>

using json = nlohmann::json;

namespace tablePrint{
    void printTable(json columns,json rows){
        std::vector<std::string> columnsOrder;
        int rowCounter = 0;
        int dashCount = columns.size()*12 + columns.size() -1 + 2;
        std::cout << " " << std::string(dashCount,'-');
        std::cout << std::endl<< " | ";
        for(auto it:columns) {
            const std::string col = it["name"];
            std::cout << std::left << std::setw(10) << col << " | ";
            columnsOrder.push_back(it["name"]);
        }
        std::cout << std::endl;
        std::cout << " " << std::string(dashCount,'-') << std::endl;
        // std::cout << std::endl; // extra line removed between column names and rows
        std::cout << " " << std::string(dashCount,'-') << std::endl;
        std::cout << " | ";
        for(auto it:rows) {
            for(auto it2:columnsOrder){
                if(it.find(it2) == it.end()){
                    std::cout << std::setw(10) << "NULL" << " | ";
                    continue;
                }
                else{
                    const std::string rowData = *it.find(it2);
                    std::cout << std::left << std::setw(10) << rowData << " | ";
                }  
            }
            rowCounter++;
            if(rowCounter < rows.size()){
                std::cout << std::endl << " | ";
            }
        }
        std::cout << std::endl;
        std::cout << " " << std::string(dashCount,'-') << std::endl;
        std::cout << std::endl << std::endl;
        return;
    }

    void printTable(json table){
        json columns = table["columns"];
        json rows = table["rows"];
        printTable(columns,rows);
    }

    void printTable(json database, std::string tableName){
        json table = database[tableName];
        printTable(table);
    }

    void printTable(json columns,json rows,int limit){
        std::vector<std::string> columnsOrder;
        int rowCounter = 0;
        int dashCount = columns.size()*12 + columns.size() -1 + 2;
        std::cout << " " << std::string(dashCount,'-');
        std::cout << std::endl<< " | ";
        for(auto it:columns) {
            const std::string col = it["name"];
            std::cout << std::left << std::setw(10) << col << " | ";
            columnsOrder.push_back(it["name"]);
        }
        std::cout << std::endl;
        std::cout << " " << std::string(dashCount,'-') << std::endl;
        // std::cout << std::endl; // extra line removed between column names and rows
        std::cout << " " << std::string(dashCount,'-') << std::endl;
        std::cout << " | ";
        for(auto it:rows) {
            for(auto it2:columnsOrder){
                if(it.find(it2) == it.end()){
                    std::cout << std::setw(10) << "NULL" << " | ";
                    continue;
                }
                else{
                    const std::string rowData = *it.find(it2);
                    std::cout << std::left << std::setw(10) << rowData << " | ";
                }  
            }
            rowCounter++;
            if(rowCounter < limit && rowCounter < rows.size()){
                std::cout << std::endl << " | ";
            }
            else{
                break;
            }
        }
        std::cout << std::endl;
        std::cout << " " << std::string(dashCount,'-') << std::endl;
        std::cout << std::endl << std::endl;
        return;
    }

    void printColumns(json columns){
        int dashCount = columns.size()*12 + columns.size() -1 + 2;
        std::cout << " " << std::string(dashCount,'-');
        std::cout << std::endl<< " | ";
        for(auto it:columns) {
            const std::string col = it["name"];
            std::cout << std::left << std::setw(10) << col << " | ";
        }
        std::cout << std::endl;
        std::cout << " " << std::string(dashCount,'-') << std::endl;
        std::cout << std::endl;
    }
}


namespace dbtable{

    json findTable(json database, std::string tableName){
        json table;
        for(auto it:database["tables"]){
            if(it["name"] == tableName){
                table = it;
                break;
            }
        }
        return table;
    }
}