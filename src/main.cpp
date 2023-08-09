//*       #  #####  ####### #     # ######  ######
//*       # #     # #     # ##    # #     # #     #     // A command line tool for generating json database
//*       # #       #     # # #   # #     # #     #     // Version: 0.0.1
//*       #  #####  #     # #  #  # #     # ######      // Author: Bijoy Kar
//* #     #       # #     # #   # # #     # #     #
//* #     # #     # #     # #    ## #     # #     #
//*  #####   #####  ####### #     # ######  ######

/********************************************************\
 *                                                      *
 *                                                      *
 *                                                      *
 *                                                      *
 *                                                      *
 *                                                      *   
\********************************************************/

//?        _______ ____  _   __   ____  ___  _________    ____  ___   _____ ______
//?       / / ___// __ \/ | / /  / __ \/   |/_  __/   |  / __ )/   | / ___// ____/
//?  __  / /\__ \/ / / /  |/ /  / / / / /| | / / / /| | / __  / /| | \__ \/ __/
//? / /_/ /___/ / /_/ / /|  /  / /_/ / ___ |/ / / ___ |/ /_/ / ___ |___/ / /___
//? \____//____/\____/_/ |_/  /_____/_/  |_/_/ /_/  |_/_____/_/  |_/____/_____/


// ! Use Better Comments vscode extension for better experience




#include "cli.cpp"


int main(int argc, char **argv) {
    
    cli::CliClient *cliClient = new cli::CliClient(argc, argv);
    int returnCode = cliClient->parse();
    cliClient->tasksAfterParse();

    delete(cliClient);
    return returnCode;
}
