#include <iostream>
#include <vector>
#include <time.h>
#include <string>
#include <stdlib.h>
#include <conio.h>
using namespace std;

#define WINDOWS

#ifdef WINDOWS
	#include <windows.h>
#endif

struct chess{
	private:
	
	#ifdef WINDOWS
		HANDLE col = GetStdHandle(STD_OUTPUT_HANDLE);
	#endif
	
	string user_, user_1;

	bool ext = false;

	char* date(){
		time_t t = time(0);
		char* dt = ctime(&t);
		return dt;
	}

	void c_col(int c){
    	#ifdef  WINDOWS
        	SetConsoleTextAttribute(col, c);
	    #else
    	    c &= 15;
        	if(c == 1)
    	    	cout << "\033[35m";
	        else if(c == 6)
        		cout << "\033[32m";
    		else if(c == 8)
    			cout << "\033[31m";
	    	else if(c == 9)
    			cout << "\033[34m";
    		else if(c == 10)
    			cout << "\033[33m";
	    	else if(c == 14)
    			cout << "\033[36m";
    		else if(c == 15)
	    		cout << "\033[37m";
	    #endif
    	return;
	}

	void cls(){
    	#ifdef WINDOWS
        	system("cls");
	    #else
    	    system("clear");
    	#endif
    	return;
	}

	void head_(){
		cls();
		c_col(10);
		cout << "Chess\n";
		cout << "Created by Kasra Fouladi and Pouria Golsorkhi\n";
		c_col(15);
		cout << "_______________________________________________\n";
		if(user_1.size()){
			c_col(1);
			cout << "~ " << user_1 << "\n";
			c_col(15);
			cout << "_______________________________________________\n";
		}
		c_col(14);
		cout << "Local time: " << date() << '\n';
		c_col(15);
		return;
	}

	long long times = 30;
	int sen = 0, alaf = 2, T = 12, pts[2] = {}, cnt[2] = {4, 4};
	time_t timer = time(0);
	string board[8];
	vector<char> killed[2];
	vector<pair<string, string> > mvs[2];

	struct cor{
    	int x, y;
	};

	pair<cor, long long> pre[8][8];

	void chess_preprocess(){
    	for(int i = 0; i < 8; ++i){
        	for(int j = 0; j < 2; ++j)
            	pre[i][j] = {{-1, -1}, 0};
	        for(int j = 2; j < 6; ++j)
    	        pre[i][j] = {{-2, -2}, 1};
        	for(int j = 6; j < 8; ++j)
            	pre[i][j] = {{-1, -1}, 0};
	    }
    	board[0] = "rhbqkbhr";
	    board[1] = "ssssssss";
    	for(int i = 2; i < 6; ++i)
        	board[i] = "        ";
	    board[6] = "SSSSSSSS";
    	board[7] = "RHBQKBHR";
   		return;
	}

	void c_role(){
	    cout << "Choose one you prefer for players roles(Player1 v Player2) (1/2/3/4): " << '\n';
    	cout << "1-p v p" << '\n' << "2-p v bot" << '\n' << "3-bot v p" << '\n' << "4-bot v bot" << endl;
	    cin >> sen;
    	if(sen == 2)
        	sen = 3;
	    else if(sen == 3)
    	    sen = 2;
	    sen = 4 - sen;
    	if(sen != 3){
        	cout << "How many seconds you prefer to be between 2 players?" << endl;
	        cin >> alaf;
    	}
    	if(!sen){
	       	cout << "After how many seconds you want to check the situation?" << endl;
        	cin >> T;
    	}
    	return;
	}

	void print_board_chess(){
		head_();
		cout << "Player1: " << pts[0] << ", Player2: " << pts[1] << '\n';
		cout << '\n';
		cout << "Killed pawns: " << '\n';
    	for(int i = 0; i < 2; ++i){
        	cout << "Player" << i + 1 << "'s pawns: ";
        	if(i)
        		c_col(8);
        	else
        		c_col(6);
        	for(int j = 0; j < killed[i].size(); ++j)
            	cout << (char)toupper(killed[i][j]) << " ";
        	c_col(15);
        	cout << '\n';
    	}
    	cout << '\n' << '\n';
		cout << "Roles: king = K, queen = Q, bishop = B, knight = H, rook = R, solider = S;" << '\n';
		cout << '\n' << "  __________________________" << '\n';
		for(int i = 0; i < 8; ++i){
        	cout << "  |";
    	    for(int j = 0; j < 8; ++j){
	        	c_col((i ^ j) & 1 ? 0 : 16 * 14);
        		cout << "   ";
			}
			c_col(15);
			cout << "|" << '\n';
			cout << 8 - i << " |";
			for(int j = 0; j < 8; ++j){
				if(board[i][j] <= 'z' && 'a' <= board[i][j])
        			c_col(((i ^ j) & 1 ? 0 : 16 * 14) + 8);
        		else if(board[i][j] <= 'Z' && 'A' <= board[i][j])
    	    		c_col(((i ^ j) & 1 ? 0 : 16 * 14) + 6);
	        	else
        			c_col((i ^ j) & 1 ? 0 : 16 * 14);
				cout << " " << (char)toupper(board[i][j]) << " ";
			}
			c_col(15);
			cout << "|" << '\n';
		}
		c_col(15);
		cout << "  __________________________" << '\n';
    	cout << "    A  B  C  D  E  F  G  H" << '\n' << endl;
    	cout << "\n--------------\nif you want to quit type i-quit\n--------------\n" << endl;
    	if(!sen && time(0) - timer > T){
       		timer = time(0);
        	cout << "To continue enter an integer(Its bot v bot mode's option): " << endl;
        	string x;
    	    cin >> x;
    	    if(x == "i-quit"){
    	    	ext = true;
    	    	return;
			}
	        if(x == "ramz")
        	    c_role();
    	}
		return;
	}

	bool can_do(cor be, cor af, int p){
    	if(be.x == af.x && be.y == af.y)
        	return false;
    	if('A' + 32 * p <= board[af.x][af.y] && board[af.x][af.y] <= 'Z' + 32 * p)
        	return false;
		char rol = board[be.x][be.y];
		if(rol == 'S' + 32 * p){
        	if(be.y == af.y && (af.x - be.x) * (p * 2 - 1) == 1)
				return board[af.x][af.y] == ' ';
			if(be.y == af.y && (af.x - be.x) * (p * 2 - 1) == 2){
				cor mid  = {(be.x + af.x) / 2, (be.y + af.y) / 2};
				return be.x == 1 + 5 * (1 - p) && board[af.x][af.y] == ' ' && board[mid.x][mid.y] == ' ';
			}
			if((af.x - be.x) * (p * 2 - 1) == 1 && abs(be.y - af.y) == 1)
				return board[af.x][af.y] != ' ';
			return false;
		}
		if(rol == 'H' + 32 * p)
        	return (abs(be.x - af.x) == 2 && abs(be.y - af.y) == 1) || (abs(be.x - af.x) == 1 && abs(be.y - af.y) == 2);
		if(rol == 'K' + 32 * p)
			return max(abs(be.x - af.x), abs(be.y - af.y)) <= 1;
		if(rol == 'B' + 32 * p){
        	if(abs(af.x - be.x) != abs(af.y - be.y))
    	        return false;
			int dx = (2 * (af.x - be.x) + 1) % 2, dy = (2 * (af.y - be.y) + 1) % 2;
			cor tmp = be;
			tmp.x += dx, tmp.y += dy;
			while(!(tmp.x == af.x && tmp.y == af.y)){
				if(board[tmp.x][tmp.y] != ' ')
					return false;
				tmp.x += dx, tmp.y += dy;
			}
			return tmp.x == af.x && tmp.y == af.y;
		}
		if(rol == 'R' + 32 * p){
	    	if(be.x - af.x != 0 && be.y - af.y != 0)
    	        return false;
			int dx = (2 * (af.x - be.x) + 1) % 2, dy = (2 * (af.y - be.y) + 1) % 2;
			if(be.x == af.x)
				dx = 0;
			if(be.y == af.y)
				dy = 0;
			cor tmp = be;
			tmp.x += dx, tmp.y += dy;
			while(!(tmp.x == af.x && tmp.y == af.y)){
				if(board[tmp.x][tmp.y] != ' ')
					return false;
				tmp.x += dx, tmp.y += dy;
			}
			return true;
		}
		if(rol == 'Q' + 32 * p){
			board[be.x][be.y] = 'R' + 32 * p;
			bool can1 = can_do(be, af, p);
			board[be.x][be.y] = 'B' + 32 * p;
			bool can2 = can_do(be, af, p);
			board[be.x][be.y] = 'Q' + 32 * p;
			return can1 || can2;
		}
		return false;
	}

	bool is_checked(int p){
    	cor K = {-1, -1};
    	for(int i = 0; K.x < 0 && i < 8; ++i)
        	for(int j = 0; K.x < 0 && j < 8; ++j)
            	if(board[i][j] == 'K' + 32 * p)
                	K = {i, j};
    	for(int i = 0; i < 8; ++i)
        	for(int j = 0; j < 8; ++j)
            	if('A' + 32 * (1 - p) <= board[i][j] && board[i][j] <= 'Z' + 32 * (1 - p))
                	if(can_do({i, j}, K, 1 - p))
                    	return true;
    	return false;
	}

	bool will_checked(cor be, cor af, int p){
    	char tmp1 = board[be.x][be.y];
    	char tmp2 = board[af.x][af.y];
	    board[be.x][be.y] = ' ';
    	board[af.x][af.y] = tmp1;
    	bool res = is_checked(p);
    	board[be.x][be.y] = tmp1;
    	board[af.x][af.y] = tmp2;
    	return res;
	}

	bool check_swap(cor be, cor af, int p){
    	if(board[be.x][be.y] != 'K' + 32 * p || !(af.x == be.x && abs(af.y - be.y) == 2))
        	return false;
    	int dy = (2 * (af.y - be.y) + 1) % 2;
	    cor c = be;
    	while(1 <= c.y && c.y < 7){
        	if(c.y != be.y && board[c.x][c.y] != ' ')
            	return false;
        	c.y += dy;
    	}
    	if(board[c.x][c.y] != 'R' + 32 * p || pre[c.x][c.y].first.x == -2 || pre[be.x][be.y].first.x == -2)
        	return false;
    	cor tmp = {be.x, be.y + 1 * dy};
    	pre[c.x][c.y] = {{-2, -2}, 1};
    	board[c.x][c.y] = ' ';
    	board[tmp.x][tmp.y] = 'R' + 32 * p;
    	pre[c.x][c.y] = {{-2, -2}, 1};
    	pre[tmp.x][tmp.y] = {c, ++times};
    	return true;
	}

	bool check_en(cor be, cor af, int p){
	    if(board[af.x][af.y] != ' ')
        	return false;
    	if(be.x == af.x && be.y == af.y)
    	    return false;
	    if('A' + 32 * p <= board[af.x][af.y] && board[af.x][af.y] <= 'Z' + 32 * p)
    	    return false;
		char rol = board[be.x][be.y];
		if(rol == 'S' + 32 * p)
			if((af.x - be.x) * (p * 2 - 1) == 1 && abs(be.y - af.y) == 1 && board[af.x][af.y] == ' '){
            	if(board[be.x][af.y] != 'S' + 32 * (1 - p))
        	        return false;
    	        int dx = (2 * (af.x - be.x) + 1) % 2;
	            if(pre[be.x][af.y].first.x == af.x + dx && pre[be.x][af.y].first.y == af.y)
            	    if(pre[be.x][be.y].second < pre[be.x][af.y].second){
        	            board[be.x][af.y] = ' ';
    	                pre[be.x][af.y] = {{-2, -2}, 1};
	                    killed[1 - p].push_back('S' + 32 * (1 - p));
                    	++pts[p];
                	    return true;
            	    }
			}
    	return false;
	}

	bool is_valid(string &X, string &Y, int p, bool silent = false){
    	if('a' <= X[0])
        	X[0] -= 32;
    	if('a' <= Y[0])
        	Y[0] -= 32;
		if(X.size() != 2 || Y.size() != 2)
			return false;
		if(!('A' <= X[0] && X[0] <= 'H') || !('1' <= X[1] && X[1] <= '8'))
    		return false;
    	if(!('A' <= Y[0] && Y[0] <= 'H') || !('1' <= Y[1] && Y[1] <= '8'))
    		return false;
    	cor c[2];
	    c[0] = {7 - (X[1] - '1'), X[0] - 'A'};
    	c[1] = {7 - (Y[1] - '1'), Y[0] - 'A'};
	    bool kr = check_swap(c[0], c[1], p);
    	bool en_p = check_en(c[0], c[1], p);
    	if(board[c[0].x][c[0].y] == ' ')
        	return false;
    	if(!('A' + p * 32 <= board[c[0].x][c[0].y] && board[c[0].x][c[0].y] <= 'Z' + p * 32))
    		return false;
    	if('A' + p * 32 <= board[c[1].x][c[1].y] && board[c[1].x][c[1].y] <= 'Z' + p * 32)
    		return false;
    	if(will_checked(c[0], c[1], p)){
    	    if(!silent)
 	           if(sen & (1 << p))
            	    cout << "You can't do this move. You'll be checked!" << endl;
			return false;
    	}
    	return en_p || kr || can_do(c[0], c[1], p);
	}

	void sol_to_end(int p){
    	int i = 0 + p * 7;
	    char ch = 'S' + p * 32;
    	for(int j = 0; j < 8; ++j)
	        if(board[i][j] == ch){
        	    cout << "Hey player" << p + 1 << '!' << '\n';
    	        cout << "Your solider has reached the board, Now you can choose it's new role(Q/R/E/H):" << '\n';
	            cout << "Q-queen" << '\n' << "R-rook" << '\n';
            	cout << "B-bishop" << '\n' << "H-knight" << endl;
        	    string choice[4] = {"Q", "R", "B", "H"};
    	        string s = choice[(rand() + timer) % 4];
	            if(sen & (1 << p))
                	cin >> s;
                if(s == "i-quit"){
                	cout << "Player " << 2 - p << "wins!" << '\n';
                	cout << "press any key to continue";
                	getch();
                	ext = true;
                	return;
                }
            	if(s[0] > 'Z')
        	        s[0] -= 32;
    	        while(true){
 	               if(s == "Q" || s == "R" || s == "B" || s == "H"){
                    	if(s == "B" || s == "H")
                        	++cnt[p];
                    	board[i][j] = s[0] + p * 32;
                    	break;
                	}
                	cout << "Invalid choice, try again" << endl;
                	cin >> s;
                	if(s[0] > 'Z')
                	    s[0] -= 32;
            	}
            	break;
        	}
    	return;
	}

	void is_draw(int p){
    	int szm = mvs[1].size();
    	if(p && szm >= 6)
	        if(mvs[1][szm - 1] == mvs[1][szm - 3] && mvs[1][szm - 3] == mvs[1][szm - 5])
        	if(mvs[1][szm - 2] == mvs[1][szm - 4] && mvs[1][szm - 4] == mvs[1][szm - 6])
    	    if(mvs[0][szm - 1] == mvs[0][szm - 3] && mvs[0][szm - 3] == mvs[0][szm - 5])
	        if(mvs[0][szm - 2] == mvs[0][szm - 4] && mvs[0][szm - 4] == mvs[0][szm - 6]){
        	    cout << "Draw" << '\n' << endl;
    	        if(pts[0] != pts[1]){
	                cout << "But if you want you can choose the winner based on points" << '\n';
                	int ply = (int)(pts[0] < pts[1]) + 1;
            	    cout << "In this case Player" << ply << " wins!" << endl;
        	    }
    	        ext = true;
	            cout << "Press any key to continue ";
            	getch();
        	    return;
    	    }
	    if(killed[0].size() > 13 && cnt[0] == 15 - killed[0].size())
        	if(killed[1].size() > 13 && cnt[1] == 15 - killed[1].size()){
    	        cout << "Draw" << '\n' << endl;
	            if(pts[0] != pts[1]){
                	cout << "But if you want you can choose the winner based on points" << '\n';
            	    int ply = (int)(pts[0] < pts[1]) + 1;
        	        cout << "In this case Player" << ply << " wins!" << endl;
    	        }
	            ext = true;
            	cout << "Press any key to continue ";
            	getch();
            	return;
        	}
    	return;
	}

	void is_end(int p){
    	print_board_chess();
    	is_draw(p);
    	if(ext)
    		return;
    	bool ch = is_checked(p), res = true;
    	vector<string> poz;
    	for(int i = 0; i < 8; ++i)
        	for(int j = 0; j < 8; ++j)
    	        if('A' + 32 * p <= board[i][j] && board[i][j] <= 'Z' + 32 * p){
	                string e = "00";
                	e[0] = 'A' + j, e[1] = '8' - i;
            	    poz.push_back(e);
        	    }
    	for(int k = 0; k < poz.size(); ++k)
    	    for(int i = 0; i < 8; ++i)
	            for(int j = 0; j < 8; ++j){
                	string poz2 = "00";
            	    poz2[0] = 'A' + j, poz2[1] = '8' - i;
        	        if(is_valid(poz[k], poz2, p, true))
    	                return;
	            }
	    poz.clear();
    	if(ch){
	        cout << "Check mate! Player" << 2 - p << " wins!" << endl;
        	ext = true;
    	    cout << "Press any key to continue ";
	        getch();
        	return;
    	}
	    cout << "Draw" << '\n' << endl;
    	if(pts[0] != pts[1]){
	        cout << "But if you want you can choose the winner based on points" << '\n';
        	int ply = (int)(pts[0] < pts[1]) + 1;
        	cout << "In this case Player" << ply << " wins!" << endl;
    	}
    	ext = true;
	    cout << "Press any key to continue ";
    	getch();
    	return;
	}

	void move_(string X, string Y, int p){
   		cor c[2];
    	c[0] = {7 - (X[1] - '1'), X[0] - 'A'};
	    c[1] = {7 - (Y[1] - '1'), Y[0] - 'A'};
    	char t0 = board[c[0].x][c[0].y];
	    char t = board[c[1].x][c[1].y];
    	if(t == 'S' + (1 - p) * 32)
        	pts[p] += 1, killed[1 - p].push_back(t);
    	else if(t == 'H' + (1 - p) * 32)
        	pts[p] += 3, killed[1 - p].push_back(t), --cnt[1 - p];
   		else if(t == 'B' + (1 - p) * 32)
    	    pts[p] += 3, killed[1 - p].push_back(t), --cnt[1 - p];
    	else if(t == 'R' + (1 - p) * 32)
        	pts[p] += 5, killed[1 - p].push_back(t);
    	else if(t == 'Q' + (1 - p) * 32)
	        pts[p] += 9, killed[1 - p].push_back(t);
    	board[c[0].x][c[0].y] = ' ';
    	board[c[1].x][c[1].y] = t0;
    	pre[c[0].x][c[0].y] = {{-2, -2}, 1};
    	pre[c[1].x][c[1].y] = {c[0], ++times};
    	print_board_chess();
    	sol_to_end(p);
	    is_end(1 - p);
    	return;
	}

	void check_quit(string X, int p){
    	if(X == "ramz")
        	c_role();
    	if(X == "im-kasra"){
	        check_quit("i-quit", 1 - p);
        	if(ext)
    	    	return;
	    }
    	if(X == "i-quit"){
	        cout << "Player" << 2 - p << " wins!" << '\n';
        	ext = true;
    	    cout << "Press any key to continue ";
	        getch();
        	return;
    	}
    	if(X == "end-match"){
        	if(pts[0] == pts[1])
            	cout << '\n' << "Draw" << '\n';
        	else if(pts[0] < pts[1])
            	cout << '\n' << "Player2 wins!" << '\n';
        	else
            	cout << '\n' << "Player1 wins!" << '\n';
        	ext = true;
        	cout << "Press any key to continue ";
        	getch();
			return;
    	}
    	return;
	}

	string rpoz(){
    	string result = "00";
    	result[0] = 'A' + (rand() + time(0)) % 8;
    	result[1] = '1' + (rand() + time(0)) % 8;
    	return result;
	}

	void get_input(string &X, string &Y, int p){
    	X = rpoz(), Y = rpoz();
    	if(sen & (1 << p)){
        	cout << "Enter the cordinate of your pawn in the following line" << '\n';
        	cin >> X;
    	}
    	check_quit(X, p);
	    if(ext)
    		return;
    	if(sen & (1 << p)){
	        cout << "Enter the place that you want move your pawn to it" << '\n';
        	cin >> Y;
    	}
    	check_quit(Y, p);
    	if(ext)
    		return;
    	return;
	}

	void my_chess(){
		head_();
    	chess_preprocess();
    	c_role();
    	for(int p = 0; true; p = 1 - p){
        	print_board_chess();
        	cout << "For your turn you have to write some thing like B1 C7 (and it must be valid)" << '\n';
        	cout << "Palyer" << p + 1 << "'s turn : " << endl;
        	if(is_checked(p))
            	cout << "Player" << p + 1 << " checked by Player" << 2 - p << '!' << endl;
        	string X, Y;
    	    get_input(X, Y, p);
	        if(ext)
        		return;
    	    while(!is_valid(X, Y, p)){
	            if(sen & (1 << p)){
            	    cout << "Invalid move" << '\n';
        	        cout << "Do another thing:" << endl;
    	        }
	            get_input(X, Y, p);
            	if(ext)
        	    	return;
    	    }
	        if(!sen || (!(sen & (1 << p)) && (sen & (1 << (1 - p))))){
        	    cout << "Bot's move: " << X << " to " << Y << endl;
    	        time_t alafi = time(0);
	            while(time(0) - alafi < alaf){}
        	}
    	    mvs[p].push_back({X, Y});
	        move_(X, Y, p);
        	print_board_chess();
        	if(ext)
        		return;
    	}
    	return;
	}

	public:

	void mychess(string u1, string u){
		user_ = u;
		user_1 = u1;
    	head_();
		cout << "Hi! Wanna play chess? (n:no/any other key:yes)" << endl;
		if(getch() == 'n'){
        	cout << "bye bye!" << endl;
        	cout << "Press any key to continue ";
        	getch();
        	return;
		}
		my_chess();
		return;
	}

};