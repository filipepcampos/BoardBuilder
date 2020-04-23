#include <iostream>
#include <cstdio>
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

    openWordsFile();
    m_words_file.seekg(0, std::ifstream::end);
    m_total_lines = m_words_file.tellg() / 50;
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
    remove( m_tmp_file_name);
}

void Board::openWordsFile() {
    std::ifstream file;
    file.open(m_words_file_name);

    mkstemp(m_tmp_file_name);
    std::ofstream new_file;
    new_file.open(m_tmp_file_name);

    std::string buffer;
    while(getline(file, buffer, '\n')){
        buffer.resize(49, ' ');
        transform(buffer.begin(), buffer.end(), buffer.begin(), ::toupper);
        new_file << buffer << '\n';
    }

    std::cout << m_tmp_file_name << "   " << m_tmp_file_name << std::endl;
    file.close();
    new_file.close();
    m_words_file.open(m_tmp_file_name);
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
    int v = (word.orientation == 'V') ? 1 : 0, h = 1 - v;
    int v_pos = word.vertical_char - 'A', h_pos = word.horizontal_char - 'a';
    std::transform(word.text.begin(), word.text.end(), word.text.begin(), ::toupper);

    if(validateWord(word)){
        for(int i = 0; i < word.text.length(); ++i){
            Tile *tile = &m_board[v_pos + i * v][h_pos + i * h];
            tile->letter = word.text[i];
            tile->placed[word.orientation] = true;
        }
        m_file << word.vertical_char << word.horizontal_char << " " << word.orientation << " " << word.text << std::endl;
        return true;
    }
    return false;
}

bool Board::validateWord(Word &word){
    int v = (word.orientation == 'V') ? 1 : 0, h = 1 - v;
    int v_pos = word.vertical_char - 'A', h_pos = word.horizontal_char - 'a';

    if(v_pos + word.text.length() * v > m_height || h_pos + word.text.length() * h > m_width){
        return false;
    }
    for(int i = 0; i < word.text.length(); ++i){
        Tile *tile = &m_board[v_pos + i * v][h_pos + i * h];
        bool overlap = tile->placed[word.orientation];
        if(overlap || (tile->letter != ' ' && tile->letter != word.text[i]) ){
            return false;
        }
    }
    return searchWord(word.text);
}

bool Board::searchWord(const std::string &word){
    long min = 0, max = m_total_lines;
    while(min <= max){
        long m = min + (max - min) / 2;
        std::string buffer;
        m_words_file.seekg(50 * m);
        m_words_file >> buffer;
        int compare_result = buffer.compare(word);
        if(compare_result == 0){
            return true;
        }
        if(compare_result < 0){
            min = m + 1;
        }
        if(compare_result > 0){
            max = m - 1;
        }
    }
    return false;
}