#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <queue>
#include <algorithm>

using namespace std;

// 학생들은, 입력으로 주어진 순서대로 탑승
// 
// 격자를 벗어나지 않는 4방향으로 인접한 칸 중 앉아있는 좋아하는 친구의 수가 가장 많은 위치로 => like map check

// 만약 1번 조건을 만족하는 칸의 위치가 여러 곳이라면, 그 중 인접한 칸 중 비어있는 칸의 수가 가장 많은 위치로 갑니다.단 이때 격자를 벗어나는 칸은 비어있는 칸으로 간주하지 않습니다.

// 만약 2번 조건까지 동일한 위치가 여러 곳이라면, 그 중 행 번호가 가장 작은 위치로 갑니다.

// 만약 3번 조건까지 동일한 위치가 여러 곳이라면, 그 중 열 번호가 가장 작은 위치로 갑니다.
//

int map[21][21];
int N;

int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, 1, 0, -1 };


struct buddy {
	int me;
	int b[4];
};

struct pos {
	int y, x;
};

struct infomap {
	int lbcnt, emptycnt, y, x; // 좋아하는 친구 수, 비어있는 칸의 수
};

queue<buddy> likebuddy; // 들어온 학생 순서대로 친한친구

int friends[442][442];

pos students[442]; // 학생 위치



void init() {

}

void input() {
	cin >> N;
	buddy ip;
	while (cin >> ip.me>>ip.b[0] >> ip.b[1] >> ip.b[2] >> ip.b[3]) {
		likebuddy.push(ip);
		friends[ip.me][ip.b[0]] = 1;
		friends[ip.me][ip.b[1]] = 1;
		friends[ip.me][ip.b[2]] = 1;
		friends[ip.me][ip.b[3]] = 1;
	}
}

buddy now;

bool cmp(infomap a, infomap b) {
	if (a.lbcnt == b.lbcnt) {
		if (a.emptycnt == b.emptycnt) {
			if (a.y == b.y) {
				return a.x < b.x;
			}
			return a.y < b.y;
		}
		return a.emptycnt > b.emptycnt;
	}
	return a.lbcnt > a.lbcnt;
}

int scoreF[5] = { 0, 1, 10, 100, 1000 };
int ans;

void calScore() {

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			int cntF = 0;

			for (int d = 0; d < 4; d++) {
				int ny = i + dy[d];
				int nx = j + dx[d];
				if (ny<1 || nx<1 || ny>N || nx>N) continue;
				if (friends[map[i][j]][map[ny][nx]] == 1) {
					cntF++;
				}
			}
			ans += scoreF[cntF];
		}
	}

}

void process() {
	

	while (!likebuddy.empty()) {
		infomap tempmap[21][21] = {0}; // map 의 정보

		now = likebuddy.front(); // 학생 순서대로
		likebuddy.pop();


		for (int i = 0; i < 4; i++) {
			int likeB = now.b[i]; // 현재 좋아하는 친구 번호
			pos likeBpos = students[likeB];  // 현재 좋아하는 친구 위치


			if (likeBpos.y == 0 && likeBpos.x == 0) {

				continue;
			}

			// 좋아하는 친구의 주변 살펴보기
			for (int d = 0; d < 4; d++) {
				int ny = likeBpos.y + dy[d];
				int nx = likeBpos.x + dx[d];

				if (ny<1 || nx<1 || ny>N || nx>N) continue;
				if (map[ny][nx] != 0) continue; // 이미 다른 친구가 있을 때

				tempmap[ny][nx].lbcnt++;

				for (int b = 0; b < 4; b++) {
					int cky = ny + dy[b];
					int ckx = nx + dx[b];
					if(cky<1 || ckx<1 || cky>N || ckx>N) continue;
					if (map[cky][ckx] != 0) continue; // 이미 다른 친구가 있을 때
					tempmap[ny][nx].emptycnt++;
				}
			}
		}
		vector<infomap> forsort;

		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= N; j++) {
				if (tempmap[i][j].lbcnt == 0) continue;
				
				forsort.push_back({ tempmap[i][j].lbcnt, tempmap[i][j].emptycnt, i, j });
			}
		}
		sort(forsort.begin(), forsort.end(), cmp);

		map[forsort[0].y][forsort[0].x] = now.me;
	}

	calScore();
}
int main() {
	freopen("sample_input.txt", "r", stdin);

	input();
	process();
	cout << ans;

	return 0;
}