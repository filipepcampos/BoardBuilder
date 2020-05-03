#include <iostream>
#include <string>
#include "Board.h"
#include "Word.h"
#include "InputHandler.h"

int main() {
    InputHandler input_handler{};
    std::string filename = input_handler.readFileName();
    if(filename.empty()){
        return 1;
    }

    short height, width;
    if(input_handler.readSize(height, width) == -1){
        return 1;
    }

    Board b(height, width, filename);
    std::cout << "Please write words as follows: 'Aa H WORD'\n" << "Aa is the position of the first letter\n" <<
              "H or V represents the orientation of the word (vertical or horizontal)" <<
              "WORD is the word you wish to output\n" << "If you wish to see current board type 'display'\n" <<
              "When done please type 'exit' with no whitespace\n";

    bool exit = false;
    while(!exit){
        Word word;
        int status = input_handler.readWord(word);

        switch(status){
            case -1: exit = true; continue;
            case 1: b.print(); continue;
        }

        if(!b.addWord(word)){
            std::cout << "Invalid action, please try again" << std::endl;
        }
    }
    b.save();
    std::cout << std::endl;
    return 0;
}