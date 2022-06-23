#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <queue>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <algorithm>
#define North 0
#define East 1
#define South 2
#define West 3
using namespace std;
typedef pair<int, int> ii;
typedef pair<string, int> si;


//******* Global Var ***********
char maze[50][50];
int maze_w, maze_h;
int size_pixel;
queue <ii> q;
int f[100][100];
int dx[] = { 0, 0, 1, -1 };
int dy[] = { 1, -1, 0, 0 };
ii start;
ii path[1005];
si rank_easy[5];
si rank_medium[5];
si rank_hard[5];
int choose_rank;
int i_easy = 0;
int i_medium = 0;
int i_hard = 0;

//********* SDL Var ****************
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 700;

SDL_Window* window;
SDL_Renderer* ren;
SDL_Texture* bg_play, * choose_startGame[3], * instruction, * choose_levelPlay[3], * level_rank, * choose_levelRank[3], * Name,
* cheese, * pressBorS, * rank_board, * solve_trace, * startGame, * wall, * you, * win, * your_trace, * enterName, * pink, * bg_name;
Mix_Music* intro_sound = NULL;
Mix_Chunk* step_sound, * win_sound, * click_sound;

//***** Init and Close SDL Prototypes ***********
bool init();
SDL_Texture* loadTexture(string path);
void renderTexture(SDL_Texture* tex, int x, int y, int w, int h);
void renderTexture(SDL_Texture* tex, int x, int y);
void WaitUntilKeyPressed();
void close();

//********* SDL Function Prototypes **********
void loadMedia();
void showGame();
void showChooseInStartGame(SDL_Texture* tex);
void showEnterName();
void show_bgPlay();
void renderYou(int x, int y);
void renderWall(int x, int y);
void renderYourTrace(int x, int y);
void renderCheese(int x, int y);
void showWin();
void renderSolveTrace(int x, int y);
void renderStep(SDL_Texture* tex);
void renderText(string str_val, int x, int y);
void showStep(int x);
void renderBackOrSolve();
void show_levelRank();
void showChoose_levelRank(SDL_Texture* tex);
void show_rankBoard();
void showInstruction();
void playIntro_sound(bool ok);
void playStep_sound();
void playClick_sound();
void playWin_sound();

//************** Solve Maze Prototypes ********
void resetMaze();
int XYtoIndex(int x, int y);
ii IndextoXY(int x);
int isInBounds(int x, int y);
void visit(int x, int y);
void bfs(ii s);
void setSolve();

//*********** Main Function In Game Prototypes ****************
void chooseStartGame();
void EnterName();
void renderMaze();
void proccessInGame();

//****
bool cmp(si a, si b) {
    return a.second < b.second;
}

//*********** &&&&&&&&&&&& *********
int main(int argc, char** argv)
{
    loadMedia();
    int num;
    //cin >> num;
    num = 20;
    for (int i = 0; i < num; i++) {
        playIntro_sound(true);
        showGame();
        chooseStartGame();
        show_bgPlay();
        renderMaze();
        proccessInGame();
        switch (choose_rank) {
        case 0:
            i_easy++;
            break;
        case 1:
            i_medium++;
            break;
        case 2:
            i_hard++;
            break;
        }
    }
    WaitUntilKeyPressed();
    close();
    return 0;
}

//*********** Main Function In Game **************
void chooseStartGame() {
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        SDL_Delay(10);
        if (SDL_WaitEvent(&e) == 0) continue;
        if ((e.type == SDL_QUIT) || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
            close();
            exit(1);
        }
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            x = e.button.x;
            y = e.button.y;
            if (e.button.button == SDL_BUTTON_LEFT || e.button.button == SDL_BUTTON_RIGHT || e.button.button == SDL_BUTTON_MIDDLE) {
                if ((x > 186 && y > 276) && (x < 812 && y < 340)) {
                    //choose start game -> choose level play -> enter name -> proccess in game
                    playClick_sound();
                    showChooseInStartGame(choose_startGame[0]);
                    SDL_Delay(500);
                    show_levelRank();
                    bool quit1 = false;
                    SDL_Event e1;
                    while (!quit1) {
                        SDL_WaitEvent(&e1);
                        if ((e1.type == SDL_QUIT) || (e1.type == SDL_KEYDOWN && e1.key.keysym.sym == SDLK_ESCAPE)) {
                            close();
                            exit(1);
                        }
                        bool quit1 = false;
                        SDL_Event e1;
                        while (!quit1) {
                            SDL_WaitEvent(&e1);
                            if ((e1.type == SDL_QUIT) || (e1.type == SDL_KEYDOWN && e1.key.keysym.sym == SDLK_ESCAPE)) {
                                close();
                                exit(1);
                            }
                            SDL_Delay(200);
                            // choose level play
                            if (e1.type == SDL_MOUSEBUTTONDOWN) {
                                int x, y;
                                x = e1.button.x;
                                y = e1.button.y;
                                if (e1.button.button == SDL_BUTTON_LEFT || e1.button.button == SDL_BUTTON_RIGHT || e1.button.button == SDL_BUTTON_MIDDLE) {
                                    if ((x > 287 && y > 228) && (x < 698 && y < 311)) {
                                        // choose level easy -> enter name
                                        playClick_sound();
                                        choose_rank = 0;
                                        showChoose_levelRank(choose_levelRank[0]);
                                        SDL_Delay(500);
                                        maze_h = 14; maze_w = 16; size_pixel = 50;
                                        showEnterName();
                                        EnterName();
                                        show_bgPlay();
                                        return;
                                        quit = true;
                                        quit1 = true;
                                    }
                                    else if ((x > 287 && y > 383) && (x < 698 && y < 458)) {
                                        // choose level medium -> enter name
                                        playClick_sound();
                                        choose_rank = 1;
                                        showChoose_levelRank(choose_levelRank[1]);
                                        SDL_Delay(500);
                                        maze_h = 17; maze_w = 20; size_pixel = 40;
                                        showEnterName();
                                        EnterName();
                                        show_bgPlay();
                                        return;
                                        quit = true;
                                        quit1 = true;
                                    }
                                    else if ((x > 287 && y > 523) && (x < 698 && y < 606)) {
                                        // choose level hard -> enter name
                                        playClick_sound();
                                        choose_rank = 2;
                                        showChoose_levelRank(choose_levelRank[2]);
                                        SDL_Delay(500);
                                        maze_h = 22; maze_w = 25; size_pixel = 32;
                                        showEnterName();
                                        EnterName();
                                        show_bgPlay();
                                        return;
                                        quit = true;
                                        quit1 = true;
                                    }
                                }
                            }
                        }
                    }
                }
                else if ((x > 186 && y > 386) && (x < 812 && y < 455)) {
                    //choose instruction
                    playClick_sound();
                    showChooseInStartGame(choose_startGame[1]);
                    SDL_Delay(500);
                    showInstruction();
                    bool quit1 = false;
                    SDL_Event e1;
                    while (!quit1) {
                        SDL_WaitEvent(&e1);
                        if ((e1.type == SDL_QUIT) || (e1.type == SDL_KEYDOWN && e1.key.keysym.sym == SDLK_ESCAPE)) {
                            close();
                            exit(1);
                        }
                        if (e1.type == SDL_MOUSEBUTTONDOWN) {
                            int x, y;
                            x = e1.button.x;
                            y = e1.button.y;
                            if (e1.button.button == SDL_BUTTON_LEFT || e1.button.button == SDL_BUTTON_RIGHT || e1.button.button == SDL_BUTTON_MIDDLE) {
                                if ((x > 0 && y > 0) && (x < 93 && y < 65)) {
                                    playClick_sound();
                                    showGame();
                                    quit1 = true;
                                }
                            }
                        }
                    }
                }
                else if ((x > 186 && y > 501) && (x < 812 && y < 565)) {
                    //choose rank -> choose level rank
                    playClick_sound();
                    showChooseInStartGame(choose_startGame[2]);
                    SDL_Delay(500);
                    show_levelRank();
                    SDL_Delay(200);
                    bool quit1 = false;
                    SDL_Event e1;
                    while (!quit1) {
                        SDL_WaitEvent(&e1);
                        if ((e1.type == SDL_QUIT) || (e1.type == SDL_KEYDOWN && e1.key.keysym.sym == SDLK_ESCAPE)) {
                            close();
                            exit(1);
                        }
                        if (e1.type == SDL_MOUSEBUTTONDOWN) {
                            int x, y;
                            x = e1.button.x;
                            y = e1.button.y;
                            if (e1.button.button == SDL_BUTTON_LEFT || e1.button.button == SDL_BUTTON_RIGHT || e1.button.button == SDL_BUTTON_MIDDLE) {
                                if ((x > 287 && y > 228) && (x < 698 && y < 311)) {
                                    // choose level easy
                                    playClick_sound();
                                    showChoose_levelRank(choose_levelRank[0]);
                                    SDL_Delay(600);
                                    show_rankBoard();
                                    // render text rank board easy
                                    sort(rank_easy, rank_easy + i_easy, cmp);
                                    int k = 0;
                                    for (int i = 0; i < i_easy && k < 4; i++)
                                        if (rank_easy[i].second != 0) {
                                            renderText(to_string(k + 1) + ". " + rank_easy[i].first + ": " + to_string(rank_easy[i].second) + " steps", 150, 197 + k * 120);
                                            k++;
                                        }
                                    bool quit2 = false;
                                    SDL_Event e2;
                                    while (!quit2) {
                                        SDL_WaitEvent(&e2);
                                        if ((e2.type == SDL_QUIT) || (e2.type == SDL_KEYDOWN && e2.key.keysym.sym == SDLK_ESCAPE)) {
                                            close();
                                            exit(1);
                                        }
                                        if (e2.type == SDL_MOUSEBUTTONDOWN) {
                                            int x, y;
                                            x = e2.button.x;
                                            y = e2.button.y;
                                            if (e2.button.button == SDL_BUTTON_LEFT || e2.button.button == SDL_BUTTON_RIGHT || e2.button.button == SDL_BUTTON_MIDDLE)
                                                if ((x > 0 && y > 0) && (x < 93 && y < 65)) {
                                                    playClick_sound();
                                                    showGame();
                                                    quit2 = true;
                                                }
                                        }
                                    }
                                    quit1 = true;
                                }
                                else if ((x > 287 && y > 383) && (x < 698 && y < 458)) {
                                    // choose level medium
                                    playClick_sound();
                                    showChoose_levelRank(choose_levelRank[1]);
                                    SDL_Delay(600);
                                    show_rankBoard();
                                    // render text rank board medium
                                    sort(rank_medium, rank_medium + i_medium, cmp);
                                    int k = 0;
                                    for (int i = 0; i < i_medium && k < 4; i++)
                                        if (rank_medium[i].second != 0) {
                                            renderText(to_string(k + 1) + ". " + rank_medium[i].first + ": " + to_string(rank_medium[i].second) + " steps", 150, 197 + k * 120);
                                            k++;
                                        }
                                    bool quit2 = false;
                                    SDL_Event e2;
                                    while (!quit2) {
                                        SDL_WaitEvent(&e2);
                                        if ((e2.type == SDL_QUIT) || (e2.type == SDL_KEYDOWN && e2.key.keysym.sym == SDLK_ESCAPE)) {
                                            close();
                                            exit(1);
                                        }
                                        if (e2.type == SDL_MOUSEBUTTONDOWN) {
                                            int x, y;
                                            x = e2.button.x;
                                            y = e2.button.y;
                                            if (e2.button.button == SDL_BUTTON_LEFT || e2.button.button == SDL_BUTTON_RIGHT || e2.button.button == SDL_BUTTON_MIDDLE)
                                                if ((x > 0 && y > 0) && (x < 93 && y < 65)) {
                                                    playClick_sound();
                                                    showGame();
                                                    quit2 = true;
                                                }
                                        }
                                    }
                                    quit1 = true;
                                }
                                else if ((x > 287 && y > 523) && (x < 698 && y < 606)) {
                                    // choose level hard
                                    playClick_sound();
                                    showChoose_levelRank(choose_levelRank[2]);
                                    SDL_Delay(600);
                                    show_rankBoard();
                                    // render text rank board hard
                                    sort(rank_hard, rank_hard + i_hard, cmp);
                                    int k = 0;
                                    for (int i = 0; i < i_hard && k < 4; i++)
                                        if (rank_hard[i].second != 0) {
                                            renderText(to_string(k + 1) + ". " + rank_hard[i].first + ": " + to_string(rank_hard[i].second) + " steps", 150, 197 + k * 120);
                                            k++;
                                        }
                                    bool quit2 = false;
                                    SDL_Event e2;
                                    while (!quit2) {
                                        SDL_WaitEvent(&e2);
                                        if ((e2.type == SDL_QUIT) || (e2.type == SDL_KEYDOWN && e2.key.keysym.sym == SDLK_ESCAPE)) {
                                            close();
                                            exit(1);
                                        }
                                        if (e2.type == SDL_MOUSEBUTTONDOWN) {
                                            int x, y;
                                            x = e2.button.x;
                                            y = e2.button.y;
                                            if (e2.button.button == SDL_BUTTON_LEFT || e2.button.button == SDL_BUTTON_RIGHT || e2.button.button == SDL_BUTTON_MIDDLE)
                                                if ((x > 0 && y > 0) && (x < 93 && y < 65)) {
                                                    playClick_sound();
                                                    showGame();
                                                    quit2 = true;
                                                }
                                        }
                                    }
                                    quit1 = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void renderMaze() {
    srand(time(0));
    resetMaze();
    visit(maze_h - 1, maze_w - 1);
    bool check = false;
    for (int i = 0; i < maze_h; i++) {
        for (int j = 0; j < maze_w; j++)
            if (maze[i][j] == ' ') {
                check = true;
                start = ii(i, j);
                break;
            }
        if (check) break;
    }
    int cnt = 0;
    for (int i = 0; i < maze_h; i++) {
        cnt = 0;
        for (int j = 0; j < maze_w; j++) {
            if (maze[i][j] == ' ') cnt++;
            else renderWall(j * size_pixel, i * size_pixel);
        }
    }
    renderCheese((maze_w - 1) * size_pixel, (maze_h - 1) * size_pixel);
    SDL_Delay(100);
    renderYou(start.second * size_pixel, start.first * size_pixel);
    SDL_Delay(100);
}

void proccessInGame() {
    playIntro_sound(false);
    int cnt = 0;
    showStep(cnt);
    ii trace = start;
    SDL_Event e;
    bool quit = false;
    while (!quit) {
        SDL_Delay(10);
        if (SDL_WaitEvent(&e) == 0) continue;
        if ((e.type == SDL_QUIT) || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
            close();
            exit(1);
        }
        // press up down left right to play
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_UP:
                playStep_sound();
                cnt++;
                showStep(cnt);
                if (maze[trace.first - 1][trace.second] == ' ' && isInBounds(trace.first - 1, trace.second)) {
                    renderYourTrace(trace.second * size_pixel, trace.first * size_pixel);
                    trace = ii(trace.first - 1, trace.second);
                    renderYou(trace.second * size_pixel, trace.first * size_pixel);
                }
                break;
            case SDLK_DOWN:
                playStep_sound();
                cnt++;
                showStep(cnt);
                if (maze[trace.first + 1][trace.second] == ' ' && isInBounds(trace.first + 1, trace.second)) {
                    renderYourTrace(trace.second * size_pixel, trace.first * size_pixel);
                    trace = ii(trace.first + 1, trace.second);
                    renderYou(trace.second * size_pixel, trace.first * size_pixel);
                }
                break;
            case SDLK_LEFT:
                playStep_sound();
                cnt++;
                showStep(cnt);
                if (maze[trace.first][trace.second - 1] == ' ' && isInBounds(trace.first, trace.second - 1)) {
                    renderYourTrace(trace.second * size_pixel, trace.first * size_pixel);
                    trace = ii(trace.first, trace.second - 1);
                    renderYou(trace.second * size_pixel, trace.first * size_pixel);
                }
                break;
            case SDLK_RIGHT:
                playStep_sound();
                cnt++;
                showStep(cnt);
                if (maze[trace.first][trace.second + 1] == ' ' && isInBounds(trace.first, trace.second + 1)) {
                    renderYourTrace(trace.second * size_pixel, trace.first * size_pixel);
                    trace = ii(trace.first, trace.second + 1);
                    renderYou(trace.second * size_pixel, trace.first * size_pixel);
                }
                break;
            }
        }
        // if you win
        if (trace.first == maze_h - 1 && trace.second == maze_w - 1) {
            SDL_Delay(500);
            playWin_sound();
            showWin();
            // save your data (name and score (steps))
            switch (choose_rank)
            {
            case 0:
                if (rank_easy[i_easy].second == 0) rank_easy[i_easy].second = cnt;
                else rank_easy[i_easy].second = min(rank_easy[i_easy].second, cnt);
                break;
            case 1:
                if (rank_medium[i_medium].second == 0) rank_medium[i_medium].second = cnt;
                else rank_medium[i_medium].second = min(rank_medium[i_medium].second, cnt);
                break;
            case 2:
                if (rank_hard[i_hard].second == 0) rank_hard[i_hard].second = cnt;
                else rank_hard[i_hard].second = min(rank_hard[i_hard].second, cnt);
                break;
            }
            bool quit3 = false;
            SDL_Event e3;
            while (!quit3) {
                SDL_WaitEvent(&e3);
                if ((e3.type == SDL_QUIT) || (e3.type == SDL_KEYDOWN && e3.key.keysym.sym == SDLK_ESCAPE)) {
                    close();
                    exit(1);
                }
                if (e3.type == SDL_KEYDOWN && e3.key.keysym.sym == SDLK_RETURN) {
                    quit3 = true;
                    quit = true;
                }
            }
        }
        // don't wanna play, you choose solve or new or back
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            x = e.button.x;
            y = e.button.y;
            if (e.button.button == SDL_BUTTON_LEFT || e.button.button == SDL_BUTTON_RIGHT || e.button.button == SDL_BUTTON_MIDDLE) {
                if ((x > 805 && y > 0) && (x < 988 && y < 117)) {
                    //press solve -> your score = 0
                    playClick_sound();
                    bfs(start);
                    setSolve();
                    int cnt1 = 0;
                    for (int i = 0; i < maze_h; i++) {
                        cnt1 = 0;
                        for (int j = 0; j < maze_w; j++) {
                            if (maze[i][j] != 'e') cnt1++;
                            else renderSolveTrace(j * size_pixel, i * size_pixel);
                        }
                    }
                    bool quit1 = false;
                    SDL_Event e1;
                    while (!quit1) {
                        SDL_WaitEvent(&e1);
                        if ((e1.type == SDL_QUIT) || (e1.type == SDL_KEYDOWN && e1.key.keysym.sym == SDLK_ESCAPE)) {
                            close();
                            exit(1);
                        }
                        if (e1.type == SDL_KEYDOWN && e1.key.keysym.sym == SDLK_RETURN) {
                            renderBackOrSolve();
                            SDL_Delay(2000);
                            quit1 = true;
                        }
                    }
                    quit = true;
                }
                else if ((x > 805 && y > 133) && (x < 988 && y < 242)) {
                    //press new -> reset step
                    playClick_sound();
                    resetMaze();
                    show_bgPlay();
                    renderMaze();
                    cnt = 0;
                    showStep(cnt);
                    trace = start;
                    SDL_Delay(100);
                }
                else if ((x > 805 && y > 258) && (x < 988 && y < 372)) {
                    //press back -> your score = 0
                    playClick_sound();
                    renderBackOrSolve();
                    SDL_Delay(2000);
                    quit = true;
                }
            }
        }
    }
}

void EnterName() {
    bool quit = false;
    SDL_Event e;
    string name = "";
    while (!quit) {
        if (SDL_WaitEvent(&e) == 0) continue;
        if ((e.type == SDL_QUIT) || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
            close();
            exit(1);
        }
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_a:
                name += "a";
                break;
            case SDLK_b:
                name += "b";
                break;
            case SDLK_c:
                name += "c";
                break;
            case SDLK_d:
                name += "d";
                break;
            case SDLK_e:
                name += "e";
                break;
            case SDLK_f:
                name += "f";
                break;
            case SDLK_g:
                name += "g";
                break;
            case SDLK_h:
                name += "h";
                break;
            case SDLK_i:
                name += "i";
                break;
            case SDLK_j:
                name += "j";
                break;
            case SDLK_k:
                name += "k";
                break;
            case SDLK_l:
                name += "l";
                break;
            case SDLK_m:
                name += "m";
                break;
            case SDLK_n:
                name += "n";
                break;
            case SDLK_o:
                name += "o";
                break;
            case SDLK_p:
                name += "p";
                break;
            case SDLK_q:
                name += "q";
                break;
            case SDLK_r:
                name += "r";
                break;
            case SDLK_s:
                name += "s";
                break;
            case SDLK_t:
                name += "t";
                break;
            case SDLK_u:
                name += "u";
                break;
            case SDLK_v:
                name += "v";
                break;
            case SDLK_w:
                name += "w";
                break;
            case SDLK_x:
                name += "x";
                break;
            case SDLK_y:
                name += "y";
                break;
            case SDLK_z:
                name += "z";
                break;
            case SDLK_0:
                name += "0";
                break;
            case SDLK_1:
                name += "1";
                break;
            case SDLK_2:
                name += "2";
                break;
            case SDLK_3:
                name += "3";
                break;
            case SDLK_4:
                name += "4";
                break;
            case SDLK_5:
                name += "5";
                break;
            case SDLK_6:
                name += "6";
                break;
            case SDLK_7:
                name += "7";
                break;
            case SDLK_8:
                name += "8";
                break;
            case SDLK_9:
                name += "9";
                break;
            case SDLK_RETURN:
                // save your data (your name)
                switch (choose_rank)
                {
                case 0:
                    rank_easy[i_easy].first = (name == "") ? "anonymous" : name;
                    break;
                case 1:
                    rank_medium[i_medium].first = (name == "") ? "anonymous" : name;
                    break;
                case 2:
                    rank_hard[i_hard].first = (name == "") ? "anonymous" : name;
                    break;
                }
                // show your name 
                TTF_Font* font = TTF_OpenFont("Roboto-Black.ttf", 80);
                SDL_Color color = { 0, 0, 255 };
                SDL_Surface* surface = TTF_RenderText_Solid(font, name.c_str(), color);
                Name = SDL_CreateTextureFromSurface(ren, surface);
                SDL_FreeSurface(surface);
                renderTexture(Name, (800 - 50 * name.size()) / 2 + 100, 145);
                SDL_RenderPresent(ren);
                TTF_CloseFont(font);
                quit = true;
                break;
            }
            renderTexture(bg_name, 100, 140);
            TTF_Font* font = TTF_OpenFont("Roboto-Black.ttf", 80);
            SDL_Color color = { 0, 0, 255 };
            SDL_Surface* surface = TTF_RenderText_Solid(font, name.c_str(), color);
            Name = SDL_CreateTextureFromSurface(ren, surface);
            SDL_FreeSurface(surface);
            renderTexture(Name, (800 - 50 * name.size()) / 2 + 100, 145);
            SDL_RenderPresent(ren);
            TTF_CloseFont(font);
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            x = e.button.x;
            y = e.button.y;
            if (e.button.button == SDL_BUTTON_LEFT || e.button.button == SDL_BUTTON_RIGHT || e.button.button == SDL_BUTTON_MIDDLE) {
                if ((x > 0 && y > 0) && (x < 93 && y < 65)) {
                    playClick_sound();
                    showGame();
                    quit = true;
                }
            }
        }
    }
}

//************ Solve Maze **************
void resetMaze() {
    for (int i = 0; i < maze_h; i++)
        for (int j = 0; j < maze_w; j++)
            maze[i][j] = '#';
    for (int i = 0; i < maze_h; i++)
        for (int j = 0; j < maze_w; j++)
            f[i][j] = 1;
}

int XYtoIndex(int x, int y) {
    return x * maze_w + y;
}

ii IndextoXY(int x) {
    return ii(x / maze_w, x % maze_w);
}

int isInBounds(int x, int y) {
    if (x < 0 || x >= maze_h) return false;
    if (y < 0 || y >= maze_w) return false;
    return true;
}

void visit(int x, int y) {
    maze[x][y] = ' ';
    int d[4];
    d[0] = North;
    d[1] = East;
    d[2] = South;
    d[3] = West;
    for (int i = 0; i < 4; i++) {
        int r = rand() % 4;
        swap(d[r], d[i]);
    }
    for (int i = 0; i < 4; i++) {
        int dx = 0, dy = 0;
        switch (d[i]) {
        case North:
            dx = -1;
            break;
        case South:
            dx = 1;
            break;
        case East:
            dy = 1;
            break;
        case West:
            dy = -1;
            break;
        }
        int x2 = x + 2 * dx;
        int y2 = y + 2 * dy;
        if (isInBounds(x2, y2)) {
            if (maze[x2][y2] == '#') {
                maze[x2 - dx][y2 - dy] = ' ';
                visit(x2, y2);
            }
        }
    }
}

void bfs(ii s) {
    q.push(s);
    f[s.first][s.second] = 0;
    while (!q.empty()) {
        ii u = q.front();
        q.pop();
        if (u.first == maze_h - 1 && u.second == maze_w - 1) return;
        for (int i = 0; i < 4; i++) {
            if (f[u.first + dx[i]][u.second + dy[i]] == 1 && maze[u.first + dx[i]][u.second + dy[i]] == ' ') {
                f[u.first + dx[i]][u.second + dy[i]] = 0;
                q.push(ii(u.first + dx[i], u.second + dy[i]));
                path[XYtoIndex(u.first + dx[i], u.second + dy[i])] = ii(u.first, u.second);
            }
        }
    }
}

void setSolve() {
    ii x = ii(maze_h - 1, maze_w - 1);
    while (path[XYtoIndex(x.first, x.second)] != ii(0, 0)) {
        maze[x.first][x.second] = 'e';
        x = path[XYtoIndex(x.first, x.second)];
    }
    maze[start.first][start.second] = '1';
    maze[maze_h - 1][maze_w - 1] = 'n';
}

//************ SDL Function **********
void loadMedia() {
    if (!init()) cout << "Fail init" << endl;
    else {
        intro_sound = Mix_LoadMUS("sound_effects/intro_sound.mp3");
        step_sound = Mix_LoadWAV("sound_effects/step_sound.wav");
        click_sound = Mix_LoadWAV("sound_effects/click_sound.wav");
        win_sound = Mix_LoadWAV("sound_effects/win_sound.wav");
        pink = loadTexture("images/pink.bmp");
        bg_play = loadTexture("images/bg_play.bmp");
        instruction = loadTexture("images/instruction.bmp");
        level_rank = loadTexture("images/level_rank.bmp");
        rank_board = loadTexture("images/rank_board.bmp");
        enterName = loadTexture("images/enterName.bmp");
        cheese = loadTexture("images/cheese.bmp");
        solve_trace = loadTexture("images/solve_trace.bmp");
        startGame = loadTexture("images/startGame.bmp");
        wall = loadTexture("images/wall.bmp");
        you = loadTexture("images/you.bmp");
        win = loadTexture("images/win.bmp");
        bg_name = loadTexture("images/bg_name.bmp");
        your_trace = loadTexture("images/your_trace.bmp");
        choose_startGame[0] = loadTexture("images/choose_play.bmp");
        choose_startGame[1] = loadTexture("images/choose_instruction.bmp");
        choose_startGame[2] = loadTexture("images/choose_rank.bmp");
        choose_levelPlay[0] = loadTexture("images/level_rank_easy.bmp");
        choose_levelPlay[1] = loadTexture("images/level_rank_medium.bmp");
        choose_levelPlay[2] = loadTexture("images/level_rank_hard.bmp");
        choose_levelRank[0] = loadTexture("images/level_rank_easy.bmp");
        choose_levelRank[1] = loadTexture("images/level_rank_medium.bmp");
        choose_levelRank[2] = loadTexture("images/level_rank_hard.bmp");
        pressBorS = loadTexture("images/press_solve_play.bmp");
    }
}

void renderBackOrSolve() {
    renderTexture(pressBorS, 136, 244);
    SDL_RenderPresent(ren);
}

void renderCheese(int x, int y) {
    renderTexture(cheese, x, y, size_pixel, size_pixel);
    SDL_RenderPresent(ren);
}

void renderSolveTrace(int x, int y) {
    renderTexture(solve_trace, x, y, size_pixel, size_pixel);
    SDL_RenderPresent(ren);
}

void renderStep(SDL_Texture* tex) {
    renderTexture(pink, 802, 444);
    renderTexture(tex, 860, 466);
    SDL_RenderPresent(ren);
}

void renderText(string str_val, int x, int y) {
    TTF_Font* font = TTF_OpenFont("Roboto-Black.ttf", 70);
    SDL_Color color = { 0, 0, 255 };
    SDL_Surface* surface = TTF_RenderText_Solid(font, str_val.c_str(), color);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surface);
    renderTexture(tex, x, y);
    SDL_RenderPresent(ren);
    TTF_CloseFont(font);
    SDL_DestroyTexture(tex);
    SDL_FreeSurface(surface);
}

void renderWall(int x, int y) {
    renderTexture(wall, x, y, size_pixel, size_pixel);
    SDL_RenderPresent(ren);
}

void renderYou(int x, int y) {
    renderTexture(you, x, y, size_pixel, size_pixel);
    SDL_RenderPresent(ren);
}

void renderYourTrace(int x, int y) {
    renderTexture(your_trace, x, y, size_pixel, size_pixel);
    SDL_RenderPresent(ren);
}

void show_bgPlay() {
    SDL_RenderClear(ren);
    renderTexture(bg_play, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_RenderPresent(ren);
}

void show_levelRank() {
    SDL_RenderClear(ren);
    renderTexture(level_rank, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_RenderPresent(ren);
}

void show_rankBoard() {
    SDL_RenderClear(ren);
    renderTexture(rank_board, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_RenderPresent(ren);
}

void showChoose_levelRank(SDL_Texture* tex) {
    SDL_RenderClear(ren);
    renderTexture(tex, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_RenderPresent(ren);
}

void showChooseInStartGame(SDL_Texture* tex) {
    SDL_RenderClear(ren);
    renderTexture(tex, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_RenderPresent(ren);
}

void showGame() {
    SDL_RenderClear(ren);
    renderTexture(startGame, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_RenderPresent(ren);
}

void showEnterName() {
    SDL_RenderClear(ren);
    renderTexture(enterName, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_RenderPresent(ren);
}

void showWin() {
    renderTexture(win, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_RenderPresent(ren);
}

void showInstruction() {
    SDL_RenderClear(ren);
    renderTexture(instruction, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_RenderPresent(ren);
}

void showStep(int x) {
    string str_val = to_string(x);
    TTF_Font* font = TTF_OpenFont("Roboto-Black.ttf", 50);
    SDL_Color color = { 0, 0, 255 };
    SDL_Surface* surface = TTF_RenderText_Solid(font, str_val.c_str(), color);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, surface);
    renderStep(tex);
    TTF_CloseFont(font);
    SDL_DestroyTexture(tex);
    SDL_FreeSurface(surface);
}

void playIntro_sound(bool ok) {
    if (ok) {
        if (Mix_PlayingMusic() == 0) Mix_PlayMusic(intro_sound, -1);
    }
    else Mix_HaltMusic();
}

void playStep_sound() {
    Mix_PlayChannel(-1, step_sound, 0);
}

void playWin_sound() {
    Mix_PlayChannel(-1, win_sound, 0);
}

void playClick_sound() {
    Mix_PlayChannel(-1, click_sound, 0);
}


//************ Init and Close SDL ************
bool init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        cout << "SDL could not be initialized" << endl;
        return false;
    }
    else {
        TTF_Init();
        SDL_Init(SDL_INIT_AUDIO);
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
        {
            printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
            return false;
        }
        window = SDL_CreateWindow("ESCAPE MAZE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            cout << "Window could not be created" << endl;
            return false;
        }
        else {
            ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        }
    }
    return true;
}

SDL_Texture* loadTexture(string path) {
    SDL_Texture* tex = NULL;
    SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
    if (loadedSurface == NULL) cout << "Unable to load image surface" << endl;
    else {
        tex = SDL_CreateTextureFromSurface(ren, loadedSurface);
        SDL_FreeSurface(loadedSurface);
    }
    return tex;
}

void renderTexture(SDL_Texture* tex, int x, int y) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

void renderTexture(SDL_Texture* tex, int x, int y, int w, int h) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

void WaitUntilKeyPressed() {
    SDL_Event e;
    while (true) {
        if (SDL_WaitEvent(&e) != 0 && (e.type == SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_QUIT)) {
            return;
            SDL_Delay(100);
        }
    }
}

void close() {
    Mix_FreeChunk(click_sound);
    Mix_FreeChunk(win_sound);
    Mix_FreeChunk(step_sound);
    Mix_FreeMusic(intro_sound);
    SDL_DestroyTexture(bg_play);
    SDL_DestroyTexture(enterName);
    SDL_DestroyTexture(instruction);
    SDL_DestroyTexture(level_rank);
    SDL_DestroyTexture(cheese);
    SDL_DestroyTexture(rank_board);
    SDL_DestroyTexture(solve_trace);
    SDL_DestroyTexture(startGame);
    SDL_DestroyTexture(wall);
    SDL_DestroyTexture(you);
    SDL_DestroyTexture(win);
    SDL_DestroyTexture(Name);
    SDL_DestroyTexture(your_trace);
    SDL_DestroyTexture(pink);
    SDL_DestroyTexture(bg_name);
    for (int i = 0; i < 3; i++) {
        SDL_DestroyTexture(choose_startGame[i]);
        SDL_DestroyTexture(choose_levelPlay[i]);
        SDL_DestroyTexture(choose_levelRank[i]);
    }
    SDL_DestroyTexture(pressBorS);
    SDL_DestroyRenderer(ren);
    ren = NULL;
    SDL_DestroyWindow(window);
    window = NULL;
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}