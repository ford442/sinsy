#include <iostream>
#include <string>
#include "../lib/util/InputFile.h"
#include "../lib/xml/XmlParser.h"

int main(int argc, char** argv){
    if (argc < 2){
        std::cerr << "Usage: parse_test <path-to-xml>\n";
        return 1;
    }
    try {
        sinsy::InputFile infile(argv[1]);
        if (!infile.isValid()){
            std::cerr << "Failed to open " << argv[1] << "\n";
            return 2;
        }
        std::string enc;
        sinsy::XmlParser parser;
        sinsy::XmlData* root = parser.read(infile, enc);
        if (root){
            std::cout << "Parsed root tag: " << root->getTag() << "\n";
            delete root;
        } else {
            std::cout << "Parsed no root\n";
        }
    } catch (const std::exception& ex){
        std::cerr << "Exception: " << ex.what() << "\n";
        return 3;
    }
    return 0;
}
