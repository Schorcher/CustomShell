//
// Created by david on 3/21/18.
//

#ifndef CUSTOMSHELL_TOKEN_H
#define CUSTOMSHELL_TOKEN_H

#include <string>

using namespace std;

class Token {

public:

    enum token_t {
        END,
        EOL,
        SEQUENCE,
        PIPE,
        INPUT,
        OUTPUT,
        APPEND,
        BACKGROUND,
        WORD
    };

private:

    token_t type;
    string text;

    explicit
    Token(token_t t) : type(t) {}
    Token(token_t t, const string &s) : type(t), text(s) {}

public:

    token_t getType() const { return type; }
    string& getText() { return text; }


    static Token *nextToken(istream &is);

    friend ostream& operator<<(ostream& os, const Token& token);
};


#endif //CUSTOMSHELL_TOKEN_H
