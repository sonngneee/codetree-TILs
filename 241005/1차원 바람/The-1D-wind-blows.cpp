#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int Y, X, B;
int map[101][101];

struct info {
	int r;
	char d;
	int ud;
};

info windinfo[101];

int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, 1, 0, -1 };

queue<info>q;

void Input() {
	cin >> Y >> X >> B;
	for (int i = 0; i < Y; i++) {
		for (int j = 0; j < X; j++) {
			cin >> map[i][j];
		}
	}
	int r;
	char d;
	for (int i = 0; i < B; i++) {
		cin >> r >> d;
		windinfo[i] = { r - 1, d, 0 };
	}
}

void Move(info now) {
	if (now.d == 'L') {
		int temp = map[now.r][X - 1];
		for (int i = X - 1; i > 0; i--) {
			map[now.r][i] = map[now.r][i - 1];
		}
		map[now.r][0] = temp;
	}

	else {
		int temp = map[now.r][0];
		for (int i = 0; i < X - 1; i++) {
			map[now.r][i] = map[now.r][i + 1];
		}
		map[now.r][X - 1] = temp;
	}
}

void Compare(info now) {
	if (now.ud == 1) {
		// 위와 비교
		info next = { now.r - 1, 'n' };
		if (now.d == 'R') next.d = 'L';
		else next.d = 'R';

		if (next.d < 0) return;
		for (int i = 0; i < X; i++) {
			if (map[now.r][i] == map[next.r][i]) {
				next.ud = 1;
				q.push(next);
				return;
			}
		}
	}
	else {
		// 아래와 비교
		info next = { now.r + 1, 'n' };
		if (now.d == 'R') next.d = 'L';
		else next.d = 'R';

		if (next.r >= Y) return;
		for (int i = 0; i < X; i++) {
			if (map[now.r][i] == map[next.r][i]) {
				q.push(next);
				next.ud = 2;
				return;
			}
		}
	}
}

void Process() {
	for (int b = 0; b < B; b++) {
		info nowinfo = windinfo[b];
		
		// 방향에 따른 칸 이동
		Move(nowinfo);
		
		// 위, 아래와 비교
		nowinfo.ud = 1;
		Compare(nowinfo);
		nowinfo.ud = 2;
		Compare(nowinfo);

		while (!q.empty()) {
			info nextinfo = q.front();
			q.pop();
			Move(nextinfo);
			Compare(nextinfo);
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
int main() {

	freopen("input.txt", "r", stdin);
	Input();
	Process();
	Print();
	return 0;
}