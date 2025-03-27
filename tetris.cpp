#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <sstream>

using namespace std;
const int WIDTH = 10;
const int HEIGHT = 20;

vector<string> colors = {"\033[34m#\033[0m", "\033[33m#\033[0m", "\033[35m#\033[0m", "\033[32m#\033[0m", "\033[31m#\033[0m", "\033[36m#\033[0m", "\033[37m#\033[0m"};

vector<vector<vector<int>>> tetrominoes = {
    {{1, 1, 1, 1}},             // i 
    {{1, 1}, {1, 1}},           // o
    {{0, 1, 0}, {1, 1, 1}},     // t
    {{1, 1, 0}, {0, 1, 1}},     // z
    {{0, 1, 1}, {1, 1, 0}},     // s
    {{1, 0, 0}, {1, 1, 1}},     // l 
    {{0, 0, 1}, {1, 1, 1}}      // j
};

class Tetris {
private:
    vector<vector<int>> grid; 
    vector<vector<int>> colorGrid;
    int score, level;
    int fallInterval;
    pair<int, int> pos;
    vector<vector<int>> currentPiece;
    int currentColor;
    HANDLE hConsole;
    bool gameOver;

public:
    Tetris() : gameOver(false) {
        grid = vector<vector<int>>(HEIGHT, vector<int>(WIDTH, 0));
        colorGrid = vector<vector<int>>(HEIGHT, vector<int>(WIDTH, -1));
        score = 0;
        level = 1;
        fallInterval = 500;
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        spawnPiece();
    }

    void spawnPiece() {
        int index = rand() % tetrominoes.size();
        currentPiece = tetrominoes[index];
        currentColor = index;
        pos = {0, WIDTH / 2 - (int)currentPiece[0].size() / 2};

        if (!isValidMove(pos.first, pos.second, currentPiece)) {
            gameOver = true;
            Beep(400, 500); 
        }
    }

    bool isValidMove(int newRow, int newCol, const vector<vector<int>> &piece) {
        for (int i = 0; i < piece.size(); i++) {
            for (int j = 0; j < piece[i].size(); j++) {
                if (piece[i][j]) {
                    int r = newRow + i, c = newCol + j;
                    if (r >= HEIGHT || c < 0 || c >= WIDTH || (r >= 0 && grid[r][c]))
                        return false;
                }
            }
        }
        return true;
    }

    vector<vector<int>> rotatePiece(const vector<vector<int>> &piece) {
        int rows = piece.size(), cols = piece[0].size();
        vector<vector<int>> rotated(cols, vector<int>(rows, 0));
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                rotated[j][rows - 1 - i] = piece[i][j];
            }
        }
        return rotated;
    }

    void rotate() {
        vector<vector<int>> rotated = rotatePiece(currentPiece);
        if (isValidMove(pos.first, pos.second, rotated)) {
            currentPiece = rotated;
            Beep(500, 100); 
        }
    }

    void movePiece(int dx) {
        if (isValidMove(pos.first, pos.second + dx, currentPiece)) {
            pos.second += dx;
            Beep(300, 50); 
        }
    }

    bool moveDown() {
        if (isValidMove(pos.first + 1, pos.second, currentPiece)) {
            pos.first++;
            return true;
        }
        return false;
    }

    void dropPiece() {
        while (isValidMove(pos.first + 1, pos.second, currentPiece))
            pos.first++;
        Beep(600, 100); 
    }

    void lockPiece() {

        for (int i = 0; i < currentPiece.size(); i++) {
            for (int j = 0; j < currentPiece[i].size(); j++) {
                if (currentPiece[i][j] && pos.first + i >= 0) {
                    grid[pos.first + i][pos.second + j] = 1;
                    colorGrid[pos.first + i][pos.second + j] = currentColor;
                }
            }
        }
        clearLines();
        spawnPiece();
    }

    void clearLines() {
        for (int i = HEIGHT - 1; i >= 0; i--) {
            bool full = true;
            for (int j = 0; j < WIDTH; j++) {
                if (!grid[i][j]) {
                    full = false;
                    break;
                }
            }
            if (full) {
                grid.erase(grid.begin() + i);
                grid.insert(grid.begin(), vector<int>(WIDTH, 0));
                colorGrid.erase(colorGrid.begin() + i);
                colorGrid.insert(colorGrid.begin(), vector<int>(WIDTH, -1));
                score += 100;
                Beep(800, 100); 
            }
        }

        int newLevel = (score / 200) + 1; 
        if (newLevel > level) {
            level = newLevel;
            fallInterval = max(100, 500 - (level * 50)); 
            Beep(1000, 150); 
        }
    }

    void render() {
        ostringstream screen;
        screen << "\n";

        for (int i = 0; i < HEIGHT; i++) {
            screen << "| ";
            for (int j = 0; j < WIDTH; j++) {
                bool isPiece = false;
                for (int pi = 0; pi < currentPiece.size(); pi++) {
                    for (int pj = 0; pj < currentPiece[pi].size(); pj++) {
                        if (currentPiece[pi][pj] && pos.first + pi == i && pos.second + pj == j)
                            isPiece = true;
                    }
                }
                if (isPiece)
                    screen << colors[currentColor] << " ";
                else if (grid[i][j])
                    screen << colors[colorGrid[i][j]] << " ";
                else
                    screen << ". ";
            }
            screen << "|\n";
        }

        screen << "Score: " << score << "  Level: " << level << "\n"; 

        if (gameOver) {
            screen << "GAME OVER!\n";
        }

        SetConsoleCursorPosition(hConsole, {0, 0});
        cout << screen.str();
    }

    void play() {
        DWORD lastFallTime = GetTickCount();
        while (!gameOver) {
            if (_kbhit()) {
                char key = _getch();
                if (key == 75) movePiece(-1);//left
                else if (key == 77) movePiece(1);//right
                else if (key == 80) { if (!moveDown()) lockPiece(); }
                else if (key == 32) { dropPiece(); lockPiece(); }// space bar
                else if (key == 72) rotate();// up
            }
            DWORD currentTime = GetTickCount();
            if (currentTime - lastFallTime >= (DWORD)fallInterval) {
                if (!moveDown()) lockPiece();
                lastFallTime = currentTime;
            }
            render();
            Sleep(16);
        }
    }
};

int main() {
    srand((unsigned int)time(0));
    Tetris game;
    game.play();
    return 0;
}