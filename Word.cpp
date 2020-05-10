#include "Word.h"
#include <utility>
#include <iostream>
#include <algorithm>

Word::Word() : m_vertical_pos(0), m_horizontal_pos(0), m_line(H), m_text(""){}

Word::Word(char vertical_char, char horizontal_char, char orientation, std::string text)
{
    m_line = orientation == 'V' ? V : H;
    m_vertical_pos = vertical_char - 'A';
    m_horizontal_pos = horizontal_char - 'a';
    m_text = std::move(text);
    std::transform(m_text.begin(), m_text.end(), m_text.begin(), ::toupper);
}

void Word::setValues(char vertical_char, char horizontal_char, char orientation, std::string text) {
    m_line = orientation == 'V' ? V : H;
    m_vertical_pos = vertical_char - 'A';
    m_horizontal_pos = horizontal_char - 'a';
    m_text = std::move(text);
    std::transform(m_text.begin(), m_text.end(), m_text.begin(), ::toupper);
}

std::pair<short, short> Word::getPosition() const{
    return std::pair<short, short>{m_vertical_pos, m_horizontal_pos};
}

orientation Word::getOrientation() const{
    return m_line;
}

std::string Word::getText() const{
    return m_text;
}

std::ostream& operator<<(std::ostream &os, const Word &word) {
    char vertical_char = word.m_vertical_pos + 'A';
    char horizontal_char = word.m_horizontal_pos + 'a';
    char orientation_char = word.m_line == H ? 'H' : 'V';
    os << vertical_char << horizontal_char << " " << orientation_char << " " << word.m_text<< std::endl;
    return os;
}

