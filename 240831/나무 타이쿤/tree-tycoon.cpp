#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <queue>
#include <cstring>
using namespace std;

int n, m; // 격자 크기, 나무 키우는 년수
int map[16][16];
int d, p; // 이동 방향, 이동 칸 수

int dy[8] = { 0, -1, -1, -1, 0, 1, 1, 1 };
int dx[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };

struct yearInfo {
	int dir, move;
};

yearInfo years[101];

struct pos {
	int y, x;
};

vector<pos> vitapos;
int vitamap[16][16];

int ans;
int tempmap[16][16];

void input() {
	cin >> n >> m;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
		{
			cin >> map[i][j];
		}
	}
	int d, p;
	for (int i = 0; i < m; i++) {
		cin >> d >> p;
		years[i] = { d - 1, p };
	}
	for (int i = 1; i <= 2; i++) {
		for (int j = 0; j < 2; j++) {
			vitamap[n - i][j] = 1;
			vitapos.push_back({ n - i, j });
		}
	}
}

// pp: put pos
void crossVita(pos pp) {
	
	pos np;
	
	//  3. 대각선 위치 확인 -> 높이가 1이상이면 해당 나무 높이 ++ (벽 넘어가기 불가능)
	for (int i = 1; i < 8; i += 2) {
		np.y = pp.y + dy[i];
		np.x = pp.x + dx[i];

		if (np.y < 0 || np.y >= n || np.x < 0 || np.x >= n) continue;
		if (map[np.y][np.x] < 1) continue;

		tempmap[pp.y][pp.x]++;
	}
}

void moveVita(yearInfo moveinfo) {
	//	1. 영양제를 이동 규칙에 따라 이동 (벽 넘어가기 가능)
	int vcnt = vitapos.size();
	pos nowvita, next;

	for (int i = 0; i < vcnt; i++) {
		nowvita = vitapos[i];

		// 비타민 맵 이동
		// 비타민 위치 리스트에 추가 nowvita.push
		vitamap[nowvita.y][nowvita.x] --;
		
		next.y = ((nowvita.y + moveinfo.move * dy[moveinfo.dir]) + n) % n;
		next.x = ((nowvita.x + moveinfo.move * dx[moveinfo.dir]) + n) % n;

		vitamap[next.y][next.x] ++;

		//	2. 특수 영양제를 투입 -> 높이 1 증가
		map[next.y][next.x]++;
		vitapos[i] = next;
	}

	for (int i = 0; i < n; i++) {
		memset(tempmap[i], 0, sizeof(tempmap[i]));
	}

	for (int i = 0; i < vcnt; i++) {
		crossVita(vitapos[i]);
	}

	for (int i = 0; i < vcnt; i++) {
		map[vitapos[i].y][vitapos[i].x] += tempmap[vitapos[i].y][vitapos[i].x];
	}
}

void cutTree() {
	//  4. 영양제 투입 위치 제외하고 -> 높이가 2 이상이면 -> 높이 -2 하고, 영양제 올려둠
	vitapos.clear();

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (vitamap[i][j] == 1) {
				vitamap[i][j] = 0;
				continue;
			}
			if (map[i][j] < 2) continue;
			map[i][j] -= 2;
			vitapos.push_back({i, j});
			vitamap[i][j] = 1;
		}
	}
}

void process(yearInfo year) {
	moveVita(year);
	cutTree();
}

void score() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (map[i][j] == 0) continue;
			ans += map[i][j];
		}
	}
}

int main() {
	
	//freopen("sample_input.txt", "r", stdin);

	input();
	for (int y = 0; y < m; y++) {
		process(years[y]);
	}

	score();
	cout << ans;
	return 0;
}