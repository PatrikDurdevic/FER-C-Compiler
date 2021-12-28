#include <bits/stdc++.h>
using namespace std;

#define REP(i, n) for(int i = 0; i < (n); i++)

struct cvor {
	int tip = 0; 
	// 0 - undefined, 1 - int, 2 - char, 3 - const(int), 4 - const(char)
	// 5 - niz(int), 6 - niz(char), 7 - niz(const(int)), 8 - niz(const(char))
	// 9 - pov, 10 - funkcija(void -> pov), 11 - funkcija(params -> pov), 12 - void
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

//
// IZRAZI
// 
// begin
//
void rek(cvor *rod);
bool isInt(string s) {
	try {
    	int i = stoi(s);
		return true;
	}
	catch (const std::out_of_range& oor) {
		return false;
	}
}
bool isChar(string s) {
	// TODO: check
	if (s.length() == 1) {
		return true;
	}
	return false;
}
bool isString(string s) {
	// TODO
	return true;
}
bool canCast(int from, int to) {
	// TODO
	return true;
}
int toConst(int type) {
	// TODO: check
	return (type == 1 || type == 2 || type == 5 || type == 6) ? type + 4 : 0;
}
bool canCastToInt(int type) {
	// TODO: check
	return (type >= 1 && type <= 4);
}

void primarni_izraz(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "IDN") {
		if (cv->djeca[0]->ime == "") {
			kraj(cv);
		}

		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "BROJ") {
		if (!isInt(cv->djeca[0]->jedinka)) {
			kraj(cv);
		}

		cv->tip = 1;
		cv->l_izraz = 0;
	}
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "ZNAK") {
		if (!isChar(cv->djeca[0]->jedinka)) {
			kraj(cv);
		}

		cv->tip = 2;
		cv->l_izraz = 0;
	}
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "NIZ_ZNAKOVA") {
		if (!isString(cv->djeca[0]->jedinka)) {
			kraj(cv);
		}

		cv->tip = 8;
		cv->l_izraz = 0;
	}
	if (cv->djeca.size() == 3 &&
		cv->djeca[0]->uniformni_znak == "L_ZAGRADA" &&
		cv->djeca[1]->uniformni_znak == "NIZ_ZNAKOVA" &&
		cv->djeca[2]->uniformni_znak == "D_ZAGRADA") {
		rek(cv->djeca[1]);

		cv->tip = cv->djeca[1]->tip;
		cv->l_izraz = cv->djeca[1]->l_izraz;
	}
}

void postfiks_izraz(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "PRIMARNI_IZRAZ") {
		rek(cv->djeca[0]);

		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 4 &&
		cv->djeca[0]->uniformni_znak == "POSTFIKS_IZRAZ" &&
		cv->djeca[1]->uniformni_znak == "L_UGL_ZAGRADA" &&
		cv->djeca[2]->uniformni_znak == "IZRAZ" &&
		cv->djeca[3]->uniformni_znak == "D_UGL_ZAGRADA") {
		//
		// TODO
		//
		rek(cv->djeca[0]);
		if (!(cv->djeca[0]->tip == toConst(cv->tip))) {
			kraj(cv);
		}
		rek(cv->djeca[2]);
		if (!canCastToInt(cv->djeca[2]->tip)) {
			kraj(cv);
		}
		cv->djeca[2]->tip = 1;
	}
	if (cv->djeca.size() == 3 &&
		cv->djeca[0]->uniformni_znak == "POSTFIKS_IZRAZ" &&
		cv->djeca[1]->uniformni_znak == "L_ZAGRADA" &&
		cv->djeca[2]->uniformni_znak == "D_ZAGRADA") {
		rek(cv->djeca[0]);
		if (cv->djeca[0]->tip != 10) {
			kraj(cv);
		}

		cv->tip = 9;
		cv->l_izraz = 0;
	}
	if (cv->djeca.size() == 4 &&
		cv->djeca[0]->uniformni_znak == "POSTFIKS_IZRAZ" &&
		cv->djeca[1]->uniformni_znak == "L_ZAGRADA" &&
		cv->djeca[2]->uniformni_znak == "LISTA_ARGUMENATA" &&
		cv->djeca[3]->uniformni_znak == "D_ZAGRADA") {
		rek(cv->djeca[0]);
		rek(cv->djeca[2]);
		if (cv->djeca[0]->tip != 11) {
			kraj(cv);
		}
		if (cv->djeca[0]->tipovi.size() != cv->djeca[2]->tipovi.size()) {
			kraj(cv);
		}
		for (int i = 0; i < cv->djeca[0]->tipovi.size(); i++) {
			if (cv->djeca[0]->tipovi[i] != cv->djeca[2]->tipovi[i]) {
				kraj(cv);
			}
		}

		cv->tip = 9;
		cv->l_izraz = 0;
	}
	if (cv->djeca.size() == 2 &&
		cv->djeca[0]->uniformni_znak == "POSTFIKS_IZRAZ" &&
		(cv->djeca[1]->uniformni_znak == "OP_INC" ||
		cv->djeca[1]->uniformni_znak == "OP_DEC")) {
		rek(cv->djeca[0]);
		if (!(cv->djeca[0]->l_izraz == 1 && canCastToInt(cv->djeca[0]->tip))) {
			kraj(cv);
		}

		cv->tip = 1;
		cv->l_izraz = 0;
	}
}

void lista_argumenata(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "IZRAZ_PRIDRUZIVANJA") {
		rek(cv->djeca[0]);
		
		cv->tipovi = {cv->djeca[0]->tip};
	}
	if (cv->djeca.size() == 3 &&
		cv->djeca[0]->uniformni_znak == "LISTA_ARGUMENATA" &&
		cv->djeca[1]->uniformni_znak == "ZAREZ" &&
		cv->djeca[2]->uniformni_znak == "IZRAZ_PRIDRUZIVANJA") {
		cv->tipovi = cv->djeca[0]->tipovi;
		cv->tipovi.push_back(cv->djeca[2]->tip);

		rek(cv->djeca[0]);
		rek(cv->djeca[2]);
	}
}

void unarni_izraz(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "POSTFIKS_IZRAZ") {
		rek(cv->djeca[0]);
		
		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 2 &&
		(cv->djeca[0]->uniformni_znak == "OP_INC" ||
		cv->djeca[0]->uniformni_znak == "OP_DEC") &&
		cv->djeca[1]->uniformni_znak == "UNARNI_IZRAZ") {
		rek(cv->djeca[1]);
		if (!(cv->djeca[1]->l_izraz == 1 && canCastToInt(cv->djeca[1]->tip))) {
			kraj(cv);
		}

		cv->tip = 1;
		cv->l_izraz = 0;
	}
	if (cv->djeca.size() == 2 &&
		cv->djeca[0]->uniformni_znak == "UNARNI_OPERATOR" &&
		cv->djeca[1]->uniformni_znak == "CAST_IZRAZ") {
		rek(cv->djeca[1]);
		if (!canCastToInt(cv->djeca[1]->tip >= 1)) {
			kraj(cv);
		}

		cv->tip = 1;
		cv->l_izraz = 0;
	}
}

void cast_izraz(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "UNARNI_IZRAZ") {
		rek(cv->djeca[0]);
		
		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 4 &&
		cv->djeca[0]->uniformni_znak == "L_ZAGRADA" &&
		cv->djeca[1]->uniformni_znak == "IME_TIPA" &&
		cv->djeca[2]->uniformni_znak == "D_ZAGRADA" &&
		cv->djeca[3]->uniformni_znak == "CAST_IZRAZ") {
		rek(cv->djeca[1]);
		rek(cv->djeca[3]);
		if (!canCast(cv->djeca[3]->tip, cv->djeca[1]->tip)) {
			kraj(cv);
		}

		cv->tip = cv->djeca[1]->tip;
		cv->l_izraz = 0;
	}
}

void ime_tipa(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "SPECIFIKATOR_TIPA") {
		rek(cv->djeca[0]);
		
		cv->tip = cv->djeca[0]->tip;
	}
	if (cv->djeca.size() == 2 &&
		cv->djeca[0]->uniformni_znak == "KR_CONST" &&
		cv->djeca[1]->uniformni_znak == "SPECIFIKATOR_TIPA") {
		rek(cv->djeca[1]);
		if (cv->djeca[1]->tip == 12) {
			kraj(cv);
		}
		
		cv->tip = toConst(cv->djeca[1]->tip);
	}
}

void specifikator_tipa(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "KR_VOID") {
		cv->tip = 12;
	}
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "KR_CHAR") {
		cv->tip = 2;
	}
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "KR_INT") {
		cv->tip = 1;
	}
}

void multiplikativni_izraz(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "CAST_IZRAZ") {
		rek(cv->djeca[0]);
		
		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 3 &&
		cv->djeca[0]->uniformni_znak == "MULTIPLIKATIVNI_IZRAZ" &&
		(cv->djeca[1]->uniformni_znak == "OP_PUTA" ||
		cv->djeca[1]->uniformni_znak == "OP_DIJELI" ||
		cv->djeca[1]->uniformni_znak == "OP_MOD") &&
		cv->djeca[2]->uniformni_znak == "CAST_IZRAZ") {
		rek(cv->djeca[0]);
		if (!canCastToInt(cv->djeca[0]->tip)) {
			kraj(cv);
		}
		rek(cv->djeca[2]);
		if (!canCastToInt(cv->djeca[2]->tip)) {
			kraj(cv);
		}

		cv->tip = 1;
		cv->l_izraz = 0;
	}
}

void aditivni_izraz(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "MULTIPLIKATIVNI_IZRAZ") {
		rek(cv->djeca[0]);
		
		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 3 &&
		cv->djeca[0]->uniformni_znak == "ADITIVNI_IZRAZ" &&
		(cv->djeca[1]->uniformni_znak == "PLUS" ||
		cv->djeca[1]->uniformni_znak == "MINUS") &&
		cv->djeca[2]->uniformni_znak == "MULTIPLIKATIVNI_IZRAZ") {
		//
		// TODO: možda je ovo OP_PLUS i OP_MINUS a ne PLUS i MINUS
		//
		rek(cv->djeca[0]);
		if (!canCastToInt(cv->djeca[0]->tip)) {
			kraj(cv);
		}
		rek(cv->djeca[2]);
		if (!canCastToInt(cv->djeca[2]->tip)) {
			kraj(cv);
		}

		cv->tip = 1;
		cv->l_izraz = 0;
	}
}

void odnosni_izraz(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "ADITIVNI_IZRAZ") {
		rek(cv->djeca[0]);
		
		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 3 &&
		cv->djeca[0]->uniformni_znak == "ODNOSNI_IZRAZ" &&
		(cv->djeca[1]->uniformni_znak == "OP_LT" ||
		cv->djeca[1]->uniformni_znak == "OP_GT" ||
		cv->djeca[1]->uniformni_znak == "OP_LTE" ||
		cv->djeca[1]->uniformni_znak == "OP_GTE") &&
		cv->djeca[2]->uniformni_znak == "ADITIVNI_IZRAZ") {
		rek(cv->djeca[0]);
		if (!canCastToInt(cv->djeca[0]->tip)) {
			kraj(cv);
		}
		rek(cv->djeca[2]);
		if (!canCastToInt(cv->djeca[2]->tip)) {
			kraj(cv);
		}

		cv->tip = 1;
		cv->l_izraz = 0;
	}
}

void jednakosni_izraz(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "ODNOSNI_IZRAZ") {
		rek(cv->djeca[0]);
		
		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 3 &&
		cv->djeca[0]->uniformni_znak == "JEDNAKOSNI_IZRAZ" &&
		(cv->djeca[1]->uniformni_znak == "OP_EQ" ||
		cv->djeca[1]->uniformni_znak == "OP_NEQ") &&
		cv->djeca[2]->uniformni_znak == "ODNOSNI_IZRAZ") {
		rek(cv->djeca[0]);
		if (!canCastToInt(cv->djeca[0]->tip)) {
			kraj(cv);
		}
		rek(cv->djeca[2]);
		if (!canCastToInt(cv->djeca[2]->tip)) {
			kraj(cv);
		}

		cv->tip = 1;
		cv->l_izraz = 0;
	}
}

void bin_i_izraz(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "JEDNAKOSNI_IZRAZ") {
		rek(cv->djeca[0]);
		
		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 3 &&
		cv->djeca[0]->uniformni_znak == "BIN_I_IZRAZ" &&
		(cv->djeca[1]->uniformni_znak == "OP_BIN_I") &&
		cv->djeca[2]->uniformni_znak == "JEDNAKOSNI_IZRAZ") {
		rek(cv->djeca[0]);
		if (!canCastToInt(cv->djeca[0]->tip)) {
			kraj(cv);
		}
		rek(cv->djeca[2]);
		if (!canCastToInt(cv->djeca[2]->tip)) {
			kraj(cv);
		}

		cv->tip = 1;
		cv->l_izraz = 0;
	}
}

void bin_xili_izraz(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "BIN_I_IZRAZ") {
		rek(cv->djeca[0]);
		
		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 3 &&
		cv->djeca[0]->uniformni_znak == "BIN_XILI_IZRAZ" &&
		(cv->djeca[1]->uniformni_znak == "OP_BIN_XILI") &&
		cv->djeca[2]->uniformni_znak == "BIN_I_IZRAZ") {
		rek(cv->djeca[0]);
		if (!canCastToInt(cv->djeca[0]->tip)) {
			kraj(cv);
		}
		rek(cv->djeca[2]);
		if (!canCastToInt(cv->djeca[2]->tip)) {
			kraj(cv);
		}

		cv->tip = 1;
		cv->l_izraz = 0;
	}
}

void bin_ili_izraz(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "BIN_XILI_IZRAZ") {
		rek(cv->djeca[0]);
		
		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 3 &&
		cv->djeca[0]->uniformni_znak == "BIN_ILI_IZRAZ" &&
		(cv->djeca[1]->uniformni_znak == "OP_BIN_ILI") &&
		cv->djeca[2]->uniformni_znak == "BIN_XILI_IZRAZ") {
		rek(cv->djeca[0]);
		if (!canCastToInt(cv->djeca[0]->tip)) {
			kraj(cv);
		}
		rek(cv->djeca[2]);
		if (!canCastToInt(cv->djeca[2]->tip)) {
			kraj(cv);
		}

		cv->tip = 1;
		cv->l_izraz = 0;
	}
}

void log_i_izraz(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "BIN_ILI_IZRAZ") {
		rek(cv->djeca[0]);
		
		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 3 &&
		cv->djeca[0]->uniformni_znak == "LOG_I_IZRAZ" &&
		(cv->djeca[1]->uniformni_znak == "OP_I") &&
		cv->djeca[2]->uniformni_znak == "BIN_ILI_IZRAZ") {
		rek(cv->djeca[0]);
		if (!canCastToInt(cv->djeca[0]->tip)) {
			kraj(cv);
		}
		rek(cv->djeca[2]);
		if (!canCastToInt(cv->djeca[2]->tip)) {
			kraj(cv);
		}

		cv->tip = 1;
		cv->l_izraz = 0;
	}
}

void log_ili_izraz(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "LOG_I_IZRAZ") {
		rek(cv->djeca[0]);
		
		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 3 &&
		cv->djeca[0]->uniformni_znak == "LOG_ILI_IZRAZ" &&
		(cv->djeca[1]->uniformni_znak == "OP_ILI") &&
		cv->djeca[2]->uniformni_znak == "LOG_I_IZRAZ") {
		rek(cv->djeca[0]);
		if (!canCastToInt(cv->djeca[0]->tip)) {
			kraj(cv);
		}
		rek(cv->djeca[2]);
		if (!canCastToInt(cv->djeca[2]->tip)) {
			kraj(cv);
		}

		cv->tip = 1;
		cv->l_izraz = 0;
	}
}

void izraz_pridruzivanja(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "LOG_ILI_IZRAZ") {
		rek(cv->djeca[0]);
		
		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 3 &&
		cv->djeca[0]->uniformni_znak == "POSTFIKS_IZRAZ" &&
		(cv->djeca[1]->uniformni_znak == "OP_PRIDRUZI") &&
		cv->djeca[2]->uniformni_znak == "LOG_ILI_IZRAZ") {
		rek(cv->djeca[0]);
		if (cv->djeca[0]->l_izraz != 1) {
			kraj(cv);
		}
		rek(cv->djeca[2]);
		if (cv->djeca[2]->tip != cv->djeca[0]->tip) {
			kraj(cv);
		}

		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = 0;
	}
}
//
// IZRAZI
// 
// end
//

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
