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
#include <locale>

#define VERSION "0.0.1"

using namespace std;

vector<Game> games;

Command::Command(string _cmd)
{
    cmd = _cmd;
}

Command::Command(){}

/**
* Setup new game
* @param attributes additional attributes
* @return 0
**/
int Cli::newGame(vector<string> &attributes)
{
    Game game;
    game.setup();
    games.push_back(game);

    printState(game);
    return 0;
}

/**
* Print card
* @param card particular game card
**/
void Cli::printCard(Card &card)
{
    string c;
    if (card.visible) {
        c = to_string((int)card.type) + "-" + to_string((int) card.color);
    } else {
        c = "none";
    }
    cout << cardChar.find(c)->second;
}

/**
* Print pile
* @param pile pile of cards
**/
void Cli::printPile(Pile &pile)
{
    for (auto card: pile.cards) {
        printCard(card);
    }
    cout << endl;
}

/**
* Print game statue - piles and cards
* @param game current game
**/
void Cli::printState(Game &game)
{
    cout << "PICK" << endl;
    printPile(game.pickPile);

    cout << "DROP" << endl;
    printPile(game.dropPile);

    int counter = 1;
    for (auto pile: game.topPiles) {
        cout << "T" << counter << endl;
        printPile(pile);
        counter++;
    }

    counter = 1;

    for (auto pile: game.bottomPiles) {
        cout << "B" << counter << endl;
        printPile(pile);
        counter++;
    }
}

/**
* Quits the application saving or discarding changes
* @param attributes #Command attributes for exit - "save"
* @return -1 if save was successful or not required
**/
int Cli::exitGame(vector<string> &attributes)
{
    //TODO save - if exit was called with attribute save. Ask [y,N] if wasn't.
    for (auto str: attributes) {
        cout << str << endl;
    }
    cout << endl;
    return -1;
}

/**
* Parse input from CLI into command object
* @return #Command object
**/
Command Cli::parse()
{
    string in;
    getline(cin, in);
    if (cin.eof()) {
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
* Execute commands form CLI
* @return 0 if successful
**/
int Cli::run()
{
    setlocale(LC_CTYPE, "");

    cout << "Welcome to Pasians " << VERSION << "\n"
         << "Type 'help' for some tips and tricks." << endl;
    while(true) {
        cout << ">> " << flush;
        Command cmd = parse();

        if (cmd.cmd.empty()) {
            continue;
        }

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
* Print help for commands specified as attributes, or whole help
* if no command is specified.
* @attributes: list of commands to print
* @return 0 if success, 1 if command was not found
**/
int Cli::printHelp(vector<string> &attributes)
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

int Cli::draw(vector<string> &attributes)
{
    games[0].draw();
    printState(games[0]);
    return 0;
}

int Cli::undo(vector<string> &attributes)
{
    games[0].undo();
    printState(games[0]);
    return 0;
}

int main()
{
    Cli cli;
    return cli.run();
}
