#include <iostream>
#include <string>
#include "Board.h"
#include "Word.h"
#include "IO.h"

int main() {
    IO::displayTitle();
    std::string filename = IO::readFileName();
    if(filename.empty()){
        return 1;
    }
    short height, width;
    if(IO::readSize(height, width) <= 0){
        return 1;
    }

    Board b(height, width, filename);
    IO::instructions();

    bool exit = false;
    while(!exit){
        b.print();
        Word word;
        int status = IO::readWordInput(word);

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