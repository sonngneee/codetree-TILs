#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

using namespace std;


//[0]
//회전중심좌표(1~n - 1) >> 90, 180, 270 다 해보고 >> 중심좌표에 따른 각 가치 기록 >> max인 좌표로 유물 획득
//
//[함수]
//
//중심위치에 따른 회전 함수
//bfs 가치 기록하는 함수
//>> 해당 map, 획득할 유물 pos vector 을 담는 vector
//
//
//
//[1] 탐사
//선택된 격자는 시계 방향으로 90도, 180도, 270도 중 하나의 각도만큼 회전시킬 수 있습니다
//
//* sort or priority_queue
//
//회전목표
//
//(1) 유물 1차 획득 가치를 최대화
//(2) 회전한 각도가 가장 작은 방법
//(3) 회전 중심 좌표의 열이 가장 작은 구간
//(4) 열이 같다면 행이 가장 작은 구간을 선택
//
//>> 선택한 격자마다 >> 90, 180, 270도 회전 >> 1, 2, 3, 4 비교해서 sort
//
//
//
//[2] 유물 1차 획득
//
//상하좌우로 인접한 같은 종류의 유물 조각은 서로 연결되어 있습니다
//이 조각들이 3개 이상 연결된 경우, 조각이 모여 유물이 되고 사라집니다
//유물의 가치 = 모인 조각의 개수
//>> 조각 사라지면서 >> 채우기 위해 "pos 저장하는 vector"에 추가
//
//사라진 이후
//
//>> "pos 저장하는 vector" sort
//빈공간 > 유적의 벽면에 새로 생겨나는 조각 순서대로 새로운 조각 생김
//
//빈공간의
//(1) 열 번호가 작은 순
//(2) 행 번호가 큰 순
//순으로 채워짐
//
//
//>> 벽면에 써있는 숫자 > queue
//
//[3] 유물연쇄획득
//
//더 이상 조각이 3개 이상 연결되지 않아 유물이 될 수 없을 때까지[2] 반복
//
//
//
//* *[1]~[3]이 1턴
//* *각 턴마다 획득한 유물의 가치의 총합을 출력하는 프로그램
//* *K번의 턴을 진행하지 못했지만, 탐사 진행 과정에서 어떠한 방법을 사용하더라도 유물을 획득할 수 없었다면 모든 탐사는 그 즉시 종료
//>> 이 경우 얻을 수 있는 유물이 존재하지 않음으로, 종료되는 턴에 아무 값도 출력하지 않음


#include <vector>
#include <queue>
#include <algorithm>

int K, M; // 탐사 횟수, 벽 유물조각 수

int map[6][6];
queue<int> new_remain; // 새로운 유물


struct pos {
    int y, x;
};

struct info {
    int v, a, c, r; // 가치, 각도, 열, 행,
    vector<pos> l;  // 사라질 유물 위치
    vector<vector<int>> m;    // 그때의 tempmap
};

vector<info> forSelectR;

struct posNroute {
    int y, x, num;
    vector<pos> l;
};

int isBreak = 0;
int ansC = 0;


bool cmp(info a, info b){
    if (a.v == b.v) {
        if (a.a == b.a) {
            if (a.c == b.c) {
                return a.r < b.r;
            }
            return a.c < b.c;
        }
        return a.a < b.a;
    }
    return a.v > b.v;
}

int tempmap[6][6];
int tempminimap[3][3];

int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, 1, 0, -1 };

void input() {
    
    cin >> K >> M;
    
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            cin >> map[i][j];
        }
    }
    int a;
    for (int i = 0; i < M; i++) {
        cin >> a;
        new_remain.push(a);
    }
}



// 미니맵 회전
void minimapRotation(int r) {
    int tmm[3][3] = { 0 };

    // 270
    if (r == 3) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                tmm[i][j] = tempminimap[j][3 - i - 1];
            }
        }

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                tempminimap[i][j] = tmm[i][j];
            }
        }
    }

    for (int rt = 0; rt < r; rt++) {

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                tmm[i][j] = tempminimap[3 - j - 1][i];
            }
        }

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                tempminimap[i][j] = tmm[i][j];
            }
        }

    }
}

// 미니맵 회전해서 tempmap에 붙여넣기
void minimapR(int row, int col, int r) {
    // r = 90, 180, 270 에 따라 minimap 회전하고, tempmap에 저장

    // tempmap, tempminimap 초기화
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            tempmap[i][j] = map[i][j];
        }
    }
    int a = 0;
    int b = 0;
    for (int i = row - 1; i <= row + 1; i++) {
        b = 0;
        for (int j = col - 1; j <= col + 1; j++) {
            tempminimap[a][b] = map[i][j];
            b++;
        }
        a++;
    }

    // 미니맵 회전
    minimapRotation(r);

    // tempmap 에 tempminimap 붙여넣기
    a = 0;
    for (int i = row - 1; i <= row + 1; i++) {
        b = 0;
        for (int j = col - 1; j <= col + 1; j++) {
            tempmap[i][j] = tempminimap[a][b];
            b++;
        }
        a++;
    }
}

// tempmap 에서 bfs
void bfs(vector<pos>* R, int* C) {
    
    int firstC = *C;

    // // 사라질 좌표, 개수 저장해야 하는데 마리지..
    // forSelectR에 추가하셈~!

    // int y, x, num;    vector<pos> l;
    queue<posNroute>q;
    posNroute now, next;
    int visited[5][5] = { 0 };
    int cnt = 0;
    vector<pos> route_save;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (visited[i][j] == 1)continue;
            route_save.clear();
            cnt = 0;
            
            now.y = i;
            now.x = j;
            now.num = tempmap[i][j];

            visited[i][j] = 1;
            q.push(now);
            
            
            while (!q.empty()) {
                now = q.front();
                q.pop();
                cnt++;
                route_save.push_back({ now.y, now.x });
               
                for (int d = 0; d < 4; d++) {
                    next.y = now.y + dy[d];
                    next.x = now.x + dx[d];
                    next.num = tempmap[next.y][next.x];

                    if (next.y < 0 || next.x < 0 || next.y>=5 || next.x >= 5) continue;
                    if (visited[next.y][next.x] == 1) continue;
                    if (next.num != now.num) continue;

                    q.push(next);
                    visited[next.y][next.x] = 1;
                }
            }

            if (cnt < 3) continue;
            *C += cnt;
            for (int i = 0; i < route_save.size(); i++) {
                R->push_back(route_save[i]);
            }
            isBreak = 1;
        }
    } 
}

void findR() {
    //[0]
    //회전중심좌표(1~n - 1) >> 90, 180, 270 다 해보고 >> 중심좌표에 따른 각 가치 기록 >> max인 좌표로 유물 획득
    //
    //[함수]
    //
    //중심위치에 따른 회전 함수
    //bfs 가치 기록하는 함수
    //>> 해당 map, 획득할 유물 pos vector 을 담는 vector
    //
    //
    //
    //[1] 탐사
    //선택된 격자는 시계 방향으로 90도, 180도, 270도 중 하나의 각도만큼 회전시킬 수 있습니다
    //
    //* sort or priority_queue
    //
    //회전목표
    //
    //(1) 유물 1차 획득 가치를 최대화
    //(2) 회전한 각도가 가장 작은 방법
    //(3) 회전 중심 좌표의 열이 가장 작은 구간
    //(4) 열이 같다면 행이 가장 작은 구간을 선택
    //
    //>> 선택한 격자마다 >> 90, 180, 270도 회전 >> 1, 2, 3, 4 비교해서 sort
    forSelectR.clear();
    vector<pos> routeT;
    int cntTt = 0;
    vector<vector<int>> savemap (6, vector<int>(6));
    int cutTt = 0;


    for (int i = 1; i < 4; i++) {
        for (int j = 1; j < 4; j++) {
            // 각 중심좌표(i, j) 마다 회전 3번씩
            for (int r = 1; r <= 3; r++) {
                routeT.clear();
                cutTt = max(cntTt, cutTt);
                cntTt = 0;

                minimapR(i, j, r);
                bfs(&routeT, &cntTt); // 사라질 좌표, 개수 저장해야 하는데 마리지..

                if (cntTt == 0) continue;
                if (cutTt > cntTt) continue;

                for (int a = 0; a < 5; a++) {
                    for (int b = 0; b < 5; b++) {
                        savemap[a][b] = tempmap[a][b];
                    }
                }
                forSelectR.push_back({ cntTt, r, j, i, routeT, savemap });
            }
        }
    }

    std::sort(forSelectR.begin(), forSelectR.end(), cmp);
}

bool cmp_rc(pos a, pos b) {
    if (a.x == b.x) {
        return a.y > b.y;
    }
    return a.x < b.x;
}

void getR() {
    //[2] 유물 1차 획득
    //
    //상하좌우로 인접한 같은 종류의 유물 조각은 서로 연결되어 있습니다
    //이 조각들이 3개 이상 연결된 경우, 조각이 모여 유물이 되고 사라집니다
    //유물의 가치 = 모인 조각의 개수
    //>> 조각 사라지면서 >> 채우기 위해 "pos 저장하는 vector"에 추가
    //
    //사라진 이후
    //
    //>> "pos 저장하는 vector" sort
    //빈공간 > 유적의 벽면에 새로 생겨나는 조각 순서대로 새로운 조각 생김
    //
    //빈공간의
    //(1) 열 번호가 작은 순
    //(2) 행 번호가 큰 순
    //순으로 채워짐
    //
    //>> 벽면에 써있는 숫자 > queue
    //
    // new_remain 에서 front, pop하면서 채워넣기!
    // 
    
    //forSelectR[0] // 핏한 유물정보..
    // 맵 업그레이드
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            map[i][j] = forSelectR[0].m[i][j];
        }
    }
    // 빈공간 채우기
    std::sort(forSelectR[0].l.begin(), forSelectR[0].l.end(), cmp_rc);
    int nR = 0;

    for (int i = 0; i < forSelectR[0].l.size(); i++) {
        nR = new_remain.front();
        new_remain.pop();

        map[forSelectR[0].l[i].y][forSelectR[0].l[i].x] = nR;
    }
}

void get_secondR(vector<pos>* DR) {
    // 빈공간 채우기
    std::sort(DR->begin(), DR->end(), cmp_rc);

    int nR = 0;

    for (int i = 0; i < DR->size(); i++) {
        nR = new_remain.front();
        new_remain.pop();
        map[DR[0][i].y][DR[0][i].x] = nR;

    }

}

void process() {
    for (int k = 0; k < K; k++) {
        isBreak = 0;
        ansC = 0;

        findR(); // 유물 찾기
        if (forSelectR.size() == 0) {
            return;
        }
        getR();  // 1차 유물 얻기
        
        if (isBreak == 0) {
            //1턴 마무리
            cout << ansC << ' ';
            continue;
        }

        ansC = forSelectR[0].v;
        
        // 연쇄유물찾기
        while (isBreak) {
            isBreak = 0;
            vector<pos> delete_route;
            // 유물 찾기

            for (int i = 0; i < 5; i++) {
                for (int j = 0; j < 5; j++) {
                    tempmap[i][j] = map[i][j];
                }
            }
            

            bfs(&delete_route, &ansC);
            // 유물얻기
            if (isBreak == 0) break;
            get_secondR(&delete_route);
        }
        cout << ansC << ' ';

    }

}



int main() {

    // freopen("input.txt", "r", stdin);

    input();
    process();

    return 0;
}

//int map[3][3] = { {1, 2, 3}, {4, 5,6}, {7, 8, 9} };
//int tempmap[3][3];
//
//int main() {
//	for (int i = 0; i < 3; i++) {
//		for (int j = 0; j < 3; j++) {
//			// tempmap[i][j] = map[3 - j - 1][i]; // 90회전
//			tempmap[i][j] = map[j][3 - i - 1]; // 반시계 90
//		}
//	}
//
//	return 1;
//}