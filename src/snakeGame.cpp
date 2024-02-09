#include <unistd.h>
#include "snakeGame.h"

using namespace std;

int KeyPressed;

//CharPosition
CharPosition::CharPosition(int col, int row)
{
    x = col;
    y = row;
}
CharPosition::CharPosition()
{
    x = 0;
    y = 0;
}

//snakeGmae
snakeGame::snakeGame(int level)
{
    if (level == 1)
        getKey();

    //shape
    snakeShape = 'x';
    edgechar = 'o'; 
    growthItemShape = '+';
    poisionItemShape = '-';
    speedItemShape = '*';

    growthItem.x = 0;
    growthItem.y = 0;
    poisonItem.x = 0;
    poisonItem.y = 0;
    speedItem.x = 0;
    speedItem.y = 0;
    currentLength = 3;
    isClear = false;

    requiredLength = level + 3; // 다음 단계로 넘어가기 위해 만족해야 할 뱀의 길이
    requiredGrowthItem = level; // 다음 단계로 넘어가기 위해 만족해야 할 Growth Item 먹은 수
    requiredPoisonItem = level; // 다음 단계로 넘어가기 위해 만족해야 할 Poison Item 먹은 수
    requiredGate = level;       // 다음 단계로 넘어가기 위해 만족해야 할 Gate 통과 횟수
    requiredSpeedItem = level;  // 다음 단계로 넘어가기 위해 만족해야 할 Speed Item 먹은 수

    scoreGrowthItem = 0;
    scorePoisonItem = 0;
    scoreGate = 0;
    scoreSpeedItem = 0;
    snakeSpeed = 90000;
    maxItemTime = 120; // 뱀이 아무것도 먹지 않을 때 아이템 위치가 대기하는 시간
    maxGateTime = 120;
    eatGrowthItem = 0;
    eatPoisionItem = 0;
    atGate1 = 0;
    atGate2 = 0;
    direction = 'r';
    growthItemTimer = 0;
    poisonItemTimer = 0;
    speedItemTimer = 0;
    gateTimer = 0;
    srand(time(NULL));

    setWindow(level);
    renderWindow(level);
    placeGrowthItem();
    placePoisonItem();
    placeSpeedItem();
    placeGate();
    renderSnake();
    displayScore();
    refresh();
}

snakeGame::~snakeGame()
{
    nodelay(stdscr, false);
    getch();
    endwin();
}

// window 초기화
void snakeGame::setWindow(int level)
{
    initscr(); // initialise the screen
    nodelay(stdscr, TRUE);
    keypad(stdscr, true);                  
    noecho(); // user input is not displayed on the screen
    curs_set(0); // cursor symbol is not not displayed on the screen (Linux)
    getmaxyx(stdscr, maxHeight, maxWidth); // define dimensions of game window
    maxHeight -= (level - 1) * 3;
    maxWidth -= (level - 1) * 6;
    return;
}

// window 그리기
void snakeGame::renderWindow(int level)
{
    int k, q;
    for (int i = 1; i < maxWidth - 12; i++) // 윗벽
    {
        wall.push_back(CharPosition(i, 0));
        start_color();
        init_pair(3, COLOR_WHITE, COLOR_WHITE);
        attron(COLOR_PAIR(3));
        move(0, i);
        addch(edgechar);
        attroff(COLOR_PAIR(3));
        refresh();
    }

    for (int i = 1; i < maxWidth - 12; i++) // 아래 벽
    {
        wall.push_back(CharPosition(i, maxHeight - 1));
        start_color();
        init_pair(3, COLOR_WHITE, COLOR_WHITE);
        attron(COLOR_PAIR(3));
        move(maxHeight - 1, i);
        addch(edgechar);
        attroff(COLOR_PAIR(3));
        refresh();
    }

    for (int i = 1; i < maxHeight - 1; i++) // 왼쪽 벽
    {
        wall.push_back(CharPosition(0, i));
        start_color();
        init_pair(3, COLOR_WHITE, COLOR_WHITE);
        attron(COLOR_PAIR(3));
        move(i, 0);
        addch(edgechar);
        attroff(COLOR_PAIR(3));
        refresh();
    }

    for (int i = 1; i < maxHeight - 1; i++) // 오른쪽 벽
    {
        wall.push_back(CharPosition(maxWidth - 12, i));
        start_color();
        init_pair(3, COLOR_WHITE, COLOR_WHITE);
        attron(COLOR_PAIR(3));
        move(i, maxWidth - 12);
        addch(edgechar);
        attroff(COLOR_PAIR(3));
        refresh();
    }

    int H = (maxHeight - 1) / 5;

    // 레벨에 따라 맵 구성
    switch (level)
    {
    case 1: {// KMU 모양의 장애물
        for (int i = 0; i < (maxWidth - 12) / 10; i++) {
            start_color();
            init_pair(3, COLOR_WHITE, COLOR_WHITE);
            attron(COLOR_PAIR(3));

            wall.push_back(CharPosition((maxWidth - 12) / 10 * 1, H));
            move(H, (maxWidth - 12) / 10 * 1);
            addch(edgechar);

            wall.push_back(CharPosition((maxWidth - 12) / 10 * 2 - i, H));
            move(H, (maxWidth - 12) / 10 * 2 - i);
            addch(edgechar);
            attroff(COLOR_PAIR(3));
            refresh();
            H++;
        }
        for (int i = 0; i < (maxWidth - 12) / 10; i++) {
            start_color();
            init_pair(3, COLOR_WHITE, COLOR_WHITE);
            attron(COLOR_PAIR(3));

            wall.push_back(CharPosition((maxWidth - 12) / 10 * 1, H));
            move(H, (maxWidth - 12) / 10 * 1);
            addch(edgechar);

            wall.push_back(CharPosition((maxWidth - 12) / 10 * 1 + i, H));
            move(H, (maxWidth - 12) / 10 * 1 + i);
            addch(edgechar);
            attroff(COLOR_PAIR(3));
            refresh();
            H++;
        }

    // M
        H = (maxHeight - 1) / 5;
        for (int i = 0; i < (maxWidth - 12) / 10; i++) {
            start_color();
            init_pair(3, COLOR_WHITE, COLOR_WHITE);
            attron(COLOR_PAIR(3));

            wall.push_back(CharPosition((maxWidth - 12) / 10 * 4, H));
            move(H, (maxWidth - 12) / 10 * 4);
            addch(edgechar);

            wall.push_back(CharPosition((maxWidth - 12) / 10 * 6, H));
            move(H, (maxWidth - 12) / 10 * 6);
            addch(edgechar);

            wall.push_back(CharPosition((maxWidth - 12) / 10 * 4 + i, H));
            move(H, (maxWidth - 12) / 10 * 4 + i);
            addch(edgechar);

            wall.push_back(CharPosition((maxWidth - 12) / 10 * 6 - i, H));
            move(H, (maxWidth - 12) / 10 * 6 - i);
            addch(edgechar);

            attroff(COLOR_PAIR(3));
            refresh();
            H++;
        }
        for (int i = 0; i < (maxWidth - 12) / 10; i++) {
            start_color();
            init_pair(3, COLOR_WHITE, COLOR_WHITE);
            attron(COLOR_PAIR(3));

            wall.push_back(CharPosition((maxWidth - 12) / 10 * 4, H));
            move(H, (maxWidth - 12) / 10 * 4);
            addch(edgechar);

            wall.push_back(CharPosition((maxWidth - 12) / 10 * 6, H));
            move(H, (maxWidth - 12) / 10 * 6);
            addch(edgechar);

            attroff(COLOR_PAIR(3));
            refresh();
            H++;
        }

        H = (maxHeight - 1) / 5;
        //U
        for (int i = 0; i < (maxWidth - 12) / 10; i++) {
            start_color();
            init_pair(3, COLOR_WHITE, COLOR_WHITE);
            attron(COLOR_PAIR(3));

            wall.push_back(CharPosition((maxWidth - 12) / 10 * 8, H));
            move(H, (maxWidth - 12) / 10 * 8);
            addch(edgechar);

            wall.push_back(CharPosition((maxWidth - 12) / 10 * 10, H));
            move(H, (maxWidth - 12) / 10 * 10);
            addch(edgechar);

            attroff(COLOR_PAIR(3));
            refresh();
            H++;
        }
        for (int i = 0; i < (maxWidth - 12) / 10; i++) {
            start_color();
            init_pair(3, COLOR_WHITE, COLOR_WHITE);
            attron(COLOR_PAIR(3));

            wall.push_back(CharPosition((maxWidth - 12) / 10 * 8, H));
            move(H, (maxWidth - 12) / 10 * 8);
            addch(edgechar);

            wall.push_back(CharPosition((maxWidth - 12) / 10 * 10, H));
            move(H, (maxWidth - 12) / 10 * 10);
            addch(edgechar);

            if (i == (maxWidth - 12) / 10 - 1) {
                for (int j = 0; j < (maxWidth - 12) / 10 *2 + 1; j++) {
                    wall.push_back(CharPosition((maxWidth - 12) / 10 * 8 + j, H));
                    move(H, (maxWidth - 12) / 10 * 8 + j);
                    addch(edgechar);
                }
            }

            attroff(COLOR_PAIR(3));
            refresh();
            H++;
        }
        break;
    }
    case 2:{//3분할 장애물
        int squareHeight = 4;
        int squareWidth = 4;
        int squareRowStart = (maxHeight - squareHeight) / 2;
        int squareRowEnd = squareRowStart + squareHeight;
        int squareColStart = (maxWidth - squareWidth - 12) / 2;
        int squareColEnd = squareColStart + squareWidth;

        for (int row = 0; row < maxHeight; row++) {
            for (int col = 0; col < maxWidth - 12; col++) {
                if ((col == (maxWidth - 13) / 4 && row >= (maxHeight - 1) / 2 - 2 && row <= (maxHeight - 1) / 2 + 2) ||
                    (col == 3 * (maxWidth - 13) / 4 && row >= (maxHeight - 1) / 2 - 2 && row <= (maxHeight - 1) / 2 + 2))
                    continue;
                if (row == 0 || row == maxHeight - 1 || col == 0 || col == maxWidth - 13 ||
                    col == (maxWidth - 13) / 4 || col == 3 * (maxWidth - 13) / 4 ||
                    ((row >= squareRowStart && row < squareRowEnd) && (col >= squareColStart && col < squareColEnd))) {
                    wall.push_back(CharPosition(col, row));
                    start_color();
                    init_pair(3, COLOR_WHITE, COLOR_WHITE);
                    attron(COLOR_PAIR(3));
                    move(row, col);
                    addch(edgechar);
                    attroff(COLOR_PAIR(3));
                    refresh();
                }
            }
        }
        break;
    }
    case 3: {// X모양의 장애물
        q = 4;
        for (int i = 2; i < maxWidth - 13; i++)
        {
            int cnt = 4;
            while (cnt--) {
                wall.push_back(CharPosition(i, q));
                start_color();
                init_pair(3, COLOR_WHITE, COLOR_WHITE);
                attron(COLOR_PAIR(3));
                move(q, i);
                addch(edgechar);
                attroff(COLOR_PAIR(3));
                refresh();
                i++;

            }
            q++;
        }

        q = 4;
        for (int i = maxWidth - 13; i > 2; i--)
        {
            int cnt = 4;
            while (cnt--) {
                wall.push_back(CharPosition(i, q));
                start_color();
                init_pair(3, COLOR_WHITE, COLOR_WHITE);
                attron(COLOR_PAIR(3));
                move(q, i);
                addch(edgechar);
                attroff(COLOR_PAIR(3));
                refresh();
                i--;

            }
            q++;
        }
        break;

    }
    case 4: //9분할 장애물
        {
        int sixthHeight = (maxHeight - 1) / 6;
        int sixthWidth = (maxWidth - 12) / 6;
        int p1 = sixthHeight;
        int p2 = 3 * sixthHeight;
        int p3 = 5 * sixthHeight;
        int ph1 = sixthWidth;
        int ph2 = 3 * sixthWidth;
        int ph3 = 5 * sixthWidth;

        for (int row = 0; row < maxHeight; row++) {
            for (int col = 0; col < maxWidth - 12; col++) {
                if ((row == 2 * sixthHeight || row == 4 * sixthHeight) && 
                    !(col >= ph1 - 1 && col <= ph1 + 1) && 
                    !(col >= ph2 - 1 && col <= ph2 + 1) &&
                    !(col >= ph3 - 1 && col <= ph3 + 1)
                    || (col == 2 * sixthWidth || col == 4 * sixthWidth) && 
                    !(row >= p1 - 1 && row <= p1 + 1) &&
                    !(row >= p2 - 1 && row <= p2 + 1) &&
                    !(row >= p3 - 1 && row <= p3 + 1)) {
                    wall.push_back(CharPosition(col, row));
                    start_color();
                    init_pair(3, COLOR_WHITE, COLOR_WHITE);
                    attron(COLOR_PAIR(3));
                    move(row, col);
                    addch(edgechar);
                    attroff(COLOR_PAIR(3));
                    refresh();
                }
            }
        }

        }
    break;
    }

    // 모서리 부분을 다른 색으로 표시
    start_color();
    init_pair(4, COLOR_BLACK, COLOR_BLACK);
    attron(COLOR_PAIR(4));
    move(0, 0);
    addch(edgechar);
    move(0, maxWidth - 12);
    addch(edgechar);
    move(maxHeight - 1, 0);
    addch(edgechar);
    move(maxHeight - 1, maxWidth - 12);
    addch(edgechar);
    attroff(COLOR_PAIR(4));
    refresh();
}

// 뱀의 body
void snakeGame::renderSnake()
{
    for (int i = 0; i < 3; i++)
    {
        snake.push_back(CharPosition(7 - i, 1));
    }

    for (int i = 0; i < snake.size(); i++)
    {
        move(snake[i].y, snake[i].x);
        addch(snakeShape);
    }
    return;
}


// 점수 출력
void snakeGame::displayScore()
{   
    //Score Board
    move(0, maxWidth - 11);
    printw("Score Board");

    move(1, maxWidth - 9);
    printw("B : (%d)", currentLength);

    move(2, maxWidth - 9);
    printw("+ : (%d)", scoreGrowthItem);

    move(3, maxWidth - 9);
    printw("- : (%d)", scorePoisonItem);

    move(4, maxWidth - 9);
    printw("* : (%d)", scoreSpeedItem);

    move(5, maxWidth - 9);
    printw("G : (%d)", scoreGate);

    //Mission
    move(7, maxWidth - 11);
    printw("Mission");

    move(8, maxWidth - 9);
    if (requiredLength - currentLength <= 0)
    {
        printw("B : (%s)", "v");
    }
    else
    {
        printw("B : (%d)", requiredLength - currentLength);
    }

    move(9, maxWidth - 9);
    if (requiredGrowthItem - scoreGrowthItem <= 0)
    {
        printw("+ : (%s)", "v");
    }
    else
    {
        printw("+ : (%d)", requiredGrowthItem - scoreGrowthItem);
    }

    move(10, maxWidth - 9);
    if (requiredPoisonItem - scorePoisonItem <= 0)
    {
        printw("- : (%s)", "v");
    }
    else
    {
        printw("- : (%d)", requiredPoisonItem - scorePoisonItem);
    }

    move(11, maxWidth - 9);
    if (requiredSpeedItem - scoreSpeedItem <= 0)
    {
        printw("* : (%s)", "v");
    }
    else
    {
        printw("* : (%d)", requiredSpeedItem - scoreSpeedItem);
    }

    move(12, maxWidth - 9);
    if (requiredGate - scoreGate <= 0)
    {
        printw("G : (%s)", "v");
    }
    else
    {
        printw("G : (%d)", requiredGate - scoreGate);
    }

    // 뱀 그래픽
    for (int i = 1; i < 8; i++) {
        start_color();
        init_pair(3, COLOR_WHITE, COLOR_WHITE);
        attron(COLOR_PAIR(3));
        move(14, maxWidth - 9 + i);
        addch(edgechar);
        attroff(COLOR_PAIR(3));

    }

    for (int i = 1; i < 8; i++) {
        attron(COLOR_PAIR(3));
        move(18, maxWidth - 9 + i);
        addch(edgechar);
        attroff(COLOR_PAIR(3));

    }

    for (int i = 1; i < 5; i++) {
        attron(COLOR_PAIR(3));
        move(14 + i, maxWidth - 9);
        addch(edgechar);
        attroff(COLOR_PAIR(3));

    }

    for (int i = 1; i < 5; i++) {
        attron(COLOR_PAIR(3));
        move(14 + i, maxWidth - 1);
        addch(edgechar);
        attroff(COLOR_PAIR(3));

    }

    //뱀 혀
    attron(COLOR_PAIR(3));
    move(19, maxWidth - 6);
    addch(edgechar);
    attroff(COLOR_PAIR(3));

    attron(COLOR_PAIR(3));
    move(20, maxWidth - 7);
    addch(edgechar);
    attroff(COLOR_PAIR(3));

    //뱀 눈
    attron(COLOR_PAIR(3));
    move(16, maxWidth - 7);
    addch(edgechar);
    attroff(COLOR_PAIR(3));

    attron(COLOR_PAIR(3));
    move(16, maxWidth - 5);
    addch(edgechar);
    attroff(COLOR_PAIR(3));



}

//다음 스테이지로 넘어가는 기준 확인
bool snakeGame::nextStage()
{
    if (currentLength >= requiredLength && scoreGrowthItem >= requiredGrowthItem && scorePoisonItem >= requiredPoisonItem && scoreGate >= requiredGate && scoreSpeedItem >= requiredSpeedItem)
    {
        return true;
    }
    return false;
}


//gate
void snakeGame::placeGate() //게이트 생성
{
    // 난수를 통해 idx를 받음
    int gate_idx1 = rand() % wall.size();
    int gate_idx2 = rand() % wall.size();
    while (gate_idx2 == gate_idx1) //중복되지않도록
    {
        gate_idx2 = rand() % wall.size();
    }
    gate_1 = wall[gate_idx1];
    gate_2 = wall[gate_idx2];
    start_color();
    init_pair(5, COLOR_BLUE, COLOR_BLUE);
    attron(COLOR_PAIR(5));
    move(gate_1.y, gate_1.x);
    addch(edgechar);
    move(gate_2.y, gate_2.x);
    addch(edgechar);
    attroff(COLOR_PAIR(5));
    refresh();
}

// Gate 위치 변경
void snakeGame::gateTime()
{
    gateTimer++;
    if (gateTimer % maxGateTime == 0)
    {
        attron(COLOR_PAIR(3));
        move(gate_1.y, gate_1.x);
        addch(edgechar);
        move(gate_2.y, gate_2.x);
        addch(edgechar);
        attroff(COLOR_PAIR(3));
        refresh();
        placeGate();
        gateTimer = 0;
    }
}

// Gate에 들어갔는지 확인
bool snakeGame::checkGate()
{
    if (snake[0].x == gate_1.x && snake[0].y == gate_1.y)
    {
        gateTimer = maxGateTime - snake.size() - 1;
        scoreGate++;
        displayScore();
        return atGate1 = true;
    }
    else if (snake[0].x == gate_2.x && snake[0].y == gate_2.y)
    {
        gateTimer = maxGateTime - snake.size() - 1;
        scoreGate++;
        displayScore();
        return atGate2 = true;
    }
}


// 맵에 GrowthItem 표현
void snakeGame::placeGrowthItem()
{
    int tmpx, tmpy;
    bool clear = false;
    while (!clear)
    {
        tmpx = rand() % (maxWidth - 13) + 1;
        tmpy = rand() % (maxHeight - 2) + 1;
        clear = true;

        for (int i = 0; i < snake.size(); i++)
        {
            if (snake[i].x == tmpx && snake[i].y == tmpy)
            {
                clear = false;
                break;
            }
        }

        for (int i = 0; i < wall.size(); i++)
        {
            if (wall[i].x == tmpx && wall[i].y == tmpy)
            {
                clear = false;
                break;
            }
        }
    }

    growthItem.x = tmpx;
    growthItem.y = tmpy;
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_GREEN);
    attron(COLOR_PAIR(1));
    move(growthItem.y, growthItem.x);
    addch(growthItemShape);
    attroff(COLOR_PAIR(1));
    refresh();
}

// GrowthItem 위치 변경
void snakeGame::growthItemTime()
{
    growthItemTimer++;
    if (growthItemTimer % maxItemTime == 0)
    {
        move(growthItem.y, growthItem.x);
        printw(" ");
        placeGrowthItem();
        growthItemTimer = 0;
    }
}

// GrowthItem 먹었는지 확인
bool snakeGame::checkGrowth()
{
    if (snake[0].x == growthItem.x && snake[0].y == growthItem.y)
    {
        growthItemTimer = 0;
        placeGrowthItem();
        currentLength++;
        scoreGrowthItem++;
        displayScore();
        return eatGrowthItem = true;
    }
    else
    {
        return eatGrowthItem = false;
    }
}


// 맵에 PoisonItem 표현
void snakeGame::placePoisonItem()
{
    int tmpx, tmpy;
    bool clear = false;
    while (!clear)
    {
        tmpx = rand() % (maxWidth - 13) + 1; 
        tmpy = rand() % (maxHeight - 2) + 1;
        clear = true;

        for (int i = 0; i < snake.size(); i++)
        {
            if (snake[i].x == tmpx && snake[i].y == tmpy)
            {
                clear = false;
                break;
            }
        }

        for (int i = 0; i < wall.size(); i++)
        {
            if (wall[i].x == tmpx && wall[i].y == tmpy)
            {
                clear = false;
                break;
            }
        }
    }
    poisonItem.x = tmpx;
    poisonItem.y = tmpy;
    start_color();
    init_pair(2, COLOR_WHITE, COLOR_RED);
    attron(COLOR_PAIR(2));
    move(poisonItem.y, poisonItem.x);
    addch(poisionItemShape);
    attroff(COLOR_PAIR(2));
    refresh();
}

// PoisonItem 위치 변경
void snakeGame::poisonItemTime() 
{
    poisonItemTimer++;
    if (poisonItemTimer % maxItemTime == 0)
    {
        move(poisonItem.y, poisonItem.x);
        printw(" ");
        placePoisonItem();
        poisonItemTimer = 0;
    }
}

// PoisonItem 먹었는지 확인
bool snakeGame::checkPoison()
{
    if (snake[0].x == poisonItem.x && snake[0].y == poisonItem.y)
    {
        poisonItemTimer = 0;
        placePoisonItem();
        currentLength--;
        scorePoisonItem++;
        displayScore();
        return eatPoisionItem = true;
    }
    else
    {
        return eatPoisionItem = false;
    }
}


// 맵에 SpeedItem 표현
void snakeGame::placeSpeedItem()
{
    int tmpx, tmpy;
    bool clear = false;
    while (!clear)
    {
        tmpx = rand() % (maxWidth - 13) + 1; 
        tmpy = rand() % (maxHeight - 2) + 1;
        clear = true;

        for (int i = 0; i < snake.size(); i++)
        {
            if (snake[i].x == tmpx && snake[i].y == tmpy)
            {
                clear = false;
                break;
            }
        }

        for (int i = 0; i < wall.size(); i++)
        {
            if (wall[i].x == tmpx && wall[i].y == tmpy)
            {
                clear = false;
                break;
            }
        }
    }
    speedItem.x = tmpx;
    speedItem.y = tmpy;
    start_color();
    init_pair(6, COLOR_WHITE, COLOR_YELLOW);
    attron(COLOR_PAIR(6));
    move(speedItem.y, speedItem.x);
    addch(speedItemShape);
    attroff(COLOR_PAIR(6));
    refresh();
}

// SpeedItem 위치 변경
void snakeGame::speedItemTime() 
{
    speedItemTimer++;
    if (speedItemTimer % maxItemTime == 0)
    {
        move(speedItem.y, speedItem.x);
        printw(" ");
        placeSpeedItem();
        speedItemTimer = 0;
    }
}

// SpeedItem 먹었는지 확인
void snakeGame::checkSpeed()
{
    if (snake[0].x == speedItem.x && snake[0].y == speedItem.y)
    {
        speedItemTimer = 0;
        placeSpeedItem();
        snakeSpeed -= 10000;
        scoreSpeedItem++;
        displayScore();
    }
}


// Snake 움직임 정의
void snakeGame::moveSnake()
{
    KeyPressed = getch();
    if (KeyPressed == leftKey) {
        if (direction != 'r')
        {
            direction = 'l';
        }
        else
            checkCollision();
    }
    else if (KeyPressed == rightKey) {
        if (direction != 'l')
        {
            direction = 'r';
        }
        else
            checkCollision();
    }
    else if (KeyPressed == upKey) {
        if (direction != 'd')
        {
            direction = 'u';
        }
        else
            checkCollision();
    }
    else if (KeyPressed == downKey) {
        if (direction != 'u')
        {
            direction = 'd';
        }
        else
            checkCollision();
    }
    else if (KeyPressed == KEY_BACKSPACE)
        direction = 'q';

    if (!(eatGrowthItem || eatPoisionItem))
    {
        move(snake[snake.size() - 1].y, snake[snake.size() - 1].x); 
        printw(" ");                                               
        refresh();
        snake.pop_back(); 
    }
    else if (eatPoisionItem) 
    {
        move(snake[snake.size() - 1].y, snake[snake.size() - 1].x);
        printw(" ");
        snake.pop_back();
        move(snake[snake.size() - 1].y, snake[snake.size() - 1].x);
        printw(" ");
        refresh();
        snake.pop_back();
    }

    if (direction == 'l')
    {
        snake.insert(snake.begin(), CharPosition(snake[0].x - 1, snake[0].y));
    }
    else if (direction == 'r')
    {
        snake.insert(snake.begin(), CharPosition(snake[0].x + 1, snake[0].y));
    }
    else if (direction == 'u')
    {
        snake.insert(snake.begin(), CharPosition(snake[0].x, snake[0].y - 1));
    }
    else if (direction == 'd')
    {
        snake.insert(snake.begin(), CharPosition(snake[0].x, snake[0].y + 1));
    }

    if (atGate1)
    {
        atGate1 = false;
        direction = getWarpDirection(direction, gate_2);
        if (direction == 'r')
        {
            snake[0] = CharPosition(gate_2.x + 1, gate_2.y);
        }
        else if (direction == 'l')
        {
            snake[0] = CharPosition(gate_2.x - 1, gate_2.y);
        }
        else if (direction == 'd')
        {
            snake[0] = CharPosition(gate_2.x, gate_2.y + 1);
        }
        else if (direction == 'u')
        {
            snake[0] = CharPosition(gate_2.x, gate_2.y - 1);
        }
    }
    else if (atGate2)
    {
        atGate2 = false;
        direction = getWarpDirection(direction, gate_1);
        if (direction == 'r')
        {
            snake[0] = CharPosition(gate_1.x + 1, gate_1.y);
        }
        else if (direction == 'l')
        {
            snake[0] = CharPosition(gate_1.x - 1, gate_1.y);
        }
        else if (direction == 'd')
        {
            snake[0] = CharPosition(gate_1.x, gate_1.y + 1);
        }
        else if (direction == 'u')
        {
            snake[0] = CharPosition(gate_1.x, gate_1.y - 1);
        }
    }


    move(snake[0].y, snake[0].x);
    addch(snakeShape); 
    refresh();
    return;
}

// 게이트를 통과할 때
char snakeGame::getWarpDirection(char d, CharPosition gate)
{
    char result;
    CharPosition leftBlock(gate.x - 1, gate.y);
    CharPosition rightBlock(gate.x + 1, gate.y);
    CharPosition upBlock(gate.x, gate.y - 1);
    CharPosition downBlock(gate.x, gate.y + 1);

    bool isLeftWall = false;
    bool isRightWall = false;
    bool isUpWall = false;
    bool isDownWall = false;

    for (int i = 0; i < wall.size(); i++)
    {
        if (wall[i].x == leftBlock.x && wall[i].y == leftBlock.y)
        {
            isLeftWall = true;
        }
        if (wall[i].x == rightBlock.x && wall[i].y == rightBlock.y)
        {
            isRightWall = true;
        }
        if (wall[i].x == upBlock.x && wall[i].y == upBlock.y)
        {
            isUpWall = true;
        }
        if (wall[i].x == downBlock.x && wall[i].y == downBlock.y)
        {
            isDownWall = true;
        }
    }

    if ((leftBlock.x == 0 && leftBlock.y == 0) || (leftBlock.x == 0 && leftBlock.y == maxHeight - 1))
    {
        isLeftWall = true;
    }
    if ((rightBlock.x == maxWidth - 12 && rightBlock.y == 0) || (rightBlock.x == maxWidth - 12 && rightBlock.y == maxHeight - 1))
    {
        isRightWall = true;
    }
    if ((upBlock.x == 0 && upBlock.y == 0) || (upBlock.x == maxWidth - 12 && upBlock.y == 0))
    {
        isUpWall = true;
    }
    if ((downBlock.x == 0 && downBlock.y == maxHeight - 1) || (downBlock.x == maxWidth - 12 && downBlock.y == maxHeight - 1))
    {
        isDownWall = true;
    }

    if (gate.x == 0)
    {
        isLeftWall = true;
        isUpWall = true;
        isDownWall = true;
    }
    if (gate.x == maxWidth - 12)
    {
        isRightWall = true;
        isUpWall = true;
        isDownWall = true;
    }
    if (gate.y == 0)
    {
        isUpWall = true;
        isRightWall = true;
        isLeftWall = true;
    }
    if (gate.y == maxHeight - 1)
    {
        isDownWall = true;
        isRightWall = true;
        isLeftWall = true;
    }

    if (d == 'l')
    {
        if (!isLeftWall)
        {
            result = 'l';
        }
        else if (!isUpWall)
        {
            result = 'u';
        }
        else if (!isRightWall)
        {
            result = 'r';
        }
        else
        {
            result = 'd';
        }
    }

    if (d == 'u')
    {
        if (!isUpWall)
        {
            result = 'u';
        }
        else if (!isRightWall)
        {
            result = 'r';
        }
        else if (!isDownWall)
        {
            result = 'd';
        }
        else
        {
            result = 'l';
        }
    }

    if (d == 'r')
    {
        if (!isRightWall)
        {
            result = 'r';
        }
        else if (!isDownWall)
        {
            result = 'd';
        }
        else if (!isLeftWall)
        {
            result = 'l';
        }
        else
        {
            result = 'u';
        }
    }

    if (d == 'd')
    {
        if (!isDownWall)
        {
            result = 'd';
        }
        else if (!isLeftWall)
        {
            result = 'l';
        }
        else if (!isUpWall)
        {
            result = 'u';
        }
        else
        {
            result = 'r';
        }
    }
    return result;
}


// 게임 종료 조건
bool snakeGame::checkCollision()
{
    for (int i = 0; i < wall.size(); i++)
    {
        if (snake[0].x == wall[i].x && snake[0].y == wall[i].y)
        {
            if (!((snake[0].x == gate_1.x && snake[0].y == gate_1.y) || (snake[0].x == gate_2.x && snake[0].y == gate_2.y)))
            {
                return true;
                break;
            }
        }
    }

    for (int i = 2; i < snake.size(); i++)
    {
        if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
        {
            return true;
        }
    }

    if (snake.size() < 3) 
    {
        return true;
    }
    if (direction == 'r' && KeyPressed == leftKey) 
    {
        return true;
    }
    if (direction == 'l' && KeyPressed == rightKey)
    {
        return true;
    }
    if (direction == 'u' && KeyPressed == downKey)
    {
        return true;
    }
    if (direction == 'd' && KeyPressed == upKey)
    {
        return true;
    }

    return false;
}


// SnakeGame 실행
void snakeGame::runGame()
{
    isClear = true;
    while (!nextStage())
    {
        if (checkCollision())
        {
            move((maxHeight - 2) / 2, (maxWidth - 20) / 2);
            printw("GAME OVER");
            endwin();
            isClear = false;
            break;
        }
        checkGrowth();
        checkPoison();
        checkGate();
        checkSpeed();
        growthItemTime();
        poisonItemTime();
        speedItemTime();
        gateTime();
        moveSnake();
        if (direction == 'q') 
        {
            break;
        }
        usleep(snakeSpeed); 
    }
    if (isClear)
    {
        move((maxHeight - 2) / 2, (maxWidth - 30) / 2);
        printw("Mission Complete");
        endwin();
}
    }

// 게임 클리어 여부 확인
bool snakeGame::getClear() 
{
    return isClear;
}

// 방향키 입력 받기
void snakeGame::getKey()
{
    initscr();
    cbreak();
    noecho();

    keypad(stdscr, TRUE);

    getmaxyx(stdscr, maxHeight, maxWidth);
    move((maxHeight / 2.5), (maxWidth / 3.5));
    printw("Press right key : ");
    rightKey = getch();
    refresh();
    clear();

    getmaxyx(stdscr, maxHeight, maxWidth);
    move((maxHeight / 2.5), (maxWidth / 3.5));
    printw("Press left key : ");
    leftKey = getch();
    refresh();
    clear();

    getmaxyx(stdscr, maxHeight, maxWidth);
    move((maxHeight / 2.5), (maxWidth / 3.5));
    printw("Press up key : ");
    upKey = getch();
    refresh();
    clear();

    getmaxyx(stdscr, maxHeight, maxWidth);
    move((maxHeight / 2.5), (maxWidth / 3.5));
    printw("Press down key : ");
    downKey = getch();
    refresh();

    endwin();
    clear();
}