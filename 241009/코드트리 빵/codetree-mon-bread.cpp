#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <cstring>

using namespace std;

int N, M;	// 격자 크기, 사람 수
int map[16][16];	// 0빈, 1 베캠
int pp[16][16]; // 맵에 위치한 사람 수
int dontgo[16][16]; // 못 가는 구역
int dontgotemp[16][16];	// 못가는 구역 업데이트 위함
int arrivepp[31]; // 도착한 사람


int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, -1, 0, 1 };

struct pos {
	int y, x;
};

struct poscnt {
	int y, x, cnt;
};

struct poscntlo {
	int y, x, cnt;
	pos n;
};

vector<pos> cu; // 사람들이 가고싶어하는 편의점 위치
vector<pos> bc;	// bc 리스트


pos targetbc;
vector<pos> onmap;	// 현재 맵 위에 있는 사람의 위치 (첫번째 사람부터 차례로 들어감) 
int bcsize;	// 베캠 크기
int isEnd;
int t;

bool cmp(pos a, pos b) {
	if (a.y == b.y) {
		return a.x < b.x;
	}
	return a.y < b.y;
}

void Input() {
	cin >> N >> M;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cin >> map[i][j];
			if (map[i][j] == 1) {
				bc.push_back({ i, j });
			}
		}
	}
	bcsize = bc.size();
	std::sort(bc.begin(), bc.end(), cmp);

	int y, x;
	for (int i = 0; i < M; i++) {
		cin >> y >> x;
		cu.push_back({ y - 1, x - 1 });

	}
}

bool isRange(pos now) {
	return (now.y >= 0 && now.x >= 0 && now.y < N && now.x < N);
}


int bfs(pos cu, pos base) {	// 거리 return;
	int visited[16][16] = { 0 };
	int cnt;

	poscnt now, next;
	queue<poscnt>q;
	q.push({cu.y, cu.x, 0});
	visited[cu.y][cu.x] = 1;
	int min_val = 21e8;

	while (!q.empty()) {
		now = q.front();
		q.pop();

		if (now.y == base.y && now.x == base.x) {
			if (now.cnt < min_val) {
				min_val = now.cnt;
			}
			continue;
		}

		for (int i = 0; i < 4; i++) {
			next = { now.y + dy[i], now.x + dx[i], now.cnt+1};
			if (!isRange({ next.y, next.x })) continue;
			if (dontgo[next.y][next.x] == 1 || visited[next.y][next.x] == 1) continue;

			q.push(next);
			visited[next.y][next.x] = 1;
		}
	}
	return min_val;
}
// 사람 num = 1 부터 

void GoBasecamp(int nowppnum) {
	/*3.
		현재 시간이 t분이고 t ≤ m를 만족한다면
		t번 사람은 자신이 가고 싶은 편의점과 가장 가까이 있는 베이스 캠프에 들어갑니다.
		(가장 가까이에 있다는 뜻 역시 1에서와 같이 최단거리에 해당하는 곳)

		최단 거리로 움직이는 방법이 여러가지라면 ↑, ←, →, ↓ 의 우선 순위로 움직이게 됩니다


		(가장 가까운 베이스캠프가 여러 가지인 경우에는 그 중 행이 작은 베이스캠프,
			행이 같다면 열이 작은 베이스 캠프로 들어갑니다.)

		t번 사람이 베이스 캠프로 이동하는 데에는 시간이 전혀 소요되지 않습니다

		이때부터 다른 사람들은 해당 베이스 캠프가 있는 칸을 지나갈 수 없게 됩

		t번 사람이 편의점을 향해 움직이기 시작했더라도
		해당 베이스 캠프는 앞으로 절대 지나갈 수 없음에 유의
		해당 턴 격자에 있는 사람들이 모두 이동한 뒤에 해당 칸을 지나갈 수 없어짐*/

	pos wantcu = cu[nowppnum - 1];	// 지금 사람이 가고싶어하는 편의점 위치

	int min_dist = 21e8;

	for (int b = 0; b < bcsize; b++) {
		int dist = bfs(wantcu, bc[b]); // 원하는 편의점 위치, 체크할 베캠 위치
		if (dist < min_dist) {
			min_dist = dist;
			targetbc = bc[b];
		}
	}

	// 확정된 targetbc
	dontgo[targetbc.y][targetbc.x] = 1;
	onmap.push_back(targetbc);
}

pos gobfs(pos st, pos ed) {
	int visited[16][16] = { 0 };
	int cnt;

	poscntlo now, next;
	queue<poscntlo>q;
	pos target;

	q.push({ st.y, st.x, 0, {} });
	visited[st.y][st.x] = 1;
	int min_val = 21e8;

	while (!q.empty()) {
		now = q.front();
		q.pop();

		if (now.y == ed.y && now.x == ed.x) {
			if (now.cnt < min_val) {
				min_val = now.cnt;
				target = now.n;
			}
			continue;
		}

		for (int i = 0; i < 4; i++) {
			next.y = now.y + dy[i];
			next.x = now.x + dx[i];
			next.cnt = now.cnt + 1;

			if (!isRange({ next.y, next.x })) continue;
			if (dontgo[next.y][next.x] == 1 || visited[next.y][next.x] == 1) continue;

			if (now.y == st.y && now.x == st.x) {
				next.n = { next.y, next.x };
			}
			else {
				next.n = now.n;
			}

			q.push(next);
			visited[next.y][next.x] = 1;
		}
	}
	return target;

}

void Move(int P) {
	// P명의 사람들 이동
	/*1.
		격자에 있는 사람들 모두가 본인이 가고 싶은 편의점 방향을 향해서 1 칸 움직
		최단거리로 움직이며
		(상하좌우 인접한 칸 중 이동가능한 칸으로만 이동하여
			도달하기까지 거쳐야 하는 칸의 수가 최소가 되는 거리)
		최단 거리로 움직이는 방법이 여러가지라면 ↑, ←, →, ↓ 의 우선 순위로 움직이게 됩니다*/

	pos targetcu, ppst;
	for (int p = 0; p < P; p++) {
		if (arrivepp[p] == 1) continue;
		ppst = onmap[p];
		targetcu = cu[p];

		onmap[p] = gobfs(ppst, targetcu);
	}
}

int endcnt;

void Checkcu(int sz) {
	for (int i = 0; i < sz;i++) {
		if (arrivepp[i] == 1) continue;

		pos nowpp = onmap[i];
		pos nowcu = cu[i];
		if (nowpp.y == nowcu.y && nowpp.x == nowcu.x) {
			dontgo[nowpp.y][nowpp.x] = 1;
			arrivepp[i] = 1;
			endcnt++;
		}
	}

}

void Process() {

	while (endcnt < M) {
		/*빵을 구하고자 하는 m명의 사람
		n* n 크기의 격자 위에서 진행

		1번 사람은 정확히 1분에, 2번 사람은 정확히 2분에, ..., m번 사람은 정확히 m 분에
		각자의 베이스캠프에서 출발하여 편의점으로 이동

		이동하는 도중 동일한 칸에 둘 이상의 사람이 위치하게 되는 경우 역시 가능함


		1.
		격자에 있는 사람들 모두가 본인이 가고 싶은 편의점 방향을 향해서 1 칸 움직
		최단거리로 움직이며
		(상하좌우 인접한 칸 중 이동가능한 칸으로만 이동하여
			도달하기까지 거쳐야 하는 칸의 수가 최소가 되는 거리)
		최단 거리로 움직이는 방법이 여러가지라면 ↑, ←, →, ↓ 의 우선 순위로 움직이게 됩니다

		2.
		편의점에 도착한다면 해당 편의점에서 멈추게 되고,
		이때부터 다른 사람들은 해당 편의점이 있는 칸을 지나갈 수 없게 됩니다
		격자에 있는 사람들이 모두 이동한 뒤에 해당 칸을 지나갈 수 없어짐에 유의
		>> 저장만 해두고 나중에 업데이트

		3.
		현재 시간이 t분이고 t ≤ m를 만족한다면
		t번 사람은 자신이 가고 싶은 편의점과 가장 가까이 있는 베이스 캠프에 들어갑니다.
		(가장 가까이에 있다는 뜻 역시 1에서와 같이 최단거리에 해당하는 곳)
		(가장 가까운 베이스캠프가 여러 가지인 경우에는 그 중 행이 작은 베이스캠프,
			행이 같다면 열이 작은 베이스 캠프로 들어갑니다.)

		t번 사람이 베이스 캠프로 이동하는 데에는 시간이 전혀 소요되지 않습니다

		이때부터 다른 사람들은 해당 베이스 캠프가 있는 칸을 지나갈 수 없게 됩

		t번 사람이 편의점을 향해 움직이기 시작했더라도
		해당 베이스 캠프는 앞으로 절대 지나갈 수 없음에 유의
		해당 턴 격자에 있는 사람들이 모두 이동한 뒤에 해당 칸을 지나갈 수 없어짐*/
		
		t++; // 지금 시간
		int movesize = onmap.size();

		// 격자에 있는 사람들 한칸씩 이동
		Move(movesize);
		
		// 편의점에 도달한다면?
		Checkcu(movesize);

		if (t <= M) {
			// 베캠으로 이동
			GoBasecamp(t);
		}

	}

}

int main() {

	//freopen("input.txt", "r", stdin);
	Input();
	Process();
	cout << t;
	return 0;
}