#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>

using namespace std;

int N, M, K, C; 
// 격자의 크기 n, 박멸이 진행되는 년 수 m, 제초제의 확산 범위 k, 제초제가 남아있는 년 수 c
int map[21][21];

int drug[21][21]; // 땅에 제초제 남아있는 수, 나무의 제초제 남은 수

struct pos {
	int y, x;
};

struct findpos {
	int y, x, cnt;
	vector<pos> v;
};

int ans; // 죽은 나무의 수

int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, 1, 0, -1 };

int fdy[4] = { -1, 1, 1, -1 };
int fdx[4] = { 1, 1, -1, -1 };

vector<pos> treepos; // 1* 나무의 pos 저장할 벡터

findpos drubtree;	// 제초제 뿌릴 나무 정보

pos Nextpos(pos now, int dir) {
	return { now.y + dy[dir], now.x + dx[dir] };
}

bool isRange(pos now) {
	return (now.y >= 0 && now.x >= 0 && now.y < N && now.x < N);
}

void Input() {
	cin >> N >> M >> K >> C;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> map[i][j];
			if (map[i][j] > 0) {
				treepos.push_back({ i, j });
			}
		}
	}
}

void Init() {
	// init + 제초제 -1
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (drug[i][j] == 0) continue;
			drug[i][j]--;
		}
	}

	// 나무 위치 초기화 + 추가
	treepos.clear();
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (map[i][j] > 0) {
				treepos.push_back({ i, j });
			}
		}
	}
	drubtree = {}; // 초기화 이렇게 하는거 맞나..
}

void Growth() {
	int temp[21][21] = { 0 };
	int treescnt = treepos.size();
	pos now;
	pos next;

	for (int i = 0; i < treescnt; i++) {
		now = treepos[i];	// 현재 나무 위치

		for (int d = 0; d < 4; d++) {
			next = Nextpos(now, d);
			if (!isRange(next)) continue;
			// 제초제, 성있는 곳, 빈칸
			if (map[next.y][next.x] <= 0 || drug[next.y][next.x] > 0) continue;

			temp[now.y][now.x]++;
		}
	}
	// 성장은 모든 나무에게 동시에 일어납니다.
	for (int i = 0; i < treescnt; i++) {
		now = treepos[i];	// 현재 나무 위치
		map[now.y][now.x] += temp[now.y][now.x];
	}
}

void Breeding() {
	int empty[21][21] = { 0 };
	int treescnt = treepos.size();
	pos now;
	pos next;
	int son = 0;
	vector<pos> possible;

	for (int i = 0; i < treescnt; i++) {
		now = treepos[i];	// 현재 나무 위치
		son = 0;
		possible.clear();

		for (int d = 0; d < 4; d++) {
			next = Nextpos(now, d);
			if (!isRange(next)) continue;
			if (map[next.y][next.x] == -1 || map[next.y][next.x] != 0 || drug[next.y][next.x] > 0) continue;
			son++; // 번식이 가능한 칸의 개수
			possible.push_back(next);
		}

		int extree = 0;
		if (son == 0) {
			extree = 0;
		}
		else {
			extree = map[now.y][now.x] / son;	// 번식할 나무의 개수
		}


		// int extree = map[now.y][now.x] / son;	// 번식할 나무의 개수
		for (int p = 0; p < son; p++) {
			empty[possible[p].y][possible[p].x] += extree;
		}
	}

	treepos.clear();
	// map 업데이트
	for (int r = 0; r < N; r++) {
		for (int c = 0; c < N; c++) {
			map[r][c] += empty[r][c];
			if (map[r][c] > 0) treepos.push_back({ r, c });
		}
	}

}

bool cmp(findpos a, findpos b) {
	if (a.cnt == b.cnt) {
		if (a.y == b.y) {
			return a.x < b.x;
		}
		return a.y < b.y;
	}
	return a.cnt > b.cnt;
};

void Find() {
	int treescnt = treepos.size();
	int deadcnt = 0; // += map[][]
	int temp[21][21] = { 0 };	// 해당 위치에 뿌렸을 때, 죽을 나무의 수 저장
	pos now;
	pos next;
	int dou = 0;

	vector<findpos> forsort;	// sort 용

	vector<pos> ddlist; // 제초제 뿌릴 위치 저장용

	for (int i = 0; i < treescnt; i++) {
		now = treepos[i];
		deadcnt = map[now.y][now.x];
		ddlist.clear();
		ddlist.push_back(now);

		for (int d = 0; d < 4; d++) {
			dou = 0;
			while (dou < K) {
				dou++;
				next = { now.y + dou * fdy[d], now.x + dou * fdx[d] };// 대각선 방향으로 한칸씩
				if (!isRange(next)) break;

				if (map[next.y][next.x] == 0) {
					ddlist.push_back(next);
					break;
				}
				else if (map[next.y][next.x] == -1) break;

				ddlist.push_back(next);
				deadcnt += map[next.y][next.x];
			}
		}
		forsort.push_back({ now.y, now.x, deadcnt, ddlist });
	}
	std::sort(forsort.begin(), forsort.end(), cmp);
	drubtree = forsort[0]; // 제초제 뿌릴 나무 위치
}

void Drop() {
	ans += drubtree.cnt;
	for (auto dr : drubtree.v) {
		drug[dr.y][dr.x] = C+1;
		map[dr.y][dr.x] = 0;
	}
}


void Process() {
	for (int year = 0; year < M; year++) {
		if (year != 0) {
			Init();
		}

		if (treepos.size() == 0) return;
		// 성장
		Growth();

		// 번식
		Breeding();

		//// 제초제 위치 선정
		Find();

		//// 제초제 뿌리기
		Drop();
	}
}

int main() {

	//freopen("input.txt", "r", stdin);
	Input();
	Process();
	cout << ans;
	return 0;
}