//
// Created by david on 3/21/18.
//

#include "command.h"
#include "shell.h"
#include <cstdio>
#include <cassert>
#include <iostream>
#include <fcntl.h>
#include <limits>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream>
#include <dirent.h>
#include <cerrno>
#include <zconf.h>
#include <sys/stat.h>
#include <ctime>
#include <pwd.h>
#include <grp.h>
#include <sys/ioctl.h>
#include <csignal>
#include <sys/wait.h>
#include <thread>
#include <time.h>

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
    // Basic Functionality
    if (str == "ls") return ls;
    if (str == "ll") return ll;
    if (str == "cp") return cp;
    if (str == "cat") return cat;
    if (str == "grep") return grep;
    if (str == "stat") return statf;
    if (str == "clear") return clear;
    if (str == "exit") return exit;
    if (str == "help") return help;

    // Final Replacement Functions
    if (str == "cd") return cd;
    if (str == "mkdir") return mkdir;
    if (str == "rmdir") return rmdir;
    if (str == "stat") return statf;
    if (str == "sleep") return sleepf;
    if (str == "kill") return killf;
    if (str == "diff") return diff;
    if (str == "env") return env;
    if (str == "timeout") return timeout;
    if (str == "wait") return wait;
}

void Command::execute() {
    switch(hash(arguments.at(0)))
    {
        // Basic Functions
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
        case statf:
            stat_func();
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

        // Advanced Functions
        case cd:
            cd_func();
            break;
        case mkdir:
            mkdir_func();
            break;
        case rmdir:
            rmdir_func();
            break;
        case statf:
            stat_func();
            break;
        case sleepf:
            sleep_func();
            break;
        case killf:
            kill_func();
            break;
        case diff:
            diff_func();
            break;
        case env:
            env_func();
            break;
        case timeout:
            timeout_func();
            break;
        case wait:
            wait_func();
            break;

        // Default function
        default:
            cout << "Error: Unknown command!" << endl;
            help_func();
            break;
    }
}


// Basic Functions
void Command::ls_func() {
    DIR* dir;
    dirent* pdir;
    dir = opendir(".");

    if(arguments.front() == "ll" || arguments.size() > 1) {
        // Process arguments to ls
        if(arguments.front() == "ll" || arguments.at(1) == "-l") {
            // Standard ls -l implementation with 1 args
            char* first = nullptr;
            char* second = nullptr;

            while (pdir = readdir(dir)) {

                struct stat filestat;
                stat(pdir->d_name, &filestat);

                // Create permissions
                char perms[11] = "--------\0";
                mode_t fm = filestat.st_mode;
                if(pdir->d_type == DT_DIR)
                    perms[0] = 'd';
                if(fm & S_IRUSR)
                    perms[1] = 'r';
                if(fm & S_IWUSR)
                    perms[2] = 'w';
                if(fm & S_IXUSR)
                    perms[3] = 'x';
                if(fm & S_IRGRP)
                    perms[4] = 'r';
                if(fm & S_IWGRP)
                    perms[5] = 'w';
                if(fm & S_IXGRP)
                    perms[6] = 'x';
                if(fm & S_IROTH)
                    perms[7] = 'r';
                if(fm & S_IWOTH)
                    perms[8] = 'w';
                if(fm & S_IXOTH)
                    perms[9] = 'x';

                // Create time
                time_t tt = filestat.st_mtime;
                char strTime[128];
                struct tm ltime = *localtime(&tt);
                strftime(strTime, 127, "%b %I:%M:%S", &ltime);

                struct passwd userdat = *getpwuid(filestat.st_uid);
                struct group groupdat = *getgrgid(filestat.st_gid);

                if(strcmp(pdir->d_name, ".") == 0) {
                    first = (char*)malloc(1024);
                    snprintf(first, 1023, "%s. %li %s:%s %li %s" ANSI_COLOR_BLUE " %s" ANSI_COLOR_RESET,perms,
                             filestat.st_nlink, userdat.pw_name, groupdat.gr_name, filestat.st_size, strTime,
                             pdir->d_name);
                } else if(strcmp(pdir->d_name, "..") == 0) {
                    second = (char*)malloc(1024);
                    snprintf(second, 1023, "%s. %li %s:%s %li %s" ANSI_COLOR_BLUE " %s" ANSI_COLOR_RESET,perms,
                             filestat.st_nlink, userdat.pw_name, groupdat.gr_name, filestat.st_size, strTime,
                             pdir->d_name);
                }
            }

            if(first != nullptr) {
                cout << first << endl;
                free(first);
            }
            if(second != nullptr) {
                cout << second << endl;
                free(second);
            }
            dir = nullptr;
            dir = opendir(".");
            //seekdir(dir,0);
            while (pdir = readdir(dir)) {

                if(strcmp(pdir->d_name, ".")==0 || strcmp(pdir->d_name, "..")==0) {
                    continue;
                }

                struct stat filestat;
                stat(pdir->d_name, &filestat);

                // Create permissions
                char perms[11] = "--------\0";
                mode_t fm = filestat.st_mode;
                if(pdir->d_type == DT_DIR)
                    perms[0] = 'd';
                if(fm & S_IRUSR)
                    perms[1] = 'r';
                if(fm & S_IWUSR)
                    perms[2] = 'w';
                if(fm & S_IXUSR)
                    perms[3] = 'x';
                if(fm & S_IRGRP)
                    perms[4] = 'r';
                if(fm & S_IWGRP)
                    perms[5] = 'w';
                if(fm & S_IXGRP)
                    perms[6] = 'x';
                if(fm & S_IROTH)
                    perms[7] = 'r';
                if(fm & S_IWOTH)
                    perms[8] = 'w';
                if(fm & S_IXOTH)
                    perms[9] = 'x';

                // Create time
                time_t tt = filestat.st_mtime;
                char strTime[128];
                struct tm ltime = *localtime(&tt);
                strftime(strTime, 127, "%b %I:%M:%S", &ltime);

                struct passwd userdat = *getpwuid(filestat.st_uid);
                struct group groupdat = *getgrgid(filestat.st_gid);

                if(pdir->d_type == DT_DIR) {
                    printf("%s. %li %s:%s %li %s" ANSI_COLOR_BLUE " %s\n" ANSI_COLOR_RESET,perms,
                             filestat.st_nlink, userdat.pw_name, groupdat.gr_name, filestat.st_size, strTime,
                             pdir->d_name);
                } else {
                    printf("%s. %li %s:%s %li %s %s\n",perms,
                             filestat.st_nlink, userdat.pw_name, groupdat.gr_name, filestat.st_size, strTime,
                             pdir->d_name);
                }

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
        //cout << ANSI_COLOR_RED << strerr << ANSI_COLOR_RESET << endl;
        return false;
    } else {
        int res = ftruncate(dstfile, totalbytes);
        if(res < 0) {
            char* strerr = strerror(errno);
            //cout << ANSI_COLOR_RED << strerr << ANSI_COLOR_RESET << endl;
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
             files.push_back(arguments.at(i));
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

        streamfile(srcFile, dstFile);
        close(srcFile);
        close(dstFile);
    }
}

void Command::cat_func() {
    char line[100];
    FILE *fp;

    if(arguments.at(arguments.size()-2) == ">") {
        // TODO: Do multiple file output to file
        vector<string> files;
        for(int i=1; i<arguments.size()-1; i++){
            files.push_back(arguments.at(i));
        }

        const char* output;
        const char* srcFile;
        output = arguments.back().c_str();

        int dstFile = open(output, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
        if(dstFile < 0)
        {
            char* strerr = strerror(errno);
            cout << strerr << endl;
            return;
        }

        for(int i=0; i<files.size(); ++i) {

            srcFile = files.at(i).c_str();
            int fileOpen = open(srcFile, O_RDONLY);

            streamfile(fileOpen,dstFile);
            close(fileOpen);
        }

        close(dstFile);


    } else {
        // initialsing the file pointer to read
        //fp = fopen(arguments.at(1).c_str(),"r");

        if(! (fp = fopen(arguments.at(1).c_str(),"r")))
        {
            //char* strerr = strerror(errno);
            cout << "cat: " << arguments.at(1).c_str() << ": No such file or directory" << endl;
            return;
        }


        // reading line by line and comparing each word in line
        while(fscanf(fp , "%[^\n]\n" , line)!=EOF)
        {
            // print that line
            cout << "\t" << line << endl;
            //printf("%s\n" , line);
        }
        fclose(fp);
    }
}

void Command::grep_func() {

    char line[100];
    FILE *fp;

    for(int i=2; i<arguments.size(); ++i) {
        // initialsing the file pointer to read
        fp = fopen(arguments.at(i).c_str(),"r");

//        if(arguments.at(1).front() == "\"") {
//            arguments.erase(0,1);
//        }
        
        if(arguments.at(1).front() == '\"') {
            arguments.at(1).front() = '\0';
        }
        if(arguments.at(1).back() == '\"'){
            arguments.at(1).back() = '\0';
        }

        // reading line by line and comparing each word in line
        while(fscanf(fp , "%[^\n]\n" , line)!=EOF)
        {
            if(strstr(line , arguments.at(1).c_str()) != nullptr)
            {
                // print that line
                cout << "\nFrom file: " << arguments.at(i) << endl;
                cout << "\t" << line << endl;
                //printf("%s\n" , line);
            }
            else
            {
                continue;
            }
        }
        fclose(fp);
    }

}

void Command::stat_func(){
    struct stat s;
    const char* file_path;

    file_path = arguments.at(1).c_str();
    if(stat(file_path, &s)==1){
        printf("Error unable to locat %s", file_path);
    }
    else{

        char perms[11] = "---------\0";
                mode_t fm = s.st_mode;
                if(fm & S_IFDIR)
                    perms[0] = 'd';
                if(fm & S_IRUSR)
                    perms[1] = 'r';
                if(fm & S_IWUSR)
                    perms[2] = 'w';
                if(fm & S_IXUSR)
                    perms[3] = 'x';
                if(fm & S_IRGRP)
                    perms[4] = 'r';
                if(fm & S_IWGRP)
                    perms[5] = 'w';
                if(fm & S_IXGRP)
                    perms[6] = 'x';
                if(fm & S_IROTH)
                    perms[7] = 'r';
                if(fm & S_IWOTH)
                    perms[8] = 'w';
                if(fm & S_IXOTH)
                    perms[9] = 'x';

        struct tm * timeinfo;
        time_t access_time= s.st_atimespec.tv_sec;
        char access_buff[20];
        timeinfo = localtime (&(access_time));
        strftime(access_buff, 20, "%Y-%m%d %T", timeinfo);
        time_t modify_time = s.st_mtimespec.tv_sec;
        char modify_buff[20];
        timeinfo = localtime(&(modify_time));
        strftime(modify_buff, 20,"%Y-%m%d %T",timeinfo);
        time_t change_time = s.st_ctimespec.tv_sec;
        char change_buff[20];
        timeinfo = localtime(&(change_time));
        strftime(change_buff, 20,"%Y-%m%d %T",timeinfo);
        
        printf("File: %s\n", file_path);
        printf("Size: %d     Blocks: %d     IOBlocks: %d \n", s.st_size, s.st_blksize, s.st_blocks);
        printf("Device: %d     Inode: %d     Links: %d   \n", s.st_dev, s.st_ino, s.st_nlink);
        printf("Access: (%s)     Uid:(%d)     Gid:(%d) \n", perms,s.st_uid,s.st_gid);
        printf("Access: %s\n", &access_buff);
        printf("Modify: %s\n", &modify_buff);
        printf("Change: %s\n", &change_buff);
    }
    
}

void Command::clear_func() {
    int i;
    for (i=0; i<10; i++)
        cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
}

void Command::exit_func() {
    this->parent->shellExit();
}

// Final Replacement Functions
void Command::cd_func() {
    if (arguments.size() > 1) {
        if(arguments.at(1) == "--help" || arguments.at(1) == "-h") {
            printf("wait: usage: wait [id]\n");
            return;
        }



    }
}

void Command::mkdir_func() {
    if (arguments.size() > 1) {
        if(arguments.at(1) == "--help" || arguments.at(1) == "-h") {
            printf("wait: usage: wait [id]\n");
            return;
        }
    }
}

void Command::rmdir_func() {
    if (arguments.size() > 1) {
        if(arguments.at(1) == "--help" || arguments.at(1) == "-h") {
            printf("wait: usage: wait [id]\n");
            return;
        }
    }
}

void Command::stat_func() {
    if (arguments.size() > 1) {
        if(arguments.at(1) == "--help" || arguments.at(1) == "-h") {
            printf("wait: usage: wait [id]\n");
            return;
        }
    }
}

void Command::sleep_func() {
    if (arguments.size() > 1) {
        if(arguments.at(1) == "--help" || arguments.at(1) == "-h") {
            printf("Usage: sleep NUMBER(in seconds)...\n");
            printf("Pause for NUMBER seconds.\n");
            printf("NUMBER must be an integer.\n");
            printf("Any number of NUMBER values may be supplied. The system\n");
            printf("will pause for the the time specified by the number.\n");
            return;
        }

        int seconds = std::stoi(arguments.at(1));

        sleep(seconds);


    } else {
        printf("sleep: missing operand.\n");
        printf("Try 'sleep --help' for more information.\n");
        return;
    }
}

void Command::kill_func() {
    if (arguments.size() > 1) {
        if(arguments.at(1) == "--help" || arguments.at(1) == "-h") {
            printf("kill: usage: kill [signum] pid\n");
            return;
        }

        bool hasSigNum = false;
        int pidNum = 1;
        int sigNum = 1;
        int sig = 9;

        if(arguments.size() > 2) {
            hasSigNum = true;
            pidNum = 2;
            sigNum = 1;
        }

        const char* option_sig = arguments.at(sigNum).c_str();
        const char* option_pid = arguments.at(pidNum).c_str();

        pid_t pid = atoi(option_pid);

        if(hasSigNum) {
            sig = atoi(option_sig);
        } else {
            sig = 9;
        }

        if (kill(pid, sig) == -1) {
            printf("kill: error: %s", strerror(errno));
        }
    }
}

void Command::diff_func() {
    if (arguments.size() > 1) {
        if(arguments.at(1) == "--help" || arguments.at(1) == "-h") {
            printf("wait: usage: wait [id]\n");
            return;
        }
    }
}

void Command::env_func() {
    for (char** env = this->parent->getEnvPtr(); *env != NULL; env++)
    {
        printf("%s\n", *env);
    }
}

void Command::timeout_func() {
    if (arguments.size() > 1) {
        if(arguments.at(1) == "--help" || arguments.at(1) == "-h") {
            printf("wait: usage: wait [id]\n");
            return;
        }
    }
}

void Command::wait_func() {
    if (arguments.size() > 1) {
        if(arguments.at(1) == "--help" || arguments.at(1) == "-h") {
            printf("wait: usage: wait [id]\n");
            return;
        }

        pid_t pid = stoi(arguments.at(1).c_str());

        //now check if the process with pid actually exists
        //and if it does exist, keep checking until kill returns -1
        //kill is used because we aren't always dealing with child processes
        while(kill(pid, 0) != -1)
        {
            usleep(250000); //wait 0.25 seconds in between checks
        }

        //at this point kill has failed for some reason
        //return from this function if kill failed because the pid terminated

        //if kill failed for a different reason, then print the error
        //because this time there was actually an error
        if(errno != ESRCH)
            printf("wait: error: %s\n", strerror(errno));
    }
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


