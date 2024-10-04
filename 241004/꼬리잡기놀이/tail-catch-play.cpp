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
int team_cnt = 0;

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

void Input() {
	cin >> N >> M >> K;
	queue<pos> hd;
	queue<pos> tl;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> map[i][j];
			if (map[i][j] == 1) hd.push({ i, j });
			else if (map[i][j] == 3) tl.push({ i, j });
		}
	}

	int visited[21][21];
	int cnt = 1;
	pos now, next;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (map[i][j] == 0) continue;
			if (visited[i][j] == 1) continue;
			
			queue<pos>q;
			q.push({i, j});
			visited[i][j] = 1;
			
			while (!q.empty()) {
				now = q.front();
				q.pop();
				team_map[now.y][now.x] = cnt;
				for (int d = 0; d < 4; d++) {
					next = { now.y + dy[d], now.x + dx[d] };
					if (next.y < 0 || next.x < 0 || next.y >= N || next.x >= N) continue;
					if (visited[next.y][next.x] == 1) continue;
					if (map[next.y][next.x] == 0) continue;

					q.push(next);
					visited[next.y][next.x] = 1;
				}
			}
			cnt++;
		}
	}
	team_cnt = cnt-1;

	pos posht;
	while (!hd.empty()) {
		posht = hd.front();
		hd.pop();
		head_tail[team_map[posht.y][posht.x]].head = { posht.y, posht.x };
	}
	while (!tl.empty()) {
		posht = tl.front();
		tl.pop();
		head_tail[team_map[posht.y][posht.x]].tail = { posht.y, posht.x };
	}

}

void Move() {
	pos now, next;
	pos save4, save23;

	for (int i = 1; i <= team_cnt; i++) {
		//i = 팀 번호
		now = head_tail[i].head;
		int visited[21][21] = {0};
		int isEnd = 0, isThree = 0;

		while (!isEnd) {
			for (int d = 0; d < 4; d++) {
				next = { now.y + dy[d], now.x + dx[d] };
				if (next.y < 0 || next.x < 0 || next.y >= N || next.x >= N) continue;
				if (visited[next.y][next.x] == 1) continue;
				if (map[next.y][next.x] == 0) continue;

				if (map[next.y][next.x] == 4) save4 = next;
				else if (map[next.y][next.x] == 2) save23 = next;
				else if (map[next.y][next.x] == 3) {
					save23 = next;
					isEnd = 1;
				}
			}
			if (map[now.y][now.x] == 1) {
				head_tail[team_map[now.y][now.x]].head = save4;
				map[save4.y][save4.x] = map[now.y][now.x];
			}
			visited[now.y][now.x] = 1;
			map[now.y][now.x] = map[save23.y][save23.x];
			if (isEnd == 1) {
				head_tail[team_map[now.y][now.x]].tail = now;
				map[save23.y][save23.x] = 4;
			}
			// map[save23.y][save23.x] = 4;
			now = save23;
			
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
			
			q.push({ next.y, next.x, now.cnt + 1 });
			visited[next.y][next.x] = 1;
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
	
	team_score[team_map[now.y][now.x]] += (chk * chk);
	pos temp = head_tail[team_map[now.y][now.x]].head;
	head_tail[team_map[now.y][now.x]].head = head_tail[team_map[now.y][now.x]].tail;
	head_tail[team_map[now.y][now.x]].tail = temp;
	map[head_tail[team_map[now.y][now.x]].head.y][head_tail[team_map[now.y][now.x]].head.x] = 1;
	map[head_tail[team_map[now.y][now.x]].tail.y][head_tail[team_map[now.y][now.x]].tail.x] = 3;
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

	//freopen("input.txt", "r", stdin);
	Input();
	Process();
	Ans();

	return 0;
}