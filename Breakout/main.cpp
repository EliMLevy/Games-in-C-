#include <iostream>
#include <conio.h>
#include <thread>
#include <mutex>
#include <Windows.h>
#include <ProcessEnv.h>

/**
 *     *
 * OOOOOOO
 */

using namespace std;

#define LEFT 97
#define RIGHT 100
#define RESET 114
#define EXIT 120

const int width = 20, height = 13, brickStart = 3, brickEnd = 6;
int ballCol = 10, ballRow = 10, ballHorizontalVel = 1, ballVerticalVel = -1;
int paddleCol = 10, paddleRow = height ;
bool map[brickEnd][width] = {};
mutex printMutex;
HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);

void drawMap();
void moveBall();
void reset();
void GoToXY(int column, int line);

int main()
{

    reset();

    thread t1(moveBall);

    while (true)
    {
        

        drawMap();

        int move = getch();

        if (move == LEFT)
        {
            if(paddleCol > 1) {
                paddleCol--;
            }
        }
        else if (move == RIGHT)
        {
            if(paddleCol < width - 2) {
                paddleCol++;
            }
        } 

        if (move == EXIT)
        {
            cout << "Goodbye!" << endl;
            break;
        } else if(move == RESET) {
            reset();
        }
    }

    return 0;
}

void drawMap()
{
    printMutex.lock();
    // system("cls");
    GoToXY(0,0);
    for (int i = 0; i < brickEnd; i++)
    {
        cout << "||";
        for (int j = 0; j < width; j++)
        {
            if(i == ballRow && j == ballCol) {
                cout << "@";
            } else {
                if (map[i][j])
                    cout << "O";
                else
                    cout << " ";

            }
        }
        cout << "||" << endl;
    }

    for (int i = 0; i < height - brickEnd; i++)
    {
        cout << "||";
        if (i == ballRow - brickEnd)
        {
            for (int j = 0; j < ballCol; j++)
            {
                cout << " ";
            }
            cout << "@";
            for (int j = 0; j < width - ballCol - 1; j++)
            {
                cout << " ";
            }
        }
        else
        {
            for (int j = 0; j < width; j++)
            {
                cout << " ";
            }
        }
        cout << "||" << endl;
    }
    cout << "||";

    for (int i = 0; i < paddleCol - 1; i++)
    {
        cout << " ";
    }
    cout << "TTT";
    for (int i = 0; i < width - paddleCol - 2; i++)
    {
        cout << " ";
    }
    cout << "||" << endl;
    printMutex.unlock();
}

void moveBall()
{
    while (true)
    {

        int nextBallCol = ballCol + ballHorizontalVel;
        int nextBallRow = ballRow + ballVerticalVel;

        if (nextBallRow < brickEnd && map[nextBallRow][nextBallCol])
        {
            map[nextBallRow][nextBallCol] = false;
            ballVerticalVel *= -1;
            nextBallRow = ballRow + ballVerticalVel;
        }

        if (nextBallCol <= 0 || nextBallCol >= width)
        {
            ballHorizontalVel *= -1;
            nextBallCol = ballCol + ballHorizontalVel;
        }

        if(nextBallRow <= 0) {
            ballVerticalVel *= -1;
            nextBallRow = ballRow + ballHorizontalVel;
        }

        if(nextBallRow == paddleRow) {
            if(nextBallCol == paddleCol - 1) {
                ballVerticalVel *= -1;
                ballHorizontalVel = -1;
                nextBallRow = ballRow + ballVerticalVel;
                nextBallCol = ballCol + ballHorizontalVel;
            } else if(nextBallCol == paddleCol + 1) {
                ballVerticalVel *= -1;
                ballHorizontalVel = 1;
                nextBallRow = ballRow + ballVerticalVel;
                nextBallCol = ballCol + ballHorizontalVel;
            } else if(nextBallCol == paddleCol) {
                ballVerticalVel *= -1;
                ballHorizontalVel = 0;
                nextBallRow = ballRow + ballVerticalVel;
                nextBallCol = ballCol;
            }
        }

        ballCol = nextBallCol;
        ballRow = nextBallRow;

        drawMap();
        this_thread::sleep_for(chrono::milliseconds(200));
    }
}

void reset() {
    system("cls");
    for (int i = brickStart; i < brickEnd; i++)
    {
        for (int j = 0; j < width; j++)
        {
            map[i][j] = true;
        }
    }

    ballCol = 10, ballRow = 10, ballHorizontalVel = 1, ballVerticalVel = -1;
    paddleCol = 10, paddleRow = height ;


}

void GoToXY(int column, int line)
{
    // Create a COORD structure and fill in its members.
    // This specifies the new position of the cursor that we will set.
    COORD coord;
    coord.X = column;
    coord.Y = line;

    // Obtain a handle to the console screen buffer.
    // (You're just using the standard console, so you can use STD_OUTPUT_HANDLE
    // in conjunction with the GetStdHandle() to retrieve the handle.)
    // Note that because it is a standard handle, we don't need to close it.

    // Finally, call the SetConsoleCursorPosition function.
    if (!SetConsoleCursorPosition(outputHandle, coord))
    {
        // Uh-oh! The function call failed, so you need to handle the error.
        // You can call GetLastError() to get a more specific error code.
        // ...
    }
}