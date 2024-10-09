#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>
using namespace std;

int N;
int map[101][101];
int r, c, m1, m2, m3, m4, dir;
int dy[4] = { -1, -1, 1, 1};
int dx[4] = { 1, -1, -1, 1 };

struct pos {
	int y, x;
};

int vsize;

vector<pos> route;
vector<int> value;

void Input() {
	cin >> N;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> map[i][j];
		}
	}
	cin >> r >> c >> m1 >> m2 >> m3 >> m4 >> dir;
	r--;
	c--;

	vsize = m1 + m2 + m3 + m4;
}

void Preprocessing() {
	// 벡터에 pos 저장
	//	벡터에 val 저장
	pos now = { r, c };
	pos next;
	route.push_back(now);
	value.push_back(map[now.y][now.x]);
	for (int i = 0; i < m1; i++) {
		next = { now.y + dy[0], now.x + dx[0] };
		route.push_back(next);
		value.push_back(map[next.y][next.x]);
		now = next;
	}
	for (int i = 0; i < m2; i++) {
		next = { now.y + dy[1], now.x + dx[1] };
		route.push_back(next);
		value.push_back(map[next.y][next.x]);
		now = next;
	}
	for (int i = 0; i < m3; i++) {
		next = { now.y + dy[2], now.x + dx[2] };
		route.push_back(next);
		value.push_back(map[next.y][next.x]);
		now = next;
	}
	for (int i = 0; i < m4; i++) {
		next = { now.y + dy[3], now.x + dx[3] };
		route.push_back(next);
		value.push_back(map[next.y][next.x]);
		now = next;
	}
}

void Process() {
	// 방향에 따라 val 업데이트
	// 반시계
	if (dir == 0) {
		int temp = value[vsize - 1];
		for (int i = vsize - 1; i > 0; i--) {
			value[i] = value[i - 1];
		}
		value[0] = temp;
	}
	else {
		int temp = value[0];
		for (int i = 0; i < vsize-1; i++) {
			value[i] = value[i + 1];
		}
		value[vsize - 1] = temp;
	}

	// map 업데이트
	for (int i = 0; i < vsize; i++) {
		map[route[i].y][route[i].x] = value[i];
	}
}

void Print() {

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << map[i][j] << " ";
		}
		cout << '\n';
	}
}
int main() {

	//freopen("input.txt", "r", stdin);
	Input();
	Preprocessing();
	Process();
	Print();

	return 0;
}