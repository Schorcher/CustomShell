//
// Created by david on 3/21/18.
//

#ifndef CUSTOMSHELL_COMMAND_H
#define CUSTOMSHELL_COMMAND_H

#include <vector>
#include <string>
#include <sstream>
#include "shell.h"

using namespace std;

class Command {


private:

    enum COMMAND_CODES{
        ls,
        ll,
        cp,
        cd,
        cat,
        grep,
        statf,
        diff,
        clear,
        exit,
        help,
        cd,
        mkdir,
        rmdir,
        statf,
        sleepf,
        killf,
        diff,
        env,
        timeout,
        wait
    };

    // Vector of the arguments including the command itself
    vector<string> arguments;

    int numberOfArgs;

    Shell *parent;

    string input;

    void addArgument(string& arg);

    COMMAND_CODES hash (string const& str);

    // Basic Functions
    void ls_func();
    void cp_func();
    void cd_func();
    void cat_func();
    void grep_func();
    void stat_func();
    void diff_func();
    void clear_func();
    void exit_func();
    void help_func();

    // Final Replacement Functions
    void cd_func();
    void mkdir_func();
    void rmdir_func();
    void stat_func();
    void sleep_func();
    void kill_func();
    void diff_func();
    void env_func();
    void timeout_func();
    void wait_func();

    bool streamfile(int srcfile, int dstfile);



public:

    Command(Shell *parent);



    void setInput(string& input);

    bool hasInput() const { return !input.empty(); }

    bool isEmpty() const;

    int parse(stringstream &ss);

    void execute();
};


#endif //CUSTOMSHELL_COMMAND_H
