#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <map>
#include <vector>

class Parser {
    public:
        int  parseLine(std::string);
        void makeMapFromString(std::string);
        int getMyId();
        std::map<std::string, int> getNickname();
        std::vector<int> getParseMap();
        std::vector<int> getMap();
        std::map<int, int> getPosOfPlayer();

    private:
        int myId;
        std::vector<int> parseMap;
        std::map<int, int> posOfPlayer;
        std::map<std::string, int> nickname;
};

#endif // PARSER_H
