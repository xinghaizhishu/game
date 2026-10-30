#include <stdio.h>

#define ROW 7
#define COL 7

// 游戏地图定义
// # 墙
// ' ' 空地
// @ 玩家
// + 玩家站在目标点上
// $ 箱子
// * 箱子在目标点上
// . 目标点
char map[ROW][COL] = {
    {'#','#','#','#','#','#','#'},
    {'#','.',' ','$',' ',' ','#'},
    {'#',' ','#',' ','#',' ','#'},
    {'#',' ',' ','@',' ',' ','#'},
    {'#',' ','#',' ','#',' ','#'},
    {'#',' ',' ',' ',' ',' ','#'},
    {'#','#','#','#','#','#','#'}
};

int playerX, playerY;

// 显示地图
void showMap()
{
    int i, j;
    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++)
        {
            printf("%c ", map[i][j]);
        }
        printf("\n");
    }
}

// 寻找玩家初始位置
void findPlayer()
{
    int i, j;
    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++)
        {
            if (map[i][j] == '@' || map[i][j] == '+')
            {
                playerY = i;
                playerX = j;
            }
        }
    }
}

// 判断是否胜利
// 没有 $ 表示所有箱子都在目标点上，变成了 *
int isWin()
{
    int i, j;
    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++)
        {
            if (map[i][j] == '$')
                return 0;
        }
    }
    return 1;
}

// 恢复玩家离开后的原位置
// 如果玩家原来站在目标点 + 上，则恢复为 .
// 否则恢复为空格
char getPlayerOldCell()
{
    if (map[playerY][playerX] == '+')
        return '.';
    else
        return ' ';
}

// 处理玩家移动
void movePlayer(int dx, int dy)
{
    int nx = playerX + dx;
    int ny = playerY + dy;

    // 边界检查
    if (nx < 0 || nx >= COL || ny < 0 || ny >= ROW)
        return;

    // 遇到墙，不能移动
    if (map[ny][nx] == '#')
        return;

    // 遇到箱子，包括箱子在目标点上的情况
    if (map[ny][nx] == '$' || map[ny][nx] == '*')
    {
        int bx = nx + dx;
        int by = ny + dy;

        // 箱子前方边界检查
        if (bx < 0 || bx >= COL || by < 0 || by >= ROW)
            return;

        // 箱子前方只能是空地或目标点
        if (map[by][bx] == ' ' || map[by][bx] == '.')
        {
            char boxOldCell = map[ny][nx];

            // 箱子推到前方
            if (map[by][bx] == '.')
                map[by][bx] = '*';   // 箱子到达目标点
            else
                map[by][bx] = '$';   // 箱子到达空地

            // 玩家进入箱子原来的位置
            if (boxOldCell == '*')
                map[ny][nx] = '+';   // 箱子原来在目标点，玩家站上去变 +
            else
                map[ny][nx] = '@';   // 玩家进入普通位置

            // 恢复玩家原来位置
            map[playerY][playerX] = getPlayerOldCell();

            playerX = nx;
            playerY = ny;
        }

        return;
    }

    // 遇到空地或目标点，可以移动
    if (map[ny][nx] == ' ' || map[ny][nx] == '.')
    {
        char dest = map[ny][nx];

        // 玩家进入新位置
        if (dest == '.')
            map[ny][nx] = '+';   // 玩家站在目标点上
        else
            map[ny][nx] = '@';

        // 恢复玩家原来位置
        map[playerY][playerX] = getPlayerOldCell();

        playerX = nx;
        playerY = ny;
    }
}

int main()
{
    char cmd;

    findPlayer();

    while (1)
    {
        showMap();

        if (isWin())
        {
            printf("恭喜你，推箱子成功！\n");
            break;
        }

        printf("使用 W A S D 控制移动：");
        scanf(" %c", &cmd);

        switch (cmd)
        {
            case 'w':
            case 'W':
                movePlayer(0, -1);
                break;

            case 's':
            case 'S':
                movePlayer(0, 1);
                break;

            case 'a':
            case 'A':
                movePlayer(-1, 0);
                break;

            case 'd':
            case 'D':
                movePlayer(1, 0);
                break;

            default:
                printf("无效输入！\n");
        }
    }

    return 0;
}