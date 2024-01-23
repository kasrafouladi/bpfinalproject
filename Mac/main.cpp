#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>
#include <stdlib.h>
#include <conio.h>
#include "game_play.hpp"
#include "chess.hpp"
using namespace std;

//#define WINDOWS

#ifdef WINDOWS
    #include <windows.h>
    HANDLE col =  GetStdHandle(STD_OUTPUT_HANDLE);
#endif

void c_col(int c){
    #ifdef  WINDOWS
        SetConsoleTextAttribute(col, c);
    #else
	    bool ch = c > 15;
		c &= 15;
	    if(c == 1)
        	cout << (ch ? "\033[47m\033[35m" : "\033[35m");
        else if(c == 6)
    	    cout << (ch ? "\033[47m\033[32m" : "\033[32m");
	    else if(c == 8)
    		cout << (ch ? "\033[47m\033[31m" : "\033[31m");
    	else if(c == 9)
    		cout << (ch ? "\033[47m\033[34m" : "\033[34m");
	    else if(c == 10)
    		cout << (ch ? "\033[47m\033[33m" : "\033[33m");
    	else if(c == 14)
    		cout << (ch ? "\033[47m\033[36m" : "\033[36m");
	    else if(c == 15)
	    	cout << (ch ? "\033[47m\033[37m" : "\033[0m");
	    else if(!c)
    		cout << (ch? "\033[47m\033[37m": "\033[0m\033[0m");
   	#endif
	return;
}


void cls(){
    #ifdef WINDOWS
        system("cls");
    #else
        cout << "\033[2J\033[1;1H";
    #endif
    return;
}

using ll = long long;

void sing_up(vector<pair<string, string>> &users);
void sing_in(vector<pair<string, string>> &users);
void menu();
void enter();
void history();
void leaderboard();

string user, user1;

string hs(string &s){
	ll bs[5] = {259, 258, 257, 256, 263};
	ll md[5] = {1000000021, 1000000009, 1000000007, 998244353, 2000000011};
	ll sum = 0;
	string res = "", st;
	for(int k = 0; k < 5; ++k, res += st, sum = 0)
		for(int i = 0; i < s.size(); ++i){
			sum = (sum * bs[k]) % md[k];
			sum += s[i];
			sum %= md[k];
			st = to_string(sum);
			for(int j = 0; j < 11 - st.size(); ++j)
				res.push_back('0');
		}
	return res;
}

char* date(){
	time_t t = time(0);
	char* dt = ctime(&t);
	return dt;
}

void head(){
    cls();
	c_col(10);
	cout << "Maze Maverick\n";
	cout << "Created by: Kasra Fouladi and Pouria Golsorkhi\n";
	c_col(15);
	cout << "_______________________________________________\n";
	if(user.size()){
		c_col(1);
		cout << "~ " << user1 << "\n";
		c_col(15);
		cout << "_______________________________________________\n";
	}
	c_col(14);
	cout << "Local time: " << date() << "\n";
	c_col(15);
	return;
}

int get_ind(vector<pair<string, string>> &users, string &s){
	string s1 = hs(s);
	for(int i = 0; i < users.size(); ++i)
		if(users[i].first == s1)
			return i;
	return -1;
}

void psw(string &s){
	s = "";
	for(bool b = false; true; b = true){
		char c = getch();
		if(c == 13)
			return;
		else if(c == '\b'){
			if(s.size()){
				s.pop_back();
				cout << "\b \b";
			}
		}
		else{
			cout << c;
			for(int i = 0; i < 300000000; ++i);
			cout << "\b*";
			s.push_back(c);
		}
	}
	return;
}

void sing_up(vector<pair<string, string>> &users){
	string s1, s2, s3;
	for(bool b = false; true; b = true){
		head();
		cout << "If you have an account and want to sing in write \"sing in\" and press enter" << '\n';
		cout << "-------------------\n";
		if(b && s1 != "")
			cout << "username is taken" << '\n';
		if(b && s1 == "")
			cout << "username can't be null" << '\n';
		cout << "username: ";
		getline(cin, s1);
		if(s1 == "sing in"){
			sing_in(users);
			return;
		}
		int ind = get_ind(users, s1);
		if(ind == -1 && s1 != "")
			break;
	}
	for(bool b = false; true; b = true){
		head();
		cout << "If you have an account and want to sing in write \"sing in\" and press enter" << '\n';
		cout << "-------------------\n";
		if(b){
			cout << "password isn't match" << '\n';
			cout << "-------------------\n";
		}
		cout << "username: " << s1 << '\n';
		cout << "password: ";
		psw(s2);
		if(s2 == "sing in"){
			sing_in(users);
			return;
		}
		cout << '\n';
		cout << "confirm passwaord: ";
		psw(s3);
		if(s3 == "sing in"){
			sing_in(users);
			return;
		}
		if(s2 == s3)
			break;
	}
	ofstream us("./accounts/users.txt");
	ofstream ps("./accounts/pass.txt");
	for(auto &e: users){
		us << e.first << '\n';
		ps << e.second << '\n';
	}
	user = hs(s1);
	user1 = s1;
	us << user << '\n';
	ps << hs(s2) << '\n';
	us.close();
	ps.close();
	ofstream games("./accounts/games/" + user + ".txt");
	games.close();
	ifstream rank("./accounts/ranking.txt");
	vector<string> rnk;
	while(getline(rank, s3))
		rnk.push_back(s3);
	rank.close();
	ofstream rank1("./accounts/ranking.txt");
	rnk.push_back(s1);
	rnk.push_back("0");
	for(auto &e: rnk)
		rank1 << e << '\n';
	rank1.close();
	return;
}

void sing_in(vector<pair<string, string>> &users){
	string s1, s2;
	for(bool b = false; true; b = true){
		head();
		cout << "If want to create account write \"sing up\" and press enter" << '\n';
		cout << "-------------------\n";
		if(b)
			cout << "username or password is in correct" << '\n';
		cout << "username: ";
		getline(cin, s1);
		if(s1 == "sing up"){
			sing_up(users);
			return;
		}
		cout << "password: ";
		psw(s2);
		if(s2 == "sing up"){
			sing_up(users);
			return;
		}
		int ind = get_ind(users, s1);
		if(ind != -1 && users[ind].first == hs(s1) && users[ind].second == hs(s2)){
			user = hs(s1);
			user1 = s1;
			return;
		}
	}
	return;
}

void enter(){
	ifstream sli("./accounts/saved_login.txt");
	string l1;
	int l2;
	getline(sli, l1);
	sli >> l2;
	if(l1.size() && time(0) - l2 <= 7 * 60 * 24 * 60){
		user1 = l1, user = hs(user1);
		menu();
		return;
	}
	sli.close();
	ifstream us("./accounts/users.txt");
	ifstream ps("./accounts/pass.txt");
	vector<pair<string, string>> users;
	string s;
	while(getline(us, s)){
		users.push_back({});
		users.back().first = s;
		getline(ps, s);
		users.back().second = s;
	}
	us.close();
	ps.close();
	for(bool b = false; true; b = true){
		head();
		cout << "Have an account? (y/n)" << '\n';
		if(b)
			cout << "invalid input, try again" << endl;
		getline(cin, s);
		if(s == "y" || s == "n")
			break;
	}
	if(s[0] == 'y')
		sing_in(users);
	else
		sing_up(users);
    cout << "\nDo you want to save your login for a week? (1:yes, any other keys:no)";
    char c = getch();
    if(c == '1'){
    	ofstream slo("./accounts/saved_login.txt");
    	slo << user1 << '\n';
    	slo << time(0) << '\n';
	}
	menu();
	return;
}

void history(){
    string s1;
	ifstream games("./accounts/games/" + user + ".txt");
	head();
	vector<string> act;
	while(getline(games, s1))
		act.push_back(s1);
	cout << "Your history:" << '\n';
	cout << "___________________________________________________________________________________________" << '\n';
	cout << "|";
	c_col(9);
	cout << "Time";
	c_col(15);
	cout << "                    |";
	c_col(9);
	cout << "Map Name";
	c_col(15);
	cout << "                  |";
	c_col(9);
	cout << "Solved(Y/N)";
	c_col(15);
	cout << "|";
	c_col(9);
	cout << "Timer(sec)";
	c_col(15);
	cout << "|";
	c_col(9);
	cout << "Rating Changes";
	c_col(15);
	cout << "|" << '\n';
	for(int i = 0; true; ++i){
		cout << "|________________________|__________________________|___________|__________|______________|" << '\n';
		if(i == act.size())
			break;
		cout << "|";
		c_col(10);
		cout << act[i];
		c_col(15);
		cout << "|";
		c_col(10);
		++i;
		for(int j = 0; j < 26; ++j){
			if(j < act[i].size())
				cout << act[i][j];
			else
				cout << " ";
		}
		c_col(15);
		cout << "|     ";
		c_col(10);
		++i;
		cout << act[i];
		c_col(15);
		cout << "     |";
		c_col(10);
		++i;
		for(int j = 0; j < 10; ++j){
			if(j < act[i].size())
				cout << act[i][j];
			else
				cout << " ";
		}
		c_col(15);
		cout << "|";
		c_col(10);
		++i;
		cout << act[i];
		for(int j = 0; j < 14 - (int)act[i].size(); ++j)
			cout << " ";
		c_col(15);
		cout << "|" << '\n';
	}
	cout << "\nTo back into the menu press any key" << endl;
	getch();
	return;
}

void leaderboard(){
	ifstream ranking("./accounts/ranking.txt");
	string s1, s2;
	vector<pair<string, int>> standing;
	while(getline(ranking, s1)){
		getline(ranking, s2);
		int num = 0;
		for(int i = 0; i < s2.size(); ++i)
			num = num * 10 + s2[i] - '0';
		standing.push_back({s1, num});
	}
	sort(standing.begin(), standing.end(), [&](pair<string, int> e1, pair<string, int> e2){return e1.second > e2.second;});
	head();
	cout << "_______________________________________" << '\n';
	cout << "|";
	c_col(9);
	cout << "Ranking";
	c_col(15);
	cout << "|";
	c_col(9);
	cout << "Handle                ";
	c_col(15);
	cout << "|";
	c_col(9);
	cout << "Rating";
	c_col(15);
	cout << "|" << '\n';
	for(int i = 0; true; ++i){
		cout << "|_______|______________________|______|" << '\n';
		if(i == standing.size())
			break;
		int l;
		cout << "|";
		c_col(10);
		cout << "#" << i + 1;
		l = ceil(log10(i + 2));
		for(int j = 0; j < 6 - l; ++j)
			cout << " ";
		c_col(15);
		cout << "|";
		c_col(10);
		cout << standing[i].first;
		for(int j = 0; j < 22 - standing[i].first.size(); ++j)
			cout << " ";
		c_col(15);
		cout << "|";
		c_col(10);
		cout << standing[i].second;
		l = max(1LL, (int)ceil(log10(standing[i].second + 1)));
		for(int j = 0; j < 6 - l; ++j)
			cout << " ";
		c_col(15);
		cout << "|" << '\n';
	}
	cout << "\nTo back into the menu press any key" << endl;
	getch();
	return;
}

void menu(){
	bool p = true, back = false;
	string s = "1", s1 = ".";
	for(bool b = false; true; b = true){
		head();
		cout << "Menu:" << '\n';
		if(s[0] == '1')
			c_col(6);
		cout << "  1. Create New Map" << '\n';
		if(s == "1.1")
			c_col(8);
		cout << "    - 1.1 Easy" << '\n';
		if(s == "1.1")
			c_col(6);
		if(s == "1.2")
			c_col(8);
		cout << "    - 1.2 Hard" << '\n';
		if(s == "1.2")
			c_col(6);
		if(s[0] == '1')
			c_col(15);
		if(s[0] == '2')
			c_col(6);
		cout << "  2. Playground" << '\n';
		if(s == "2.1")
			c_col(8);
		cout << "    - 2.1 Choose from Existing Maps" << '\n';
		if(s == "2.1")
			c_col(6);
		if(s == "2.2")
			c_col(8);
		cout << "    - 2.2 Import a Custom Map" << '\n';
		if(s == "2.2")
			c_col(6);
		if(s[0] == '2')
			c_col(15);
		if(s[0] == '3')
			c_col(6);
		cout << "  3. Solve a Maze" << '\n';
		if(s == "3.1")
			c_col(8);
		cout << "    - 3.1 Choose from Existing Maps" << '\n';
		if(s == "3.1")
			c_col(6);
		if(s == "3.2")
			c_col(8);
		cout << "    - 3.2 Import a Custom Map" << '\n';
		if(s == "3.2")
			c_col(6);
		if(s[0] == '3')
			c_col(15);
		if(s[0] == '4')
			c_col(6);
		cout << "  4. History" << '\n';
		if(s[0] == '4')
			c_col(15);
		if(s[0] == '5')
			c_col(6);
		cout << "  5. Leaderboard" << '\n';
		if(s[0] == '5')
			c_col(15);
		if(s[0] == '6')
			c_col(6);
		cout << "  6. Exit" << '\n';
		if(s[0] == '6')
			c_col(15);
		if(s[0] == '7')
			c_col(6);
		cout << "  7. Sing out" << '\n';
		if(s[0] == '7')
			c_col(15);
		if(s[0] == '8')
			c_col(6);
		cout << "  8. Chess (Just for fun)" << endl;
		if(s[0] == '8')
			c_col(15);
		cout << "----------\nIf you want to select an option press it's section number\nAfter you set the section you want to go press enter key\n----------" << '\n';
		if(!((s.size() == 1 && s[0] <= '8' && s[0] >= '1') || (s.size() == 3 && s[1] == '.' && s[2] <= '2' && s[2] >= '1' && s[0] <= '3' && s[0] >= '1')) && b){
			cout << "invalid input, try again" << endl;
			p = false;
		}
		else
			p = true;
		getline(cin, s1);
		if(s1 != "")
			s = s1;
		else if(p && !back){
			if(s[0] == '8'){
				chess match;
				match.mychess(user1, user);
				s = "8";
				s1 = ".";
				back = true;
				continue;
			}
			if(s[0] == '7'){
				ofstream slo("./accounts/saved_login.txt");
				slo << "" << '\n';
				slo << -1 << '\n';
				slo.close();
				user1 = "";
				user = "";
				enter();
				return;
			}
			if(s[0] == '6'){
                head();
                cout << "Are you sure you want to exit? (n:no, any other key:yes)" << '\n';
                char c = getch();
                if(c == 'n')
                    continue;
                else{
                    head();
                    cout << "Have a nice day!" << '\n';
                    exit(0);
                }
			}
			if(s[0] == '5'){
				leaderboard();
				s = "5";
				s1 = ".";
				back = true;
				continue;
			}
			if(s[0] == '4'){
				history();
				s = "4";
				s1 = ".";
				back = true;
				continue;
			}
			if(s[0] == '3'){
				gameplay game;
				game.create(user, user1, s);
				s1 = ".";
				back = true;
				continue;
			}
			if(s[0] == '2'){
				gameplay map;
				map.create(user, user1, s);
				s1 = ".";
				back = true;
				continue;
			}
			if(s[0] == '1'){
				gameplay map;
				map.create(user, user1, s);
				back = true;
				continue;
			}
		}
		back = false;
	}
	return;
}

signed main(){
	enter();
	return 0;
}