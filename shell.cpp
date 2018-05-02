//
// Created by david on 3/21/18.
//

#include <iostream>
#include <sstream>
#include <zconf.h>
#include <pwd.h>
#include "shell.h"
#include "command.h"
#include <unistd.h>



Shell::Shell(char **env) {
    this->keepRunning = true;
    this->env_ptr = env;
    //this->dir = opendir(".");
    this->cwd = ".";
}


void Shell::run() {
    printWelcome();
    cwd = getcwd(NULL, 0);
    gethostname(computername, 128);
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

void Shell::shellPrompt() {
    cout << "# " << ANSI_COLOR_YELLOW << getUsername() << "@" << computername << ANSI_COLOR_RESET << "  " << ANSI_COLOR_BLUE << getCWD() << ANSI_COLOR_RESET << "\n-> ";
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
    free(cwd);
    cwd = getcwd(NULL, 0); //REMEMBER TO free() later
    return this->cwd;
}

void Shell::setCWD(char* str) {
    this->cwd = str;
}

string Shell::getUsername() {
    register struct passwd *pw;
    register uid_t uid;
    int c;

    uid = geteuid ();
    pw = getpwuid (uid);
    if (pw)
    {
        return std::string(pw->pw_name);
    }
    return std::string("");
}

char* Shell::getHostname() {
    char hostname[1024];
    //gethostname(hostname, 1024);
    //puts(hostname);


    return hostname;
}