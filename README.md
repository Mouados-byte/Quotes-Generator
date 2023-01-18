# Markov Chain Quotes Generator
This project is a markov chain based quotes generator. It reads a text file that contains quotes and builds a markov chain using the words in those quotes. The markov chain is then used to generate new quotes.

# Dependencies

C++11 or higher
C++ Standard Template Library (STL)

# How to use

Clone the repository to your local machine.
Compile the program using your preferred C++ compiler.
Run the program, providing the name of the text file containing the quotes as a command line argument.
The program will output a generated quote.

# How it works

The program reads in the text file containing the quotes, and tokenizes them using regular expressions to split them into individual words.
These words are then used to build a markov chain, where each word is a key in a hashmap, and the value is another hashmap representing the next word and its probability of occuring after the current word.
To generate a quote, the program starts with a randomly chosen word from the markov chain and generates the next word based on the probabilities stored in the markov chain.
The process is repeated until the generated quote reaches the specified length.

# Customizing the generated quotes

You can customize the generated quotes by modifying the parameters passed to the **generateQuote()** function. The function takes in 3 parameters:

- A string representing the starting word of the quote.
- An unordered map representing the markov chain.
- An int representing the maximum number of words in the generated quote.


# Saving and loading the markov chain

You can save the markov chain to a file by calling the **outputTokensInFile()** function and providing the desired output file and the markov chain as parameters.

You can then load the markov chain from a file using the **reverseMapFromFile()** function, which takes an input file stream and returns the markov chain as an unordered map.

# Limitations

* The program currently only supports quotes in the format of "quote" and does not handle quotes within quotes or other forms of punctuation.
* The program also assumes that all quotes in the input text file are separated by a newline character.

# Future improvements

* Add support for quotes in different formats and forms of punctuation
* Implement a GUI for the program
* Add the ability to input multiple text files and build a larger markov chain
* Implement a way to specify the type of quote or source of the quotes to tailor the output.
