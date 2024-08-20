#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <queue>
#include <algorithm>

using namespace std;



int map[21][21];
int N;
int scoreF[5] = { 0, 1, 10, 100, 1000 };

int ans;
int friends[442][442];

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

pos students[442]; // 학생 위치

buddy now;

vector<infomap> forsort;



void input() {
	cin >> N;
	buddy ip;
	while (cin >> ip.me >> ip.b[0] >> ip.b[1] >> ip.b[2] >> ip.b[3]) {
		likebuddy.push(ip);
		friends[ip.me][ip.b[0]] = 1;
		friends[ip.me][ip.b[1]] = 1;
		friends[ip.me][ip.b[2]] = 1;
		friends[ip.me][ip.b[3]] = 1;
	}
}

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

void calScore() {
	int cntF = 0;
	int ny = 0, nx = 0;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			cntF = 0;

			for (int d = 0; d < 4; d++) {
				ny = i + dy[d];
				nx = j + dx[d];

				if (ny<1 || nx<1 || ny>N || nx>N) continue;
				if (friends[map[i][j]][map[ny][nx]] == 1) {
					cntF++;
				}
			}
			ans += scoreF[cntF];
		}
	}
}

void check_likeF(buddy nowC) {
	// 격자를 벗어나지 않는 4방향으로 인접한 칸 중 앉아있는 좋아하는 친구의 수가 가장 많은 위치로 => like map check
	pos nowF;
	int ny=0, nx = 0;
	int likeCntmap[21][21] = { 0 };
	vector<pos> likecntV[5];

	for (int i = 0; i < 4; i++) {
		nowF = students[nowC.b[i]]; // 친한친구 위치
		for (int j = 0; j < 4; j++) {
			ny = nowF.y + dy[j];
			nx = nowF.x + dx[j];
			if (ny<1 || nx<1 || ny>N || nx>N) continue;
			if (map[ny][nx] != 0) continue;
			likeCntmap[ny][nx] += 1;
			// 끝나기 전에 체크하는 방법은 없나? 흠 ...
			likecntV[likeCntmap[ny][nx]].push_back({ ny, nx });

		}
		
	}

	for (int i = 4; i >= 0; i--) {
		if (likecntV[i].size() == 0) continue;
		for (pos c : likecntV[i]) {
			forsort.push_back({ i, 0, c.y, c.x }); // lbcnt, emptycnt, y, x; // 좋아하는 친구 수, 비어있는 칸의 수
		}
		break;
	}
}

void check_emptyC() {
	// 만약 1번 조건을 만족하는 칸의 위치가 여러 곳이라면, 
	// 그 중 인접한 칸 중 비어있는 칸의 수가 가장 많은 위치로 갑니다.
	// 단 이때 격자를 벗어나는 칸은 비어있는 칸으로 간주하지 않습니다.
	int ny = 0, nx = 0;
	int empty_cnt = 0;

	for (int ce = 0; ce < forsort.size();ce++) {
		empty_cnt = 0;
		for (int i = 0; i < 4; i++) {
			ny = forsort[ce].y + dy[i];
			nx = forsort[ce].x + dx[i];
			if (ny<1 || nx<1 || ny>N || nx>N) continue;
			if (map[ny][nx] > 0) continue;
			empty_cnt++;
		}
		forsort[ce].emptycnt = empty_cnt;
	}
}

void process() {
	// 학생들은, 입력으로 주어진 순서대로 탑승

	while (!likebuddy.empty()) {
		now = likebuddy.front(); // me, {b1,b2,b3,b4}
		likebuddy.pop();


		check_likeF(now);
		if (forsort.size() > 0) {

			check_emptyC();

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