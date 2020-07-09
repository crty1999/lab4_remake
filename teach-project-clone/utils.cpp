#include "utils.h"

#include <iostream>
#include <sstream>
#include <string>
using namespace std;

istringstream stdin_line_stream() {
    string s; 
    getline(cin, s);
    return istringstream(s);
}

