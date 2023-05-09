#include <iostream>
#include <bitset>
#include <string>

using namespace std;

/**
 * Board will be represented by vertices with edges coming out of them
 * in the south and east directions. So we will use the lower two bits
 * of a char to store this state:
 * 0010 => south edge
 * 0001 => east edge
 * These bytes can be '&' together to store the information of any
 * edge state possiblity.
 * The owner of a square will be stored in the higher two bits (of the
 * vertex to the top left) likes this:
 * 1000 => X
 * 0100 => O
 * For example, a 3 x 3 board that looks like this:
 * 1---2   3
 * | X |
 * 4---5---6
 *     |
 * 7   8---9
 *
 * 0000
 * 8421
 *
 * Can be represented as [1011, 0010, 0000,
 *                        0001, 0011, 0000,
 *                        0000, 0001, 0000]
 *
 * Every horizontal edge is represented by two userscores _
 * Every vertical edge is a single pipe |
 *
 */

const int width = 5, height = 5;
char board[height][width] = {};
bool xTurn = true;
int scoreX = 0, scoreO = 0;

#define HAS_EAST_EDGE(row, col) (board[row][col] & 1)
#define HAS_SOUTH_EDGE(row, col) (board[row][col] & 2)
#define PLACE_POINT (8 / (!xTurn + 1))

#define RESET "\033[0m"
#define RED "\033[31m"
#define BLUE "\033[94m"
#define WHITE "\033[97m"

void drawBoard();
bool placePoints(int a, int b);
int stringToInt(string a);

int main()
{
    system("cls");
    drawBoard();
    while (true)
    {
        cout << "Score:\n\tX: " << BLUE << scoreX << RESET << "\n\tO: " << RED << scoreO << RESET << endl;
        cout << "Turn: " << (xTurn ? BLUE : RED)  << (xTurn ? "X" : "O") << RESET << endl;
        cout << "Point one: ";
        string pointOneStr;
        getline(cin, pointOneStr);
        int pointOne = stringToInt(pointOneStr);

        cout << "Point two: ";
        string pointTwoStr;
        getline(cin, pointTwoStr);
        int pointTwo = stringToInt(pointTwoStr);

        cout << pointOne << "," << pointTwo << endl;
        bool success = placePoints(pointOne, pointTwo);
        system("cls");
        drawBoard();
        if (!success)
        {
            cout << "invalid point" << endl;
        }
        else
        {
            xTurn = !xTurn;
        }
    }

    return 0;
}

void drawBoard()
{
    int maxCell = width * height;
    int padding = (to_string(maxCell)).length();
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            char cell = board[row][col];
            int cellNum = row * width + col;
            int paddingNeeded = padding - to_string(cellNum).length();
            for (int pad = 0; pad < paddingNeeded; pad++)
            {
                cout << " ";
            }
            cout << cellNum;
            // Look for east edge
            if (1 & cell)
            {
                cout << "---";
            }
            else
            {
                cout << "   ";
            }
        }
        cout << endl;
        for (int col = 0; col < width; col++)
        {
            char cell = board[row][col];
            int cellNum = row * width + col;
            int paddingNeeded = padding - to_string(cellNum).length();
            for (int pad = 0; pad < padding - 1; pad++)
            {
                cout << " ";
            }
            // Look for south edge
            if (2 & cell)
            {
                cout << "| ";
            }
            else
            {
                cout << "  ";
            }
            if (8 & cell)
            {
                cout << BLUE << "X " << RESET;
            }
            else if (4 & cell)
            {
                cout << RED << "O " << RESET;
            }
            else
            {
                cout << "  ";
            }
        }
        cout << endl;
    }
}

bool placePoints(int a, int b)
{
    if (a > width * height || b > width * height || a == b ) {
        return false;
    }

    int smaller = a < b ? a : b;
    int larger = a < b ? b : a;

    int smallerRow = (int)smaller / height;
    int smallerCol = smaller % width;
    char cell = board[smallerRow][smallerCol];
    if (larger - smaller == 1)
    {
        // This is a west-east edge
        if (board[smallerRow][smallerCol] & 4)
        {
            return false;
        }
        else
        {
            board[smallerRow][smallerCol] = board[smallerRow][smallerCol] | 1;
            // Check the above and below for a complete box
            // Above
            bool gotPoint = false;
            if (smallerRow > 0 &&
                HAS_EAST_EDGE(smallerRow - 1, smallerCol) &&
                HAS_SOUTH_EDGE(smallerRow - 1, smallerCol) &&
                HAS_SOUTH_EDGE(smallerRow - 1, smallerCol + 1))
            {
                board[smallerRow - 1][smallerCol] = board[smallerRow - 1][smallerCol] | PLACE_POINT;
                if (xTurn) 
                    scoreX++;
                else
                    scoreO++;
                gotPoint = true;
            }
            if (smallerRow < height - 1 &&
                HAS_EAST_EDGE(smallerRow + 1, smallerCol) &&
                HAS_SOUTH_EDGE(smallerRow, smallerCol) &&
                HAS_SOUTH_EDGE(smallerRow, smallerCol + 1))
            {
                board[smallerRow][smallerCol] = board[smallerRow][smallerCol] | PLACE_POINT;
                if (xTurn)
                    scoreX++;
                else
                    scoreO++;
                gotPoint = true;
            }
            if (gotPoint) xTurn = !xTurn;
            return true;
        }
    }
    else if (larger - smaller == width)
    {
        // this is a south-north edge
        if (board[smallerRow][smallerCol] & 2)
        {
            return false;
        }
        else
        {
            board[smallerRow][smallerCol] = board[smallerRow][smallerCol] | 2;
            // Check the left and right for a complete box
            bool gotPoint = false;
            if (smallerCol > 0 &&
                HAS_EAST_EDGE(smallerRow, smallerCol - 1) &&
                HAS_SOUTH_EDGE(smallerRow, smallerCol - 1) &&
                HAS_EAST_EDGE(smallerRow + 1, smallerCol - 1))
            {
                board[smallerRow][smallerCol - 1] = board[smallerRow][smallerCol - 1] | PLACE_POINT;
                if (xTurn)
                    scoreX++;
                else
                    scoreO++;
                gotPoint = true;
                
            }
            if (smallerCol < width - 1 &&
                HAS_EAST_EDGE(smallerRow, smallerCol) &&
                HAS_SOUTH_EDGE(smallerRow, smallerCol + 1) &&
                HAS_EAST_EDGE(smallerRow + 1, smallerCol))
            {
                board[smallerRow][smallerCol] = board[smallerRow][smallerCol] | PLACE_POINT;
                if (xTurn)
                    scoreX++;
                else
                    scoreO++;
                gotPoint = true;
                
            }
            if (gotPoint) xTurn = !xTurn;

            return true;
        }
    }
    else
    {
        return false;
    }
}

int stringToInt(string a)
{
    int sum = 0;
    for (int i = 0; i < a.length(); i++)
    {
        sum = (sum * 10) + (a[i] - '0');
    }
    return sum;
}