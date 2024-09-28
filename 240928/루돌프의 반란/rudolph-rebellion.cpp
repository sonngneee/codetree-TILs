#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include<algorithm>
#include <vector>
#include <cmath>

using namespace std;

int N, M, P, C, D;    //맵크기, 게임턴수,산타수,루돌프힘,산타힘
int map[51][51]; // 산타 번호 , 루돌프 번호 -1

struct pos {
    int y, x, dir;
};
struct info {
    int y, x, dir, sleep;
};
int out[41];    // 탈락 산타 1, 0
int score[41]; //산타 점수
pos dol;
info santa[41]; // 산타 번호에 따른 info

int dyd[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
int dxd[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
int dys[4] = { -1, 0, 1, 0 };
int dxs[4] = { 0, 1, 0, -1 };

int sleep[41];

void Input() {
    cin >> N >> M >> P >> C >> D;

    int a, b, n;
    cin >> a >> b;

    dol = { a - 1, b - 1, -1 };
    map[dol.y][dol.x] = -1;

    for (int i = 0; i < P; i++) {
        cin >> n >> a >> b;
        santa[n] = { a - 1, b - 1, -1, 0 };
        map[a - 1][b - 1] = n;
    }

}

bool cmp(pos a, pos b) {
    if (a.y == b.y) {
        return a.x > b.x;
    }
    return a.y > b.y;
}

void Interaction(pos pre, pos now) {
    
    pos next;
    int presantanum = map[pre.y][pre.x];
    int nowsantanum = map[now.y][now.x];

    map[pre.y][pre.x] = 0;
    map[now.y][now.x] = presantanum;
    santa[presantanum] = { now.y, now.x, santa[presantanum].dir, santa[presantanum].sleep };

    pos temp = now;

    while (true) {
        presantanum = nowsantanum;
        next.y = temp.y + dyd[temp.dir];
        next.x = temp.x + dxd[temp.dir];
        next.dir = temp.dir;
        

        if (next.y < 0 || next.x < 0 || next.y >= N || next.x >= N) {
            out[map[temp.y][temp.x]] = 1;
            break;
        }
        nowsantanum = map[next.y][next.x];

        if (nowsantanum == 0) {
            map[next.y][next.x] = presantanum;
            santa[presantanum] = { next.y, next.x, next.dir };
            break;
        }

        map[next.y][next.x] = map[temp.y][temp.x];
        santa[map[temp.y][temp.x]] = { next.y, next.x, next.dir };
        temp = next;
    }

}


void Crush(int num, pos now, int snum) {
    // 루돌프 움직여서 충돌
    pos nextS;
    if (num == 1) {
        // now = 루돌프가 이동할 위치에 있는 산타

        int nowSantanum = map[now.y][now.x];
        score[nowSantanum] += C;
        sleep[nowSantanum] = 2;

        nextS = { now.y + C * dyd[now.dir], now.x + C * dxd[now.dir], now.dir };

        if (nextS.y < 0 || nextS.x < 0 || nextS.y >= N || nextS.x >= N) {
            // 벽 밖으로 나갈 경우
            out[nowSantanum] = 1;
            map[dol.y][dol.x] = 0;
            map[now.y][now.x] = -1;
            dol = now;


            return;
        }
        // 다른 산타가 있을 때, 상호작용
        if (map[nextS.y][nextS.x] != 0) {
            
            Interaction(now, nextS);
        }
        map[dol.y][dol.x] = 0;
        map[now.y][now.x] = -1;
        dol = now;
        map[nextS.y][nextS.x] = nowSantanum;
        santa[nowSantanum].y = nextS.y;
        santa[nowSantanum].x = nextS.x;


    }

    else {
        score[snum] += D;
        santa[snum].dir = (santa[snum].dir + 4) % 8;

        nextS.y = now.y + D*dyd[santa[snum].dir];
        nextS.x = now.x + D*dxd[santa[snum].dir];
        nextS.dir = santa[snum].dir;
        
        sleep[snum] = 2;

        if (nextS.y < 0 || nextS.x < 0 || nextS.y >= N || nextS.x >= N) {
            // 벽 밖으로 나갈 경우
            out[snum] = 1;
            map[santa[snum].y][santa[snum].x] = 0;
            santa[snum] = { nextS.y, nextS.x, nextS.dir, 2 };

            return;
        }

        map[santa[snum].y][santa[snum].x] = 0;
        if (map[nextS.y][nextS.x] != 0) {

            Interaction({santa[snum].y, santa[snum].x, santa[snum].dir}, nextS);
        }
        
        map[nextS.y][nextS.x] = snum;
        santa[snum].y = nextS.y;
        santa[snum].x = nextS.x;

    }


}


void MoveDol() {
    int minlen = 1000;
    int len = 0;
    vector<pos> selectS;
    pos targetS;
    pos nextD, moveD;

    for (int p = 1; p <= P; p++) {
        // 모든 산타와의 거리 계산
        if (out[p] == 1) continue; //죽은 산타 제외
        len = pow((dol.y - santa[p].y), 2) + pow((dol.x - santa[p].x), 2);

        if (len > minlen) continue;
        else if (len < minlen) {
            selectS.clear();
            selectS.push_back({ santa[p].y, santa[p].x, 0 });
            minlen = len;
        }
        else {
            selectS.push_back({ santa[p].y, santa[p].x, 0 });
        }
    }
    std::sort(selectS.begin(), selectS.end(), cmp);
    targetS = selectS[0];    // 산타 결정

    minlen = 1000;

    // 산타로 갈 수 있는 방향 결정
    for (int i = 0; i < 8; i++) {
        nextD.y = dol.y + dyd[i];
        nextD.x = dol.x + dxd[i];
        nextD.dir = i;

        len = pow((targetS.y - nextD.y), 2) + pow((targetS.x - nextD.x), 2);
        if (len < minlen) {
            moveD = nextD;
            minlen = len;

        }
    }

    // 루돌프 이동!
    // 

    // 충돌있을 경우
    if (map[moveD.y][moveD.x] != 0) {
        Crush(1, moveD, 0);

    }

    // 빈 칸일 경우
    else {
        //// 루돌프 맵 업데이트
        map[dol.y][dol.x] = 0;

        //// 루돌프 위치, 방향 저장
        dol = moveD;

        map[dol.y][dol.x] = -1;

    }

}



void MoveSanta(int snum) {
    pos move = { -1, -1, -1 };
    pos next;
    int minlen = pow((dol.y - santa[snum].y), 2) + pow((dol.x - santa[snum].x), 2);
    int len = 0;

    for (int i = 0; i < 4; i++) {
        next.y = santa[snum].y + dys[i];
        next.x = santa[snum].x + dxs[i];

        if (next.y < 0 || next.x < 0 || next.y >= N || next.x >= N) continue;
        if (map[next.y][next.x] > 0) continue; //다른 산타 있는 경우

        len = pow((dol.y - next.y), 2) + pow((dol.x - next.x), 2);

        if (len < minlen) {
            move = { next.y, next.x, i * 2 };
            minlen = len;
        }
    }
    if (move.y == -1) return; // 움직일수 있는 방향이 없을 경우

    // 움직이기!

    // 움직인 방향에 루돌프 있는 경우
    if (map[move.y][move.x] == -1) {
        santa[snum].dir = move.dir;

        Crush(2, move, snum);

    }

    // 빈 공간인 경우
    else {
        map[santa[snum].y][santa[snum].x] = 0;
        map[move.y][move.x] = snum;
        santa[snum] = { move.y, move.x, move.dir };
    }
}

void Process() {
    // 게임 턴수만큼

    for (int m = 0; m < M; m++) {
        int alive = 0;

        for (int i = 1; i <= P; i++) {
            if (sleep[i] > 0) {
                sleep[i]--;
            }
        }

        MoveDol();
        
        for (int p = 1; p <= P; p++) {

            // 탈락한 산타, 기절한 넘기기
            if (out[p] == 1) continue;
            if (sleep[p] > 0) {
                continue;
            }
            
            MoveSanta(p);
        }
        

        // 살아남은 산타 점수주기
        for (int p = 1; p <= P; p++) {
            if (out[p] == 1) continue; // 탈락한 산타는 넘기기
            alive++;
            score[p]++;
        }

        if (alive == 0) {
            for (int p = 1; p <= P; p++) {
                cout << score[p]<< ' ' ;
                return;
            }
        }
    }
    for (int p = 1; p <= P; p++) {
        cout << score[p] << ' ';
    }
}


int main() {

    //freopen("input.txt", "r", stdin);

    Input();
    Process();
    return 0;
}