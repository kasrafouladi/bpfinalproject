#include "pretty.hpp"

struct game{

	#define rand() (abs(rand()) % 720 + time(nullptr) % 720) % 720

	int maxn = 21, N = 5, M = 17, tries = 0;

	vector<vector<int>> a, lst, ok;
	vector<vector<bool>> exs;
	vector<vector<bool>> blast;
	string modes[3] = {"timer", "infinite", "normal"}, mode;
	time_t tb;
	int pts = 0, canon, ini, rows, tl = 120;
	bool frombot;

	int dx[6] = {0, 0, 1, 1, -1, -1}, dy[6] = {2, -2, -1, 1, -1, 1};

	int rnd(){
		int res = 0, k;
		if(rand() % 8 == 0)
			res = (rand() % 5 + 1) * 16;
		k = rand() % 5 + 1;
		int cnt = 21;
		while(k == res / 16 && cnt--){
			k = rand() % 5 + 1;
			if(!cnt)
				k = 0;
		}
		res += k;
		if(res / 16 > res % 16)
			return (res % 16) * 16 + res / 16;
		return res;
	}

	void add_row(){
		ini = (1 - ini);
		for(int i = maxn - 1; i; --i)
			for(int j = 0; j < M; ++j){
				swap(a[i][j], a[i - 1][j]);
				swap(exs[i][j], exs[i - 1][j]);
				swap(blast[i][j], blast[i - 1][j]);
			}
		for(int i = 0; i < M; ++i){
			if(ini != i && rand() % 8 == 1)
				a[0][i] = a[0][i - 2];
			else
				a[0][i] = rnd();
			exs[0][i] = true;
			blast[0][i] = false;
		}
		return;
	}

	void sit(){
		head();
		for(int i = 0; i < maxn; ++i, cout << '\n')
			for(int j = 0; j < M; ++j)
				cout << "(" << blast[i][j] << "/" << exs[i][j] << ")  ";
		cout << endl;
		getch();
		prt_scr(canon);
		return;
	}

	void gen(){
		ini = 0, pts = 0;
		rows = 10;
		++tries;
		tb = time(nullptr);
		a.clear(), exs.clear(), blast.clear();
		lst.clear();
		for(int i = 0; i < maxn; ++i){
			a.push_back({}), exs.push_back({}), blast.push_back({});
			for(int j = 0; j < M; ++j)
				if(j % 2 == i % 2 && i < N){
					if(j != i % 2 && rand() % 8 == 1)
						a[i].push_back(a[i][j - 2]);
					else
						a[i].push_back(rnd());
					exs[i].push_back(true);
					blast[i].push_back(false);
				}
				else
					a[i].push_back(0), exs[i].push_back(0), blast[i].push_back(0);
		}
		for(int i = 1; i < M; i += 2)
			lst.push_back({5, i});
		prt_scr(rnd());
		return;
	}

	void prt_scr(int color){
		head();
		if(frombot)
			cout << "tries: " << tries << '\n';
		cout << "------------------------\n";
		mode[0] = toupper(mode[0]);
		cout << "Mode: " << mode << '\n';
		mode[0] += 32;
		c_col(6);
		cout << "Score: " << pts << "\n\n";
		c_col(9);
		cout << "Timer: " << time(nullptr) - tb << "s " << (mode == "timer" ? "of 120s\n" : "\n");
		c_col(15);
		cout << "------------------------\n\n";
		cout << "  ";
		for(int i = 0; i < M; ++i)
			cout << (char)('A' + i);
		cout << '\n';
		cout << " ___________________\n";
		for(int i = 0; i < maxn; ++i){
			cout << (char)('A' + i) << "|";
			for(int j = (i + ini) % 2; j < M; j += 2){
				if((i + ini) % 2)
					cout << " ";
				if(exs[i][j]){
					c_col(a[i][j]);
					cout << (char)('A' + j);
					c_col(15);
				}
				else{
					if(blast[i][j])
						cout << "X";
					else if(a[i][j])
						cout << "-";
					else
						cout << ((ini && i == maxn - 1) ? "~" : " ");
					blast[i][j] = a[i][j] = 0;
				}
				if(!((i + ini) % 2) && j != M - 1)
					cout << ((!ini && i == maxn - 1)? "~" : " ");
			}
			if((i + ini) % 2)
				cout << " ";
			cout << "|\n";
		}
		cout << (char)('A' + maxn) << "|       " << "(";
		canon = color;
		c_col(canon);
		cout << '0';
		c_col(15);
		cout << ")" << "       " << "|\n";
		cout << "\n---------------------------------\n";
		cout << "to choose a cell you have to write it's cordinate e.g. AA or FB and it should be valid\n";
		cout << "---------------------------------\n" << endl;
		return;
	}

	bool not_null(int i){
		for(int j = (i + ini) % 2; j < M; j += 2)
			if(a[i][j])
				return true;
		return false;
	}

	bool check_end(){
		if(not_null(maxn - 1)){
			cout << (mode[0] != 'i' ? "\n======YOU LOSE!=====\n" : "\n======GAME OVER=====\n") << endl;
			return true;
		}
		if(mode == "timer" && time(0) - tb > tl){
			cout << "\n======GAME OVER=====\n" << endl;
			return true;
		}
		if(!not_null(0)){
			if(mode[0] == 'i'){
				add_row();
				prt_scr(canon);
			}
			cout << "\n=======YOU WIN!======\n";
			cout << "press any key to continue" << endl;
			getch();
			return true;
		}
		return false;
	}

	bool adj_blast(vector<int> c, vector<int> p){
		if(!(-1 < c[0] && c[0] < maxn) || !(-1 < c[1] && c[1] < M))
			return false;
		if(!exs[c[0]][c[1]])
			return false;
		if(a[p[0]][p[1]] % 16 == a[c[0]][c[1]] % 16)
			return true;
		if(a[p[0]][p[1]] / 16 == a[c[0]][c[1]] % 16)
			return true;
		if(a[p[0]][p[1]] % 16 == a[c[0]][c[1]] / 16)
			return true;
		if(a[p[0]][p[1]] / 16 == a[c[0]][c[1]] / 16 && a[c[0]][c[1]] > 16)
			return true;
		return false;
	}

	void dfs_blast(vector<int> c, bool tst = false){
		if(!tst)
			++pts;
		blast[c[0]][c[1]] = true;
		exs[c[0]][c[1]] = false;
		for(int i = 0; i < 6; ++i)
			if(adj_blast({c[0] + dx[i], c[1] + dy[i]}, c))
				dfs_blast({c[0] + dx[i], c[1] + dy[i]});
		return;
	}

	bool adj_conct(vector<int> c){
		if(!(-1 < c[0] && c[0] < maxn) || !(-1 < c[1] && c[1] < M))
			return false;
		return !exs[c[0]][c[1]] && !blast[c[0]][c[1]] && a[c[0]][c[1]];
	}

	int dfs_conct(vector<int> c){
		int res = 1;
		exs[c[0]][c[1]] = true;
		for(int i = 0; i < 6; ++i)
			if(adj_conct({c[0] + dx[i], c[1] + dy[i]}))
				res += dfs_conct({c[0] + dx[i], c[1] + dy[i]});
		return res;
	}

	int check_good(vector<int> c, bool sen = true){
		int cnt = 0, mx = 0;
		for(int i = 0; i < 6; ++i)
			if(adj_blast({c[0] + dx[i], c[1] + dy[i]}, c)){
				if(sen)
					mx = max(mx, check_good({c[0] + dx[i], c[1] + dy[i]}, false));
				++cnt;
			}
		return (sen ? (int)(max(mx, cnt) > 1) : cnt);
	}

	void fall(){
		for(int i = 0; i < maxn; ++i)
			for(int j = 0; j < M; ++j)
				exs[i][j] = false;
		for(int i = ini; i < M; i += 2)
			if(!blast[0][i] && a[0][i])
				dfs_conct({0, i});
		return;
	}

	int willfall(int i){
		auto a1 = a;
		auto exs1 = exs;
		auto blast1 = blast;
		int x = lst[i][0], y = lst[i][1];
		a[x][y] = canon, blast[x][y] = false, exs[x][y] = true;
		if(!check_good({x, y})){
			a = a1, exs = exs1, blast = blast1;
			return 1000000020;
		}
		dfs_blast({x, y});
		fall();
		int cnt = 0;
		for(int i = 0; i < maxn; ++i)
			for(int j = (ini + i) % 2; j < M; j += 2)
				cnt += (int)exs[i][j];
		a = a1, exs = exs1, blast = blast1;
		return cnt;
	}

	void dfs_ok(vector<int> c){
		ok[c[0]][c[1]] = 1;
		for(int i = 0; i < 6; ++i)
			if(-1 < min(c[0] + dx[i], c[1] + dy[i]) && c[0] + dx[i] < maxn && c[1] + dy[i] < M && !ok[c[0] + dx[i]][c[1] + dy[i]])
				dfs_ok({c[0] + dx[i], c[1] + dy[i]});
		return;
	}

	void updlst(){
		lst.clear();
		ok.clear();
		for(int i = 0; i < maxn; ++i){
			ok.push_back({});
			for(int j = (i + ini) % 2; j < M; j += 2){
				if((i + ini) % 2)
					ok[i].push_back(0);
				if(!exs[i][j])
					ok[i].push_back(0);
				else
					ok[i].push_back(-1);
				if(!((i + ini) % 2) && j != M - 1)
					ok[i].push_back(0);
			}
		}
		for(int j = (maxn - 1 + ini) % 2; j < M; j += 2)
			if(!ok[maxn - 1][j])
				dfs_ok({maxn - 1, j});
		for(int i = 0; i < maxn; ++i)
			for(int j = (ini + i) % 2; j < M; j += 2)
				if(!exs[i][j] && ok[i][j] == 1){
					bool good = false;
					for(int k = 0; k < 6 && !good; ++k)
						if(i + dx[k] < maxn && -1 < i + dx[k])
							if(j + dy[k] < M && -1 < j + dy[k])
								if(exs[i + dx[k]][j + dy[k]])
									lst.push_back({i, j}), good = true;
				}
		return;
	}

	void upd_rnd(){
		int num = 0, bs = 71, mod = 1000000021;
		for(int i = 0; i < maxn; ++i)
			for(int j = 0; j < M; ++j)
				num = ((num * bs) % mod + a[i][j]) % mod;
		num %= 100;
		while(num-- >= 0)
			rand();
		return;
	}

	void gameplay(){
		gen();
		int x, y, mvs = 0;
		while(true){
			if(check_end())
				break;
			string s;
			if(frombot){
				updlst();
				int mn = 1000000021;
				for(int i = 0; i < lst.size(); ++i){
					int balls = willfall(i);
					if(mn > balls)
						mn = balls, x = lst[i][0], y = lst[i][1];
					else if(mn == balls && rand() % 2)
						x = lst[i][0], y = lst[i][1];
				}
			}
			else{
				cin >> s;
				if(s == "q"){
					cout << "\n======GAME OVER=====\n";
					break;
				}
				if(s == "s"){
					sit();
					continue;
				}
				x = s[0] - 'A', y = s[1] - 'A';
			}
			a[x][y] = canon, blast[x][y] = false, exs[x][y] = true;
			++mvs;
			if(!check_good({x, y})){
				if(mode == "normal" && rows-- >= 0)
					add_row();
				else if(mode != "normal" && rows-- >= 0)
					add_row();
				--mvs;
				prt_scr(rnd());
				if(check_end())
					return;
				if(mvs % 3 == 0)
					add_row();
				prt_scr(canon);
				if(check_end())
					return;
				continue;
			}
			dfs_blast({x, y}), --pts;
			fall();
			prt_scr(canon);
			upd_rnd();
			if(!frombot)
				Beep(414, 300);
			if(mode == "normal" && mvs % 2 && rows-- >= 0)
				add_row();
			else if(mode != "normal" && mvs % 2)
				add_row();
			prt_scr(rnd());
		}
		return;
	}

	void play(){
		head();
		cout << "Game Modes:" << '\n';
		cout << "  1. Timer Mode" << '\n';
		cout << "  2. Infinite Mode" << '\n';
		cout << "  3. Normal Mode" << '\n';
		cout << "  4. Back to the menu" << '\n';
		cout << "\n----------------------------\n";
		cout << "to choose an option write it's section number" << '\n';
		cout << "----------------------------\n";
		char index = getch();
		if(index == '4')
			return;
		//if(index != 'm'){
			mode = modes[index - '1'];
			gameplay();
			update();
		//}
		/*
		else{
			head();
			cout << "Do you want to use bot? (y/n)" << '\n';
			frombot = ('y' == getch());
			mode = (frombot ? "Miner-Bot" : "Miner-Manual");
			gameplay();
			update();
		}
		*/
		cout << "press any key to back into the menu ";
		getch();
		return;
	}

	void update(){
		string tmp = mode;
		if(mode[0] == 'm')
			mode = "infinite";
		int timer = time(nullptr) - tb, r_changes = pts;
		string s = ctime(&tb), ln;
		ifstream hs("./accounts/games/" + user + ".txt");
		vector<string> vec;
		while(getline(hs, ln))
			vec.push_back(ln);
		hs.close();
		ofstream histo("./accounts/games/" + user + ".txt");
		histo << s;
		histo << mode << '\n';
		histo << timer << '\n';
		if(r_changes > 0)
			histo << '+';
		histo << r_changes << '\n';
		for(string &e: vec)
			histo << e << '\n';
		histo.close();
		vec.clear();
		ifstream rnk("./accounts/ranking.txt");
		while(getline(rnk, ln))
			vec.push_back(ln);
		for(int i = 0; i < vec.size(); i += 2)
			if(user1 == vec[i]){
				vec[i + 1] = to_string(stoi(vec[i + 1]) + r_changes);
				break;
			}
		ofstream rank("./accounts/ranking.txt");
		for(int i = 0; i < vec.size(); ++i)
			rank << vec[i] << '\n';
		rank.close();
		vec.clear();
		ifstream rnk1("./accounts/ranking" + mode + ".txt");
		while(getline(rnk1, ln))
			vec.push_back(ln);
		for(int i = 0; i < vec.size(); i += 2)
			if(user1 == vec[i]){
				vec[i + 1] = to_string(stoi(vec[i + 1]) + r_changes);
				break;
			}
		ofstream rank1("./accounts/ranking" + mode + ".txt");
		for(int i = 0; i < vec.size(); ++i)
			rank1 << vec[i] << '\n';
		rank1.close();
		mode = tmp;
		return;
	}
};
