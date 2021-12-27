#include <bits/stdc++.h>
using namespace std;

#define REP(i, n) for(int i = 0; i < (n); i++)

struct cvor {
	int tip = 0; // 0 - undefined, 1 - int, 2 - char, 3 - const(int), 4 - const(char), 5 - niz(int), 6 - niz(char), 7 - niz(const(int)), 8 - niz(const(char))
	bool l_izraz = 0;
	vector <cvor*> djeca;
	cvor *rod = NULL;
	string ime = "";
	int br_elem = -1, ntip = -1;
	vector <int> tipovi;
	
	// svi
	string uniformni_znak = "";
	// zavrsni
	int red = -1;
	string jedinka = "";
	cvor(string _uniformni_znak) {
		uniformni_znak = _uniformni_znak;
	}
	cvor(string _uniformni_znak, int _red, string _jedinka) {
		uniformni_znak = _uniformni_znak;
		red = _red;
		jedinka = _jedinka;
	}
};

void unisti_stablo(cvor *tr) {
	for(cvor *dijete : tr -> djeca) unisti_stablo(dijete);
	delete tr;
	return;
}

void kraj(cvor *zadnji) {
	// bla bla
}

map <string, vector <cvor> > varijable;

void rek(cvor *rod) {
	//
}

int main() {
	string red;
	getline(cin, red);
	cvor *root = new cvor(red);
	int uvucenje = 0;
	vector <pair <cvor*, int> > rod = {{root, uvucenje}};
	while(getline(cin, red)) {
		int br = 0;
		REP(i, (int)red.size()) {
			if(red[i] != ' ') break;
			br++;
		}
		cvor *tr;
		int ptr = 0;
		while(red[ptr] == ' ') ptr++;
		if(red[ptr] == '<') tr = new cvor(red.substr(ptr));
		else {
			string uniformni_znak = "";
			while(red[ptr] != ' ') {
				uniformni_znak.push_back(red[ptr]);
				ptr++;
			}
			int br_reda = 0;
			ptr++;
			while(red[ptr] != ' ') {
				br_reda *= 10;
				br_reda += red[ptr] - '0';
				ptr++;
			}
			string jedinka = red.substr(ptr + 1);
			tr = new cvor(uniformni_znak, br_reda, jedinka);
		}
		while(rod.back().second >= br) rod.pop_back();
		rod.back().first -> djeca.push_back(tr);
		tr -> rod = rod.back().first;
		rod.push_back(make_pair(tr, br));
		//cout << tr -> rod -> uniformni_znak << " -> " << tr -> uniformni_znak << endl;
	}
	unisti_stablo(root);
	return 0;
}
