#include <iostream>
#include <string>
#include <vector>

//* utilitiy functions 
namespace utils{


//* utilitiy functions for string manipulation
    namespace string{

        //* replace all occurences of a character in a string
        void replaceStr(std::string &str, const char from, const char to) {
            std::string::iterator it;
            for(it = str.begin(); it < str.end(); it++) {
                if(*it == from) {
                    *it = to;
                }
            }
        }

        //* split string by delimiter
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

        //* trim string from both sides
        std::string trim(const std::string& str)
        {
            size_t first = str.find_first_not_of(' ');
            if (std::string::npos == first)
            {
                return str;
            }
            size_t last = str.find_last_not_of(' ');
            return str.substr(first, (last - first + 1));
        }

        int stringToInt(std::string str){
            int num = 0;
            for(int i = 0; i < str.length(); i++){
                num = num*10 + (str[i] - '0');
            }
            return num;
        }
    }
}