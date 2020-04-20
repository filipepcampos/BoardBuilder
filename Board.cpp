#include <cstring>
#include <iostream>
#include "Board.h"

Board::Board(int height, int width, std::string file_name)
{
    board = new char* [height];
    for(int i = 0; i < height; i++){
        board[i] = new char[width];
        memset(board[i], ' ', width*sizeof(char));
    }
    this->width = width;
    this->height = height;
    file.open(file_name);
    file << height << " x " << width << std::endl;
}

Board::~Board(){
    for(int i=0; i < height; i++){
        delete [] board[i];
    }
    delete [] board;
    if(file.is_open()){
        file.close();
    }
}

bool Board::addWord(Word &word) {
    int v_pos = word.vertical_char - 'A';
    int h_pos = word.horizontal_char - 'a';

    bool valid = word.orientation == 'H' ? addHorizontal(v_pos, h_pos, word.word) : addVertical(v_pos, h_pos, word.word);
    if(valid){
        file << word.vertical_char << word.horizontal_char << " " << word.orientation << " " << word.word << std::endl;
    }
    return valid;
}

void Board::save(){
    print(file);
    file.close();
}

void Board::print(std::ostream &stream){
    stream << "\n  ";
    for(char c = 'a'; c < 'a' + width; c++){
        stream << c << " ";
    }
    stream << '\n';
    for(int i = 0; i < height; i++){
        char row_char = 'A' + i;
        stream << row_char << " ";
        for(int j = 0; j < width; j++){
            stream << board[i][j] << " ";
        }
        stream << '\n';
    }
}

bool Board::addHorizontal(int v_pos, int h_pos, const std::string &word) {
    if(h_pos + word.length() > width){
        return false;
    }
    for(int i = 0; i < word.length(); i++){
        if(board[v_pos][h_pos + i] != ' ' && board[v_pos][h_pos + i] != word[i]){
            return false;
        }
    }
    for(int i = 0; i < word.length(); i++){
        board[v_pos][h_pos + i] = word[i];
    }
    return true;
}

bool Board::addVertical(int v_pos, int h_pos, const std::string &word) {
    if(v_pos + word.length() > height){
        return false;
    }
    for(int i = 0; i < word.length(); i++){
        if(board[v_pos + i][h_pos] != ' ' && board[v_pos + i][h_pos] != word[i]){
            return false;
        }
    }
    for(int i = 0; i < word.length(); i++){
        board[v_pos + i][h_pos] = word[i];
    }
    return true;
}
