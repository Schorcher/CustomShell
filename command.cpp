//
// Created by david on 3/21/18.
//

#include "command.h"
#include "shell.h"
#include <cassert>
#include <iostream>
#include <fcntl.h>
#include <limits>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream>
#include <dirent.h>
#include <zconf.h>
#include <sys/stat.h>

#define ANSI_COLOR_RED      "\x1b[91m"
#define ANSI_COLOR_GREEN    "\x1b[92m"
#define ANSI_COLOR_YELLOW   "\x1b[93m"
#define ANSI_COLOR_BLUE     "\x1b[94m"
#define ANSI_COLOR_MAGENTA  "\x1b[95m"
#define ANSI_COLOR_CYAN     "\x1b[91m"
#define ANSI_COLOR_RESET    "\x1b[0m"
#define ANSI_TEXT_BOLD      "\x1b[1m"


using namespace std;

Command::Command(Shell *parent) {
    numberOfArgs = 0;
    this->parent = parent;
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
    if (str == "ll") return ll;
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
        case ll:
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
    DIR* dir;
    dirent* pdir;
    dir = opendir(".");

    if(arguments.front() == "ll" || arguments.size() > 1) {
        // Process arguments to ls
        if(arguments.front() == "ll" || arguments.at(1) == "-l") {
            // Standard ls -l implementation with 1 args
            while (pdir = readdir(dir)) {
                cout << "\t" << pdir->d_name <<endl;
            }
        } else {
            cout << "Error: Unsupported argument" << endl;
        }

    } else {
        // Standard ls implementation with no args
        while (pdir = readdir(dir)) {
            cout << "\t" << pdir->d_name;
        }
        cout << endl;
    }
    closedir(dir);
}

bool Command::streamfile(int srcfile, int dstfile) {
    char buffer[1024];
    int numBytes = 0;
    off_t totalbytes = 0;

    do{
        numBytes = read(srcfile,&buffer,1024);
        if(numBytes < 0)
            break;
        int nbw = write(dstfile, &buffer, numBytes);
        if(nbw < 0)
            numBytes = -1;
        else
            totalbytes += numBytes;
    } while(numBytes >= 1024);

    if(numBytes < 0) {
        char* strerr = strerror(errno);
        cout << strerr << endl;
        return false;
    } else {
        int res = ftruncate(dstfile, totalbytes);
        if(res < 0) {
            char* strerr = strerror(errno);
            cout << strerr << endl;
            return false;
        }
        return true;
    }
}

void Command::cp_func() {
    if(arguments.size() > 3){
        // Do multiple file copy
        vector<string> files;
        const char* dir;

        //DIR* dir;

        for(int i=1; i<arguments.size()-1; i++){
             files.push_back(arguments.at(1));
        }
        dir = arguments.back().c_str();

        // Do the copy
        size_t lendir = strlen(dir);
        struct stat dirstat;
        stat(dir, &dirstat);
        if(!S_ISDIR(dirstat.st_mode)){
            cout << "The destination \"" << dir << "\" is not a directory." << endl;
        }
        DIR* directory = opendir(dir);
        if(directory == nullptr) {
           char* strerr = strerror(errno);
            cout << strerr << endl;
            return;
        }
        for (int i = 0; i < files.size(); ++i) {
            auto destfile = (char *)malloc(strlen(files.at(i).c_str()) + 2 + lendir);
            strcpy(destfile,dir);
            strcat(destfile,"/");
            strcat(destfile, files.at(i).c_str());

            int srcFile = open(files.at(i).c_str(), O_RDONLY);
            if(srcFile < 0)
            {
                char* strerr = strerror(errno);
                cout << strerr << endl;
                return;
            }
            int dstFile = open(destfile, O_CREAT | O_WRONLY);
            if(dstFile < 0)
            {
                char* strerr = strerror(errno);
                cout << strerr << endl;
                close(srcFile);
                return;
            }
            streamfile(srcFile,dstFile);
            close(srcFile);
            close(dstFile);
            free(destfile);
        }
    } else {
        // Do file duplication
        // Set the files
        string file1 = arguments.at(1);
        string file2 = arguments.at(2);

        // Do the copy
        int srcFile = open(file1.c_str(), O_RDONLY);
        if(srcFile < 0)
        {
            char* strerr = strerror(errno);
            cout << strerr << endl;
            return;
        }
        int dstFile = open(file2.c_str(),O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        if(dstFile < 0)
        {
            char* strerr = strerror(errno);
            cout << strerr << endl;
            close(srcFile);
            return;
        }
    }
}

void Command::cat_func() {

}

void Command::grep_func() {

}

void Command::clear_func() {
    int i;
    for (i=0; i<10; i++)
        cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
}

void Command::exit_func() {
    this->parent->shellExit();
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
