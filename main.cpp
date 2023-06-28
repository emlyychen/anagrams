//
//  main.cpp
//  anagrams-game
//
//  Created by Emily Chen on 4/30/23.
//

#include <iostream>
#include <unordered_set>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>
#include <random>

using namespace std;

unordered_set<string> all_words;
vector<string> six_letter_words;

struct wordLess{
    bool operator()(string a, string b){
        if(a.length() < b.length())
            return true;
        if(b.length() < a.length())
            return false;
        //equal length
        return a < b;
    }
};

class Anagrams{
public:
    int score = 0;
    string play_word;
    void play(){
        play_word = choose_word();
        generate_valid_words(play_word, 0);
        string attempt;
        cout << "You have 60 seconds to create as many words as you can with the given letters.\n";
        cout << "Time starts NOW!!\n";
        cout << "Letters: ";
        for(char c : play_word){
            cout << c << " ";
        }
        cout << "\n";
        time_t startTimeSeconds = time(NULL);
        while(time(NULL) - startTimeSeconds < 60){
            cin >> attempt;
            //word invalid
            if (valid_words.find(attempt) == valid_words.end()){
                cout << attempt << " is not a valid word!";
                cout << right << setw(19 - (int)attempt.size()) << "Score: " << score;
                if(time(NULL) - startTimeSeconds >= 10)
                    cout << ", time used: 0:" << time(NULL) - startTimeSeconds << "\n";
                else
                    cout << ", time used: 0:0" << time(NULL) - startTimeSeconds << "\n";
                continue;
            }
            //word already used
            if(used.find(attempt) != used.end()){
                cout << attempt << " has already been used!";
                cout << right << setw(17 - (int)attempt.size()) << "Score: " << score;
                if(time(NULL) - startTimeSeconds >= 10)
                    cout << ", time used: 0:" << time(NULL) - startTimeSeconds << "\n";
                else
                    cout << ", time used: 0:0" << time(NULL) - startTimeSeconds << "\n";
                continue;
            }
            used.insert(attempt);
            //add points
            switch(attempt.length()){
                case 3:
                    score+=100;
                    break;
                case 4:
                    score+=400;
                    break;
                case 5:
                    score+=1200;
                    break;
                case 6:
                    score+=2000;
                    break;
            }
            cout << left << setfill('.') << setw(20) << attempt;
            cout << right << setfill('.') << setw(20) << "Score: " << score;
            if(time(NULL) - startTimeSeconds > 10)
                cout << ", time used: 0:" << time(NULL) - startTimeSeconds << "\n";
            else
                cout << ", time used: 0:0" << time(NULL) - startTimeSeconds << "\n";
        }
        cout << "\n";
        cout << "You scored a total of " << score << " points!\n";
        cout << "\n";
        cout << "Longest words:\n";
        for (auto word : longest_words) {
            cout << word << " ";
        }
        cout << "\n";
        cout << "See all valid words? [yes/no]\n";
        cin >> attempt;
        if(attempt == "yes"){
            int value = 0;
            wordLess w;
            vector<string> valid(valid_words.begin(), valid_words.end());
            sort(valid.begin(), valid.end(), w);
            for(string word : valid){
                switch(word.length()){
                    case 3:
                        value = 100;
                        break;
                    case 4:
                        value = 400;
                        break;
                    case 5:
                        value = 1200;
                        break;
                    case 6:
                        value = 2000;
                        break;
                }
                cout << left << setfill('.') << setw(20) << word;
                cout << right << setfill('.') << setw(20) << value << "\n";
            }
        }
    }
    
private:
    unordered_set<string> valid_words;
    unordered_set<string> used;
    string unscrambled;
    unordered_set<string> longest_words;
    string choose_word(){
        srand((unsigned)time(NULL));
        unsigned random = (unsigned)rand() % six_letter_words.size();
        unscrambled = six_letter_words[random];
        longest_words.insert(unscrambled);
        string chosen = unscrambled;
        uint32_t seed = static_cast <uint32_t> (chrono::system_clock::now().time_since_epoch().count());
        shuffle(chosen.begin(),chosen.end(), std::default_random_engine(seed));
        return chosen;
    }
    void generate_valid_words(string word, size_t permLength){
        string add = word.substr(0, permLength);
        if (all_words.find(add) != all_words.end() && permLength > 2) {
            valid_words.insert(add);
            if(permLength == 6){
                longest_words.insert(add);
                return;
            }
        }  // if ..valid word found
        for (size_t i = permLength; i < word.size(); ++i) {
            swap(word[permLength], word[i]);
            generate_valid_words(word, permLength + 1);
            swap(word[permLength], word[i]);
        }  // for ..unpermuted elements
    }
};

void initialize_dictionary(){
    ifstream infile;
    infile.open("words_alpha.txt");
    assert(infile.is_open());
    string word;
    while(infile >> word){
        if(word.length() > 2)
            all_words.insert(word);
        if(word.length() == 6){
            six_letter_words.push_back(word);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cout << "Loading dictionary...\n";
    initialize_dictionary();
    cout << "Play Anagrams?\n[PLAY][QUIT][HELP]\n";
    string cmd;
    do{
        Anagrams a;
        cin >> cmd;
        switch(cmd[0]){
            case 'P':
                a.play();
                cout << "To play again: [PLAY]\n";
                cout << "To quit: [QUIT]\n";
                cout << "For more help: [HELP]\n";
                break;
            case 'Q':
                cout << "Thanks for playing Anagrams!\n";
                break;
            case 'H':
                cout << "Type as many words as you can with the 6 letters given!\n";
                cout << "3 letters: 100 points\n";
                cout << "4 letters: 400 points\n";
                cout << "5 letters: 1200 points\n";
                cout << "6 letters: 2000 points\n";
                cout << "To play: [PLAY]\n";
                cout << "To quit: [QUIT]\n";
                break;
            default:
                cout << "Invalid command\n";
                cout << "To play: [PLAY]\n";
                cout << "To quit: [QUIT]\n";
                cout << "For help: [HELP]\n";
                break;
        }
    }
    while(cmd != "QUIT");
}
