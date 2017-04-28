#ifndef CARDS_H
#define CARDS_H

#include <unordered_map>

// 2-10, J (11), Q (12), K (13), A (14)
// 0-3, hearts, clubs, diamonds, spades
const std::unordered_map<std::string, std::string> cardImg {
    {"2-0",  ":/res/2_of_hearts.png"},
    {"3-1",  ":/res/3_of_clubs.png"},
    {"4-2",  ":/res/4_of_diamonds.png"},
    {"5-3",  ":/res/5_of_spades.png"},
    {"6-0",  ":/res/6_of_hearts.png"},
    {"7-1",  ":/res/7_of_clubs.png"},
    {"8-2",  ":/res/8_of_diamonds.png"},
    {"9-3",  ":/res/9_of_spades.png"},
    {"10-0", ":/res/10_of_hearts.png"},
    {"11-1", ":/res/jack_of_clubs.png"},
    {"12-2", ":/res/queen_of_diamonds.png"},
    {"13-3", ":/res/king_of_spades.png"},
    {"14-0", ":/res/ace_of_hearts.png"},
    {"2-1",  ":/res/2_of_clubs.png"},
    {"3-2",  ":/res/3_of_diamonds.png"},
    {"4-3",  ":/res/4_of_spades.png"},
    {"5-0",  ":/res/5_of_hearts.png"},
    {"6-1",  ":/res/6_of_clubs.png"},
    {"7-2",  ":/res/7_of_diamonds.png"},
    {"8-3",  ":/res/8_of_spades.png"},
    {"9-0",  ":/res/9_of_hearts.png"},
    {"10-1", ":/res/10_of_clubs.png"},
    {"11-2", ":/res/jack_of_diamonds.png"},
    {"12-3", ":/res/queen_of_spades.png"},
    {"13-0", ":/res/king_of_hearts.png"},
    {"14-1", ":/res/ace_of_clubs.png"},
    {"2-2",  ":/res/2_of_diamonds.png"},
    {"3-3",  ":/res/3_of_spades.png"},
    {"4-0",  ":/res/4_of_hearts.png"},
    {"5-1",  ":/res/5_of_clubs.png"},
    {"6-2",  ":/res/6_of_diamonds.png"},
    {"7-3",  ":/res/7_of_spades.png"},
    {"8-0",  ":/res/8_of_hearts.png"},
    {"9-1",  ":/res/9_of_clubs.png"},
    {"10-2", ":/res/10_of_diamonds.png"},
    {"11-3", ":/res/jack_of_spades.png"},
    {"12-0", ":/res/queen_of_hearts.png"},
    {"13-1", ":/res/king_of_clubs.png"},
    {"14-2", ":/res/ace_of_diamonds.png"},
    {"2-3",  ":/res/2_of_spades.png"},
    {"3-0",  ":/res/3_of_hearts.png"},
    {"4-1",  ":/res/4_of_clubs.png"},
    {"5-2",  ":/res/5_of_diamonds.png"},
    {"6-3",  ":/res/6_of_spades.png"},
    {"7-0",  ":/res/7_of_hearts.png"},
    {"8-1",  ":/res/8_of_clubs.png"},
    {"9-2",  ":/res/9_of_diamonds.png"},
    {"10-3", ":/res/10_of_spades.png"},
    {"11-0", ":/res/jack_of_hearts.png"},
    {"12-1", ":/res/queen_of_clubs.png"},
    {"13-2", ":/res/king_of_diamonds.png"},
    {"14-3", ":/res/ace_of_spades.png"},
    {"none", ":/res/none.png"}
};

#endif // CARDS_H
