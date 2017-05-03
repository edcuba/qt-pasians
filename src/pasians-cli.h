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
#include "paslib.h"

using namespace std;

class Command {
public:
    string cmd;
    vector<string> attributes;
    Command(string);
    Command();
};

class Cli {
public:
    int run();
protected:
    static Command parse();

    static void printCard(Card &card);
    static void printPile(Pile &pile);
    static void printState(Game &game);

    /* cli command handlers */
    static int newGame(vector<string> &attributes);
    static int exitGame(vector<string> &attributes);
    static int printHelp(vector<string> &attributes);
    static int draw(vector<string> &attributes);
    static int undo(vector<string> &attributes);
    static int move(vector<string> &attributes);

    const unordered_map<string, int (*)(vector<string> &)> commands {
        {"new", *Cli::newGame},
        {"exit", *Cli::exitGame},
        {"help", *Cli::printHelp},
        {"draw", *Cli::draw},
        {"undo", *Cli::undo},
        {"move", *Cli::move}
    };
};

const map<string, string> helpList {
    {"new",  "       \t\tStart new game."},
    {"exit", " [save]\t\tExit. Add save to keep the game status."},
    {"help", " [cmd] \t\tPrint this list. Add command name to print only its record."}
    //{"move", "sss"}
};

// 2-10, J (11), Q (12), K (13), A (1)
// 0-3, hearts, clubs, diamonds, spades
const unordered_map<string, string> cardChar {
    {"2-0",  " 🂲"},
    {"3-1",  " 🃓"},
    {"4-2",  " 🃄"},
    {"5-3",  " 🂥"},
    {"6-0",  " 🂶"},
    {"7-1",  " 🃗"},
    {"8-2",  " 🃈"},
    {"9-3",  " 🂩"},
    {"10-0", " 🂺"},
    {"11-1", " 🃛"},
    {"12-2", " 🃍"},
    {"13-3", " 🂮"},
    {"1-0", " 🂱"},
    {"2-1",  " 🃒"},
    {"3-2",  " 🃃"},
    {"4-3",  " 🂤"},
    {"5-0",  " 🂵"},
    {"6-1",  " 🃖"},
    {"7-2",  " 🃇"},
    {"8-3",  " 🂨"},
    {"9-0",  " 🂹"},
    {"10-1", " 🃚"},
    {"11-2", " 🃋"},
    {"12-3", " 🂭"},
    {"13-0", " 🂾"},
    {"1-1", " 🃑"},
    {"2-2",  " 🃂"},
    {"3-3",  " 🂣"},
    {"4-0",  " 🂴"},
    {"5-1",  " 🃕"},
    {"6-2",  " 🃆"},
    {"7-3",  " 🂧"},
    {"8-0",  " 🂸"},
    {"9-1",  " 🃙"},
    {"10-2", " 🃊"},
    {"11-3", " 🂫"},
    {"12-0", " 🂽"},
    {"13-1", " 🃞"},
    {"1-2", " 🃁"},
    {"2-3",  " 🂢"},
    {"3-0",  " 🂳"},
    {"4-1",  " 🃔"},
    {"5-2",  " 🃅"},
    {"6-3",  " 🂦"},
    {"7-0",  " 🂷"},
    {"8-1",  " 🃘"},
    {"9-2",  " 🃉"},
    {"10-3", " 🂪"},
    {"11-0", " 🂻"},
    {"12-1", " 🃝"},
    {"13-2", " 🃎"},
    {"1-3", " 🂡"},
    {"none", " 🂠"}
};

#endif
