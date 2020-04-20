#include <cstring>
#include <iostream>
#include "Board.h"

Board::Board(int height, int width, const std::string &file_name)
    : m_height(height), m_width(width)
{
    m_board = new char* [height];
    for(int i = 0; i < height; i++){
        m_board[i] = new char[width];
        memset(m_board[i], ' ', width*sizeof(char));
    }
    m_file.open(file_name);
    m_file << height << " x " << width << std::endl;
}

Board::~Board(){
    for(int i=0; i < m_height; i++){
        delete [] m_board[i];
    }
    delete [] m_board;
    if(m_file.is_open()){
        m_file.close();
    }
}

bool Board::addWord(Word &word) {
    int v_pos = word.vertical_char - 'A';
    int h_pos = word.horizontal_char - 'a';

    bool valid = word.orientation == 'H' ? addHorizontal(v_pos, h_pos, word.word) : addVertical(v_pos, h_pos, word.word);
    if(valid){
        m_file << word.vertical_char << word.horizontal_char << " " << word.orientation << " " << word.word << std::endl;
    }
    return valid;
}

void Board::save(){
    print(m_file);
    m_file.close();
}

void Board::print(std::ostream &stream){
    stream << "\n  ";
    for(char c = 'a'; c < 'a' + m_width; c++){
        stream << c << " ";
    }
    stream << '\n';
    for(int i = 0; i < m_height; i++){
        char row_char = 'A' + i;
        stream << row_char << " ";
        for(int j = 0; j < m_width; j++){
            stream << m_board[i][j] << " ";
        }
        stream << '\n';
    }
}

bool Board::addHorizontal(int v_pos, int h_pos, const std::string &word) {
    if(h_pos + word.length() > m_width){
        return false;
    }
    for(int i = 0; i < word.length(); i++){
        if(m_board[v_pos][h_pos + i] != ' ' && m_board[v_pos][h_pos + i] != word[i]){
            return false;
        }
    }
    for(int i = 0; i < word.length(); i++){
        m_board[v_pos][h_pos + i] = word[i];
    }
    return true;
}

bool Board::addVertical(int v_pos, int h_pos, const std::string &word) {
    if(v_pos + word.length() > m_height){
        return false;
    }
    for(int i = 0; i < word.length(); i++){
        if(m_board[v_pos + i][h_pos] != ' ' && m_board[v_pos + i][h_pos] != word[i]){
            return false;
        }
    }
    for(int i = 0; i < word.length(); i++){
        m_board[v_pos + i][h_pos] = word[i];
    }
    return true;
}
