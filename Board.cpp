#include <iostream>
#include <algorithm>
#include "Board.h"

Board::Board(int height, int width, const std::string &file_name)
    : m_height(height), m_width(width)
{
    m_board = new Tile* [height];
    for(int i = 0; i < height; ++i){
        m_board[i] = new Tile[width];
    }
    m_file.open(file_name);
    m_file << height << " x " << width << std::endl;

    m_words_file.open(m_words_file_name);
}

Board::~Board(){
    for(int i=0; i < m_height; ++i){
        delete [] m_board[i];
    }
    delete [] m_board;
    if(m_file.is_open()){
        m_file.close();
    }
    if(m_words_file.is_open()){
        m_words_file.close();
    }
}

void Board::save(){
    print(m_file);
    m_file.close();
    m_words_file.close();
}

void Board::print(std::ostream &stream){
    stream << "\n  │ ";
    for(char c = 'a'; c < 'a' + m_width; ++c){
        stream << c << " ";
    }
    stream << "\n──┴";
    for(int i = 0; i < 2 * m_width; ++i){
        stream << "─";
    }
    stream << '\n';
    for(int i = 0; i < m_height; ++i){
        char row_char = 'A' + i;
        stream << row_char << " │ ";
        for(int j = 0; j < m_width; ++j){
            stream << m_board[i][j].letter << " ";
        }
        stream << '\n';
    }
    stream << '\n';
}

bool Board::addWord(Word &word) {
    if(validateWord(word)){
        orientation line = word.getOrientation();
        std::string text = word.getText();
        std::pair<char, char> p = word.getPosition();

        for(int i = 0; i < text.length(); ++i){
            Tile *tile = &m_board[p.first + i * line][p.second + i * (1-line)];
            tile->letter = text[i];
            tile->placed[line] = true;
        }
        m_file << word;
        return true;
    }
    return false;
}

bool Board::validateWord(Word &word){
    orientation line = word.getOrientation();
    std::string text = word.getText();
    std::pair<char, char> p = word.getPosition();

    if(p.first + text.length() * line > m_height || p.second + text.length() * (1-line) > m_width){
        return false;
    }
    for(int i = 0; i < text.length(); ++i){
        Tile *tile = &m_board[p.first + i * line][p.second + i * (1-line)];
        bool overlap = tile->placed[line];
        if(overlap || (tile->letter != ' ' && tile->letter != text[i]) ){
            return false;
        }
    }
    return searchWord(text);
}

bool Board::searchWord(std::string &text){
    m_words_file.seekg(0);
    std::transform(text.begin(), text.end(), text.begin(), ::tolower);
    std::string buffer;
    while(getline(m_words_file, buffer, '\n') && !buffer.empty()){
        if(text == buffer){
            return true;
        }
    }
    return false;
}