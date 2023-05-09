#include <iostream>
#include <cstdlib>

using namespace std;

#define RESET "\033[0m"
#define RED "\033[31m"
#define WHITE "\033[97m"

int main() {
    // generate the random sequence
    srand(time(0));
    int sequence[4];
    for(int i = 0; i < 4; i++) {
        sequence[i] = rand() % 6;
    }
    
    // Wait for user input
    cout << "Welcome!" << endl;
    cout << "Try guessing my sequence (numbers 0-5)!" << endl;
    cout << "Enter your guess as four space separated numbers." << endl;
    cout << "If you give up, type 'exit' to reveal the answer." << endl;
    cout << "* * * *" << endl;
    int reds, whites;
    int guesses = 0;

    while(reds != 4) {
        reds = 0, whites = 0;
        string rawinput;
        getline(cin, rawinput);

        if(rawinput == "exit") {
            cout << sequence[0] << " " << sequence[1] << " " << sequence[2] << " " << sequence[3] << endl;
            cout << "Bye!" << endl;
            return 0;
        }

        if(rawinput.length() != 7) {
            cout << "Please enter four space separated numbers..." << rawinput.length() << endl;
            continue;
        }

        guesses ++;
        int input[4];
        for(int i = 0; i < 8; i+=2) {
            input[i/2] = rawinput[i] - '0';
        }

        // For each thing in the input:
        for(int i = 0; i < 4; i++) {
            bool correctSpot = input[i] == sequence[i];
            bool incorrectSpot = false;
            for(int j = 0; j < 4; j++) {
                if (input[i] == sequence[j]) {
                    incorrectSpot = true;
                }
            }
            if(correctSpot) {
                //  Check if it is in the right spot
                reds++;
            } else if(incorrectSpot) {
                //  Check if it exists at all
                whites++;
            }
        }
        // Print output (colorful?)
        for(int i = 0; i < reds; i++) {
            cout << RED << "* ";
        }
        for(int i = 0; i < whites; i++) {
            cout << WHITE << "* ";
        }
        cout << endl;
        cout << RESET;
    }

    cout << "Congrats! You got it in " << guesses << " guesses!" << endl;
    return 0;

}