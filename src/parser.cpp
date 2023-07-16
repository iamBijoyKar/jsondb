#include <windows.h>
#include <fstream>
#include <iostream>
#include <string>

#include "../include/json.hpp"

using json = nlohmann::json;

std::string parse(std::string filePath) {
    std::ifstream f(filePath);
    nlohmann::json data = nlohmann::json::parse(f);
    return data.dump();
}


std::string GetExeFileName() {
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    return std::string(buffer);
}

std::string GetExePath() {
    std::string f = GetExeFileName();
    return f.substr(0, f.find_last_of("\\/"));
}