/**
* pasians-cli.cc
* Pasians command line - ICP 2016/2017
* Authors:
*   Eduard Cuba <xcubae00@stud.fit.vutbr.cz>
*   Ondrej Kurak <xkurak00@stud.fit.vutbr.cz>
**/

#include "paslib.h"
#include "pasians-cli.h"
#include <iostream>
#include <sstream>

#define VERSION "0.0.1"

using namespace std;

Command::Command(string _cmd)
{
    cmd = _cmd;
}

Command::Command(){}

int newGame(vector<string> &attributes)
{
    //TODO
    return 0;
}

/**
* exitGame:
* @attributes: #Command attributes for exit - "save"
* Quits the application saving or discarding changes
* Returns: -1 if save was successful or not required
**/
int exitGame(vector<string> &attributes)
{
    //TODO save - if exit was called with attribute save. Ask [y,N] if wasn't.
    for (auto str: attributes) {
        cout << str << endl;
    }
    cout << endl;
    return -1;
}

/**
* parse:
* Parse input from CLI into command object
* Returns: #Command object
**/
Command parse()
{
    string in;
    getline(cin, in);
    if (in.empty()) {
        return Command("exit");
    }
    stringstream ss(in);
    Command cmd;
    ss >> cmd.cmd;
    while (ss >> in) {
        cmd.attributes.push_back(in);
    }
    return cmd;
}

/**
* cli:
* Execute commands form CLI
* Returns: 0 if successful
**/
int cli()
{
    while(true) {
        cout << ">> " << flush;
        Command cmd = parse();

        auto it = commands.find(cmd.cmd); // check command

        if (it == commands.end()) {
            cerr << "Unknown command." << endl;
            continue;
        }

        int rc = it->second(cmd.attributes); // call command handler

        switch (rc) {
            case -1:
                return 0;
        }
    }
}

/**
* printHelp:
* @attributes: list of commands to print
*
* Print help for commands specified as attributes, or whole help
* if no command is specified.
* Returns: 0 if success, 1 if command was not found
**/
int printHelp(vector<string> &attributes)
{
    cout << "Command    \t\tDescription\n\n";
    if (attributes.empty()) {
        for (auto &it: helpList) {
            cout << it.first << it.second << endl;
        }
    } else {
        for (auto str: attributes) {
            auto it = helpList.find(str);

            if (it == helpList.end()) {
                cout << "No record for \"" << str << "\"" << endl;
                return 1;
            }
            cout << str << it->second << endl;
        }
    }
    return 0;

}

int main()
{
    cout << "Welcome to Pasians " << VERSION << "\n"
         << "Type 'help' for some tips and tricks." << endl;

    return cli();
}
