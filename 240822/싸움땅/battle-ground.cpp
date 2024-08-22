#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include<vector>
#include <algorithm>

using namespace std;

int n, m, k; // 격자 크기, 플레이어 수, 라운드 수

int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, 1, 0, -1 };

vector<int> map_gun[21][21]; // 총공격력
int map_player[21][21]; // 플레이어 번호 맵 >> 플레이어 움직일 때마다 업데이트 
int score_player[21]; // // 플레이어에 따른 포인트 arr : 플레이어 번호 1부터 시작

struct player_info {
	int num, y, x, d, s, isGun, gunS; // num, y, x, 방향, 총기여부, 총 공격력
	// num : 1부터 시작
};

vector<player_info> players;


void input() {
	cin >> n >> m >> k; // 격자 크기, 플레이어 수, 라운드 수 
	int gun = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> gun;
			if (gun == 0) continue;
			map_gun[i][j].push_back(gun);
		}
	}

	int x = 0, y = 0, d = 0, s = 0;

	for (int i = 0; i < m; i++) {
		cin >> y >> x >> d >> s; // (x, y)는 플레이어의 위치, d는 방향, s는 플레이어의 초기 능력치
		players.push_back({ i+1, y-1, x-1, d, s, 0, 0 });
		map_player[y-1][x-1] = (i + 1);
	}
}

bool cmp(int a, int b) {
	return a > b;
}

void change_gun(player_info* winP, int ny, int nx) {

	sort(map_gun[ny][nx].begin(), map_gun[ny][nx].end(), cmp);

	// 총이 없다면 ??
	if (winP->isGun == 0) {
		winP->isGun = 1;
		winP->gunS = map_gun[ny][nx].front();
		map_gun[ny][nx].erase(map_gun[ny][nx].begin());

	}
	else if (map_gun[ny][nx].empty()) return;
	else{
		if (winP->gunS < map_gun[ny][nx][0]) {
			int temp = map_gun[ny][nx].front();
			map_gun[ny][nx][0] = winP->gunS;
			winP->gunS = temp;		
		}
	}
	// gun sort
	// 제일 공격력이 센 거
	// 승리한 칸에 떨어져 있는 총들과 원래 들고 있던 총 중 가장 공격력이 높은 총을 획득
	// 나머지 총들은 해당 격자에 내려 놓습니다.

}

void fight(player_info* nowP, int ny, int nx) {

	player_info* fightP = &players[map_player[ny][nx]-1]; // 싸울 플레이어
	player_info* winP; // 이긴 플레이어
	player_info* loseP; // 진플레이어

	int nowP_score = nowP->s + nowP->gunS;
	int figP_score = fightP->s + fightP->gunS;

	
	if (nowP_score > figP_score) {
		winP = nowP;
		loseP = fightP;
	}
	else if (nowP_score < figP_score) {
		winP = fightP;
		loseP = nowP;

	}

	else {
		// 일 이 수치가 같은 경우에는 플레이어의 초기 능력치가 높은 플레이어가 승리
		if (nowP->s > fightP->s) {
			winP = nowP;
			loseP = fightP;
		}
		else {
			winP = fightP;
			loseP = nowP;
		}
	}

	//	2-1-1. 이긴 플레이어는 각 플레이어의 초기 능력치와 가지고 있는 총의 공격력의 합의 차이만큼을 포인트로 획득하게 됩니다.
	//			승리한 칸에 떨어져 있는 총들과 원래 들고 있던 총 중 가장 공격력이 높은 총을 획득	
	// `		나머지 총들은 해당 격자에 내려 놓습니다.

	score_player[winP->num] += abs(nowP_score - figP_score);

	//	2-1-2. 진 플레이어는 본인이 가지고 있는 총을 해당 격자에 내려놓고
	//			해당 플레이어가 원래 가지고 있던 방향대로 한 칸 이동
	//			다른 플레이어가 있거나 격자 범위 밖인 경우에는 오른쪽으로 90도씩 회전하여 빈 칸이 보이는 순간 이동
	//				만약 해당 칸에 총이 있다면, 해당 플레이어는 가장 공격력이 높은 총을 획득하고 나머지 총들은 해당 격자에 내려 놓습니다.
	//		

		// 맵 업데이트
	map_player[winP->y][winP->x] = 0;
	map_player[loseP->y][loseP->x] = 0;

	winP->y = ny;
	winP->x = nx;

	map_player[winP->y][winP->x] = winP->num;


	if (loseP->isGun != 0) {
		loseP->isGun = 0;
		map_gun[ny][nx].push_back(loseP->gunS);
		loseP->gunS = 0;
	}

	int ny_l = 0, nx_l = 0;

	for (int i = 0; i < 4; i++) {
		ny_l = ny + dy[loseP->d];
		nx_l = nx + dx[loseP->d];

		if (map_player[ny_l][nx_l] != 0 || ny_l < 0 || nx_l < 0 || ny_l >= n || nx_l >= n) {
			// 90도 회전
			loseP->d = (loseP->d + 1) % 4;
			continue;
		}

		loseP->y = ny_l;
		loseP->x = nx_l;

		map_player[loseP->y][loseP->x] = loseP->num;
		break;
	}

	if (!map_gun[loseP->y][loseP->x].empty()) {
		change_gun(loseP, loseP->y, loseP->x);
	}

	if (!map_gun[winP->y][winP->x].empty()) {
		change_gun(winP, ny, nx);
	}

}




void move(int np) {
	
	int ny = 0, nx = 0;
	player_info* nowP = &players[np];
	ny = nowP->y + dy[nowP->d];
	nx = nowP->x + dx[nowP->d];

	//  1. 첫 번째 플레이어부터 순차적으로 본인이 향하고 있는 방향대로 한 칸만큼 이동 /
	//		만약 해당 방향으로 나갈 때 격자를 벗어나는 경우에는 정반대 방향으로 방향을 바꾸어서 1만큼 이동합니다.

	if (ny < 0 || nx < 0 || ny >= n || nx >= n) {
		nowP->d = (nowP->d + 2) % 4; // 방향 바꾸기
		ny = nowP->y + dy[nowP->d];
		nx = nowP->x + dx[nowP->d];
	}

	if (map_player[ny][nx] != 0) {
		// 플레이어가 있는 경우 => 싸움
		fight(nowP, ny, nx);
		return;

	}
	else if (! map_gun[ny][nx].empty()) {
		// 총이 있는 경우
		change_gun(nowP, ny, nx);
	}

	// 맵 업데이트
	map_player[nowP->y][nowP->x] = 0;

	nowP->y = ny;
	nowP->x = nx;

	map_player[nowP->y][nowP->x] = nowP->num;
	
}


void process() {
	for (int r = 0; r < k; r++) {
		for (int p = 0; p < m; p ++ ) {
			move(p);
		}
	}
}

int main() {

	//freopen("sample_input.txt", "r", stdin);

	input();
	process();

	for (int i = 1; i <= m; i++) {
		cout << score_player[i] << ' ';
	}
	return 0;
}