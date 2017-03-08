/**
* pasians-cli.h
* Pasians command line header - ICP 2016/2017
* Authors:
*   Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
*   Ondrej Kurak <xkurak00@stud.fit.vutbr.cz>
**/

#ifndef _PASIANS_CLI_H
#define _PASIANS_CLI_H

#include <unordered_map>
#include <map>
#include <string>
#include <vector>

using namespace std;

class Command {
public:
    string cmd;
    vector<string> attributes;
    Command(string);
    Command();
};

int cli();
Command parse();
int newGame(vector<string> &attributes);
int exitGame(vector<string> &attributes);
int printHelp(vector<string> &attributes);

const std::unordered_map<string, int (*)(vector<string> &)> commands {
    {"new", *newGame},
    {"exit", *exitGame},
    {"help", *printHelp}
};

const std::map<string, string> helpList {
    {"new",  "       \t\tStart new game."},
    {"exit", " [save]\t\tExit. Add save to keep the game status."},
    {"help", " [cmd] \t\tPrint this list. Add command name to print only its record."}
};



#endif
