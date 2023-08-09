#include <fstream>
#include <iostream>
#include <vector>
#include <set>

#define N 5

#define STOP_WORD "."

#define RESET  "\x1B[0m" 
#define RED    "\x1B[31m"
#define GREEN  "\x1B[32m"
#define YELLOW "\x1B[33m"
#define WHITE  "\x1B[37m"
#define BLACK  "\x1B[30m"

#define MAIN_TEXT_COLOR WHITE

int main(){
  
  // read words
  std::vector<std::string> words;
  std::ifstream file; file.open("./words.txt");
  std::string word;
  while(getline(file, word)) words.push_back(word);
  file.close();

  // choose answer
  srand(time(NULL));
  std::string answer = words[rand() % words.size()];
  std::multiset<char> answer_set(answer.begin(), answer.end());
  
  // words to set for optimal complexity
  std::set<std::string> words_set(words.begin(), words.end()); words.clear();

  std::set<std::string> inputs_set;
  std::vector<bool> wrong_letters(26, false);
  std::string input;
  std::vector<std::string> colors(N, RESET);
  size_t turn = 0;
  bool won = false;

  while(true){
    std::cin >> input;

    if(input == STOP_WORD) break;

    if(input.length() != 5)                        continue;
    if(words_set.find(input) == words_set.end())   continue;
    if(inputs_set.find(input) != inputs_set.end()) continue;
    else inputs_set.insert(input);
    
    // find correct letters 
    for(size_t i = 0; i < N; i++){
      if(input[i] != answer[i]) continue;
      colors[i] = GREEN;
      answer_set.extract(input[i]);
    }

    if(answer_set.empty()) won = true;    

    // find correct letters with incorrect positions
    for(size_t i = 0; i < N; i++){
      if(input[i] == answer[i]) continue;
      
      if(answer_set.count(input[i]) != 0){
	colors[i] = YELLOW;
	answer_set.extract(input[i]);
	continue;
      }
      
      colors[i] = MAIN_TEXT_COLOR;
      wrong_letters[input[i] - 'a'] = true;
    }

    answer_set = std::multiset<char>(answer.begin(), answer.end());
    
    std::cout << "      ";	
    for(size_t i = 0; i < N; i++) std::cout << colors[i] << input[i];
    std::cout << RESET << "      ";
    for(size_t i = 0; i < 26; i++)
      std::cout << (wrong_letters[i] ? RED : MAIN_TEXT_COLOR) << char(i + 'a');
    std::cout << RESET << std::endl;

    turn++;
    if(won || turn == 6) break;
  }

  if(won) std::cout << "You've won in " << turn << " guess" << (turn > 1 ? "es" : "") << ".\n";
  else std::cout << "You've lost. The correct answer was " << answer << ".\n";
}

