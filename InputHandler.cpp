#include "InputHandler.h"
#include <iostream>
#include <limits>

std::string InputHandler::readFileName() {
    std::string filename;
    std::cout << "Board file name: "; std::cin >> filename;
    return filename;
}

void InputHandler::readSize(int &height, int &width){
    char divider;
    std::cout << "Size (HEIGHT x WIDTH, example: 10 x 10): "; std::cin >> height >> divider >> width;
    if(divider != 'x'){
        return;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}