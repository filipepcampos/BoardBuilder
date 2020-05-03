#pragma once
#include <string>

enum orientation{H, V};

class Word{
public:
    Word();
    Word(char vertical_char, char horizontal_char, char orientation, std::string text);
    std::pair<short, short> getPosition() const;
    orientation getOrientation() const;
    std::string getText() const;
    friend std::ostream& operator<<(std::ostream &os, const Word &word);

private:
    short m_vertical_pos, m_horizontal_pos;
    orientation m_line;
    std::string m_text;
};