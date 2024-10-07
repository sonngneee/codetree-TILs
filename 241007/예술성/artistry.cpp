#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

int N;
int map[30][30];	// 원래 맵
int side[900][900];	// 각 팀번호끼리 맞닿아있는 변의 수
int teammap[30][30]; // 팀 번호 적힌 맵
// 조합의 정보

struct team {
	int t1, t2;
};

struct pos {
	int y, x;
};

// 조합 체크 여부
int coorcheck[900];

// 다른 곳과 맞닿아있는 면 좌표 저장
vector<pos> sidelist[900]; 

// 팀 인원 수
int teamcnt[900];

// 팀의 원래 숫자
int teamoriginnum[900];

// 전체 팀 개수
int teamSize;

vector<team> coordination;

int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, 1, 0, -1 };

int ansScore;

int temp[30][30];


void init() {
	// 초기화 리스트 : teammap, side
	
	memset(teammap, 0, sizeof(teammap));
	memset(side, 0, sizeof(side));
	memset(coorcheck, 0, sizeof(coorcheck));
	memset(teamcnt, 0, sizeof(teamcnt));
	memset(teamoriginnum, 0, sizeof(teamoriginnum));
	memset(temp, 0, sizeof(temp));
	for (int i = 0; i < 900; i++) {
		sidelist[i].clear();
	}
	coordination.clear();
	teamSize = 0;


}
void Findteam() {
	int visited[30][30] = { 0 };
	int cnt = 0;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (visited[i][j]) continue;
			cnt++;
			int ttcnt = 0;

			queue<pos>q;
			pos now, next;
			q.push({ i, j });
			visited[i][j] = 1;

			while (!q.empty()) {
				now = q.front();
				q.pop();
				teammap[now.y][now.x] = cnt;
				ttcnt++;

				int isSame = 0;

				for (int i = 0; i < 4; i++) {
					next = { now.y + dy[i], now.x + dx[i] };
					if (next.y < 0 || next.x < 0 || next.y >= N || next.x >= N) continue;
					if (visited[next.y][next.x] == 1) continue;

					if ((! isSame) && (map[next.y][next.x] != map[now.y][now.x])) {
						isSame = 1;
						sidelist[cnt].push_back({ now.y, now.x });
						continue;
					}

					if (map[next.y][next.x] != map[now.y][now.x]) continue;

					q.push(next);
					visited[next.y][next.x] = 1;
					
				}
			}
			teamcnt[cnt] = ttcnt;
			teamoriginnum[cnt] = map[now.y][now.x];
		}
	}
	teamSize = cnt;
}

void Findteamside() {
	pos now, next;
	int sidessize = 0;
	int t1 = 0, t2 = 0;
	for (int i = 1; i <= teamSize; i++) {
		

		sidessize = sidelist[i].size();


		for (int c = 0; c < sidessize; c++) {
			now = sidelist[i][c];
			
			for (int d = 0; d < 4; d++) {
				
				next = { now.y + dy[d], now.x + dx[d] };
				if (next.y<0 || next.x<0 || next.y>=N || next.x>=N) continue;
				if ((teammap[now.y][now.x] != teammap[next.y][next.x]) && (teammap[now.y][now.x] < teammap[next.y][next.x])) {
					t1 = teammap[now.y][now.x];
					t2 = teammap[next.y][next.x];
					side[t1][t2]++;
					side[t2][t1]++;
				}
			}
		}
	}

	for (int i = 1; i <= teamSize; i++) {
		for (int j = i; j <= teamSize; j++) {
			if (side[i][j] != 0) coordination.push_back({ i, j });
		}
	}
}

void Input() {
	cin >> N;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> map[i][j];
		}
	}
}

int Score() {
	//ab = (그룹 a에 속한 칸의 수 + 그룹 b에 속한 칸의 수) x 그룹 a를 이루고 있는 숫자 값 x 그룹 b를 이루고 있는 숫자 값 x 그룹 a와 그룹 b가 서로 맞닿아 있는 변의 수
	int cal = 0;
	for (auto t : coordination) {
		cal += (teamcnt[t.t1] + teamcnt[t.t2]) * teamoriginnum[t.t1] * teamoriginnum[t.t2] * side[t.t1][t.t2];
	}
	return cal;
}

void Minisqr(int mid) {
	pos startpoint[4] = { {0, 0},{mid + 1, 0},{0, mid + 1},{mid + 1, mid + 1} };
	
	memcpy(temp, map, sizeof(map));
	for (auto sp : startpoint) {
		for (int i = 0; i < mid; i++) {
			for (int j = 0; j < mid; j++) {
				temp[sp.y + j][sp.x + mid - i -1 ] = map[sp.y + i][sp.x + j];
			}
		}
		memcpy(map, temp, sizeof(map));
	}
}



void Turn() {
	int mid = (N - 1) / 2;
	memset(temp, 0, sizeof(temp));
	// 십자가 회전
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			temp[N-j-1][i] = map[i][j];
		}
	}
	
	for (int i = 0; i < N; i++) {
		map[mid][i] = temp[mid][i];
		map[i][mid] = temp[i][mid];
	}

	// 작은 사각형 시계방향 90도
	Minisqr(mid);
}

void Process() {
	for (int t = 0; t < 3; t++) {
		
		Turn();

		init();
		// 팀 찾기
		Findteam();
		// 팀끼리 맞닿은 변의 수 + 조합 찾기
		Findteamside();

		int as = Score();
		ansScore += as;
	}
}



int main() {

	// freopen("sample_input.txt", "r", stdin);

	Input();

	// 팀 찾기
	Findteam();
	// 팀끼리 맞닿은 변의 수 + 조합 찾기
	Findteamside();

	int as = Score();
	ansScore += as;

	Process();
	cout << ansScore;
	return 0;
}