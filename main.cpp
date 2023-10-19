#include "EventDriven.h"
#include "Server.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdlib.h>

char* name;
std::vector<std::string> parameters;

int port        = 2000,
    subReactors = 0,
    threads     = 0;
    

inline bool is_digit(const std::string& s);
inline void help();
inline void inputErr(char s);
inline void uninputErr(char s);
void checkErr(decltype(parameters.end())& it, char c);

int main(int argc, char** argv) {
    name = argv[0];

    for (int i = 1; i < argc; ++i)
        parameters.push_back(argv[i]);

    for (auto it = parameters.begin(); it != parameters.end(); ++it) {
        char c = (*it)[1];
        switch (c) {
            case 'h': {
                help();
            } continue;

            case 's': {
                checkErr(it, 's');
            } continue;

            case 't': {
                checkErr(it, 't');
            } continue;

            case 'p': {
                checkErr(it, 'p');
            } continue;

            default:  {
                if (*it != "-")
                    std::cerr << name << ": error usage for \'" << *it << "\'\n"; 
                else if (c == '\0')
                        std::cerr << name << ": error useage for \'-\'\n";
                     else
                        std::cerr << name << ": invalid option -- \'" << c << "\'\n";
                std::cerr << "Try \'" << name << " -h\' for more information.\n";
                exit(EXIT_FAILURE);
            } 

            std::cerr << "Error in switch\nShouldn't be here!!\n";
            exit(EXIT_FAILURE);
        }
    }

    if (subReactors != 0 && threads == 0) {
        std::cerr << name << ": error usage for \'-s\'\n"
                  << "use -s also must use -t\n"
                  << "Try \'" << name << " -h\' for more information.\n";
        exit(EXIT_FAILURE);
    }

    EventDriven ed;
    Server server(ed, port, subReactors, threads);
    ed.start();

    return 0;
}

inline bool is_digit(const std::string& s) {
    return std::all_of(s.begin(), s.end(), isdigit);
}

inline void help() {
    std::cerr << "Usage: " << name << " [OPTION] [PARAMETER] COMMAND...\n\n"
              << " Options:\n"
              << "  -h\t\t\tview help\n"
              << "  -p\t-p [NUMBER]\tset the port of server. Defautl is 2000.\n"
              << "  -s\t-s [NUMBER]\tset the number of subReactors. Default is 0, use One Reactor mode.\n"
              << "  -t\t-t [NUMBER]\tset the number of threads. Default is 0, use One Thread mode.\n\n"
              << " If not set -s or -t option, the server default use One Reactor One Thread mode.\n"
              << " Set -t can change mode to one Reactor More Threads.\n"
              << " Set -s also must set -t, this can change mode to More Reactors More Threads.\n";
    exit(EXIT_SUCCESS);
}

inline void inputErr(char s) {
    std::cerr << name << ": invalid parameter for option -- \'" << s << "\'\n"
              << "It should be a digit.\n"
              << "Please try again.\n";
    exit(EXIT_FAILURE);
}

inline void uninputErr(char s) {
    std::cerr << name << ": no input parameter after option -- \'" << s << "\'\n"
              << "Try \'" << name << " -h\' for more information.\n";
    exit(EXIT_FAILURE);
}

void checkErr(decltype(parameters.end())& it, char c) {
    ++it;
    if (it == parameters.end())
        uninputErr(c);
    if (is_digit(*it)) {
        if (c == 's')
            subReactors = std::stoi(*it);
        if (c == 't')
            threads     = std::stoi(*it);
        if (c == 'p')
            port        = std::stoi(*it);
    } else
        inputErr(c);
}
