//
// Created by david on 3/21/18.
//

#include "command.h"
#include <cassert>
#include <iostream>
#include <fcntl.h>
#include <limits>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream>

using namespace std;

Command::Command() {
    numberOfArgs = 0;
}

void Command::addArgument(string &arg) {
    arguments.push_back(arg);
    numberOfArgs++;
}

void Command::setInput(string &inputt) {
    input = inputt;
}

bool Command::isEmpty() const {
    return input.empty() && arguments.empty();
}

int Command::parse(stringstream &ss) {
    while (!ss.eof()) {
        string temp;
        ss >> temp;
        addArgument(temp);
    }
}

Command::COMMAND_CODES Command::hash(string const &str) {
    if (str == "ls") return ls;
    if (str == "cp") return cp;
    if (str == "cat") return cat;
    if (str == "grep") return grep;
    if (str == "clear") return clear;
    if (str == "exit") return exit;
    if (str == "help") return help;
}

void Command::execute() {
    switch(hash(arguments.at(0)))
    {
        case ls:
            ls_func();
            break;
        case cp:
            cp_func();
            break;
        case cat:
            cat_func();
            break;
        case grep:
            grep_func();
            break;
        case clear:
            clear_func();
            break;
        case exit:
            exit_func();
            break;
        case help:
            help_func();
            break;
        default:
            cout << "Error: Unknown command!" << endl;
            help_func();
            break;
    }
}

void Command::ls_func() {

}

void Command::cp_func() {

}

void Command::cat_func() {

}

void Command::grep_func() {

}

void Command::clear_func() {

}

void Command::exit_func() {

}

void Command::help_func() {
    cout << "Available functions: " << endl;
    cout << "\tls" << endl;
    cout << "\tcp" << endl;
    cout << "\tcat" << endl;
    cout << "\tgrep" << endl;
    cout << "\tclear" << endl;
    cout << "\texit" << endl;
    cout << "\thelp" << endl;
    cout << endl;
}
