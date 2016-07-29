#include "main.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include "type.h"
#include "parsers.h"

using gg2aa::InputFiles;
using gg2aa::FileName;

InputFiles split(const std::string &str, char c) {
    InputFiles fnames;
    auto i   = 0;
    auto pos = str.find(c);
    if (pos == std::string::npos) {
        fnames.push_back(str);
    }
    while (pos != std::string::npos) {
        fnames.push_back(str.substr(i, pos - i));
        i   = ++pos;
        pos = str.find(c, pos);

        if (pos == std::string::npos) {
            fnames.push_back(str.substr(i, str.length()));
        }
    }
    return fnames;
}

InputFiles removeSpaces(const InputFiles &fnames) {
    auto fs = fnames;
    for (auto &f : fs) {
        f.erase(std::remove(f.begin(), f.end(), ' '), f.end());
    }
    return fs;
}

FileName removeInitDash(const FileName &fname) {
    if (fname.front() == '-') {
        return fname.substr(1, fname.length());
    }
    return fname;
}

int main(int argc, char *argv[]) {
    std::string appname = "test";
    if (argc != 2) {
        std::cerr << " Usage: " << appname << " input\n"
                  << "     input - input file\n";
        return 1;
    }

    std::ifstream infile(argv[1]);
    std::string line;
    while (std::getline(infile, line)) {
        if (line.find("#") == std::string::npos) {
            std::istringstream iss(line);
            std::cout << iss.str() << '\n';
        } else {
            continue;
        }
    }
    infile.close();

    std::istringstream test_str1("sigma: data/Sigma.dat");
    std::string parsed;
    bool isSigma = false;
    while (std::getline(test_str1, parsed)) {
        auto parsed_s = removeSpaces(split(parsed, ':'));
        if (parsed_s.front() == "sigma") {
            isSigma = true;
            std::cout << "-- test_str1: " << parsed_s.back() << '\n';
        }
    }

    std::istringstream test_str2("sigma:\n  data/Sigma.dat");
    std::map<std::string, std::string> data2;
    isSigma = false;
    while (std::getline(test_str2, parsed)) {
        auto parsed_s = removeSpaces(split(parsed, ':'));
        if (parsed_s.front() == "sigma") {
            isSigma = true;
            if (parsed_s.back().empty()) {
                continue;
            }
        }
        if (isSigma) {
            data2["sigma"] = parsed_s.front();
        }
    }
    std::cout << "-- test_str2: " << data2["sigma"] << '\n';

    gg2aa::InputData data3;
    std::istringstream test_str3(
        "  direct:\n    - data/direct.dat\n    - data/direct.dat2");
    while (std::getline(test_str3, parsed)) {
        auto parsed_s = removeSpaces(split(parsed, ':'));
        if (parsed_s.front() == "direct") {
            data3.set_status(gg2aa::InputStatus::DIRECT);
            if (parsed_s.back().empty()) {
                continue;
            }
        }
        if (data3.get_status() == gg2aa::InputStatus::DIRECT) {
            auto fname = parsed_s.front();
            if (fname.front() == '-') {
                data3.add_background("direct", fname.substr(1, fname.length()));
            }
        }
    }
    for (const auto &e : data3.background("direct")) {
        std::cout << "-- test_str3: " << e << '\n';
    }
}
