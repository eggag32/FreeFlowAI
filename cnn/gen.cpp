#include <bits/stdc++.h>
using namespace std;
const int dx[4] = {0, 1, 0, -1};
const int dy[4] = {1, 0, -1, 0};

mt19937 rng((long long)std::chrono::steady_clock::now().time_since_epoch().count());

//size of grid, max number of paths
int n = 6, c = 7;

void print_grid(vector<vector<int>>& grid){
        for(int i = 0; i < n; i++){
                for(int j = 0; j < n; j++){
                        cout << grid[i][j] << " ";
                }
                cout << "\n";
        }
        cout << "-----------\n";
}

//write data to file
void write_data(const vector<vector<vector<vector<float>>>>& grids, 
              const vector<vector<vector<float>>>& solutions, 
              const string& filename) {
        ofstream file(filename);

        for(int i = 0; i < grids.size(); i++){
                for(int j = 0; j < c; j++){
                        for(int k = 0; k < n; k++){
                                for(int l = 0; l < n; l++){
                                        file << grids[i][j][k][l] << " ";
                                }
                        }
                }
                file << "\n";
                for(int k = 0; k < n; k++){
                        for(int l = 0; l < n; l++){
                                file << solutions[i][k][l] << " ";
                        }
                }
                file << "\n";
        }

        file.close();
}

vector<vector<vector<vector<float>>>> grids;
vector<vector<vector<float>>> solutions;

//add data point in right format
void add_sol(vector<vector<int>>& vis, vector<vector<int>>& ins, int cc){
        vector<vector<vector<float>>> grid(c, vector<vector<float>>(n, vector<float>(n, 0)));
        vector<vector<float>> sol(n, vector<float>(n, 0));
        vector<int> p(cc - 1, -1);
        int ind = 0;
        for(int i = 0; i < n; i++){
                for(int j = 0; j < n; j++){
                        if(ins[i][j] && p[ins[i][j] - 1] == -1) p[ins[i][j] - 1] = ind++;
                }
        }
        for(int i = 0; i < n; i++){
                for(int j = 0; j < n; j++){
                        if(ins[i][j] > 0) grid[p[ins[i][j] - 1]][i][j] = 1;
                        sol[i][j] = p[vis[i][j] - 1];
                }
        }
        grids.push_back(grid);
        solutions.push_back(sol);
}

//fill up the rest of the grid
int fill_end(vector<vector<int>>& vis, vector<vector<int>>& ins, int cc, int s = 1){
        int bad = 0;
	while(true){
		int mn = 1e9;
		for(int i = 0; i < n; i++) mn = min(mn, *min_element(vis[i].begin(), vis[i].end()));
		if(mn > 0) break;
		vector<pair<int, int>> emptyCells;
		for(int i = 0; i < n; i++) for(int j = 0; j < n; j++){
			if(!vis[i][j]) emptyCells.push_back({i, j});
		}
		if(emptyCells.size() < 2){
			bad = 1;
			break;
		}
		int idx = rng() % emptyCells.size(); 
		queue<pair<int, int>> q;
		q.push(emptyCells[idx]);
		vector<vector<int>> vis2 = vis;
		vis2[emptyCells[idx].first][emptyCells[idx].second] = 1;
		vector<pair<int, int>> pos;
		while(!q.empty()){
			pair<int, int> cur = q.front();
			q.pop();
			for(int j = 0; j < 4; j++){
				int x = cur.first + dx[j], y = cur.second + dy[j];
				if(x < 0 || x >= n || y < 0 || y >= n) continue;
				if(vis2[x][y]) continue;
				vis2[x][y] = 1;
				pos.push_back({x, y});
				q.push({x, y});
			}
		}
		if(pos.size() < 2){
			bad = 1;
			break;
		}
		pair<int, int> prev = pos.back();
		vector<vector<int>> vis3 = vis;
		vis3[pos.back().first][pos.back().second] = 1;
		q.push(pos.back());
		pos.clear();
		vector<vector<pair<int, int>>> pre(n, vector<pair<int, int>>(n, {-1, -1}));
		while(!q.empty()){
			pair<int, int> cur = q.front();
			q.pop();
			for(int j = 0; j < 4; j++){
				int x = cur.first + dx[j], y = cur.second + dy[j];
				if(x < 0 || x >= n || y < 0 || y >= n) continue;
				if(vis3[x][y]) continue;
				vis3[x][y] = 1;
				pre[x][y] = {cur.first, cur.second};
				pos.push_back({x, y});
				q.push({x, y});
			}
		}
                pair<int, int> cur = pos.back();
                if(pos.size() > 5){
                        cur = pos[pos.size() - (1 + rng() % 5)];
                }
		ins[cur.first][cur.second] = cc;
                int plen = 1;
		while(cur != prev){
			vis[cur.first][cur.second] = cc;
			cur = pre[cur.first][cur.second];
                        plen++;
		}
		vis[prev.first][prev.second] = cc;
		ins[prev.first][prev.second] = cc;
		cc++;
                if(plen <= 3 && s){
                        bad = 1;
                        break;
                }
	}
	if(bad) return 0;
	if(cc > c + 1) return 0;
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			int num = 0;
			for(int k = 0; k < 4; k++){
				int x = i + dx[k], y = j + dy[k];
				if(x < 0 || x >= n || y < 0 || y >= n) continue;
				if(ins[i][j] == ins[x][y] && ins[i][j] != 0){
					bad = 1;
				}
			}
		}
	}
	if(bad) return 0;
	return cc;
}

void gen1(int numPts){
        int cnt = 0;
        int ctr = 0;
        while(cnt < numPts){
                if(ctr % 10000 == 0) cerr << "1: " << cnt << "\n";
                ctr++;
                vector<vector<int>> vis(n, vector<int>(n, 0));
                vector<vector<int>> ins(n, vector<int>(n, 0));
                int num = 3 + rng() % 3;
                int bad = 0;
                int cc = 1;
                for(int i = 0; i < num; i++){
                        vector<pair<int, int>> emptyCells;
                        for(int i = 0; i < n; i++) for(int j = 0; j < n; j++){
                                if(!vis[i][j]) emptyCells.push_back({i, j});
                        }
                        if(emptyCells.size() < 2){
                                bad = 1;
                                break;
                        }
                        int idx = rng() % emptyCells.size(); 
                        queue<pair<int, int>> q;
                        q.push(emptyCells[idx]);
                        vector<vector<int>> vis2 = vis;
                        vis2[emptyCells[idx].first][emptyCells[idx].second] = 1;
                        vector<pair<int, int>> pos;
                        vector<vector<pair<int, int>>> pre(n, vector<pair<int, int>>(n, {-1, -1}));
                        while(!q.empty()){
                                pair<int, int> cur = q.front();
                                q.pop();
                                for(int j = 0; j < 4; j++){
                                        int x = cur.first + dx[j], y = cur.second + dy[j];
                                        if(x < 0 || x >= n || y < 0 || y >= n) continue;
                                        if(vis2[x][y]) continue;
                                        vis2[x][y] = 1;
                                        pos.push_back({x, y});
                                        pre[x][y] = {cur.first, cur.second};
                                        q.push({x, y});
                                }
                        }
                        if(pos.size() < 2){
                                bad = 1;
                                break;
                        }
                        int idx2 = rng() % pos.size();
                        vector<pair<int, int>> path;
                        pair<int, int> cur = pos[idx2];
                        ins[cur.first][cur.second] = cc;
                        while(cur != emptyCells[idx]){
                                path.push_back(cur);
                                cur = pre[cur.first][cur.second];
                        }
                        path.push_back(emptyCells[idx]);
                        ins[emptyCells[idx].first][emptyCells[idx].second] = cc;
                        for(pair<int, int> p : path){
                                vis[p.first][p.second] = cc;
                        }
                        cc++;
                }
                if(bad) continue;
                vector<vector<int>> vis2 = vis;
                vector<vector<int>> ins2 = ins;
                for(int _ = 0; _ < 10; _++){
                        vis = vis2;
                        ins = ins2;
                        if(cnt == numPts) break;
                        int b = fill_end(vis, ins, cc, 0);
                        if(!b) continue;
                        add_sol(vis, ins, b);
                        cnt++;
                }
        }
}

void gen2(int numPts){
        int cnt = 0;
        int ctr = 0;
        while(cnt < numPts){
                if(ctr % 10000 == 0) cerr << "2: " << cnt << "\n";
                ctr++;
                vector<vector<int>> vis(n, vector<int>(n, 0));
                vector<vector<int>> ins(n, vector<int>(n, 0));
                int num = 3 + rng() % 3;
                int bad = 0;
                int cc = 1;
                for(int i = 0; i < num; i++){
                        vector<pair<int, int>> emptyCells;
                        for(int i = 0; i < n; i++) for(int j = 0; j < n; j++){
                                if(!vis[i][j]) emptyCells.push_back({i, j});
                        }
                        if(emptyCells.size() < 2){
                                bad = 1;
                                break;
                        }
                        int idx = rng() % emptyCells.size();
                        int len = 2 + rng() % 5;
                        vis[emptyCells[idx].first][emptyCells[idx].second] = cc;
                        ins[emptyCells[idx].first][emptyCells[idx].second] = cc;
                        pair<int, int> cur = emptyCells[idx];
                        while(len--){
                                int rd = rng() % 4, ok = 0;
                                int l = 1;
                                for(int j = 0; j < 4; j++){
                                        int x = cur.first + dx[(rd + j) % 4], y = cur.second + dy[(rd + j) % 4];
                                        if(x < 0 || x >= n || y < 0 || y >= n) continue;
                                        if(vis[x][y]) continue;
                                        int adj = 0;
                                        for(int k = 0; k < 4; k++){
                                                int xx = x + dx[k], yy = y + dy[k];
                                                if(xx < 0 || xx >= n || yy < 0 || yy >= n) continue;
                                                if(vis[xx][yy] == cc) adj++;
                                        }
                                        if(adj > 1) continue;
                                        vis[x][y] = cc;
                                        cur = make_pair(x, y);
                                        ok = 1;
                                        l++;
                                }
                                if(!ok || !len){
                                        if(l < 3) bad = 1;
                                        ins[cur.first][cur.second] = cc++;
                                        break;
                                }
                        }
                }
                if(bad) continue;
                vector<vector<int>> vis2 = vis;
                vector<vector<int>> ins2 = ins;
                for(int _ = 0; _ < 30; _++){
                        vis = vis2;
                        ins = ins2;
                        if(cnt == numPts) break;
                        int b = fill_end(vis, ins, cc, 0);
                        if(!b) continue;
                        add_sol(vis, ins, b);
                        cnt++;
                }
        }
}

void gen3(int numPts){
        int cnt = 0;
        int ctr = 0;
        while(cnt < numPts){
                if(ctr % 10000 == 0) cerr << "3: " << cnt << "\n";
                ctr++;
                vector<vector<int>> vis(n, vector<int>(n, 0));
                vector<vector<int>> ins(n, vector<int>(n, 0));
                int num = 3 + rng() % 3;
                int bad = 0;
                int cc = 1;
                for(int i = 0; i < num; i++){
                        vector<pair<int, int>> emptyCells;
                        for(int i = 0; i < n; i++) for(int j = 0; j < n; j++){
                                if(!vis[i][j]) emptyCells.push_back({i, j});
                        }
                        if(emptyCells.size() < 2){
                                bad = 1;
                                break;
                        }
                        int idx = rng() % emptyCells.size();
                        int len = 2 + rng() % 5;
                        vis[emptyCells[idx].first][emptyCells[idx].second] = cc;
                        ins[emptyCells[idx].first][emptyCells[idx].second] = cc;
                        pair<int, int> cur = emptyCells[idx];
                        while(len--){
                                int rd = rng() % 4, ok = 0;
                                int l = 1;
                                for(int j = 0; j < 4; j++){
                                        int x = cur.first + dx[(rd + j) % 4], y = cur.second + dy[(rd + j) % 4];
                                        if(x < 0 || x >= n || y < 0 || y >= n) continue;
                                        if(vis[x][y]) continue;
                                        int adj = 0;
                                        for(int k = 0; k < 4; k++){
                                                int xx = x + dx[k], yy = y + dy[k];
                                                if(xx < 0 || xx >= n || yy < 0 || yy >= n) continue;
                                                if(vis[xx][yy] == cc) adj++;
                                        }
                                        if(adj > 1) continue;
                                        vis[x][y] = cc;
                                        cur = make_pair(x, y);
                                        ok = 1;
                                        l++;
                                }
                                if(!ok || !len){
                                        if(l < 3) bad = 1;
                                        ins[cur.first][cur.second] = cc++;
                                        break;
                                }
                        }
                }
                if(bad) continue;
                vector<vector<int>> vis2 = vis;
                vector<vector<int>> ins2 = ins;
                for(int _ = 0; _ < 50; _++){
                        vis = vis2;
                        ins = ins2;
                        if(cnt == numPts) break;
                        int b = fill_end(vis, ins, cc);
                        if(!b) continue;
                        add_sol(vis, ins, b);
                        cnt++;
                }
        }
}

void gen4(int numPts){
        int cnt = 0;
        int ctr = 0;
        while(cnt < numPts){
                if(ctr % 10000 == 0) cerr << "4: " << cnt << "\n";
                ctr++;
                vector<vector<int>> vis(n, vector<int>(n, 0));
                vector<vector<int>> ins(n, vector<int>(n, 0));
                int num = 2 + rng() % 2;
                int bad = 0;
                int cc = 1;
                for(int i = 0; i < num; i++){
                        vector<pair<int, int>> emptyCells;
                        for(int i = 0; i < n; i++) for(int j = 0; j < n; j++){
                                if(!vis[i][j]) emptyCells.push_back({i, j});
                        }
                        if(emptyCells.size() < 2){
                                bad = 1;
                                break;
                        }
                        int idx = rng() % emptyCells.size();
                        int segNum = 2 + rng() % 2;
                        pair<int, int> cur = emptyCells[idx];
                        int pd = rng() % 4;
                        for(int j = 0; j < segNum; j++){
                                int len = 2 + rng() % 3;
                                int delta = rng() % 2 ? 1 : 3;
                                int dir = (pd + delta) % 4;
                                if(!j){
                                        vis[cur.first][cur.second] = cc;
                                        ins[cur.first][cur.second] = cc;
                                }
                                while(len--){
                                        cur = {cur.first + dx[dir], cur.second + dy[dir]};
                                        if(cur.first < 0 || cur.first >= n || cur.second < 0 || cur.second >= n){
                                                bad = 1;
                                                break;
                                        }
                                        if(vis[cur.first][cur.second]){
                                                bad = 1;
                                                break;
                                        }
                                        vis[cur.first][cur.second] = cc;
                                }
                                if(!bad && j == segNum - 1){
                                        ins[cur.first][cur.second] = cc++;
                                }
                                pd = dir;
                        }
                }
                if(bad) continue;
                vector<vector<int>> vis2 = vis;
                vector<vector<int>> ins2 = ins;
                for(int _ = 0; _ < 20; _++){
                        if(cnt == numPts) break;
                        vis = vis2;
                        ins = ins2;
                        int b = fill_end(vis, ins, cc);
                        if(!b) continue;
                        add_sol(vis, ins, b);
                        cnt++;
                }
        }
}

int main(){
        ios_base::sync_with_stdio();
        cin.tie(0);
        vector<string> f = {"6x6.txt", "test_6x6.txt"};
        int num = 100000;
        for(int i = 0; i < 2; i++){
                gen1(num);
                gen2(num);
                gen3(num);
                gen4(num);
                write_data(grids, solutions, f[i]);
                grids.clear();
                solutions.clear();
        }
        return 0;
}