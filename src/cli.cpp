#include <iostream>
#include <string>
#include"parser.cpp"

#include "../include/CLI11.hpp"
#include "../include/color.hpp"

const std::string version = "0.0.1";
const std::string description = "A command line tool for generating json database";
const std::string _dirname = GetExePath();

class CliClient {
   public:
    CLI::App app{description};

    bool isVersion {false};
    bool isAbout{false};
    bool isDbCreated{false};

    std::string dbName{"defaultDb"};


    CliClient() {
        app.require_option(0);

        app.add_flag("-v,--version", isVersion,"Prints version");
        app.add_flag("--about", isAbout,"Prints about");

        auto db = app.add_subcommand("db", "Database Mode");
        auto createDb = db->add_subcommand("create", "Create a new database");
        createDb->add_option("--name",dbName,"Create a new database");
        createDb->callback([&]() {
            isDbCreated = true;
        });

        
    }
    ~CliClient() {
    }

    int parse(int argc, char **argv) {
        try{
            app.parse(argc, argv);
        } catch (const CLI::ParseError &e) {
            return app.exit(e);
        }
    }

    void tasksAfterParse(){
        if(isVersion){
            std::cout << dye::aqua(version)  << std::endl;
        }
        if(isAbout){
            std::cout << dye::aqua(description)  << std::endl;
        }
        if(dbName != "" && isDbCreated){
            std::cout << dye::green(dbName)<< " created in " << _dirname  << std::endl; 
        }

    }
};





int cliClient(int argc, char **argv) {

    CLI::App app{description};

    bool isVersion = false;


    app.add_flag("-v,--version", isVersion ,"Prints version");

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    if(isVersion){
        std::cout << dye::aqua(version)  << std::endl;
    }

    return 0;
}