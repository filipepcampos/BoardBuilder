#pragma once
#include <string>

class InputHandler{
public:
    std::string readFileName();
    void readSize(int &height, int &width);
    int readWord(std::string &input);
private:
    int m_height, m_width;
    bool testInput(std::string input);
};