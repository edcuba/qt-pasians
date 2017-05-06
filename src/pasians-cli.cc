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
int actual_game = -1;

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
    actual_game = games.size() - 1;
    printState(game);
    return 0;
}

/**
* Print card
* @param card particular game card
**/
void Cli::printCard(Card *card)
{
    string c;
    if (card->visible) {
        c = to_string((int)card->type) + "-" + to_string((int) card->color);
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
    cout << "-----GAME "<< (int)actual_game << "-----"<< '\n';
    cout << "pick" << endl;
    printPile(game.pickPile);

    cout << "drop" << endl;
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
    games[actual_game].draw();
    printState(games[actual_game]);
    return 0;
}

int Cli::undo(vector<string> &attributes)
{
    games[actual_game].undo();
    printState(games[actual_game]);
    return 0;
}

int Cli::move(vector<string> &attributes)
{
  if (attributes.size() != 3)
  {
    cout << "Wrong number of arguments\nusage: move [FROM] [WHERE] [NUMBER]" << endl;
    return 1;
  }
  transform(attributes[0].begin(), attributes[0].end(), attributes[0].begin(), ::tolower);
  transform(attributes[1].begin(), attributes[1].end(), attributes[1].begin(), ::tolower);
  Pile *from;
  if (attributes[0] == "b1")
    from = &(games[actual_game].bottomPiles[0]);
  else if (attributes[0] == "b2")
    from = &(games[actual_game].bottomPiles[1]);
  else if (attributes[0] == "b3")
    from = &(games[actual_game].bottomPiles[2]);
  else if (attributes[0] == "b4")
    from = &(games[actual_game].bottomPiles[3]);
  else if (attributes[0] == "b5")
    from = &(games[actual_game].bottomPiles[4]);
  else if (attributes[0] == "b6")
    from = &(games[actual_game].bottomPiles[5]);
  else if (attributes[0] == "b7")
    from = &(games[actual_game].bottomPiles[6]);
  else if (attributes[0] == "t1")
    from = &(games[actual_game].topPiles[0]);
  else if (attributes[0] == "t2")
    from = &(games[actual_game].topPiles[1]);
  else if (attributes[0] == "t3")
    from = &(games[actual_game].topPiles[2]);
  else if (attributes[0] == "t4")
    from = &(games[actual_game].topPiles[3]);
  else if (attributes[0] == "drop")
    from = &(games[actual_game].dropPile);
  else
  {
    cout << "Uncorrect [FROM]" << endl;
    return 1;
  }

  Pile *where;
  if (attributes[1] == "b1")
    where = &(games[actual_game].bottomPiles[0]);
  else if (attributes[1] == "b2")
    where = &(games[actual_game].bottomPiles[1]);
  else if (attributes[1] == "b3")
    where = &(games[actual_game].bottomPiles[2]);
  else if (attributes[1] == "b4")
    where = &(games[actual_game].bottomPiles[3]);
  else if (attributes[1] == "b5")
    where = &(games[actual_game].bottomPiles[4]);
  else if (attributes[1] == "b6")
    where = &(games[actual_game].bottomPiles[5]);
  else if (attributes[1] == "b7")
    where = &(games[actual_game].bottomPiles[6]);
  else if (attributes[1] == "t1")
    where = &(games[actual_game].topPiles[0]);
  else if (attributes[1] == "t2")
    where = &(games[actual_game].topPiles[1]);
  else if (attributes[1] == "t3")
    where = &(games[actual_game].topPiles[2]);
  else if (attributes[1] == "t4")
    where = &(games[actual_game].topPiles[3]);
  else
  {
    cout << "Uncorrect [WHERE]" << endl;
    return 1;
  }

  int num = 100;
  try
  {
    num = stoi(attributes[2]);
  }
  catch (...)
  {
    cout << "Arg 3 must be a number" << endl;
    return 1;
  }

  int index  = from->cards.size() - num;

  if (from == &(games[actual_game].dropPile)  && num != 1)
  {
    cout << "Cant pick more than one card from drop pile." << endl;
    return 0;
  }

  if ((from == &(games[actual_game].topPiles[0]) ||
       from == &(games[actual_game].topPiles[1]) ||
       from == &(games[actual_game].topPiles[2]) ||
       from == &(games[actual_game].topPiles[3])
      ) && num != 1)
  {
    cout << "Cant pick more than one card from top piles." << endl;
    return 1;
  }

  if (index < 0)
  {
    cout << "Not enough cards." << endl;
    return 1;
  }

  if (!from->cards[index]->visible)
  {
    cout << "Card is not visible" << endl;
    return 1;
  }

  games[actual_game].move(from, where, index);

  printState(games[actual_game]);


  return 0;
}

int Cli::hint(vector<string> &attributes)
{
    Move move = games[actual_game].hint();
    Pile * w = move.where;
    if (!move.where || !move.from)
    {
        cout << "Cant find a hint" << endl;
        return 1;
    }

    int state = 0;
    if (move.number == -2)
    {
        games[actual_game].draw();
    }
    else
    {
        state = games[actual_game].move(move.from, w, move.number);
    }
    printState(games[actual_game]);
    if ( state== 2)
    {
        cout << "VICTORY!" << endl;
    }
    return 0;
}

int Cli::change(vector<string> &attributes)
{
    if (attributes.size() != 1)
    {
        cout << "Wrong number of arguments\nusage: change [GAME_ID]" << endl;
        return 1;
    }

    int num = 100;
    try
    {
      num = stoi(attributes[0]);
    }
    catch (...)
    {
      cout << "[GAME_ID] must be a number" << endl;
      return 1;
    }

    if (num < 0 || num > games.size() - 1)
    {
        cout << "[GAME_ID] does not exist." << endl;
        return 1;
    }

    actual_game = num;
    printState(games[actual_game]);
    return 0;
}

int Cli::show(vector<string> &attributes)
{
    (void)attributes;
    if (games.empty())
    {
        cout << "No active games" << endl;
        return 0;
    }
    for (unsigned i = 0; i < games.size(); ++i)
    {
        std::cout << "GAME " << i << endl;;
    }
    return 0;
}
int Cli::save(vector<string> &attributes)
{
    games[actual_game].save(attributes[0]);
    return 0;
}
int Cli::load(vector<string> &attributes)
{
    games[actual_game] = games[actual_game].load(attributes[0]);
    printState(games[actual_game]);
    return 0;
}
int main()
{
    Cli cli;
    return cli.run();
}
