#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <algorithm>

using namespace std;

int N, M, K;
int arrivemap[11][11], attackmap[11][11];    // 살아있는 포탑, 공격에 영향받은 포탑, 
int dy[4] = { 0, 1, 0, -1 };
int dx[4] = { 1, 0, -1, 0 };

struct info {
    int score, time;    // 공격력, 공격시점
};

struct forsort {
    int y, x, score, k, plus;
};

info poinfo[11][11]; // 포탑 정보

vector<forsort> pov;    // 살아있는 포탑

forsort attackppl;
forsort targetppl;

struct pos {
    int y, x;
};

struct forbfs {
    int y, x, cnt;
    vector<pos> r;
};

int edy[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
int edx[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };

int map[11][11];

void Makepov() {
    pov.clear();
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (arrivemap[i][j] == 0) continue;
            pov.push_back({ i, j, poinfo[i][j].score, poinfo[i][j].time, i + j });
        }
    }
}

void TEST() {
    memset(map, 0, sizeof(map));
    for (int i = 0; i < pov.size(); i++) {
        map[pov[i].y][pov[i].x] = pov[i].score;
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cout << map[i][j] << ' ';
        }
        cout << '\n';
    }
    cout << "------------------------" << '\n';
}

void Input() {
    cin >> N >> M >> K;
    int s;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cin >> s;
            if (s > 0) {
                arrivemap[i][j] = 1;
                poinfo[i][j] = { s, 0 };
            }
        }
    }
   // 살아있는 포탑의 벡터 만들기
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (arrivemap[i][j] == 0) continue;
            pov.push_back({ i, j, poinfo[i][j].score, 0, i + j });
        }
    }
}

bool cmpA(forsort a, forsort b) {
    if (a.score == b.score) {
        if (a.k == b.k) {
            if (a.plus == b.plus) {
                return a.x > b.x;
            }
            return a.plus > b.plus;
        }
        return a.k > b.k; // 최근에 공격한 포탑 : 내림차순(큰수부터)
    }
    return a.score < b.score;
}

bool cmpT(forsort a, forsort b) {
    if (a.score == b.score) {
        if (a.k == b.k) {
            if (a.plus == b.plus) {
                return a.x < b.x;
            }
            return a.plus < b.plus;
        }
        return a.k < b.k; // 오래전에 공격한 포탑 : 오름차순(작은것부터)
    }
    return a.score > b.score;
}

forsort AttackPP(int atime) {
    std::sort(pov.begin(), pov.end(), cmpA);
    forsort ap;
    ap = pov[0];

    // 공격력 증가
    poinfo[ap.y][ap.x].score += (N + M);
    ap.score = poinfo[ap.y][ap.x].score;

    return ap;
}

forsort TargetPP() {
    std::sort(pov.begin(), pov.end(), cmpT);
    forsort tp;
    tp = pov[0];

    if ((attackppl.y == tp.y) && (attackppl.x == tp.x)) {
        tp = pov[1];
    }
    return tp;
}

bool isRange(int y, int x) {
    return (y >= 0 && x >= 0 && y < N && x < M);
}

void BomB() {
    pos st, ed;
    st = { attackppl.y, attackppl.x };
    ed = { targetppl.y, targetppl.x };
    int rscore = poinfo[st.y][st.x].score;
    pos next;

    attackmap[st.y][st.x] = 1;

    poinfo[ed.y][ed.x].score -= rscore;
    if (poinfo[ed.y][ed.x].score <= 0) {
        poinfo[ed.y][ed.x].score = 0;
        arrivemap[ed.y][ed.x] = 0;
    }
    attackmap[ed.y][ed.x] = 1;

    for (int i = 0; i < 8; i++) {
        next.y = ((ed.y + edy[i]) + N)% N;
        next.x = ((ed.x + edx[i]) + M)% M;

        if (next.y == st.y && next.x == st.x) continue;
        if (arrivemap[next.y][next.x] == 0) continue;

        poinfo[next.y][next.x].score -= (rscore / 2);
        attackmap[next.y][next.x] = 1;

        if (poinfo[next.y][next.x].score <= 0) {
            poinfo[next.y][next.x].score = 0;
            arrivemap[next.y][next.x] = 0;
        }
    }
}

vector<pos> Findroute() {
    int visited[11][11] = {0};
    int min_val = 21e8;
    vector<pos> targetroute;

    forbfs now, next;
    pos st, ed;
    st = { attackppl.y, attackppl.x };
    ed = { targetppl.y, targetppl.x };

    //now = { st.y, st.x, 1, {} };
    now.y = st.y;
    now.x = st.x;
    now.cnt = 1;
    now.r = {};
    now.r.push_back({ st.y, st.x });
    visited[now.y][now.x] = 1;

    queue<forbfs>q;
    q.push(now);
    int isRoute = 0;

    while (!q.empty()) {
        now = q.front();
        q.pop();

        // cnt min 관련 로직
        if ((now.y == ed.y) && (now.x == ed.x)) {
            if (now.cnt < min_val) {
                min_val = now.cnt;
                targetroute = now.r;
                isRoute = 1;
            }
        }
        for (int d = 0; d < 4; d++) {
            next.y = ((now.y + dy[d]) + N) % N;
            next.x = ((now.x + dx[d]) + M) % M;

            if (visited[next.y][next.x] == 1 || arrivemap[next.y][next.x] == 0) continue;

            next.cnt = now.cnt + 1;
            next.r = now.r;
            next.r.push_back({ next.y, next.x });

            q.push(next);
            visited[next.y][next.x] = 1;
        }
    }
    if (!isRoute) {
        return targetroute;
    }
    else {
        return targetroute;
    }

}

void Laser(vector<pos> R, int k) {
    int rsize = R.size();
    int rscore = 0;

    for (int r = 0; r < rsize; r++) {
        if (r == 0) {
            poinfo[R[r].y][R[r].x].time = k;
            rscore = poinfo[R[r].y][R[r].x].score;
            attackmap[R[r].y][R[r].x] = 1;
            continue;
        }
        if (r == rsize - 1) {
            poinfo[R[r].y][R[r].x].score -= rscore;
        }
        else {
            poinfo[R[r].y][R[r].x].score -= (rscore / 2);
        }
        
        attackmap[R[r].y][R[r].x] = 1;

        if (poinfo[R[r].y][R[r].x].score <= 0) {
            poinfo[R[r].y][R[r].x].score = 0;
            arrivemap[R[r].y][R[r].x] = 0;
        }
    }
}

void Attack(int k) {
    /*3 - 1. 레이저 공격
        상하좌우의 4개의 방향으로 움직일 수 있습니다.
        부서진 포탑이 있는 위치는 지날 수 없습니다.
        가장자리에서 막힌 방향으로 진행하고자 한다면, 반대편으로 나옵니다.
        (예를 들어, 위의 예시에서(2, 3)에서 오른쪽으로 두번 이동한다면, 
        (2, 3) -> (2, 4) -> (2, 1) 순으로 이동합니다.)

        레이저 공격은 공격자의 위치에서 공격 대상 포탑까지의 최단 경로로 공격력
        경로의 길이가 똑같은 최단 경로가 2개 이상이라면,

        우 / 하 / 좌 / 상의 우선순위대로 먼저 움직인 경로가 선택됩니다.

        공격 대상에는 공격자의 공격력 만큼의 피해를 입히며,
        피해를 입은 포탑은 해당 수치만큼 공격력이 줄어듭니다.
        레이저 경로에 있는 포탑도 공격을 받게 되는데,
        이 포탑은 공격자 공격력의 절반 만큼의 공격을 받습니다.
        (절반이라 함은 공격력을 2로 나눈 몫을 의미합니다.)

        그러한 경로가 존재하지 않는다면
        = >> 포탄 공격*/
    memset(attackmap, 0, sizeof(attackmap));

    vector<pos> route;
    route = Findroute();

    if (route.size() == 0) {
        //cout << "bomb!" << '\n';
        BomB();
        Makepov();
        //TEST();

    }
    else {
        // 루트 따라 부서짐!
        //cout << "laser!" << '\n';
        Laser(route, k);
        Makepov();
        //TEST();
    }
}

void Clean() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (attackmap[i][j] == 1 || arrivemap[i][j] == 0) continue;
            poinfo[i][j].score++;
        }
    }
}

void Process() {
    //TEST();
    for (int k = 1; k < K+1; k++) {
        if (pov.size() == 1) {
            return;
        }
        attackppl = AttackPP(k);    // 공격자 선정

        //cout << "공격자:" << attackppl.y << "," << attackppl.x << '\n';
        targetppl = TargetPP();     // 대상 선정

        //cout << "대상:" << targetppl.y << "," << targetppl.x << '\n';
        Attack(k);
        
        Clean();
        
        Makepov();
        //TEST();
    }

}

void Print() {
    std::sort(pov.begin(), pov.end(), cmpT);
    cout << pov[0].score;
}

int main() {
    //freopen("input.txt", "r", stdin);
    Input();
    Process();
    Print();

    return 0;
}