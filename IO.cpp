#include "IO.h"
#include <iostream>
#include <sstream>
#include <limits>

#define BLUE "\u001b[36m"
#define RED "\u001b[31m"
#define RESET "\u001b[0m"

std::string IO::readFileName() {
    std::string file_name;
    bool valid;
    do {
        std::cout << "Board file name: ";
        getline(std::cin, file_name);

        if (std::cin.eof()) {
            std::cout << RED << "EOF" << RESET << " has occurred" << std::endl;
            file_name.clear();
            return file_name;
        } else if (!file_name.empty()) {
            valid = checkFileName(file_name);
        } else {
            std::cout << "File name can't be " << RED << "empty" << RESET << std::endl;
            valid = false;
        }

    } while (!valid);
    file_name.append(".txt");
    return file_name;
}

bool IO::checkFileName(const std::string &name) {
    if (name == "WORDS") {
        std::cout << "Can't use " << RED << "reserved name" << RESET " as file name" << std::endl;
        return false;
    }
    for (auto c : name) {
        if (!isalnum(c)) {
            std::cout << "File name must contain only " << RED << "alphanumeric characters without whitespace" << RESET
                      << std::endl;
            return false;
        }
    }
    return true;
}

int IO::readSize(short &height, short &width) {
    bool valid;
    do {
        valid = true;
        std::cout << "Size (10 x 10 for example): ";

        // This method is used to make sure ENTER can't be pressed without detection
        std::string buffer;
        std::getline(std::cin, buffer);
        if (std::cin.eof()) {
            std::cout << RED << "EOF" << RESET << " has occurred" << std::endl;
            return -1;
        }
        std::stringstream ss(buffer);

        char divider;
        ss >> m_height >> divider >> m_width;
        if (ss.fail() || ss.rdbuf()->in_avail() || (divider != 'x' && divider != 'X')) {
            std::cout << RED << "Invalid input" << RESET << std::endl;
            valid = false;
        } else if (m_height <= 0 || m_height > 20 || m_width <= 0 || m_width > 20 || m_width * m_height < 14) {
            std::cout << RED << "Invalid size" << RESET << std::endl;
            valid = false;
        }
    } while (!valid);
    height = m_height;
    width = m_width;
    return 0;
}

int IO::readInput(Word &word) const {
    bool valid;
    std::string input;
    std::cout << "> ";
    getline(std::cin, input);
    if (input == "exit" || std::cin.eof()) {
        return -1;
    }
    else if (input == "instructions"){
        IO::instructions();
        return 1;
    }
    valid = checkWordInput(input);
    if(!valid){
        return 1;
    }
    word.setValues(input[0], input[1], input[3], input.substr(5));
    return 0;
}

void IO::error(const std::string &s) {
    std::cout << RED << s << RESET << std::endl;
    pressToContinue();
}

bool IO::checkWordInput(const std::string &input) const {
    if (input[2] != ' ' || input[4] != ' ') {
        error("Input wrongly formatted");
        return false;
    }
    if (input[0] < 'A' || input[0] > 'A' + m_height - 1 || input[1] < 'a' || input[1] > 'a' + m_width - 1) {
        error("Position outside of board boundaries");
        return false;
    }
    if (input[3] != 'H' && input[3] != 'V') {
        error("Orientation value not allowed (Only 'H' or 'V' is allowed)");
        return false;
    }
    std::string word = input.substr(5);
    if (word.size() < 2) {
        error("Word must have at least two characters");
        return false;
    }
    for (auto c : word) {
        if (!isalpha(c)) {
            error("Word must contain only alphabetic characters without whitespace");
            return false;
        }
    }
    return true;
}

void IO::instructions() {
    std::cout << CLEAR;
    std::cout << "\n\nPlease write words as follows: '" << BLUE << "Aa H WORD" << RESET << "'\n" <<
              BLUE << "Aa" << RESET << " is the position of the first letter\n" <<
              BLUE << "H" << RESET << " or " << BLUE << "V" << RESET <<
              " represents the orientation of the word (vertical or horizontal)\n" <<
              BLUE << "WORD" << RESET << " is the word you wish to output" <<
              "\nIf you wish to see these instructions again type '" << BLUE << "instructions" << RESET <<
              "'\nWhen done please type '" << RED << "exit" << RESET <<"' with no whitespace\n" <<
              "If your board doesn't have at least 14 letters it won't be saved\n\n";
    pressToContinue();
    std::cout << CLEAR;
}

void IO::invalidAction() {
    error("Invalid action, please try again");
}

void IO::savedMessage(bool saved){
    if(saved){
        std::cout << BLUE << "Board has been saved" << RESET << std::endl << std::endl;
    }
    else{
        std::cout << RED << "Board wasn't saved due to lack of letters" << RESET << std::endl << std::endl;
    }
}

void IO::suggestionMessage(const std::string &s1, const std::string &s2){
    std::cout << "Suggestion: " << BLUE << s1 << RESET << " or " << BLUE << s2 << RESET << std::endl;
}
void IO::suggestionMessage(const std::string &s){
    std::cout << "Suggestion: " << BLUE << s << RESET << std::endl;
}

void IO::displayTitle(){
    std::cout << CLEAR;
    std::cout << BLUE << "______                     _  ______       _ _     _           \n"
                         "| ___ \\                   | | | ___ \\     (_) |   | |          \n"
                         "| |_/ / ___   __ _ _ __ __| | | |_/ /_   _ _| | __| | ___ _ __ \n"
                         "| ___ \\/ _ \\ / _` | '__/ _` | | ___ \\ | | | | |/ _` |/ _ \\ '__|\n"
                         "| |_/ / (_) | (_| | | | (_| | | |_/ / |_| | | | (_| |  __/ |   \n"
                         "\\____/ \\___/ \\__,_|_|  \\__,_| \\____/ \\__,_|_|_|\\__,_|\\___|_|   \n\n\n" << RESET;
}

void IO::pressToContinue() {
    std::cout << "\nPlease press any key to continue ...\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}