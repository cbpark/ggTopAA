#ifndef SRC_GG2AA_H_
#define SRC_GG2AA_H_

#include <iostream>
#include <string>

int failedToRead(const std::string &appname, const std::string &fname) {
    std::cerr << appname << ": failed to read `" << fname << "'.\n";
    return 1;
}

int errMsg(const std::string &appname, const std::string &msg) {
    std::cerr << appname << ": " << msg << '\n';
    return 1;
}

void message(const std::string &appname, const std::string &msg,
             std::ostream *out) {
    *out << appname << ": " << msg << '\n';
}

#endif  // SRC_GG2AA_H_
