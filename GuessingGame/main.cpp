/***********************
 * GuessingGame
 * 1. Generates a random number
 * 2. Asks the user to enter a number to reach the random generated number
 * 3. Implements too high or too low algorithm; telling the user if they guess too high or too low
 * 4. User can only get 10 tries before losing the game
 * 5. if the user guesses the right number, the user wins the game
 * 6. Prompt the user if they want to play again or not -- if not then exit the program
 * *********************
 */

#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <cmath>
#include <ctime>

std::string input = "";
int guessedNum = 0;
int tryCount = 0;
int totalGuess = 5;
int randomNum;
char answer = {0};

void guessInput()
{
    while (true) {
        std::cout << "Guess a number between 1 to 10" << std::endl;
//        std::cout << randomNum << std::endl; //Testing the random number generator
        std::getline(std::cin, input);
        std::stringstream numStream(input);
        if (numStream >> guessedNum) { break; }
        std::cout << "Please enter an integer" << std::endl;
    }
}

int main() {
    srand(time(0));
    randomNum = rand() % 10 + 1;
    guessInput();
    
    while (randomNum != -1) {
        if (guessedNum < randomNum) {
            std::cout << " You guessed too low!" << std::endl;
            totalGuess--;
            std::cout << " You have " << totalGuess << " guesses left" << std::endl;
            guessInput();
        } else if (guessedNum > randomNum) {
            std::cout << " You guessed too high!" << std::endl;
            totalGuess--;
            std::cout << " You guessed: " << guessedNum << std::endl;
            std::cout << " You have " << totalGuess << " guesses left" << std::endl;
            guessInput();
        }

        if (guessedNum == randomNum) {
            std::cout << "You win!" << std::endl;
            break;
        } else if (totalGuess == 1) {
            std::cout << "You lose!" << std::endl;
            break;
        }
    }

        std::cout << "Would you like to play again? Y/N " << std::endl;
        std::getline(std::cin, input);

    return 0;
}
