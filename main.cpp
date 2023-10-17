#include "EventDriven.h"
#include "Server.h"
#include <iostream>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "usage: ./server subReactors threads\n";
        exit(EXIT_FAILURE);
    }

    EventDriven ed;
    Server server(ed, 2000, std::stoi(argv[1]), std::stoi(argv[2]));
    ed.start();
}
