//
// Created by david on 3/21/18.
//

#ifndef CUSTOMSHELL_COMMAND_H
#define CUSTOMSHELL_COMMAND_H

#include <vector>
#include <string>
#include <sstream>

using namespace std;

class Command {


private:

    enum COMMAND_CODES{
        ls,
        cp,
        cat,
        grep,
        clear,
        exit,
        help
    };

    // Vector of the arguments including the command itself
    vector<string> arguments;

    int numberOfArgs;

    string input;

    void addArgument(string& arg);

    COMMAND_CODES hash (string const& str);

    void ls_func();
    void cp_func();
    void cat_func();
    void grep_func();
    void clear_func();
    void exit_func();
    void help_func();


public:

    Command();



    void setInput(string& input);

    bool hasInput() const { return !input.empty(); }

    bool isEmpty() const;

    int parse(stringstream &ss);

    void execute();
};


#endif //CUSTOMSHELL_COMMAND_H
