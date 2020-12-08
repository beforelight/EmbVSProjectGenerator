#include <iostream>
#include "tinyxml2.h"
using namespace tinyxml2;
int main() {

    std::cout << "Hello, World!" << std::endl;
    XMLDocument doc;
    return doc.LoadFile("ex.aml");
    return 0;


//    static const char* xml =
//            "<information>"
//            "	<attributeApproach v='2' />"
//            "	<textApproach>"
//            "		<v>2</v>"
//            "	</textApproach>"
//            "</information>";
//
//    XMLDocument doc;
//    doc.Parse( xml );
//
//    int v0 = 0;
//    int v1 = 0;
//
//    XMLElement* attributeApproachElement = doc.FirstChildElement()->FirstChildElement( "attributeApproach" );
//    attributeApproachElement->QueryIntAttribute( "v", &v0 );
//
//    XMLElement* textApproachElement = doc.FirstChildElement()->FirstChildElement( "textApproach" );
//    textApproachElement->FirstChildElement( "v" )->QueryIntText( &v1 );
//
//    printf( "Both values are the same: %d and %d\n", v0, v1 );
//
//    doc.SaveFile("ex.aml");
//    return !doc.Error() && ( v0 == v1 );
}
