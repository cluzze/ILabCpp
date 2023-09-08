#include <gtest/gtest.h>

#include "tree.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include <regex>
#include <random>
#include <iostream>
#include <fstream>

std::vector<std::pair<char, int>> parseInput(std::string& input) {

    std::vector<std::pair<char, int>> requests;

    std::regex pattern(R"(([kmn]\s-?\d+)\s*)");
    std::sregex_iterator iterator(input.begin(), input.end(), pattern);
    std::sregex_iterator end;

    while (iterator != end) {
        std::smatch match = *iterator;
        std::string matchStr = match.str();

        char type = matchStr[0];
        int value = std::stoi(matchStr.substr(2));

        requests.push_back({type, value});

        ++iterator;
    }
    
    return requests;
}

std::vector<int> genNumbers(int size, int n) {
    std::vector<int> numbers(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-n, n);

    for (int i = 0; i < size; i++) {
        numbers[i] = dis(gen);
        for (int j = 0; j < i; j++) {
            if (numbers[i] == numbers[j]) {
                i--;
                break;
            }
        }
    }

    return numbers;
}

int ERROR = -123132131;

int nth(const std::vector<int>& numbers, int n) {
    if (n < 1 || n > numbers.size())
        return ERROR;

    std::vector<int> sortedNumbers = numbers;
    std::nth_element(sortedNumbers.begin(), sortedNumbers.begin() + n - 1, sortedNumbers.end());
    return sortedNumbers[n - 1];
}

int cnt(const std::vector<int>& numbers, int n) {
    return std::count_if(numbers.begin(), numbers.end(), [&](int num) { return num < n; });
}

using namespace containers;

int size = 1000, values = 10000;
std::vector<int> numbers = genNumbers(size, values);

int main() {
    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

TEST(trees, cnt) {
    SearchTree<int> tree;
    
    for (auto i : numbers)
        tree.insert(i);

    for (int i = -values; i <= values; i++) {
        int res = tree.cnt(i);
        int exp = cnt(numbers, i);
        ASSERT_EQ(res, exp);
    } 
}

TEST(trees, nth) {
    SearchTree<int> tree;
    
    for (auto i : numbers)
        tree.insert(i);

    for (int i = 1; i < size; i++) {
        SearchTree<int>::iterator it = tree.nth_element(i);
        int res = it->key;
        int exp = nth(numbers, i);
        ASSERT_EQ(res, exp);
    } 
}

void solve(std::string& input) {
    SearchTree<int> tree;
    std::vector<int> v;

    std::vector<std::pair<char, int>> requests = parseInput(input);
    for (int i = 0; i < requests.size(); i++) {
        char type = requests[i].first;
        int value = requests[i].second;
        SearchTree<int>::iterator it;
        int res;
        int exp;
        switch (type) {
        case 'k':
            tree.insert(value);
            v.push_back(value);
            break;
        case 'm':
            it = tree.nth_element(value);
            res = it->key;
            exp = nth(v, value);
            if (it == tree.end())
                ASSERT_TRUE(exp == ERROR);
            else
                ASSERT_EQ(exp, res);
            break;
        case 'n':
            res = tree.cnt(value);
            exp = cnt(v, value);
            ASSERT_EQ(res, exp);
            break;
        default:
            break;
        }
    }
}

TEST(trees, e2e_hand_written) {
    std::string input = "k 8 k 2 k -1 m 1 m 2 n 3";
    solve(input);
}

std::string genInput(int n) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(-values, values);

    std::string result;
    for (int i = 0; i < n; ++i) {
        char letter;
        int number = dis(gen);
        switch (i % 3) {
            case 0:
                letter = 'k';
                break;
            case 1:
                letter = 'n';
                break;
            case 2:
                letter = 'm';
                break;
        }
        result += letter + " " + std::to_string(number) + " ";
    }
    // std::ofstream file;
    // file.open("../tests/test1.txt");
    // file << result;
    return result;
}

TEST(trees, e2e_gen) {
    std::string input = genInput(10);
    solve(input);
}