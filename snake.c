//使用蛇头蛇尾不同打印的方法
#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <string.h>
#include <time.h>

void food();
void background();
void gotoxy();	//控制光标
void wall();	//边框，游戏的边界
void show();
void move();
void turn();
void check();
void ini();
//全局变量
int  dx[4] = { -1, 0, 1, 0 };
int  dy[4] = { 0, 1, 0, -1 };
int sum = 5;  //蛇总长度
int over = 0;//游戏是否结束
int speed;   //刷新速度
char map[17][17];//打印的地图
int score=0;

struct snake 
{
    int x, y;  //身体坐标 
    int dir;  //方向（只有蛇头的方向是有用的） 
} s[100];


/**************************************************/
void background()//欢迎界面
{int i;
	gotoxy(15,10);
	printf("■■■■■■■■■■■■■■■■■■■■■");
		for(i=11;i<15;i++)
		{
			gotoxy(15,i);printf("■");
			gotoxy(55,i);printf("■");
		}
	gotoxy(21, 13);
	printf("Welcome to the Happy Snake");
	gotoxy(15, 15);
	printf("■■■■■■■■■■■■■■■■■■■■■");
	gotoxy(15, 18);
	printf("游戏说明：");
	printf("'w''s''a''d'控制上下左右");
	gotoxy(15, 19);
	printf("按下ENTER键以继续");

	getchar();	//暂停看界面
}

/*************************************************/

void gotoxy(int x, int y)	//光标的控制
{
	COORD pos;
	HANDLE hOutput;
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput, pos);
}

/************************************************/

void ini() //初始化界面
{  
    int i, j;
    speed = 250;
    over = 0;
    sum = 5;
	for (i = 0; i < 100; i++) //初始化蛇头和蛇身的数据，每个蛇的一个点都作为一个节点
    {  //一个节点就是一个结构体类型变量，方向和坐标
        s[i].dir = 0;
        s[i].x = 0;
        s[i].y = 0;
    }
    s[0].x = 2; s[0].y = 2;  //地图左上角初始化设置一条长度为5的蛇
    s[1].x = 2; s[1].y = 3;
    s[2].x = 2; s[2].y = 4;
    s[3].x = 2; s[3].y = 5;
    s[4].x = 2; s[4].y = 6; 
    
    s[4].dir = 1;//蛇头
    srand(time(0));//种下种子
    for (i = 0; i < 17; i++) 
    {  //设置地图
        for (j = 0; j < 17; j++) 
        {
            map[i][j] = '*';

        }
    }
    for (i = 1; i < 16; i++) //打印出方框，通过两次的写
    {
        for (j = 1; j < 16; j++) 
		{
            map[i][j] = ' ';
        }
    }
    map[s[4].x][s[4].y] ='H';  //设置蛇头
    for (i = 0; i < sum - 1; i++) {  //设置蛇身
        map[s[i].x][s[i].y] = 'X';
    }
    wall();
	food();
}
/*****************************************************/
void wall()//打印墙
{
	int i,j;
	for(i=0;i<17;i++)
	{
		for(j=0;j<17;j++)
		{
		if(i==0||i==16)
			{
				gotoxy(2*i,j);
				printf("■");
			}
		if(j==0||j==16)
			{
				gotoxy(2*i,j);
				printf("■");
			}
		}

	}
	
    for (i = 0; i < 17; i++) 
    {  
        for (j = 0; j < 17; j++) 
        {
             if(map[i][j]=='H'||map[i][j]== 'X')//初始化蛇身
			{
				gotoxy(2*i,j);
				printf("■");
			}

		}
	}
	for(i=0;i<30;i++)//打印分界线
			{
				gotoxy(40,i);
				printf("^");
			}
	gotoxy(48,5);
	printf("THE HAPPY SNAKE");
	gotoxy(48,7);
	printf("YOUR SCORE:%d",sum-5);
}

/*****************************************************/
void show() 
{								//显示界面 
	int i,j;
    while (1) 
	{
        Sleep(speed);  //界面刷新速度
        turn();//输入下一个操作，即方向，然后使蛇头节点的方向值发生改变
        move();//根据turn转弯操作对蛇的移动进行修改，在move中check检查是否over
            if (over) 
            {		//设置蛇死掉后可以进行的操作 
					gotoxy(48,15);		//游戏结束时的选择
					printf("GAME OVER!");
					gotoxy(48,17);
					printf("NEW  GAME:按'r'");
					gotoxy(48,18);
					printf("EXIT GAME:按'q'\n");
                while (1) 
                {

                char ch = _getch();
					if (ch == 113) 
					{  //输入‘q’结束 
						return;
					}
					else if (ch == 114) 
					{  //输入‘r’重新开始 
						
						for(i=1;i<16;i++)
						{
							for(j=1;j<16;j++)
							{
								gotoxy(2*i,j);
								printf("  ");
							}
						}

					sum=5;
					score=0;
					gotoxy(42,15);//清除数据
					printf("	                         ");
					gotoxy(42,17);//清除数据
					printf("	                         ");
					gotoxy(42,18);//清除数据
					printf("	                         ");
					ini();
						break;
					}
				}
            }
      }
}

/*************************************************/
void food() 
{  //生成食物
    int x, y;
    while (1) 
    {
        x = (int)(15 * rand() / (RAND_MAX + 1.0));  //随机产生一组食物坐标⊙
        y = (int)(15 * rand() / (RAND_MAX + 1.0));
        if ((map[x][y] == ' ')&&(x<16&&x>0)&&(y<16&&y>0)) //如果是空格则在该处生成食物
        {  
            map[x][y] = 'O';
			gotoxy(2*x,y);
				printf("⊙");
            break;
        }
    }
}

/**************************************************/
void move() 
{  //蛇移动
    int i;
    int t = sum;	//t记录当前蛇总长度 
    check();  //移动前检查按当前方向移动一步后的情况
    if (t == sum) {  //没有吃到苹果
        for (i = 0; i < sum - 1; i++) {
            if (i == 0) {  //蛇尾坐标处变成空格，把蛇尾坐标变成前一个蛇身的坐标 
                map[s[i].x][s[i].y] =' ';
					gotoxy(2*(s[i].x),s[i].y);
					printf("  ");
                s[i].x = s[i + 1].x;
                s[i].y = s[i + 1].y;
            }
            else {  //每个蛇身坐标都变为它前一个蛇身的坐标
                s[i].x = s[i + 1].x;
				s[i].y = s[i + 1].y;
            }
            map[s[i].x][s[i].y] = 'X';  //把地图上蛇身坐标处的字符设置成‘X’
        }
        s[sum - 1].x = s[sum - 1].x + dx[s[sum - 1].dir];  //蛇头按当前方向移动一格 
        s[sum - 1].y = s[sum - 1].y + dy[s[sum - 1].dir];
        map[s[sum - 1].x][s[sum - 1].y] = 'H';  //把地图上蛇头坐标处的字符设置成‘H’
		if(map[s[sum - 1].x][s[sum - 1].y] = 'H')
		{gotoxy(2*(s[i].x),s[i].y);
		printf("■");}
    }
    else	//吃到苹果（sum会加1）
	{  
        map[s[sum - 2].x][s[sum - 2].y] = 'X';  //把地图上原蛇头坐标处的字符设置成‘X’
				s[sum - 1].x = s[sum - 2].x + dx[s[sum - 2].dir];  //新蛇头的坐标是原蛇头沿当前方向移动一格后的坐标 
				s[sum - 1].y = s[sum - 2].y + dy[s[sum - 2].dir];
				s[sum - 1].dir = s[sum - 2].dir;					//新蛇头方向为原蛇头的方向
		map[s[sum - 1].x][s[sum - 1].y] = 'H';						//把地图上蛇头坐标处的字符设置成‘H’
		if(map[s[sum - 1].x][s[sum - 1].y] = 'H')
		{
			gotoxy(2*(s[sum - 1].x),s[sum - 1].y);
			printf("■");
		}
        food();
    }

}

/****************************************************/
void check() {  //检查是否死亡或者吃到食物
    int x, y;
    x = s[sum - 1].x + dx[s[sum - 1].dir];  //记录按当前方向移动一格后蛇头的坐标 
    y = s[sum - 1].y + dy[s[sum - 1].dir];
    if (map[x][y] == '*' || map[x][y] == 'X') 
	{  //如果地图上该坐标处字符为‘*’或‘X’就死亡 
        if (x != s[0].x || y != s[0].y) //蛇尾除外 
		{  
            over = 1;
        }
    }
    else if (map[x][y] == 'O') 
	{  //吃到苹果 
			sum++;  //蛇身总长加1 
			gotoxy(59,7);
			printf("%d",++score);
			speed = ((300- sum * 20)>100) ? (300- sum * 20) : 150; //速度加快 
    }
}

/*******************************************************/
void turn() {  //转弯
    if (_kbhit()) 
    {
        char dir = _getch();  //读取输入的键 
        switch (dir) 
        {  //改变方向 
        case 'a': s[sum - 1].dir =    (s[sum - 1].dir == 2)?2:0; break;
        case 's': s[sum - 1].dir =    (s[sum - 1].dir == 3)?3:1; break;
        case 'd': s[sum - 1].dir =    (s[sum - 1].dir == 0)?0:2; break;
        case 'w': s[sum - 1].dir =    (s[sum - 1].dir == 1)?1:3; break;
        }
    }
}

/********************************************************/
int main(int argc, char *argv[])
{
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);//隐藏控制台的光标
		CONSOLE_CURSOR_INFO cci;
		GetConsoleCursorInfo(hOut, &cci);
		cci.bVisible = FALSE;
		SetConsoleCursorInfo(hOut, &cci);

	system("mode con cols=80 lines=30  ");//改变控制台窗口的大小
    background();//打印欢迎的界面
    system("cls");//清屏，进入游戏界面
    ini();//游戏开始,初始化数据
    show();
    return 0;
}