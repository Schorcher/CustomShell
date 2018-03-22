//
// Created by david on 3/21/18.
//

#ifndef CUSTOMSHELL_SHELL_H
#define CUSTOMSHELL_SHELL_H

#include <sys/resource.h>
#include <ctime>
#include <csignal>
#include <vector>
#include <map>
#include <string>
#include <fcntl.h>
#include <dirent.h>

using namespace std;

struct child_info{
    int pid;
    int id;
    string status;
    string p_name;

    struct child_info &operator=(const struct child_info &csh){
        if(this != &csh){
            pid = csh.pid;
            id = csh.id;
            status = csh.status;
            p_name = csh.p_name;
        }
        return *this;
    }
};


class Shell {

public:

    Shell(char ** env);
    //typedef void (Shell::*Fctn)(string arg);

    void run();

    void shellExit();

    char ** getEnvPtr();

    char * getCWD();

    void setCWD(char* str);


private:

    pid_t  SH_PID;
    bool keepRunning;

    //map<string, Fctn> commands;
    map<string, string> vars;
    char ** env_ptr;

    char* cwd = nullptr;

    uid_t uid;

    void shellLoop();

    void shellPrompt();

    void printWelcome();

    void printExit();

    void clear();



};


#endif //CUSTOMSHELL_SHELL_H
