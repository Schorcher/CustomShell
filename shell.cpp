//
// Created by david on 3/21/18.
//

#include <iostream>
#include <sstream>
#include "shell.h"
#include "command.h"


Shell::Shell(char **env) {
    this->keepRunning = true;
    this->env_ptr = env;
    //this->dir = opendir(".");
    this->cwd = ".";
}


void Shell::run() {
    printWelcome();
    while(keepRunning) {
        shellLoop();
    }
    printExit();
}

void Shell::shellLoop() {
    auto cmd = new Command(this);
    string line;

    // Prompt user for input
    shellPrompt();

    // Get the users input
    getline(cin,line);

    // Use string stream to tokenize the input
    stringstream ss;
    ss << line;

    // Parse the input
    cmd->parse(ss);

    if(!cmd->isEmpty())
        cmd->execute();

}

void Shell::shellExit() {
    this->keepRunning = false;
}

void Shell::clear() {

}

void Shell::shellPrompt() {
    cout << "# -> ";
}

void Shell::printWelcome() {
    cout << "Welcome to the custom shell made by David McFall, Josh Sexton, and Alex Coca" << endl;
    cout << endl;
}

void Shell::printExit() {
    cout << "Now exiting shell..." << endl;
}

char **Shell::getEnvPtr() {
    return this->env_ptr;
}

char* Shell::getCWD() {
    return this->cwd;
}

void Shell::setCWD(char* str) {
    this->cwd = str;
}
