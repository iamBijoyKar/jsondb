#include <iostream>
#include "db.cpp"

#include "include/CLI11.hpp"
#include "include/color.hpp"

const std::string version = "0.0.1";
const std::string description = "A command line tool for generating json database";
const std::string _dirname = std::filesystem::current_path().string();
const std::string defaultDbName = "defaultDb";

namespace cli {
    class CliClient {
    private:
    // App instance
        CLI::App app{description};

        public:
        // build in args
        int argc{0};
        char **argv{nullptr};

        // flags and options
        bool isVersion {false};
        bool isAbout{false};
        bool isDbCreated{false};
        bool dbInUse{false};

        std::string dbPath{_dirname};
        std::string dbName{defaultDbName};
        std::string dbDir{_dirname};


        CliClient(int argc_, char **argv_) {
            argc = argc_;
            argv = argv_;

            app.require_option(0);

            app.add_flag("-v,--version", isVersion,"Prints version");
            app.add_flag("--about", isAbout,"Prints about");

            auto db = app.add_subcommand("db", "Database Mode");

            auto createDb = db->add_subcommand("create", "Create a new database");
            createDb->add_option("--name",dbName,"Provide a name for the database");
            createDb->add_option("--path",dbPath,"Provide a path for the database")->check(CLI::ExistingDirectory);
            createDb->add_flag("--use",dbInUse,"Want to use the database after creation?");

            createDb->callback([&]() {
                dbPtr::createDbPtr(dbName, dbPath);
                std::cout << dye::green(dbName)<< " created in " << dbPath  << std::endl; 

                if(dbInUse) {
                    std::cout << "\nDatabase created and"<< dye::purple(" in use") << std::endl;
                    dbPtr::useDbPtr(dbName, dbPath);
                } else {
                    std::cout << "Database created but not in use\n" << std::endl;
                }
            });

            auto viewDb = db->add_subcommand("view", "View the database");
            auto viewAll = viewDb->add_subcommand("all", "View all databases");
            viewAll->add_option("--path",dbPath,"Provide a path for the database")->check(CLI::ExistingDirectory);
            viewAll->callback([&]() {
                dbPtr::viewAllDbPtr(dbPath);
            });

            auto useDb = db->add_subcommand("use", "Use an existing database");
            useDb->add_option("--name",dbName,"Provide a name for the database");
            useDb->add_option("--path",dbPath,"Provide a path for the database")->check(CLI::ExistingDirectory);
            useDb->add_option("--dbDir",dbPath,"Provide a path for the database")->check(CLI::ExistingDirectory);
            useDb->callback([&]() {
                dbPtr::useDbPtr(dbName, dbPath);
            });
            
        }
        ~CliClient() {
            free(argv);
        }

        int parse() {
            try{
                app.parse(argc, argv);
            } catch (const CLI::ParseError &e) {
                return app.exit(e);
            }
            return 0;
        }

        void tasksAfterParse(){
            if(isVersion){
                std::cout << dye::aqua(version)  << std::endl;
            }
            if(isAbout){
                std::cout << dye::aqua(description)  << std::endl;
            }
        }
    };
}




// for testing purpose
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