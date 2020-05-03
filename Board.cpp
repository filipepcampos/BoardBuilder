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
        std::pair<char, char> pos = word.getPosition();
        for(int i = 0; i < text.length(); ++i){
            Tile *tile = getPosition(pos, i, line);
            tile->letter = text[i];
            tile->placed[line] = true;
            placeAdjacent(pos, i, line);
        }
        m_file << word;
        return true;
    }
    return false;
}

void Board::placeAdjacent(const std::pair<char, char> &pos, int n, orientation line) {
    std::pair<char, char> positions = pos;
    for(int i = -1; i<=1; i+=2){
        if(line == H){
            positions.first = pos.first + i;
            if(positions.first < 0 || positions.first >= m_width){
                continue;
            }
        }
        else{
            positions.second = pos.second + i;
            if(positions.second < 0 || positions.second >= m_height){
                continue;
            }
        }
        getPosition(positions, n, line)->placed[line] = true;
    }
}

bool Board::validateWord(const Word &word){
    orientation line = word.getOrientation();
    std::string text = word.getText();
    std::pair<char, char> pos = word.getPosition();

    if(pos.first + text.length() * line > m_height || pos.second + text.length() * (1-line) > m_width){
        return false;
    }
    for(int i = 0; i < text.length(); ++i){
        Tile *tile = getPosition(pos, i, line);
        orientation opposite_line = line == V ? H : V;
        if((tile->placed[line] && !tile->placed[opposite_line]) || (tile->letter != ' ' && tile->letter != text[i]) ){
            return false;
        }
    }
    return searchWord(text);
}

Tile* Board::getPosition(const std::pair<char, char> &pos, int n, orientation line) const{
    short line_int = line == V ? 1 : 0;
    return &m_board[pos.first + n * line_int][pos.second + n * (1-line_int)];
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