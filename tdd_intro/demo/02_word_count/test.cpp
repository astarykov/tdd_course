/*
Given a phrase, count the occurrences of each word in that phrase. Ignore whitespaces and punctual symbols
For example for the input "olly olly in come free please please let it be in such manner olly"
olly: 3
in: 2
come: 1
free: 1
please: 2
let: 1
it: 1
be: 1
manner: 1
such: 1
*/

#include <gtest/gtest.h>
#include <string>
#include <map>

/*
    Tets cases:
    1. " " = {}
    2. "hi" = {{"hi", 1}}
    3. "hi no " = {{"hi", 1}, {"no", 1}}
    4. "hi hi hi" = {{"hi", 3}}
    5. "one one one two two" = {{"one", 3}, {"two", 2}}
    6. "So, it's up to you and it's up to me" = {{"So", 1}, {"it's", 2}, {"up", 2}, {"to", 2}, {"you", 1}, {"and", 1}, {"me", 1}}
*/



std::map<std::string, int> getWordsCount(std::string line) {
    std::map<std::string, int> result = {};

    for(auto sign : {'.', ',', '?', '!', ';', ':'})
    {
        while(line.find(sign) != std::string::npos) {
          line.replace(line.find(sign), 1, " ");
        }
    }

    if (!line.empty() && line != " ") {
        std::istringstream iss(line);
        std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
                                         std::istream_iterator<std::string>());
        for (auto & element : results)
        {
            if (result.count(element)) {
                result[element]++;
            } else {
                result.insert({element, 1});
            }
        }
    }
    return result;
}


TEST(WordCount, emptyStringReturnsEmptyDict) {
    std::map <std::string, int> res = {};
    ASSERT_EQ(getWordsCount(" "), res);
}

TEST(WordCount, hiStringReturnsOneWordDict) {
    std::map <std::string, int> res = {{"hi", 1}};
    ASSERT_EQ(getWordsCount("hi"), res);
}

TEST(WordCount, 2WordsInStringReturnsTwoWordDict) {
    std::map <std::string, int> res = {{"hi", 1}, {"no", 1}};
    ASSERT_EQ(getWordsCount("hi no "), res);
}

TEST(WordCount, 3SameWordsInStringReturnsOneWordDict) {
    std::map <std::string, int> res = {{"hi", 3}};
    ASSERT_EQ(getWordsCount("hi hi hi"), res);
}

TEST(WordCount, LonglineWordsInStringReturnsSevenWordDict) {
    std::map <std::string, int> res = {{"So", 1}, {"it's", 2}, {"up", 2}, {"to", 2}, {"you", 1}, {"and", 1}, {"me", 1}};
    ASSERT_EQ(getWordsCount("So, it's up to you and it's up to me"), res);
}

TEST(WordCount, 3and2SameWordsInStringReturnsTwoWordDict) {
    std::map <std::string, int> res = {{"one", 3}, {"two", 2}};
    ASSERT_EQ(getWordsCount("one one one two two"), res);
}

TEST(WordCount, 3and2SameWordsWithAllPunctualSymbolsInStringReturnsTwoWordDict) {
    std::map <std::string, int> res = {{"one", 3}, {"two", 2}};
    ASSERT_EQ(getWordsCount("one? !one, one: ;two , two ?"), res);
}
