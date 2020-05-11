#include <iostream>
#include <algorithm>
#include <utility>
#include "Board.h"

Board::Board(short height, short width, std::string file_name)
    : m_height(height), m_width(width), m_file_name(std::move(file_name)), m_words_file(m_words_file_name)
{
    m_board = new Tile* [height];
    for(int i = 0; i < height; ++i){
        m_board[i] = new Tile[width];
    }
}

Board::~Board(){
    for(int i=0; i < m_height; ++i){
        delete [] m_board[i];
    }
    delete [] m_board;
}

bool Board::save(){
    int count = 0;
    for(int i = 0; i < m_height; i++){
        for(int j = 0; j < m_width; j++){
            if(m_board[i][j].letter != ' '){
                count++;
            }
        }
    }
    if(count >= 14){
        std::ofstream file;
        file.open(m_file_name);
        file << m_height << " x " << m_width << std::endl;
        file << m_input_words.rdbuf();
        print(file);
        file.close();
        m_words_file.close();
        return true;
    }
    return false;
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
            tile->reserved[line] = true;
            reserveAdjacent(pos, i, line);
        }
        m_input_words << word;
        return true;
    }
    return false;
}

void Board::reserveAdjacent(const std::pair<short, short> &pos, int n, orientation line) {
    std::pair<short, short> positions = pos;
    for(int i = -1; i<=1; i+=2){
        if(line == H){
            positions.first = pos.first + i;
            if(positions.first < 0 || positions.first >= m_height){
                continue;
            }
        }
        else{
            positions.second = pos.second + i;
            if(positions.second < 0 || positions.second >= m_width){
                continue;
            }
        }
        getPosition(positions, n, line)->reserved[line] = true;
    }
}

bool Board::validateWord(const Word &word){
    orientation line = word.getOrientation();
    int line_int = line == V ? 1 : 0;
    std::string text = word.getText();
    std::pair<char, char> pos = word.getPosition();
    std::pair<char, char> end_pos{pos.first + text.length() * line_int, pos.second + text.length() * (1-line_int)};

    if(end_pos.first >= m_height || end_pos.second >= m_width){
        return false;
    }

    // Check behind first later and beyond last letter, if not empty the word isn't valid
    Tile *tile = getPosition(pos, -1, line);
    if(tile){
        if(tile->letter != ' ') {return false;}
    }
    tile = getPosition(end_pos, 1, line);
    if(tile){
        if(tile->letter != ' '){ return false;}
    }

    // Validate each tile the word will occupy
    for(int i = 0; i < text.length(); ++i){
        tile = getPosition(pos, i, line);
        if(tile->letter != ' ' && tile->letter != text[i]){
            return false;
        }
        if((tile->reserved[line] && tile->letter != text[i])){ // A letter can be placed in reserved spot
                                                                // if it belongs to an already existing word
            return false;
        }
    }
    return searchWord(text);
}

Tile* Board::getPosition(const std::pair<short, short> &pos, int n, orientation line) const{
    short line_int = line == V ? 1 : 0;
    int v = pos.first + n * line_int;
    int h = pos.second + n * (1-line_int);
    if(v < 0 || v >= m_height || h < 0 || h >= m_width){
        return nullptr;
    }
    return &m_board[v][h];
}

bool Board::searchWord(std::string &text){
    m_words_file.seekg(0);
    std::transform(text.begin(), text.end(), text.begin(), ::tolower);

    std::string buffers[2]; // Two buffer strings are used to keep the last word read, allowing to suggest word
                            // that's immediately before and after text
    int i = 0;

    while(getline(m_words_file, buffers[i]) && !buffers[i].empty()){
        int compare_val = text.compare(buffers[i]);
        if(compare_val == 0){
            return true;
        }
        if(compare_val < 0){
            std::cout << "Suggestion: " << BLUE << buffers[0] << RESET << " or " << BLUE << buffers[1] << RESET << std::endl;
            return false;
        }
        i = ++i % 2;
    }
    m_words_file.clear(); // If code reached this point then EOF most likely has occurred.
    std::cout << "Suggestion: " << BLUE << buffers[++i % 2] << RESET << std::endl;
    return false;
}