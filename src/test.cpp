//
// Created by 17616 on 2020/12/8.
//

#include <iostream>
#include "findxml.h"
#include "pugixml.hpp"
using namespace pugi;
using namespace std;

int main() {

    std::cout << "Hello, World!" << std::endl;
    xml_document doc;

    if (doc.load_file("xmlfile/.cproject")) {
        xpath_node_set def = doc.select_nodes("//toolChain/tool/option");
        for(xpath_node i:def){
            if(string(i.node().attribute("id").value())
            .find("compiler.option.definedsymbols")
            !=string::npos){
                cout<<"id:"<<i.node().attribute("id").value()<<endl;
            }
        }
    } else {
        printf("no file\r\n");
    }
    return 0;
}
