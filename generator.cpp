#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <regex>
#include <ctime>
#include <chrono>
#include <initializer_list>

std::unordered_map<std::string, std::unordered_map<std::string, float>> tokenize(std::istream &file)
{
    std::unordered_map<std::string, std::unordered_map<std::string, float>> markovChain;
    std::string line;
    std::vector<std::string> tokens;
    long int count = 0;
    while (std::getline(file, line))
    {
        int quoteStart = line.find("“");
        int quoteEnd1 = line.find("“,");
        int quoteEnd2 = line.find("“\"");
        int quoteEnd = quoteEnd1 - quoteEnd2 >= 0 ? quoteEnd2 : quoteEnd1;
        std::string quote = line.substr(quoteStart, quoteEnd - quoteStart + 3);

        std::regex word_regex("[^\\w]+");
        std::copy(std::sregex_token_iterator(quote.begin(), quote.end(), word_regex, -1), std::sregex_token_iterator(), std::back_inserter(tokens));
        count = tokens.size();
    }

    for (size_t i = 0; i < count - 1; i++)
    {
        // if exist in the markov , just add to the unordered map , if exist in the unordered map , add the pourcentage
        std::unordered_map<std::string, float> nextToken;

        if (markovChain.count(tokens[i]) > 0)
        {
            // update the value of nextToken[tokens[i]]
            nextToken = markovChain[tokens[i]];

            if (nextToken.count(tokens[i + 1]) > 0)
            {
                // update the probability of the next token
                nextToken[tokens[i + 1]] += 1;
            }
            else
            {
                // add probability to the next token
                nextToken[tokens[i + 1]] = 1;
            }
        }
        else
        {
            // add probability to the next token
            nextToken[tokens[i + 1]] = 1;
        }
        markovChain[tokens[i]] = nextToken;
    }
    // Compute relative frequencies
    for (auto &outerPair : markovChain)
    {
        float total = 0;
        for (auto &innerPair : outerPair.second)
        {
            total += innerPair.second;
        }
        for (auto &innerPair : outerPair.second)
        {
            innerPair.second /= total;
        }
    }

    return markovChain;
}

std::unordered_map<std::string, std::unordered_map<std::string, float>> reverseMapFromFile(std::istream &file)
{
    std::unordered_map<std::string, std::unordered_map<std::string, float>> originalMap;
    std::string line;
    while (std::getline(file, line))
    {
        std::string key, nextKey;
        float value;
        std::stringstream ss(line);
        std::getline(ss, key, ':');
        while (ss >> nextKey >> value)
        {
            originalMap[key][nextKey] = value;
            ss.ignore(1);
        }
    }
    return originalMap;
}

void outputTokensInFile(std::ostream &file, std::unordered_map<std::string, std::unordered_map<std::string, float>> &markovChain)
{
    for (auto outerIter = markovChain.begin(); outerIter != markovChain.end(); ++outerIter)
    {
        file << outerIter->first << ": ";
        for (auto innerIter = outerIter->second.begin(); innerIter != outerIter->second.end(); ++innerIter)
        {
            file << innerIter->first << " " << innerIter->second << ", ";
        }
        file << "\n";
    }
}

std::string generateQuote(const std::string &startWord, const std::unordered_map<std::string, std::unordered_map<std::string, float>> &markovChain, int maxWords)
{
    if (markovChain.count(startWord) > 0)
    {
        // code for generating the next word
        std::string currentWord = startWord;
        std::string quote = startWord;
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen(seed);
        int wordCount = 0;
        while (wordCount < maxWords)
        {
            std::vector<std::string> nextWords;
            for (auto iter = markovChain.at(currentWord).begin(); iter != markovChain.at(currentWord).end(); ++iter)
            {
                nextWords.push_back(iter->first);
            }
            std::uniform_int_distribution<int> dist(0, nextWords.size() - 1);
            int randIndex = dist(gen);
            currentWord = nextWords[randIndex];
            quote += " " + currentWord;
            wordCount++;
        }
        return quote;
    }
    else
    {
        std::cout << "This word is not found" << std::endl;
        return startWord;
    }
}

int main()
{
    std::ifstream file("datasets/death_quotes.csv");
    std::ofstream ofileready("models/death.txt");
    std::ifstream ifileready("models/death.txt");
    std::string input;
    int lines;
    std::unordered_map<std::string, std::unordered_map<std::string, float>> markovChain;
    if (file.is_open())
    {
        // read new datasets
        markovChain = tokenize(file);
        // put them into a file in models
        outputTokensInFile(ofileready, markovChain);
        // or read ready data from file
        //  markovChain = reverseMapFromFile(ifileready);

        std::cout << "Input the word you want to start with :" << std::endl;
        std::cin >> input;
        std::cout << "Input the number of words you want to be generated :" << std::endl;
        std::cin >> lines;
        std::cout
            << generateQuote(input, markovChain, lines) << std::endl;
        file.close();
        ofileready.close();
        ifileready.close();
    }
    return 0;
}