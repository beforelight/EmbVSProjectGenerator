#include <iostream>
#include "prj.h"
//#include "clipp.h"
using namespace std;
//using namespace clipp;
int main(int argc, char *argv[]) {
//    bool rec = false, utf16 = false;
//    string infile = "", fmt = "csv";
//    auto cli = (
//            value("input file", infile),
//                    option("-r", "--recursive").set(rec).doc("convert files recursively"),
//                    option("-o") & value("output format", fmt),
//                    option("-utf16").set(utf16).doc("use UTF-16 encoding")
//    );



//    if (!parse(argc, argv, cli)) {
//        cout << make_man_page(cli, argv[0]);
//    }
    if(argc>=2){
        prj_ptr ptr;
        ptr.Load(argv[1]);
        ptr->Find();
    }
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
