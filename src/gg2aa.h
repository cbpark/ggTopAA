#ifndef GGTOPAA_SRC_GG2AA_H_
#define GGTOPAA_SRC_GG2AA_H_

#include <iostream>
#include <string>

void failedToRead(std::string appname, std::string fname) {
    std::cerr << appname << ": failed to read `" << fname << "'.\n";
}

void errMsg(std::string appname, std::string msg) {
    std::cerr << appname << ": " << msg << '\n';
}

#endif  // GGTOPAA_SRC_GG2AA_H_
