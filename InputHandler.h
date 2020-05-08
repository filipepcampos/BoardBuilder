#pragma once
#include <string>
#include "Word.h"
#define RED "\u001b[31m"
#define RESET "\u001b[0m"

class InputHandler{
public:
    std::string readFileName();
    int readSize(short &height, short &width);
    int readWord(Word &word);
private:
    short m_height, m_width;
    bool checkFileName(const std::string &name) const;
    bool checkWordInput(const std::string &input) const;
};