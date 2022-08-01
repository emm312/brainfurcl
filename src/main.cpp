#include <iostream>
#include <string>
#include <vector>
#include <token.h>
#include <compiler.h>
#include <fstream>
#include <streambuf>


int main(int argc, char* argv[]) {
    if (argv[1] == nullptr) {
        std::cout << "No input file specified\n";
        return 1;
    }
    std::ifstream file(argv[1]);
    std::string src((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>()    );


    

    std::string out = compile(src);
    // output out to out.urcl
    std::ofstream("out.urcl") << out;
}