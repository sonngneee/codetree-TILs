#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

//3명 이상이 한 팀
//맨 앞에 있는 사람을 머리사람, 맨 뒤에 있는 사람을 꼬리사람이라고 합니다.
//각 팀의 이동 선은 끝이 이어져있습니다.각 팀의 이동 선은 서로 겹치지 않습니다.
//
//1.머리사람을 따라서 한 칸 이동
//2. 각 라운드마다 공이 정해진 선을 따라 던져집니
//n개의 행, n개의 열이 주어진다고 했을 때 공이 던져지는 선
//4n번째 라운드를 넘어가는 경우에는 다시 1번째 라운드의 방향으로 돌아갑니다
//
//3. 공이 던져지는 경우에 해당 선에 사람이 있으면
//최초에 만나게 되는 사람만이 공을 얻게 되어 점수를 얻게 됩니다.
//>> 머리사람을 시작으로 팀 내에서 k번째 사람이라면 k의 제곱만큼 점수를 얻게 됩니다.
//무도 공을 받지 못하는 경우에는 아무 점수도 획득하지 못합니다.
//
//공을 획득한 팀의 경우에는 머리사람과 꼬리사람이 바뀝니다.즉 방향을 바꾸게 됩니다.

int N, M, K; // 격자, 팀 수, 라운드 수
int map[21][21];
int team_map[21][21];
int team_score[6];

int dy[4] = { 0, -1, 0, 1 };
int dx[4] = { 1, 0, -1, 0 };

struct pos {
	int y, x;
};

struct poscnt {
	int y, x, cnt;
};
struct ht {
	pos head, tail;
};

ht head_tail[6];	// 각 팀의 머리, 꼬리 정보

vector<pos> location[6]; // 각 팀의 경로 저장(머리-나머지-꼬리-4..)

bool isOutRange(int x, int y) {
	return !(x >= 0 && x < N && y >= 0 && y < N);
}
int tcnt[6]; // 꼬리의 위치 (앞에서 tcnt + 1번째)

void Input() {
	cin >> N >> M >> K;
	queue<pos> hd;
	queue<pos> tl;
	
	// 머리, 꼬리 위치 저장
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> map[i][j];
			if (map[i][j] == 1) hd.push({ i, j });
			else if (map[i][j] == 3) tl.push({ i, j });
		}
	}

	int cnt = 1;
	pos pre, now, next;
	// 같은팀 파악
	// 처음엔 무조건 2로 가기! => dfs 대신에 bfs로 경로 저장해볼까?
	// => 그럴거면 위에서 저장한 hd 위치에서 bfs 시작
	// team_map도 업데이트
	
	for (int i = 0; i < M; i++) {
		vector<pos> route;
		int visited[21][21] = {0};
		int tailcnt = 0;

		pre = hd.front();
		hd.pop();
		head_tail[cnt].head = pre;

		queue<pos>q;
		q.push(pre);
		visited[pre.y][pre.x] = 1;

		while (!q.empty()) {
			now = q.front();
			q.pop();
			route.push_back(now);
			team_map[now.y][now.x] = cnt;
			if (map[now.y][now.x] == 3) {
				head_tail[cnt].tail = now;
				tcnt[i + 1] = tailcnt;
			}
			tailcnt++;

			for (int d = 0; d < 4; d++) {
				next = { now.y + dy[d], now.x + dx[d] };
				if (isOutRange(next.y, next.x)) continue;
				if (map[next.y][next.x] == 0 || visited[next.y][next.x] == 1) continue;
				if (((pre.y == now.y) && (pre.x == now.x)) && map[next.y][next.x] != 2) continue;

				q.push(next);
				visited[next.y][next.x] = 1;			
			}
		}
		location[cnt] = route;
		cnt++;
	}
}


void Move() {
	for (int i = 1; i <= M; i++) {
		// 각 팀 이동
		// 팀 location 이동
		int last = location[i].size() - 1;
		pos temp = location[i][last];

		for (int z = last; z >0; z--) {
			location[i][z] = location[i][z - 1];
		}
		location[i][0] = temp;

		// 팀 location이동에 따른 map 업데이트
		for (int l = 0; l <= last;l++) {
			pos upnow = location[i][l];
			if (l == 0) {
				map[upnow.y][upnow.x] = 1;
			}
			else if (l < tcnt[i]) {
				map[upnow.y][upnow.x] = 2;
			}
			else if (l == tcnt[i]) {
				map[upnow.y][upnow.x] = 3;
			}
			else {
				map[upnow.y][upnow.x] = 4;
			}		
		}
	}
}


int bfs(pos n) {

	queue<poscnt>q;
	poscnt next, now;
	int visited[21][21] = { 0 };
	q.push({n.y, n.x, 1});
	visited[n.y][n.x] = 1;
	while (!q.empty()) {
		now = q.front();
		q.pop();
		if (map[now.y][now.x] == 1) {
			return now.cnt;
		}

		for (int i = 0; i < 4; i++) {
			next = { now.y + dy[i], now.x + dx[i] };
			if (next.y < 0 || next.x < 0 || next.y >= N || next.x >= N) continue;
			if (map[next.y][next.x] == 4 || map[next.y][next.x] == 0) continue;
			if (map[next.y][next.x] == 3) continue;
			q.push({ next.y, next.x, now.cnt + 1 });
			visited[next.y][next.x] = 1;
		}
	}
}

void Updatdmap(int tnum) {

	int teamsize = location[tnum].size();
	vector<pos> temp;
	for (int i = 0; i < teamsize; i++) {
		temp.push_back(location[tnum][(tcnt[tnum] - i + teamsize) % teamsize]);
	}
	location[tnum] = temp;

	// 팀 location이동에 따른 map 업데이트
	for (int l = 0; l < teamsize; l++) {
		pos upnow = location[tnum][l];
		if (l == 0) {
			map[upnow.y][upnow.x] = 1;
		}
		else if (l < tcnt[tnum]) {
			map[upnow.y][upnow.x] = 2;
		}
		else if (l == tcnt[tnum]) {
			map[upnow.y][upnow.x] = 3;
		}
		else {
			map[upnow.y][upnow.x] = 4;
		}
	}
}

void Ball(int dir, int rc) {
	pos st = { 0, 0 };
	pos now = { 0, 0 };
	int isCrush = 0;

	if (dir == 0) st = { rc, 0 };
	else if (dir == 1) st = { N - 1, rc };
	else if (dir == 2) st = { N - 1, N - rc-1 };
	else st = { 0, N - rc-1 };

	for (int i = 0; i < N; i++) {
		now.y = st.y + i* dy[dir];
		now.x = st.x + i* dx[dir];
		if (map[now.y][now.x] == 0 || map[now.y][now.x] == 4) continue;
		isCrush = 1;
		break;
	}
	if (isCrush == 0) return;
	// now에있는놈이 몇번째 놈인지 체크
	int chk = bfs(now);
	int crushTeam = team_map[now.y][now.x];
	team_score[crushTeam] += (chk * chk);

	// 좌표 변경
	Updatdmap(crushTeam);

}
void Process() {
	for (int k = 1; k <= K; k++) {
		Move();
		Ball((k-1)/N, (k+6)%N);
	}
}

void Ans() {
	int ans =0 ;
	for (int i = 1; i <= M; i++) {
		ans += team_score[i];
	}
	cout << ans;
}

int main() {

	// freopen("input.txt", "r", stdin);
	Input();
	Process();
	Ans();

	return 0;
}