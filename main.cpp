#include <iostream>
#include <string>
#include "Board.h"
#include "Word.h"
#include "InputHandler.h"
#define BLUE "\u001b[36m"
#define RED "\u001b[31m"
#define RESET "\u001b[0m"

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
              "When done please type 'exit' with no whitespace\n" <<
              "If your board doesn't have at least 14 letters it won't be saved\n";

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


    if(b.save()){
        std::cout << BLUE << "Board has been saved" << RESET << std::endl;
    }
    else{
        std::cout << RED << "Board wasn't saved due to lack of letters" << RESET << std::endl;
    }
    std::cout << std::endl;
    return 0;
}