#ifndef UTILS_HPP
# define UTILS_HPP

/**
  * @brief delete the first element of the vector
  **/
template <typename T>
void pop_front(std::vector<T>& vec){
    std::reverse(vec.begin(),vec.end());
    vec.pop_back();
    std::reverse(vec.begin(),vec.end());
}

void						addElementsVector(std::vector<std::string> *list, std::vector<std::string> toAdd);
//void						pop_front(std::vector<T>& vec);
bool isLetter(const char c);
bool isDigit(const char c);
bool isHexDigit(const char c);
bool isHyphen(const char c);
bool isSpecial(const char c);
std::string toString(const int nb);
int toInt(std::string string);
bool checkUsername(std::string username);
bool checkNickname(std::string nickname);
unsigned						hasher(const char *s);
bool							checkChanstring(std::string name);
bool							checkChannelName(std::string name);
std::vector<std::string>		splitComma(std::string string);
std::vector<std::string>			splitCmd(std::string toSplit, std::string delimiter);
std::vector<std::string>		split(std::string string, std::string delimiter);

#endif
