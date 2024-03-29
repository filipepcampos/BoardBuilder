#include <iostream>
#include <string>
#include "Board.h"
#include "Word.h"
#include "IO.h"

int main() {
    IO io{};
    IO::displayTitle();

    std::string filename;
    short height, width;
    try{
        filename = IO::readFileName();
        io.readSize(height, width);
    } catch(const IO::CinEof &e){
        std::cout << std::endl;
        return 1;
    }

    Board b(height, width, filename);
    IO::instructions();

    bool exit = false;
    while(!exit){
        b.print();
        Word word;
        int status = io.readWordInput(word);
        switch(status){
            case -1: exit = true; continue;
            case 1: continue;
        }
        if(!b.addWord(word)){
            IO::invalidAction();
        }
    }
    bool saved = b.save();
    IO::savedMessage(saved);
    return 0;
}