#include <random>
#include <iostream>
#include <fstream>
#include <vector>
#include <exception>
#include <string>
#include <algorithm>

std::vector<std::string> inputFilenames {
        "../input/test1.dat",
        "../input/test2.dat",
        "../input/test3.dat",
        "../input/test4.dat",
        "../input/test5.dat",
        "../input/test6.dat",
        "../input/test7.dat",
        "../input/test8.dat",
        "../input/test9.dat",
        "../input/test10.dat"
};

std::vector<std::string> answersFilenames {
        "../answers/test1_ans.dat",
        "../answers/test2_ans.dat",
        "../answers/test3_ans.dat",
        "../answers/test4_ans.dat",
        "../answers/test5_ans.dat",
        "../answers/test6_ans.dat",
        "../answers/test7_ans.dat",
        "../answers/test8_ans.dat",
        "../answers/test9_ans.dat",
        "../answers/test10_ans.dat"
};

std::vector<int> testsSizes {
    10, 20, 50, 100, 300, 500, 1000, 5000, 10000, 100000
};

std::vector<std::pair<char, int>> genTest(int testSize, int keyRange) {
    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<int> dist(-keyRange * 2, keyRange * 2);
    std::uniform_int_distribution<int> query(0, 2);
    
    std::vector<int> v(2 * keyRange + 1);
    int k = -keyRange;
    std::generate(v.begin(), v.end(), [&k](){ return k++; });
    std::shuffle(v.begin(), v.end(), g);

    int ks = 0;
    std::vector<std::pair<char, int>> queries;

    for (int i = 0; i < 3; i++) {
        queries.push_back({'k', v[ks]});
        ks++;
    }

    for (int i = 0; i < testSize; i++) {
        int q = query(g);
        switch (q) {
            case 0: {
                queries.push_back({'k', v[ks]});
                ks++;
                break;
            }
            case 1: {
                std::uniform_int_distribution<int> m_dist(1, ks);
                queries.push_back({'m', m_dist(g)});
                break;
            }
            case 2: {
                queries.push_back({'n', dist(g)});
                break;
            }
            default:
                throw std::runtime_error("invalid distribution");
        }
    }
    return queries;   
}

void writeTestToFile(std::ofstream& file, const std::vector<std::pair<char, int>>& queries) {
    for (int i = 0; i < queries.size(); i++)
        file << queries[i].first << ' ' << queries[i].second << ' ';
}

void writeAnswerToFile(std::ofstream& file, const std::vector<int>& answers) {
    for (int i = 0; i < answers.size(); i++)
        file << answers[i] << ' ';
}

int nth(const std::vector<int>& numbers, int n) {
    std::vector<int> sortedNumbers = numbers;
    std::nth_element(sortedNumbers.begin(), sortedNumbers.begin() + n - 1, sortedNumbers.end());
    return sortedNumbers[n - 1];
}

int cnt(const std::vector<int>& numbers, int n) {
    return std::count_if(numbers.begin(), numbers.end(), [&](int num) { return num < n; });
}

std::vector<int> getExpectedResult(const std::vector<std::pair<char, int>>& queries) {
    std::vector<int> cur, expectedResult;
    for (int i = 0; i < queries.size(); i++) {
        switch (queries[i].first)
        {
        case 'k':
            cur.push_back(queries[i].second);
            break;
        case 'm': {
            int res = nth(cur, queries[i].second);
            expectedResult.push_back(res);
            break;
        }
        case 'n': {
            int res = cnt(cur, queries[i].second);
            expectedResult.push_back(res);
            break;
        }
        default: throw std::runtime_error("unexpected format");
        }
    }
    return expectedResult;
}

int main() {
    std::ofstream inputFile, answerFile;
    int nTests = 10;
    for (int i = 0; i < nTests; i++) {
        inputFile.open(inputFilenames[i]);
        answerFile.open(answersFilenames[i], std::ios::app);
        auto test = genTest(testsSizes[i], testsSizes[i]);
        writeTestToFile(inputFile, test);
        auto answer = getExpectedResult(test);
        writeAnswerToFile(answerFile, answer);
        inputFile.close();
        answerFile.close();
    }
}