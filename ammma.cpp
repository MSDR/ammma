//TODO: custom seeds
//TODO: custom dictionaries
//TODO: custom word lengths
//TODO: help page

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <fstream>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <stdlib.h>
#include <string>
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define CURSORUP   "\033[A"
#define ERASELINE  "\33[2K"

#define FG_RESET   "\033[0m"
#define FG_RED     "\033[31m"
#define FG_GREEN   "\033[32m"
#define FG_YELLOW  "\033[33m"
#define FG_BOLD_WHITE  "\033[1m\033[37m"

#define BG_RESET   "\033[40m"
#define BG_GRAY    "\033[100m"
#define BG_GREEN   "\033[42m"
#define BG_YELLOW  "\033[43m"
#define BG_BOLD_RED    "\033[1m\033[41m"

#define IN_EQ_ANS (in=="answer" || in == "ans" || in == (std::string)"a")

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//reads each line from the file at dictname into dict as elements
//throws std::runtime_error if the file at dictname doesn't exist
void readDict(std::vector<std::string>* dict, std::string dictname) {
    //open file
    std::fstream f;
    f.open(dictname, std::ios::in);

    //check for non-existent file
    if(!f.is_open()) {
        throw std::runtime_error(dictname);
    }

    //read file into internal memory
    std::string word;
    while(getline(f, word)) {
        word.pop_back(); //remove the \n character on each line
        if(word.size() == 5) {
            dict->push_back(word);
        }
    }
 
    //close file
    f.close();
}

//reads a line from cin, throwing a runtime error to quit the program if asked
void readWord(std::string* word) {
    printf("%s>> %s", FG_GREEN, FG_RESET);

    std::string in;
    std::cin >> in;
    std::transform(in.begin(), in.end(), in.begin(), ::tolower); //convert input to lowercase

    //quit ammma
    if(in == "q" || in == "quit") {
        throw std::runtime_error("quit");
    }

    *word = in;
}

//check a word is a valid 5 letter word with validWord
bool allAlpha(const std::string &w) {for(char c : w) {if(!isalpha(c)) return false;} return true;}
bool validWord(const std::string &word, const std::vector<std::string> &dict) {
    return word.size() == 5 && 
           allAlpha(word) && 
           std::find(dict.begin(), dict.end(), word) != dict.end();
}

//prints a guess to the wordle format
//if printingAnswer, prints the answer
void printFormattedGuess(const std::string &guess, const std::string &answer, bool printingAnswer = false) {
    printf("   ");

    //printing answer (in all red)
    if(printingAnswer) {
        for(size_t i = 0; i < answer.size(); ++i) {
            char c = answer[i];
            printf("%s %c ", BG_BOLD_RED, c-32);
        }
        printf("%s\n", BG_RESET);
        return;
    }

    //printing wordle
    for(size_t i = 0; i < guess.size(); ++i) {
        char c = guess[i];

        if(c == answer[i]) {
            printf("%s %c ", BG_GREEN , c-32);
        } else if ((int)answer.find(c) != -1) {
            printf("%s %c ", BG_YELLOW , c-32);
        } else {
            printf("%s %c ", BG_GRAY , c-32);
        }
    }

    printf("%s\n", BG_RESET);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {
    srand(time(0));
    std::vector<std::string> dict = std::vector<std::string>();
    std::string dictname;
    if(argc == 1) {
        dictname = "dict5.txt";
    } 

    //read file at dictname or throw error and exit if file is nonexistent
    try {
        readDict(&dict, dictname);
    } catch (const std::runtime_error& err) {
        printf("%sERROR >> Dictionary not found. Check that %s is in the \\ammma folder.\n%s", FG_RED, err.what(), FG_RESET);
        return 1;
    };

    printf("AMMMA found %s%d%s words in the dictionary!\n", FG_GREEN, (int)dict.size(), FG_RESET);

    //core loop, each iteration has a new Wordle. 
    //loop breaks when entering a quit command throws a runtime_error
    bool gameLoop = true;
    while(gameLoop) {
        try {
            std::string in = "";
            std::string answer = dict[rand() % dict.size()-1];

            std::cout << "\n-----------------------------------------------\nStart your game by typing any five letter word:\n\n";

            readWord(&in);
            while(in != answer) {
                //if commanded, display the answer and end the current Wordle
                if(IN_EQ_ANS) {
                    printf("%s%s%s", CURSORUP, ERASELINE, FG_BOLD_WHITE);
                    printFormattedGuess(in, answer, true);
                    break;
                }

                //if entered word is invalid, request a new one
                while(!validWord(in, dict)) {
                    printf("\033[A\33[2K\r"); //delete invalid entered word

                    printf("%s>>%s %s %sis not a valid word.\n%s", FG_RED, FG_RESET, in.c_str(), FG_RED, FG_RESET);
                    readWord(&in);

                    if(IN_EQ_ANS) break;
                    
                    printf("%s%s%s%s\r", ERASELINE, CURSORUP, ERASELINE, ERASELINE); //delete error message and new entered word
                }

                //display the guess, or the answer if commanded
                printf("%s%s%s", CURSORUP, ERASELINE, FG_BOLD_WHITE);
                printFormattedGuess(in, answer, IN_EQ_ANS);

                readWord(&in);
            }
            if(in == answer) {
                printf("%s%s%s", CURSORUP, ERASELINE, FG_BOLD_WHITE);
                printFormattedGuess(in, answer);
            }

        } catch (const std::runtime_error& err) {break;};
    }

    return 0;
}