
// Example for getch() in C
 
#include <stdio.h>
#include <iostream>
 
// Library where getch() is stored
#include <conio.h>

using namespace std;

// W => 119
// A => 97
// S => 115
// D => 100

#define UP 119
#define LEFT 97
#define DOWN 115
#define RIGHT 100

int playerRow = 5, playerCol = 5;

void displayMap();
 
int main()
{
    while(true) {
        system("cls");

        displayMap();
        
        int move = getch();

        if(move == UP) {
            playerRow --;
        } else if(move == DOWN) {
            playerRow ++;
        } else if(move == LEFT) {
            playerCol --;
        } else if(move == RIGHT) {
            playerCol ++;
        }




        if(move == 120) {
            cout << "Goodbye!" << endl;
            break;
        }
    }

    return 0;
}

void displayMap() {
    cout << "**********" << endl;
    for(int i = 0; i < 10; i++) {
        if(i != playerRow) {
            cout << "*         *" << endl;
        } else {
            cout << "*";
            for(int j = 0; j < playerCol - 1; j++) {
                cout << " ";
            }
            cout << "X";
            for(int j = 0; j < 9 - playerCol; j++) {
                cout << " ";
            }
            cout << "*" << endl;
        }
    }
    cout << "**********" << endl;
    
}