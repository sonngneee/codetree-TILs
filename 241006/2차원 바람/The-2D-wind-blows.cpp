#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int Y, X, B;
int map[101][101];

struct pos {
	int r, c, rr, cc;
};

struct position {
	int y, x;
};
vector<pos> v;

void Input() {
	cin >> Y >> X >> B;
	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X; j++) {
			cin >> map[i][j];
		}
	}
	int r1, c1, r2, c2;
	for (int i = 0; i < B; i++) {
		cin >> r1 >> c1 >> r2 >> c2;
		v.push_back({ r1-1, c1-1, r2-1, c2-1 });
	}
}

void Rotation(pos sq) {
	int temp = map[sq.r][sq.cc];
	int temp2 = map[sq.rr][sq.cc];
	int temp3 = map[sq.rr][sq.c];

	for (int i = sq.cc; i > sq.c; i--) {
		map[sq.r][i] = map[sq.r][i - 1];
	}
	
	for (int i = sq.rr; i > sq.r; i--) {
		map[i][sq.cc] = map[i - 1][sq.cc];
	}
	for (int i = sq.c; i < sq.cc; i++) {
		map[sq.rr][i] = map[sq.rr][i + 1];
	}
	for (int i = sq.r; i < sq.rr; i++) {
		map[i][sq.c] = map[i + 1][sq.c];
	}
	map[sq.r + 1][sq.cc] = temp;
	map[sq.rr][sq.cc - 1] = temp2;
	map[sq.rr - 1][sq.c] = temp3;
}

int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, 1, 0, -1 };

void Avg(pos sq) {
	int tempmap[101][101] = {0};
	
	for (int i = sq.r; i <= sq.rr; i++) {
		for (int j = sq.c; j <= sq.cc; j++) {
			int ans = map[i][j];
			int len = 1;
			for (int d = 0; d < 4; d++) {
				position next = { i + dy[d], j + dx[d] };
				if (!(next.y >= 0 && next.y < Y && next.x >= 0 && next.x <X)) continue;
				ans += map[next.y][next.x];
				len++;

			}
			tempmap[i][j] = ans / len;
		}
	}
	for (int i = sq.r; i <= sq.rr; i++) {
		for (int j = sq.c; j <= sq.cc; j++) {
			map[i][j] = tempmap[i][j];
		}
	}
}

void Print() {
	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X; j++) {
			cout << map[i][j] << " ";
		}
		cout << '\n';
	}

}
void Process() {
	for (int b = 0; b < B; b++) {
		pos sq = v[b];

		Rotation(sq);
		Avg(sq);
	}
	Print();
}

int main() {

	//freopen("input.txt", "r", stdin);
	Input();
	Process();

	return 0;
}