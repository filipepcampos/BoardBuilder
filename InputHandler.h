#pragma once
#include <string>
#include "Word.h"

class InputHandler{
public:
    std::string readFileName();
    void readSize(int &height, int &width);
    int readWord(Word &word);
private:
    int m_height, m_width;
    bool testWordInput(const std::string &input) const;
};