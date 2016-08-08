#ifndef SRC_GG2AA_H_
#define SRC_GG2AA_H_

#include <iostream>
#include <string>

void failedToRead(std::string appname, std::string fname) {
    std::cerr << appname << ": failed to read `" << fname << "'.\n";
}

int errMsg(std::string appname, std::string msg) {
    std::cerr << appname << ": " << msg << '\n';
    return 1;
}

void message(std::string appname, std::string msg, std::ostream *out) {
    *out << appname << ": " << msg << '\n';
}

#endif  // SRC_GG2AA_H_
