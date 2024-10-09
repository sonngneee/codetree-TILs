#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

int N, M, H, K; // 격자, 도망자, 나무, 라운드 수
int map[100][100]; // 도망자 수
int treemap[100][100]; // 나무 유무


struct pos {
	int y, x;
};

struct posd {
	int y, x, dir;
};

int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, 1, 0, -1 };

pos nextPos(pos now, int d) {
	return { now.y + dy[d], now.x + dx[d] };
}

vector<posd> catchpp;	 // 술래자의 루트
vector<posd> runpp; // 도망자들 개인의 위치, 방향

posd sul;

int score;

void Input() {
	cin >> N >> M >> H >> K;

	int x, y, d = 0;
	int num = 0;
	for (int i = 0; i < M; i++) {
		num++;
		cin >> y >> x >> d;
		map[y - 1][x - 1] = 1;	// 도망자가 몇 명 있는지
		runpp.push_back({ y - 1, x - 1, d });
	}

	for (int i = 0; i < H; i++) {
		cin >> y >> x;
		treemap[y - 1][x - 1] = 1;
	}
}

bool isRange(pos now){
	return (now.y >= 0 && now.x >= 0 && now.y < N && now.x < N);
}
void Preprocessing() {
	int mid = N / 2;
	//0. 술래 루트 + 방향 저장
	pos now = { mid, mid };
	pos next;

	catchpp.push_back({ now.y, now.x, 0 });
	int go = 0, dir = 0;

	for (int i = 0; i < 2 * N - 1; i++) {
		go = i / 2 + 1; // 몇칸 갈지

		if (i == 2 * N - 2) {
			go -= 1;
		}

		for (int g = 1; g <= go; g++) {
			next = nextPos(now, dir);

			if (g == go) {
				dir = (dir + 1) % 4;
				catchpp.push_back({ next.y, next.x, dir });
				now = next;
				break;
			}

			catchpp.push_back({ next.y, next.x, dir });
			now = next;
		}
	}

	int rSize = catchpp.size();	// 25
	for (int i = rSize - 2; i > 0; i--) {
		dir = (catchpp[i - 1].dir + 2) % 4;
		catchpp.push_back({ catchpp[i].y, catchpp[i].x, dir });
	}
}

void Run() {

	int dist = 0;
	posd now, next;
	pos nnext;
	int runsize = runpp.size();

	for (int i = 0; i < runsize; i++) {
		now = runpp[i];
		dist = abs(sul.y - now.y) + abs(sul.x - now.x);

		if (dist > 3) continue; 

		nnext = nextPos({ now.y, now.x }, now.dir); //다음 x,y
		if (!isRange(nnext)) {
			now.dir = (now.dir + 2) % 4;
			nnext = nextPos({ now.y, now.x }, now.dir);
			if (sul.y == nnext.y && sul.y == nnext.x) {
				next = now;
			}
			else {
				next = { nnext.y, nnext.x, now.dir };
			}
		}
		else {
			nnext = nextPos({ now.y, now.x }, now.dir);
			if (sul.y == nnext.y && sul.y == nnext.x) {
				next = now;
			}
			else {
				next = { nnext.y, nnext.x, now.dir };
			}
		}
		runpp[i] = next;

	}

	// 이동한 도망자들 map 업데이트
	memset(map, 0, sizeof(map));
	for (int i = 0; i < runsize; i++) {
		map[runpp[i].y][runpp[i].x]++;
	}

}

void Catch(int nowround) {
	sul = catchpp[nowround % 48];
	pos now;
	for (int i = 0; i < 3; i++) {
		now = { sul.y + i * dy[sul.dir], sul.x + i * dx[sul.dir] };
		if (!isRange(now)) break;
		if (treemap[now.y][now.x] == 1 || map[now.y][now.x] == 0) continue;
		score += map[now.y][now.x] * nowround;
		map[now.y][now.x] = 0;
	}

	// 맵에 따른 도망자 제거

	int runsize = runpp.size();
	vector<posd> temp = {};

	for (int i = 0; i < runsize; i++) {
		if (map[runpp[i].y][runpp[i].x] == 0) continue;

		temp.push_back(runpp[i]);
	}
	runpp = temp;

}

void Process() {
	sul = catchpp[0];

	for (int k = 1; k <= K; k++) {
		int runsize = runpp.size();
		if (runsize == 0) return;

		Run();
		Catch(k);
	}
}

int main() {

	// freopen("input.txt", "r", stdin);
	Input();
	Preprocessing();
	Process();
	cout << score;

	return 0;
}