#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

int N, M;
int map[26][26];
int dy[4] = { 0, 1, 0, -1 };
int dx[4] = { -1, 0, 1, 0 };

struct attackinfo{
	int d, p;
};

struct pos {
	int y, x;
};

struct ddel {
	int st, ed, cnt;
};

vector<pos> position;	// 빙글빙글 좌표값
vector<int> bingmap;  // 빙글빙글 맵값

attackinfo ainfo[101]; // 명령 


int score;

void Input() {
	cin >> N >> M;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> map[i][j];
		}
	}
	attackinfo a;
	for (int i = 0; i < M; i++) {
		cin >> a.d >> a.p;
		if (a.d == 2) {
			ainfo[i] = { 0, a.p };
		}
		else if (a.d == 0) {
			ainfo[i] = { 2, a.p };
		}
		else {
			ainfo[i] = a;
		}
	}
}

void Preprocessing() {
	// 벡터에 좌표 값 저장
	// 벡터에 좌표 값에 따른 map 값 저장
	int mid = (N - 1) / 2;
	position.push_back({ mid, mid });
	bingmap.push_back({ map[mid][mid] });
	pos now = { mid, mid };
	pos next;
	int dd = 0;
	int go = 0;
	int can = 0;
	while (position.size() < N * N) {
		can = (go / 2) + 1;
		while (can > 0) {
			next = { now.y + dy[dd % 4], now.x + dx[dd % 4] };
			can--;
			if (next.x < 0) break;
			position.push_back(next);
			now = next;
			if (map[next.y][next.x] == 0) continue;
			bingmap.push_back(map[next.y][next.x]);
			// now = next;
		}
		dd++;
		go++;
	}
}

void Attack(attackinfo a) {
	//1. 플레이어는 상하좌우 방향 중 주어진 공격 칸 수만큼 몬스터를 공격하여 없앨 수 있습니다.
	//2. 비어있는 공간만큼 몬스터는 앞으로 이동하여 빈 공간을 채웁니다. (erase)
	vector<pos>at; // 공격한 위치
	pos next;
	int mid = (N - 1) / 2;
	for (int i = 1; i <= a.p; i++) {
		next = { mid + i * dy[a.d], mid + i * dx[a.d] };
		if (map[next.y][next.x] == 0) break;
		at.push_back(next);
		score += map[next.y][next.x];
	}
	int j = 0;

	vector<int>idx;	// 공격한 칸이 벡터에 있을 때의 idx
	for (int i = 0; i < position.size(); i++) {
		if (j == at.size()) break;
		if (position[i].y == at[j].y && position[i].x == at[j].x) {
			idx.push_back(i);
			j++;
		}
	}
	for (int i = at.size()-1; i >= 0; i--) {
		bingmap.erase(bingmap.begin() + idx[i]);
	}
}

void AttackF() {
	//3. 몬스터의 종류가 4번 이상 반복하여 나오면 해당 몬스터 또한 삭제 >> 동시에!!
	//	>> 뒤에서부터 삭제해서 앞으로 당기기?
	//4. 앞으로 당겨주고, 4번 이상 나오는 몬스터가 있을 경우 또 삭제
	// 5. 4번 이상 나오는 몬스터가 없을 때까지 반복
	int isEnd = 0;
	int now = 0;
	int size = 0;
	vector<int> forsave = {};
	vector<ddel> delet = {};
	int bingsize = 0;

	while (!isEnd) {
		bingsize = bingmap.size(); // 현재 남아있는 몬스터 수
		now = bingmap[bingsize - 1];
		size = 1;
		forsave.push_back(bingsize - 1);
		for (int i = bingsize - 2; i > 0; i--) {
			if (now == bingmap[i]) {
				forsave.push_back(i);
			}
			else {
				int fsize = forsave.size();
				if (fsize >= 4) {
					delet.push_back({forsave[fsize - 1], forsave[0], fsize }); // 삭제할 앞 인덱스, 뒷 인덱스
					forsave.clear();
					forsave.push_back(i);
					now = bingmap[i];
				}
				else {
					forsave.clear();
					forsave.push_back(i);
					now = bingmap[i];
				}
			}
		}
		if (delet.size() == 0) {
			isEnd = 1;
			continue;
		}

		int del_val = 0;

		// 뒤에서부터 삭제하고 앞으로 당기기
		for (int i = 0; i < delet.size(); i++) {
			del_val = bingmap[delet[i].st];
			score += del_val * delet[i].cnt;
			bingmap.erase(bingmap.begin() + delet[i].st, bingmap.begin() + delet[i].ed + 1);
		}
		delet.clear();
		forsave.clear();
	}
}
void Newmon() {
	// 6. 제가 끝난 다음에는 몬스터를 차례대로 나열했을 때 같은 숫자끼리 짝을 지어줍니다
// >> 짝을 (총 개수, 숫자의 크기)로 바꾸어서 다시 미로 속에 집어넣습니다.
//	>> 새로 생긴 배열이 원래 격자의 범위를 넘는다면 나머지 배열은 무시
	pos cntpos; // y 총개수, x 숫자의 크기
	int num = bingmap[1];
	int cnt = 1;
	vector<int> tempmap = { 0 };

	for (int i = 2; i <= bingmap.size()+1; i++) {

		if (tempmap.size() == N * N) break;

		if (i == bingmap.size()) {
			tempmap.push_back(cnt);
			if (tempmap.size() == N * N) break;
			tempmap.push_back(num);
			break;
		}

		if (bingmap[i] == num) {
			cnt++;
		}
		else {
			tempmap.push_back(cnt);
			if (tempmap.size() == N * N) break;
			tempmap.push_back(num);
			cnt = 1;
			num = bingmap[i];
		}
	}

	bingmap = tempmap;
}

void Init() {
	memset(map, 0, sizeof(map));
	int bingsize = bingmap.size();
	for (int i = 1; i < bingsize; i++) {
		map[position[i].y][position[i].x] = bingmap[i];
	}
}
void Process() {
	for (int m = 0; m < M; m++) {

		attackinfo nowA = ainfo[m];
		Attack(nowA);
		AttackF();
		Newmon();
		Init();
	}
	cout << score;
}

int main() {
	
	//freopen("input.txt", "r", stdin);

	Input();
 	Preprocessing();
	Process();

	return 0;
}