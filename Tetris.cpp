#include <GL/glut.h>
#include <iostream>
#include <GL/glut.h>
#include <conio.h>
#include <ctime>
#include <random>
#include <memory>
#include <thread>
#include <Windows.h>

using namespace std;

const GLfloat cubeVs[8][3] = { {1,1,-1},{-1,1,-1},{-1,1,1},{1,1,1},{1,-1,1},{-1,-1,1},{-1,-1,-1},{1,-1,-1} }; //立方体顶点
const GLfloat cubeCs[6][3] = { {0,1,0},{1,0.5,0},{1,0,0},{1,1,0},{0,0,1},{1,0,1} }; //立方体颜色
const int cubeVi[6][4] = { {0,1,2,3},{4,5,6,7},{3,2,5,4},{7,6,1,0},{2,1,6,5},{0,3,4,7} }; //立方体顶点序列号

#define YMAX 20
#define XMAX 10

bool bottom, drop;

const int blocks[7][4][2] = {
	{{ 0, 0}, {0, 1}, {1, 0}, {1, 1}},
	{{-1, 0}, {0, 0}, {1, 0}, {2, 0}},
	{{-1, 0}, {0, 0}, {0, 1}, {0, 2}},
	{{ 1, 0}, {0, 0}, {0, 1}, {0, 2}},
	{{-1, 0}, {0, 0}, {1, 0}, {0, 1}},
	{{-1, 0}, {0, 0}, {0, 1}, {1, 1}},
	{{-1, 1}, {0, 1}, {0, 0}, {1, 0}}
};

const uint8_t letters[26][8] = {
  {0xff, 0xe7, 0xe7, 0xdb, 0x81, 0xbd, 0x7e, 0xff}, //A
  {0xff, 0xc1, 0xbd, 0xc1, 0xbd, 0x7d, 0x81, 0xff}, //B
  {0xff, 0xc3, 0xbd, 0xfd, 0xfd, 0x79, 0x83, 0xff}, //C
  {0xff, 0xc1, 0x9d, 0xbd, 0xbd, 0x9d, 0xc1, 0xff}, //D
  {0xff, 0x81, 0xfd, 0x81, 0xfd, 0xfd, 0x81, 0xff}, //E
  {0xff, 0x81, 0xfd, 0x81, 0xfd, 0xfd, 0xfd, 0xff}, //F
  {0xff, 0xc3, 0xb9, 0xfd, 0x8d, 0xb9, 0x83, 0xff}, //G
  {0xff, 0xbd, 0xbd, 0x81, 0xbd, 0xbd, 0xbd, 0xff}, //H
  {0xff, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xff}, //I
  {0xff, 0xbf, 0xbf, 0xbf, 0xbf, 0xbd, 0xc3, 0xff}, //J
  {0xff, 0x9d, 0xed, 0xf5, 0xe9, 0xdd, 0xbd, 0xff}, //k
  {0xff, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0x81, 0xff}, //L
  {0xff, 0x99, 0x99, 0x99, 0xa5, 0xa5, 0xa5, 0xff}, //M
  {0xff, 0xb9, 0xb9, 0xb5, 0xad, 0x9d, 0x9d, 0xff}, //N
  {0xff, 0xc3, 0x99, 0xbd, 0xbd, 0x99, 0xc3, 0xff}, //O
  {0xff, 0xc1, 0xbd, 0xbd, 0xc1, 0xfd, 0xfd, 0xff}, //P
  {0xff, 0xc3, 0x99, 0xbd, 0xbd, 0xa9, 0xc3, 0xbf}, //Q
  {0xff, 0xc1, 0xbd, 0x81, 0xbd, 0xbd, 0xbd, 0xff}, //R
  {0xff, 0xc3, 0xbd, 0xc3, 0x3f, 0x7d, 0x83, 0xff}, //s
  {0xff, 0x80, 0xf7, 0xf7, 0xf7, 0xf7, 0xf7, 0xff}, //T
  {0xff, 0xbd, 0xbd, 0xbd, 0xbd, 0xbd, 0xc3, 0xff}, //U
  {0xff, 0x7e, 0xbd, 0xdd, 0xdb, 0xe3, 0xf7, 0xff}, //V
  {0xff, 0xff, 0xff, 0x66, 0xa6, 0x99, 0x99, 0xff}, //W
  {0xff, 0xdd, 0xcb, 0xf7, 0xc3, 0xd9, 0xbc, 0xff}, //X
  {0xff, 0xbc, 0xd9, 0xe3, 0xf7, 0xf7, 0xf7, 0xff}, //Y
  {0xff, 0x80, 0xdf, 0xe7, 0xfb, 0xfd, 0x80, 0xff}  //Z
};

int(*block)[2] = (int(*)[2])malloc(8 * sizeof(int));
int(*nextBlock)[2] = (int(*)[2])malloc(8 * sizeof(int));
bool gameOver;
int matrix[YMAX][XMAX];
int x, y;  // 方块所在位置
int color, nextColor;

void BlockLoop();
void RotateBlock(int(*block)[2]);
void DrawCube(GLfloat x, GLfloat y, int color) {
	glLoadIdentity();
	glTranslatef(x - 14.0f, 20.0f - y, -80.0f);
	if (color < 4) {
		glRotatef(color * 90.0f, 1.0f, 0.0f, 0.0f);
	} else {
		glRotatef(color * 180.0f, 0.0f, 1.0f, 0.0f);
	}
	glBegin(GL_QUADS);
	for (int i = 0; i < 6; i++) {
		glColor3fv(cubeCs[i]);
		for (int j = 0; j < 4; j++) {
			int VtxId = cubeVi[i][j];
			glVertex3fv(cubeVs[VtxId]);
		}
	}
	glEnd();
}

void DrawChar(int color, char c, int x, int y) {
	int i = c - 'A';
	if (i > 25 || i < 0) {  // 无此字符
		return;
	}
	uint8_t b;
	for (int n = 0; n < 8; n++) {
		b = 0b00000001;
		for (int m = 0; m < 8; m++) {
			if ((letters[i][n] & b) == 0) {
				DrawCube((x + m) * 2.0f, (y + n) * 2.0f, color);
			}
			b = (uint8_t)(b << 1);
		}
	}
}

void CreateBlock() {
	memcpy(block, nextBlock, 8 * sizeof(int));
	color = nextColor;
	memcpy(nextBlock, blocks[rand() % 7], 8 * sizeof(int));
	for (int i = 0; i < rand() % 4; i++) {
		RotateBlock(nextBlock);
	}
	nextColor = rand() % 5;
	x = 5;
	y = 0;
}

void DrawMatrix() {
	for (int y = 0; y < YMAX; y++) {
		for (int x = 0; x < XMAX; x++) {
			if (matrix[y][x]) {
				DrawCube(x * 2.0f, y * 2.0f, matrix[y][x] - 1);
			}
		}
	}
}

void DrawBlock() {
	for (int i = 0; i < 4; i++) {
		DrawCube((block[i][0] + x) * 2.0f, (block[i][1] + y) * 2.0f, color);
	}
}

void DrawNextBlock() {
	for (int i = 0; i < 4; i++) {
		DrawCube((nextBlock[i][0] + XMAX + 3) * 2.0f, (nextBlock[i][1]) * 2.0f, nextColor);
	}
}

bool DoBlock() {
	y++;
	for (int i = 0; i < 4; i++) {
		int bx = block[i][0] + x, by = block[i][1] + y + 1;
		if ((by == 20) || matrix[by][bx]) {
			return true;
		}
	}
	return false;
}

void SaveBlock() {
	for (int i = 0; i < 4; i++) {
		int bx = block[i][0] + x, by = block[i][1] + y;
		matrix[by][bx] = color + 1;
	}
}

int ClearBlock() {
	int s = 0, y;
	for (y = YMAX - 1; y > 0; y--) {
		int yy = -1;
		for (int x = 0; x < XMAX; x++) {
			if (!matrix[y][x]) {
				yy = y;
				break;
			}
		}
		if (yy == -1) {
			s++;
			yy = y + 1;
			for (; y > 1; y--) {
				for (int x = 0; x < XMAX; x++) {
					matrix[y][x] = matrix[y - 1][x];
				}
			}
			for (int x = 0; x < XMAX; x++) {
				matrix[0][x] = false;
			}
			y = yy;
		}
	}
	return s;
}

bool CanMove(int(*b)[2], int x) {
	for (int i = 0; i < 4; i++) {
		int bx = b[i][0] + x, by = b[i][1] + y;
		if (bx < 0 || bx > XMAX - 1 || matrix[by][bx]) {
			return false;
		}
	}
	return true;
}

void RotateBlock(int(*b)[2]) {
	int(*temp)[2] = (int(*)[2])malloc(8 * sizeof(int));
	if (temp == nullptr) {
		return;
	}
	memcpy(temp, b, 8 * sizeof(int));
	for (int i = 0; i < 4; i++) {
		int bx = b[i][0], by = b[i][1];
		b[i][0] = -by;
		b[i][1] = bx;  // 旋转
	}
	if (CanMove(b, x)) {
		free(temp);
	} else {
		b = temp;
	}
}

void Reset() {
	bottom = false, drop = false;
	for (int y = 0; y < YMAX; y++) {
		for (int x = 0; x < XMAX; x++) {
			matrix[y][x] = 0;
		}
	}
	memcpy(nextBlock, blocks[rand() % 7], 8 * sizeof(int));
	nextColor = rand() % 5;
	CreateBlock();
	thread t = thread(BlockLoop);
	t.detach();
}

void Initial() {
	glShadeModel(GL_SMOOTH);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_COLOR_MATERIAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	srand((int)time);
	Reset();
}

void Display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glPushMatrix();

	if (gameOver) {
		DrawChar(0, 'G', -1, -6);
		DrawChar(0, 'A', 7, -6);
		DrawChar(0, 'M', -1, 2);
		DrawChar(0, 'E', 7, 2);
		DrawChar(0, 'O', -1, 10);
		DrawChar(0, 'V', 7, 10);
		DrawChar(0, 'E', -1, 18);
		DrawChar(0, 'R', 7, 18);
	} else {
		for (int x = -1; x < XMAX + 1; x++) {
			DrawCube(x * 2.0f, YMAX * 2.0f, 5);
		}
		for (int y = -1; y < YMAX + 1; y++) {
			DrawCube(-2.0f, y * 2.0f, 5);
			DrawCube(XMAX * 2.0f, y * 2.0f, 5);
		}
		DrawNextBlock();
		DrawMatrix();
		DrawBlock();
	}

	glPopMatrix();
	glutSwapBuffers();
}

void Reshape(int width, int height) {
	if (height == 0) {
		height = 1;
	}

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void KeyBoard(unsigned char key, int xx, int yy) {
	if (gameOver) {
		Reset();
		gameOver = false;
	}
	if (key == 'a' && CanMove(block, x - 1)) {
		x--;
	} else if (key == 'd' && CanMove(block, x + 1)) {
		x++;
	} else if (key == 'w') {
		RotateBlock(block);
	} else if (key == 's') {
		drop = true;
	}
}

void BlockLoop() {
	while (true) {
		if (bottom) {
			drop = false;
			SaveBlock();
			ClearBlock();
			CreateBlock();
		}
		bottom = DoBlock();
		for (int x = 0; x < XMAX; x++) {
			if (matrix[3][x]) {
				gameOver = true;
				return;
			}
		}
		Sleep(drop ? 10 : 300);
	}
}

int main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(600, 1000);
	glutInitWindowPosition(500, 0);
	glutCreateWindow("俄罗斯方块");
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutIdleFunc(Display);
	glutKeyboardFunc(KeyBoard);
	Initial();
	glutMainLoop();
	return 0;
}