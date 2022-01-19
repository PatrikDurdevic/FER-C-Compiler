#include <bits/stdc++.h>
/*#include "/Users/patrik/Documents/stdc++.h"*/
using namespace std;

#define REP(i, n) for(int i = 0; i < (n); i++)
typedef long long int ll;

// TODO ako je petlja jednoretcana bez {}, trenutni analizator ne dopusta deklaraciju u novom lokalnom djelokrugu

struct cvor {
	int tip = 0;
	// 0 - undefined, 1 - int, 2 - char, 3 - const(int), 4 - const(char)
	// 5 - niz(int), 6 - niz(char), 7 - niz(const(int)), 8 - niz(const(char))
	// 9 - pov, 10 - funkcija(void -> pov), 11 - funkcija(params -> pov), 12 - void
	bool l_izraz = 0;
	vector <cvor*> djeca;
	cvor *rod = NULL;
	string ime = "";
	vector <string> imena;
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
}

void kraj(cvor *zadnji) {
	cout << zadnji -> uniformni_znak << " ::=";
	for(cvor *c : zadnji -> djeca) {
		cout << " " << c -> uniformni_znak;
		if(c -> red != -1) {
			cout << "(" << c -> red << "," << c -> jedinka << ")";
		}
	}
	cout << "\n";
	exit(0);
}

int br_bloka = 0;
map <string, vector <pair <int, int> > > varijable; // za svako ime varijable pamtim brojeve blokova deklaracija i tip
map <string, vector <int> > odmak_na_stogu_lok;
int tr_odmak = 1;
//map <string, vector <int> > odmak_na_stogu_glob;
map <string, bool> postoji_glob;
vector <pair <string, vector <int> > > glob_vr;

string ime_fje;

map <string, vector <pair <int, pair <vector <int>, int> > > > deklaracije;

map <string, bool> definirana_funkcija;
map <string, bool> deklarirana_funkcija;
map <string, pair <vector <int>, int> > deklaracija_funkcije;
vector <pair <string, pair <vector <int>, int> > > deklarirane_funkcije;

int br_petlji = 0;
vector <int> tipovi_povratnih_vrijednosti;

int if_label_counter = 0;

vector <string> daj_uniformne_znakove_djece(cvor *neki) {
	vector <string> ret;
	for(cvor *dijete : neki -> djeca) ret.push_back(dijete -> uniformni_znak);
	return ret;
}

bool ide_implicitna_pretvorba(int tip1, int tip2) {
	if(tip1 == 1) return tip2 == 1 || tip2 == 3;
	else if(tip1 == 2) return tip2 == 1 || tip2 == 2 || tip2 == 3 || tip2 == 4;
	else if(tip1 == 3) return tip2 == 1 || tip2 == 3;
	else if(tip1 == 4) return tip2 == 1 || tip2 == 2 || tip2 == 3 || tip2 == 4;
	else if(tip1 == 5) return tip2 == 5 || tip2 == 7;
	else if(tip1 == 6) return tip2 == 6 || tip2 == 8;
	else if(tip1 == 7) return tip2 == 7;
	else if(tip1 == 8) return tip2 == 8;
	else return 0;
}

bool vektori_jednaki(vector <int> &prvi, vector <int> &drugi) {
	if(prvi.size() != drugi.size()) return 0;
	REP(i, (int)prvi.size()) if(prvi[i] != drugi[i]) return 0;
	return 1;
}

void greska(cvor *cv) {
	cout << "nije prepoznat ni jedan oblik za unifomni znak " << cv -> uniformni_znak << endl;
	exit(1);
}

void spremi_kontekst() {
	cout << " PUSH R0\n";
	cout << " PUSH R1\n";
	cout << " PUSH R2\n";
	cout << " PUSH R3\n";
	cout << " PUSH R4\n";
	cout << " PUSH R5\n";
}

void obnovi_kontekst() {
	cout << " POP R5\n";
	cout << " POP R4\n";
	cout << " POP R3\n";
	cout << " POP R2\n";
	cout << " POP R1\n";
	cout << " POP R0\n";
}

string veliko(string ime) {
	REP(i, (int)ime.size()) {
		if(ime[i] >= 'a' && ime[i] <= 'z') ime[i] += 'A' - 'a';
	}
	return ime;
}

vector <int> konstante;

// deklaracije svih provjernih funkcija
void primarni_izraz(cvor *cv);
void postfiks_izraz(cvor *cv);
void lista_argumenata(cvor *cv);
void unarni_izraz(cvor *cv);
void unarni_operator(cvor *cv);
void cast_izraz(cvor *cv);
void ime_tipa(cvor *cv);
void specifikator_tipa(cvor *cv);
void multiplikativni_izraz(cvor *cv);
void aditivni_izraz(cvor *cv);
void odnosni_izraz(cvor *cv);
void jednakosni_izraz(cvor *cv);
void bin_i_izraz(cvor *cv);
void bin_xili_izraz(cvor *cv);
void bin_ili_izraz(cvor *cv);
void log_i_izraz(cvor *cv);
void log_ili_izraz(cvor *cv);
void izraz_pridruzivanja(cvor *cv);
void izraz(cvor *cv);
//
void slozena_naredba(cvor *cv);
void lista_naredbi(cvor *cv);
void naredba(cvor *cv);
void izraz_naredba(cvor *cv);
void naredba_grananja(cvor *cv);
void naredba_petlje(cvor *cv);
void naredba_skoka(cvor *cv);
void prijevodna_jedinica(cvor *cv);
void vanjska_deklaracija(cvor *cv);
//
void definicija_funkcije(cvor *cv);
void lista_parametara(cvor *cv);
void deklaracija_parametra(cvor *cv);
void lista_deklaracija(cvor *cv);
void deklaracija(cvor *cv);
void lista_init_deklaratora(cvor *cv);
void init_deklarator(cvor *cv);
void izravni_deklarator(cvor *cv);
void inicijalizator(cvor *cv);
void lista_izraza_pridruzivanja(cvor *cv);
//
//
// IZRAZI
// 
// begin
//
void primarni_izraz(cvor *cv) {
	vector <string> dj = daj_uniformne_znakove_djece(cv);
	if(dj[0] == "IDN") {
		string ime = cv -> djeca[0] -> jedinka;
		if(varijable[ime].size() && (deklarirana_funkcija[ime] || deklaracije[ime].size())) {
			if(deklaracije[ime].size() && deklaracije[ime].back().first > varijable[ime].back().first) {
				cv -> tip = 11;
				cv -> l_izraz = 0;
			}
			else {
				cv -> tip = varijable[ime].back().second;
				if(cv -> tip == 1 || cv -> tip == 2) cv -> l_izraz = 1;
			}
		}
		else if(varijable[ime].size()) {
			cv -> tip = varijable[ime].back().second;
			if(cv -> tip == 1 || cv -> tip == 2) cv -> l_izraz = 1;
		}
		else if(deklarirana_funkcija[ime] || deklaracije[ime].size()) {
			cv -> tip = 11;
			cv -> l_izraz = 0;
		}
		else kraj(cv);
		// TODO ak su polja ovdje ne bu dobro
		// mozda treba pripazit ak je funkcija
		if(odmak_na_stogu_lok[ime].size()) {
			cout << " MOVE %D " << 4 * odmak_na_stogu_lok[ime].back() << ", R0\n";
			cout << " SUB R5, R0, R0\n";
			cout << " LOAD R0, (R0)\n";
			cout << " PUSH R0\n";
		}
		else if(postoji_glob[ime]) {
			cout << " LOAD R0, (KONST_" << veliko(ime) << ")\n";
			cout << " PUSH R0\n";
		}
		else {
			//cout << "varijabla " << ime << " nije deklarirana ili je ime funkcije?\n";
			ime_fje = ime;
		}
	}
	else if(dj[0] == "BROJ") {
		string sbroj = cv -> djeca[0] -> jedinka;
		cv -> tip = 1;
		cv -> l_izraz = 0;
		int vr;
		try {
			vr = stoi(sbroj, nullptr, (sbroj.size() >= 2 && (sbroj[1] == 'x' || sbroj[1] == 'X')) ? 0 : 10);
		}
		catch(...) {
			kraj(cv);
		}
		//
		//cout << " MOVE %D " << vr << ", R0\n";
		cout << " LOAD R0, (KONST_" << konstante.size() << ")\n";
		cout << " PUSH R0\n";
		konstante.push_back(vr);
	}
	else if(dj[0] == "ZNAK") {
		cv -> tip = 2;
		cv -> l_izraz = 0;
		string znak = cv -> djeca[0] -> jedinka;
		if(znak.size() == 3 && (znak[1] == '\\' || znak[1] == '\'')) kraj(cv);
		else if(znak.size() == 4) {
			if(znak[1] != '\\') kraj(cv);
			if(znak[2] != 't' && znak[2] != 'n' && znak[2] != '0' && znak[2] != '\'' && znak[2] != '\"' && znak[2] != '\\') kraj(cv);
		}
		else if(znak.size() > 4) kraj(cv);
		
	}
	else if(dj[0] == "NIZ_ZNAKOVA") {
		cv -> tip = 8;
		cv -> l_izraz = 0;
		string niz_znakova = cv -> djeca[0] -> jedinka;
		for(int i = 1; i < (int)niz_znakova.size() - 1; i++) {
			if(niz_znakova[i] == '\\') {
				if(i == (int)niz_znakova.size() - 2) kraj(cv);
				i++;
				char znak = niz_znakova[i];
				if(znak != 't' && znak != 'n' && znak != '0' && znak != '\'' && znak != '\"' && znak != '\\') kraj(cv);
			}
			else if(niz_znakova[i] == '\"') kraj(cv);
		}
	}
	else if(dj[0] == "L_ZAGRADA") {
		izraz(cv -> djeca[1]);
		cv -> tip = cv -> djeca[1] -> tip;
		cv -> l_izraz = cv -> djeca[1] -> l_izraz;
	}
	else greska(cv);
}

void postfiks_izraz(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "<primarni_izraz>") {
		primarni_izraz(cv->djeca[0]);

		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 4 &&
		cv->djeca[0]->uniformni_znak == "<postfiks_izraz>" &&
		cv->djeca[1]->uniformni_znak == "L_UGL_ZAGRADA" &&
		cv->djeca[2]->uniformni_znak == "<izraz>" &&
		cv->djeca[3]->uniformni_znak == "D_UGL_ZAGRADA") {
		postfiks_izraz(cv->djeca[0]);
		if (!(cv->djeca[0]->tip >= 5 && cv->djeca[0]->tip <= 8)) {
			kraj(cv);
		}
		izraz(cv->djeca[2]);
		if (!ide_implicitna_pretvorba(cv->djeca[2]->tip, 1)) {
			kraj(cv);
		}
		
		cv->tip = cv->djeca[0]->tip;
		if(cv -> tip > 4) cv -> tip -= 4;
		cv->l_izraz = cv -> tip != 3 && cv -> tip != 4;
	}
	if (cv->djeca.size() == 3 &&
		cv->djeca[0]->uniformni_znak == "<postfiks_izraz>" &&
		cv->djeca[1]->uniformni_znak == "L_ZAGRADA" &&
		cv->djeca[2]->uniformni_znak == "D_ZAGRADA") {
		postfiks_izraz(cv->djeca[0]);
		cvor *tr = cv;
		while(tr -> uniformni_znak == "<postfiks_izraz>") tr = tr -> djeca[0];
		string ime;
		if(tr -> djeca[0] -> uniformni_znak == "IDN") { // trebalo bi uvijek vrijedit
			ime = tr -> djeca[0] -> jedinka;
			if(deklaracije[ime].size()) { // zamijenjen redoslijed tako da prvo provjerim lokalne delkaracije
				if(deklaracije[ime].back().second.first.size()) kraj(cv);
				cv -> tip = deklaracije[ime].back().second.second;
			}
			else if(deklarirana_funkcija[ime]) { // dodano
				if(deklaracija_funkcije[ime].first.size()) kraj(cv);
				cv -> tip = deklaracija_funkcije[ime].second;
			}
			else kraj(cv);
		}
		else kraj(cv);
		cv->l_izraz = 0;
		//
		spremi_kontekst();
		cout << " SUB R7, 4, R5\n";
		cout << " CALL F_" << veliko(ime) << "\n";
		obnovi_kontekst();
		//cout << (cv -> tip) << endl;
		if(cv -> tip != 12) cout << " PUSH R6\n";
	}
	if (cv->djeca.size() == 4 &&
		cv->djeca[0]->uniformni_znak == "<postfiks_izraz>" &&
		cv->djeca[1]->uniformni_znak == "L_ZAGRADA" &&
		cv->djeca[2]->uniformni_znak == "<lista_argumenata>" &&
		cv->djeca[3]->uniformni_znak == "D_ZAGRADA") {
		spremi_kontekst(); // da ne zezne parametre
		postfiks_izraz(cv->djeca[0]);
		lista_argumenata(cv->djeca[2]);
		cvor *tr = cv;
		while(tr -> uniformni_znak == "<postfiks_izraz>") tr = tr -> djeca[0];
		string ime;
		if(tr -> djeca[0] -> uniformni_znak == "IDN") { // trebalo bi uvijek vrijedit
			ime = tr -> djeca[0] -> jedinka;
			//cout << " MOVE " << (int)(cv -> djeca[2] -> tipovi).size() << ", R0\n"; // broj parametara
			//cout << " MOVE R0, R1\n";
			if(deklaracije[ime].size()) { // zamijenjen redoslijed tako da prvo provjerim lokalne deklaracije
				if(!(deklaracije[ime].back().second.first.size() == (cv -> djeca[2] -> tipovi).size())) kraj(cv);
				REP(i, (int)(cv -> djeca[2] -> tipovi).size()) {
					if(!ide_implicitna_pretvorba((cv -> djeca[2] -> tipovi)[i], deklaracije[ime].back().second.first[i])) kraj(cv);
					// kopiranje argumenata nakon povratne vrijednosti
					cout << " ADD R7, %D " << i * 4 << ", R0\n";
					cout << " LOAD R1, (R0)\n";
					cout << " SUB R7, %D " << ((int)(cv -> djeca[2] -> tipovi).size() - i + 1) * 4 << ", R0\n";
					cout << " STORE R1, (R0)\n";
				}
				cv -> tip = deklaracije[ime].back().second.second;
			}
			else if(deklarirana_funkcija[ime]) { // dodano
				if(!(deklaracija_funkcije[ime].first.size() == (cv -> djeca[2] -> tipovi).size())) kraj(cv);
				REP(i, (int)(cv -> djeca[2] -> tipovi).size()) {
					if(!ide_implicitna_pretvorba((cv -> djeca[2] -> tipovi)[i], deklaracija_funkcije[ime].first[i])) kraj(cv);
					// kopiranje argumenata nakon povratne vrijednosti
					cout << " ADD R7, %D " << i * 4 << ", R0\n";
					cout << " LOAD R1, (R0)\n";
					cout << " SUB R7, %D " << ((int)(cv -> djeca[2] -> tipovi).size() - i + 1) * 4 << ", R0\n";
					cout << " STORE R1, (R0)\n";
				}
				cv -> tip = deklaracija_funkcije[ime].second;
			}
			else kraj(cv);
		}
		else kraj(cv);
		cv->l_izraz = 0;
		cout << " SUB R7, 4, R5\n";
		cout << " CALL F_" << veliko(ime) << "\n";
		REP(i, (int)(cv -> djeca[2] -> tipovi).size()) cout << " POP R0\n";
		obnovi_kontekst();
		//cout << (cv -> tip) << endl;
		if(cv -> tip != 12) cout << " PUSH R6\n";
	}
	if (cv->djeca.size() == 2 &&
		cv->djeca[0]->uniformni_znak == "<postfiks_izraz>" &&
		(cv->djeca[1]->uniformni_znak == "OP_INC" ||
		cv->djeca[1]->uniformni_znak == "OP_DEC")) {
		postfiks_izraz(cv->djeca[0]);
		if (!(cv->djeca[0]->l_izraz && ide_implicitna_pretvorba(cv->djeca[0]->tip, 1))) {
			kraj(cv);
		}

		cv->tip = 1;
		cv->l_izraz = 0;
		if(cv -> djeca[1] -> uniformni_znak == "OP_INC") {
			cout << " POP R0\n";
			cout << " ADD R0, 1, R0\n";
			cout << " PUSH R0\n";
		}
		else {
			cout << " POP R0\n";
			cout << " SUB R0, 1, R0\n";
			cout << " PUSH R0\n";
		}
	}
}

void lista_argumenata(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "<izraz_pridruzivanja>") {
		izraz_pridruzivanja(cv->djeca[0]);
		
		cv->tipovi = {cv->djeca[0]->tip};
	}
	if (cv->djeca.size() == 3 &&
		cv->djeca[0]->uniformni_znak == "<lista_argumenata>" &&
		cv->djeca[1]->uniformni_znak == "ZAREZ" &&
		cv->djeca[2]->uniformni_znak == "<izraz_pridruzivanja>") {
		lista_argumenata(cv->djeca[0]);
		izraz_pridruzivanja(cv->djeca[2]);
		
		cv->tipovi = cv->djeca[0]->tipovi;
		cv->tipovi.push_back(cv->djeca[2]->tip);
	}
}

void unarni_izraz(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "<postfiks_izraz>") {
		postfiks_izraz(cv->djeca[0]);
		
		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 2 &&
		(cv->djeca[0]->uniformni_znak == "OP_INC" ||
		cv->djeca[0]->uniformni_znak == "OP_DEC") &&
		cv->djeca[1]->uniformni_znak == "<unarni_izraz>") {
		unarni_izraz(cv->djeca[1]);
		if (!(cv->djeca[1]->l_izraz && ide_implicitna_pretvorba(cv->djeca[1]->tip, 1))) {
			kraj(cv);
		}

		cv->tip = 1;
		cv->l_izraz = 0;
		if(cv -> djeca[0] -> uniformni_znak == "OP_INC") {
			cout << " POP R0\n";
			cout << " ADD R0, 1, R0\n";
			cout << " PUSH R0\n";
		}
		else {
			cout << " POP R0\n";
			cout << " SUB R0, 1, R0\n";
			cout << " PUSH R0\n";
		}
	}
	if (cv->djeca.size() == 2 &&
		cv->djeca[0]->uniformni_znak == "<unarni_operator>" &&
		cv->djeca[1]->uniformni_znak == "<cast_izraz>") {
		cast_izraz(cv->djeca[1]);
		if (!ide_implicitna_pretvorba(cv->djeca[1]->tip, 1)) {
			kraj(cv);
		}

		cv->tip = 1;
		cv->l_izraz = 0;
		if(cv -> djeca[0] -> djeca[0] -> uniformni_znak == "MINUS") {
			cout << " POP R0\n";
			cout << " MOVE 0, R1\n";
			cout << " SUB R1, R0, R0\n";
			cout << " PUSH R0\n";
		}
		else if(cv -> djeca[0] -> djeca[0] -> uniformni_znak == "OP_TILDA") {
			cout << " POP R0\n";
			cout << " MOVE 0, R1\n";
			cout << " SUB R1, 1, R1\n";
			cout << " XOR R0, R1, R0\n";
			cout << " PUSH R0\n";
		}
		else if(cv -> djeca[0] -> djeca[0] -> uniformni_znak == "OP_NEG") {
			cout << " POP R0\n";
			cout << " CMP R0, 0\n";
			cout << " MOVE 0, R0\n";
			cout << " CALL_EQ PUNI_R0\n";
			cout << " PUSH R0\n";
		}
		else if(cv -> djeca[0] -> djeca[0] -> uniformni_znak != "PLUS") cout << "YOOOOOO, ne valja tu nes\n";
		// TODO ostalo
	}
}

void unarni_operator(cvor *cv) {
	return;
}

void cast_izraz(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "<unarni_izraz>") {
		unarni_izraz(cv->djeca[0]);
		
		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 4 &&
		cv->djeca[0]->uniformni_znak == "L_ZAGRADA" &&
		cv->djeca[1]->uniformni_znak == "<ime_tipa>" &&
		cv->djeca[2]->uniformni_znak == "D_ZAGRADA" &&
		cv->djeca[3]->uniformni_znak == "<cast_izraz>") {
		ime_tipa(cv->djeca[1]);
		cast_izraz(cv->djeca[3]);
		if (!ide_implicitna_pretvorba(cv -> djeca[3] -> tip, cv -> djeca[1] -> tip) &&
			!(cv -> djeca[3] -> tip <= 4 && cv -> djeca[1] -> tip <= 4)) {
			kraj(cv);
		}

		cv->tip = cv->djeca[1]->tip;
		cv->l_izraz = 0;
	}
}

void ime_tipa(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "<specifikator_tipa>") {
		specifikator_tipa(cv->djeca[0]);
		
		cv->tip = cv->djeca[0]->tip;
	}
	if (cv->djeca.size() == 2 &&
		cv->djeca[0]->uniformni_znak == "KR_CONST" &&
		cv->djeca[1]->uniformni_znak == "<specifikator_tipa>") {
		specifikator_tipa(cv->djeca[1]);
		if (cv->djeca[1]->tip == 12) {
			kraj(cv);
		}
		
		cv->tip = cv->djeca[1]->tip + 2;
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
		cv->djeca[0]->uniformni_znak == "<cast_izraz>") {
		cast_izraz(cv->djeca[0]);
		
		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 3 &&
		cv->djeca[0]->uniformni_znak == "<multiplikativni_izraz>" &&
		(cv->djeca[1]->uniformni_znak == "OP_PUTA" ||
		cv->djeca[1]->uniformni_znak == "OP_DIJELI" ||
		cv->djeca[1]->uniformni_znak == "OP_MOD") &&
		cv->djeca[2]->uniformni_znak == "<cast_izraz>") {
		multiplikativni_izraz(cv->djeca[0]);
		if (!ide_implicitna_pretvorba(cv->djeca[0]->tip, 1)) {
			kraj(cv);
		}
		cast_izraz(cv->djeca[2]);
		if (!ide_implicitna_pretvorba(cv->djeca[2]->tip, 1)) {
			kraj(cv);
		}

		cv->tip = 1;
		cv->l_izraz = 0;
		// TODO
	}
}

void aditivni_izraz(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "<multiplikativni_izraz>") {
		multiplikativni_izraz(cv->djeca[0]);
		
		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 3 &&
		cv->djeca[0]->uniformni_znak == "<aditivni_izraz>" &&
		(cv->djeca[1]->uniformni_znak == "PLUS" ||
		cv->djeca[1]->uniformni_znak == "MINUS") &&
		cv->djeca[2]->uniformni_znak == "<multiplikativni_izraz>") {
		//
		// TODO: možda je ovo OP_PLUS i OP_MINUS a ne PLUS i MINUS
		//
		aditivni_izraz(cv->djeca[0]);
		if (!ide_implicitna_pretvorba(cv->djeca[0]->tip, 1)) {
			kraj(cv);
		}
		multiplikativni_izraz(cv->djeca[2]);
		if (!ide_implicitna_pretvorba(cv->djeca[2]->tip, 1)) {
			kraj(cv);
		}

		cv->tip = 1;
		cv->l_izraz = 0;
		cout << " POP R1\n"; // zamijenjen redoslijed popanja
		cout << " POP R0\n";
		if(cv -> djeca[1] -> uniformni_znak == "PLUS") cout << " ADD R0, R1, R0\n";
		else cout << " SUB R0, R1, R0\n";
		cout << " PUSH R0\n";
	}
}

void odnosni_izraz(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "<aditivni_izraz>") {
		aditivni_izraz(cv->djeca[0]);
		
		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 3 &&
		cv->djeca[0]->uniformni_znak == "<odnosni_izraz>" &&
		(cv->djeca[1]->uniformni_znak == "OP_LT" ||
		cv->djeca[1]->uniformni_znak == "OP_GT" ||
		cv->djeca[1]->uniformni_znak == "OP_LTE" ||
		cv->djeca[1]->uniformni_znak == "OP_GTE") &&
		cv->djeca[2]->uniformni_znak == "<aditivni_izraz>") {
		odnosni_izraz(cv->djeca[0]);
		if (!ide_implicitna_pretvorba(cv->djeca[0]->tip, 1)) {
			kraj(cv);
		}
		aditivni_izraz(cv->djeca[2]);
		if (!ide_implicitna_pretvorba(cv->djeca[2]->tip, 1)) {
			kraj(cv);
		}

		cv->tip = 1;
		cv->l_izraz = 0;
		cout << " POP R1\n"; // zamijenjen redoslijed popanja
		cout << " POP R0\n";
		cout << " CMP R0, R1\n";
		cout << " MOVE 0, R0\n";
		if(cv -> djeca[1] -> uniformni_znak == "OP_LT") cout << " CALL_SLT PUNI_R0\n";
		else if(cv -> djeca[1] -> uniformni_znak == "OP_GT") cout << " CALL_SGT PUNI_R0\n";
		else if(cv -> djeca[1] -> uniformni_znak == "OP_LTE") cout << " CALL_SLE PUNI_R0\n";
		else cout << " CALL_SGE PUNI_R0\n";
		cout << " PUSH R0\n";
	}
}

void jednakosni_izraz(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "<odnosni_izraz>") {
		odnosni_izraz(cv->djeca[0]);
		
		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 3 &&
		cv->djeca[0]->uniformni_znak == "<jednakosni_izraz>" &&
		(cv->djeca[1]->uniformni_znak == "OP_EQ" ||
		cv->djeca[1]->uniformni_znak == "OP_NEQ") &&
		cv->djeca[2]->uniformni_znak == "<odnosni_izraz>") {
		jednakosni_izraz(cv->djeca[0]);
		if (!ide_implicitna_pretvorba(cv->djeca[0]->tip, 1)) {
			kraj(cv);
		}
		odnosni_izraz(cv->djeca[2]);
		if (!ide_implicitna_pretvorba(cv->djeca[2]->tip, 1)) {
			kraj(cv);
		}

		cv->tip = 1;
		cv->l_izraz = 0;
		cout << " POP R0\n";
		cout << " POP R1\n";
		cout << " CMP R0, R1\n";
		cout << " MOVE 0, R0\n";
		if(cv -> djeca[1] -> uniformni_znak == "OP_EQ") cout << " CALL_EQ PUNI_R0\n";
		else cout << " CALL_NE PUNI_R0\n";
		cout << " PUSH R0\n";
	}
}

void bin_i_izraz(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "<jednakosni_izraz>") {
		jednakosni_izraz(cv->djeca[0]);
		
		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 3 &&
		cv->djeca[0]->uniformni_znak == "<bin_i_izraz>" &&
		cv->djeca[1]->uniformni_znak == "OP_BIN_I" &&
		cv->djeca[2]->uniformni_znak == "<jednakosni_izraz>") {
		bin_i_izraz(cv->djeca[0]);
		if (!ide_implicitna_pretvorba(cv->djeca[0]->tip, 1)) {
			kraj(cv);
		}
		jednakosni_izraz(cv->djeca[2]);
		if (!ide_implicitna_pretvorba(cv->djeca[2]->tip, 1)) {
			kraj(cv);
		}

		cv->tip = 1;
		cv->l_izraz = 0;
		cout << " POP R0\n";
		cout << " POP R1\n";
		cout << " AND R0, R1, R0\n";
		cout << " PUSH R0\n";
	}
}

void bin_xili_izraz(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "<bin_i_izraz>") {
		bin_i_izraz(cv->djeca[0]);
		
		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 3 &&
		cv->djeca[0]->uniformni_znak == "<bin_xili_izraz>" &&
		cv->djeca[1]->uniformni_znak == "OP_BIN_XILI" &&
		cv->djeca[2]->uniformni_znak == "<bin_i_izraz>") {
		bin_xili_izraz(cv->djeca[0]);
		if (!ide_implicitna_pretvorba(cv->djeca[0]->tip, 1)) {
			kraj(cv);
		}
		bin_i_izraz(cv->djeca[2]);
		if (!ide_implicitna_pretvorba(cv->djeca[2]->tip, 1)) {
			kraj(cv);
		}

		cv->tip = 1;
		cv->l_izraz = 0;
		cout << " POP R0\n";
		cout << " POP R1\n";
		cout << " XOR R0, R1, R0\n";
		cout << " PUSH R0\n";
	}
}

void bin_ili_izraz(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "<bin_xili_izraz>") {
		bin_xili_izraz(cv->djeca[0]);
		
		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 3 &&
		cv->djeca[0]->uniformni_znak == "<bin_ili_izraz>" &&
		cv->djeca[1]->uniformni_znak == "OP_BIN_ILI" &&
		cv->djeca[2]->uniformni_znak == "<bin_xili_izraz>") {
		bin_ili_izraz(cv->djeca[0]);
		if (!ide_implicitna_pretvorba(cv->djeca[0]->tip, 1)) {
			kraj(cv);
		}
		bin_xili_izraz(cv->djeca[2]);
		if (!ide_implicitna_pretvorba(cv->djeca[2]->tip, 1)) {
			kraj(cv);
		}

		cv->tip = 1;
		cv->l_izraz = 0;
		cout << " POP R0\n";
		cout << " POP R1\n";
		cout << " OR R0, R1, R0\n";
		cout << " PUSH R0\n";
	}
}

void log_i_izraz(cvor *cv) { // TODO short circ, pazi na redoslijed popanja
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "<bin_ili_izraz>") {
		bin_ili_izraz(cv->djeca[0]);
		
		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 3 &&
		cv->djeca[0]->uniformni_znak == "<log_i_izraz>" &&
		cv->djeca[1]->uniformni_znak == "OP_I" &&
		cv->djeca[2]->uniformni_znak == "<bin_ili_izraz>") {
		log_i_izraz(cv->djeca[0]);
		if (!ide_implicitna_pretvorba(cv->djeca[0]->tip, 1)) {
			kraj(cv);
		}
		bin_ili_izraz(cv->djeca[2]);
		if (!ide_implicitna_pretvorba(cv->djeca[2]->tip, 1)) {
			kraj(cv);
		}

		cv->tip = 1;
		cv->l_izraz = 0;
		cout << " POP R0\n";
		cout << " CMP R0, 0\n";
		cout << " CALL_NE PUNI_R0\n";
		cout << " MOVE R0, R1\n";
		cout << " POP R0\n";
		cout << " CMP R0, 0\n";
		cout << " CALL_NE PUNI_R0\n";
		cout << " AND R0, R1, R0\n";
		cout << " PUSH R0\n";
	}
}

void log_ili_izraz(cvor *cv) { // TODO short circ, pazi na redoslijed popanja
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "<log_i_izraz>") {
		log_i_izraz(cv->djeca[0]);
		
		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 3 &&
		cv->djeca[0]->uniformni_znak == "<log_ili_izraz>" &&
		cv->djeca[1]->uniformni_znak == "OP_ILI" &&
		cv->djeca[2]->uniformni_znak == "<log_i_izraz>") {
		log_ili_izraz(cv->djeca[0]);
		if (!ide_implicitna_pretvorba(cv->djeca[0]->tip, 1)) {
			kraj(cv);
		}
		log_i_izraz(cv->djeca[2]);
		if (!ide_implicitna_pretvorba(cv->djeca[2]->tip, 1)) {
			kraj(cv);
		}

		cv->tip = 1;
		cv->l_izraz = 0;
		cout << " POP R0\n";
		cout << " POP R1\n";
		cout << " OR R0, R1, R0\n";
		cout << " CMP R0, 0\n";
		cout << " CALL_NE PUNI_R0\n";
		cout << " PUSH R0\n";
	}
}

void izraz_pridruzivanja(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "<log_ili_izraz>") {
		log_ili_izraz(cv->djeca[0]);
		
		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 3 &&
		cv->djeca[0]->uniformni_znak == "<postfiks_izraz>" &&
		cv->djeca[1]->uniformni_znak == "OP_PRIDRUZI" &&
		cv->djeca[2]->uniformni_znak == "<izraz_pridruzivanja>") {
		postfiks_izraz(cv->djeca[0]);
		if (cv->djeca[0]->l_izraz != 1) {
			kraj(cv);
		}
		izraz_pridruzivanja(cv->djeca[2]);
		if (!ide_implicitna_pretvorba(cv->djeca[2]->tip, cv->djeca[0]->tip)) {
			kraj(cv);
		}

		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = 0;
		// TODO dodi do IDN ili vrijednosti na obje strane i pridruzi
	}
}

void izraz(cvor *cv) {
	if (cv->djeca.size() == 1 &&
		cv->djeca[0]->uniformni_znak == "<izraz_pridruzivanja>") {
		izraz_pridruzivanja(cv->djeca[0]);
		
		cv->tip = cv->djeca[0]->tip;
		cv->l_izraz = cv->djeca[0]->l_izraz;
	}
	if (cv->djeca.size() == 3 &&
		cv->djeca[0]->uniformni_znak == "<izraz>" &&
		cv->djeca[1]->uniformni_znak == "ZAREZ" &&
		cv->djeca[2]->uniformni_znak == "<izraz_pridruzivanja>") {
		izraz(cv->djeca[0]);
		izraz_pridruzivanja(cv->djeca[2]);

		cv->tip = cv->djeca[2]->tip;
		cv->l_izraz = 0;
	}
}
//
// IZRAZI
// 
// end
//

//
// NAREDBENA STRUKTURA PROGRAMA
//
// begin
//
void slozena_naredba(cvor *cv) {
	vector <string> dj = daj_uniformne_znakove_djece(cv);
	br_bloka++;
	if(dj[1] == "<lista_naredbi>") {
		lista_naredbi(cv -> djeca[1]);
	}
	else if(dj[1] == "<lista_deklaracija>") {
		lista_deklaracija(cv -> djeca[1]);
		lista_naredbi(cv -> djeca[2]);
	}
	else greska(cv);
	 //ovo se moze optimizirat tako da kad deklariramo novu varijablu stavimo to u globalni vektor i onda tu micemo
	map <string, vector <pair <int, int> > >::iterator it = varijable.begin();
	while(it != varijable.end()) {
		while((it -> second).size() && (it -> second).back().first == br_bloka) {
			string ime = it -> first;
			if(odmak_na_stogu_lok[ime].size()) {
				odmak_na_stogu_lok[ime].pop_back(); // trebalo bi vrijedit i za fje
				tr_odmak--;
			}
			else cout << "ne postoji varijabla " << ime << " na odmak_na_stogu_lok\n";
			(it -> second).pop_back();
		}
		it++;
	}
	map <string, vector <pair <int, pair <vector <int>, int> > > >::iterator itd = deklaracije.begin();
	while(itd != deklaracije.end()) {
		while((itd -> second).size() && (itd -> second).back().first == br_bloka) (itd -> second).pop_back();
		itd++;
	}
	br_bloka--;
}

void lista_naredbi(cvor *cv) {
	vector <string> dj = daj_uniformne_znakove_djece(cv);
	if(dj[0] == "<naredba>") {
		naredba(cv -> djeca[0]);
	}
	else if(dj[0] == "<lista_naredbi>") {
		lista_naredbi(cv -> djeca[0]);
		naredba(cv -> djeca[1]);
	}
	else greska(cv);
}

void naredba(cvor *cv) {
	vector <string> dj = daj_uniformne_znakove_djece(cv);
	if(dj[0] == "<slozena_naredba>") {
		slozena_naredba(cv -> djeca[0]);
	}
	else if(dj[0] == "<izraz_naredba>") {
		izraz_naredba(cv -> djeca[0]);
	}
	else if(dj[0] == "<naredba_grananja>") {
		naredba_grananja(cv -> djeca[0]);
	}
	else if(dj[0] == "<naredba_petlje>") {
		naredba_petlje(cv -> djeca[0]);
	}
	else if(dj[0] == "<naredba_skoka>") {
		naredba_skoka(cv -> djeca[0]);
	}
	else greska(cv);
}

void izraz_naredba(cvor *cv) {
	vector <string> dj = daj_uniformne_znakove_djece(cv);
	if(dj[0] == "TOCKAZAREZ") {
		cv -> tip = 1;
	}
	else if(dj[0] == "<izraz>") {
		izraz(cv -> djeca[0]);
		cv -> tip = cv -> djeca[0] -> tip;
	}
	else greska(cv);
}

void naredba_grananja(cvor *cv) {
	vector <string> dj = daj_uniformne_znakove_djece(cv);
	if((int)dj.size() == 5) {
		izraz(cv -> djeca[2]);
		cout << " POP R0\n";
		cout << " CMP R0, 0" << "\n";
		cout << " JP_EQ IF" << if_label_counter << "\n";
		int this_if_label_counter = if_label_counter++;
		if(!ide_implicitna_pretvorba(cv -> djeca[2] -> tip, 1)) kraj(cv); // provjeri da se tip moze implicitno pretvorit u int
		naredba(cv -> djeca[4]);
		cout << "IF" << this_if_label_counter << "\n";
	}
	else if((int)dj.size() == 7) {
		izraz(cv -> djeca[2]);
		if(!ide_implicitna_pretvorba(cv -> djeca[2] -> tip, 1)) kraj(cv); // provjeri da se tip moze implicitno pretvorit u int

		int end_if_label_counter = if_label_counter++;
		int else_label_counter = if_label_counter++;
		cout << " POP R0\n";
		cout << " CMP R0, 0" << "\n";
		cout << " JP_EQ IF" << else_label_counter << "\n";
		naredba(cv -> djeca[4]);
		cout << " JP IF" << end_if_label_counter << "\n";
		cout << "IF" << else_label_counter << "\n";
		naredba(cv -> djeca[6]);
		cout << "IF" << end_if_label_counter << "\n";
	}
	else greska(cv);
}

void naredba_petlje(cvor *cv) {
	vector <string> dj = daj_uniformne_znakove_djece(cv);
	if(dj[4] == "<naredba>") {
		izraz(cv -> djeca[2]);
		if(!ide_implicitna_pretvorba(cv -> djeca[2] -> tip, 1)) kraj(cv); // provjeri da se tip moze implicitno pretvorit u int
		br_petlji++;
		naredba(cv -> djeca[4]);
	}
	else if(dj[4] == "D_ZAGRADA") {
		izraz_naredba(cv -> djeca[2]);
		izraz_naredba(cv -> djeca[3]);
		if(!ide_implicitna_pretvorba(cv -> djeca[3] -> tip, 1)) kraj(cv); // provjeri da se tip moze implicitno pretvorit u int
		br_petlji++;
		naredba(cv -> djeca[5]);
	}
	else if(dj[4] == "<izraz>") {
		izraz_naredba(cv -> djeca[2]);
		izraz_naredba(cv -> djeca[3]);
		if(!ide_implicitna_pretvorba(cv -> djeca[3] -> tip, 1)) kraj(cv); // provjeri da se tip moze implicitno pretvorit u int
		izraz(cv -> djeca[4]);
		br_petlji++;
		naredba(cv -> djeca[6]);
		
	}
	else greska(cv);
	br_petlji--;
}

void naredba_skoka(cvor *cv) {
	vector <string> dj = daj_uniformne_znakove_djece(cv);
	if(dj[0] == "KR_CONTINUE" || dj[0] == "KR_BREAK") {
		if(!br_petlji) kraj(cv);
	}
	else if(dj[1] == "TOCKAZAREZ") {
		if(!tipovi_povratnih_vrijednosti.size() || tipovi_povratnih_vrijednosti.back() != 12) kraj(cv); // ako nema fje ili ako ne vraca void
		//
		cout << " MOVE R5, R7\n"; // brisem  lokalne varijable sa stoga
		cout << " RET\n";
	}
	else if(dj[1] == "<izraz>") {
		izraz(cv -> djeca[1]);
		if(!tipovi_povratnih_vrijednosti.size() || !ide_implicitna_pretvorba(cv -> djeca[1] -> tip, tipovi_povratnih_vrijednosti.back())) kraj(cv);
		//
		cout << " POP R0\n";
		cout << " MOVE R0, R6\n";
		cout << " MOVE R5, R7\n";
		cout << " RET\n";
	}
	else greska(cv);
}

void prijevodna_jedinica(cvor *cv) {
	vector <string> dj = daj_uniformne_znakove_djece(cv);
	if(dj[0] == "<vanjska_deklaracija>") {
		vanjska_deklaracija(cv -> djeca[0]);
	}
	else if(dj[0] == "<prijevodna_jedinica>") {
		prijevodna_jedinica(cv -> djeca[0]);
		vanjska_deklaracija(cv -> djeca[1]);
	}
	else greska(cv);
}

void vanjska_deklaracija(cvor *cv) {
	vector <string> dj = daj_uniformne_znakove_djece(cv);
	if(dj[0] == "<definicija_funkcije>") {
		definicija_funkcije(cv -> djeca[0]);
	}
	else if(dj[0] == "<deklaracija>") {
		deklaracija(cv -> djeca[0]);
	}
	else greska(cv);
}

//
// NAREDBENA STRUKTURA PROGRAMA
//
// end
//

//
// DEKLARACIJE I DEFINICIJE
//
// begin
//
void definicija_funkcije(cvor *cv) {
	vector <string> dj = daj_uniformne_znakove_djece(cv);
	if(dj[3] == "KR_VOID") {
		ime_tipa(cv -> djeca[0]);
		if(cv -> djeca[0] -> tip == 3 || cv -> djeca[0] -> tip == 4) kraj(cv);
		string ime = cv -> djeca[1] -> jedinka; // ovo bi trebalo bit ime, ali svojstvo ime je zapravo jednika
		cout << "F_" << veliko(ime) << "\n";
		if(definirana_funkcija[ime]) kraj(cv);
		if(deklarirana_funkcija[ime]) {
			if(deklaracija_funkcije[ime].first.size() || deklaracija_funkcije[ime].second != cv -> djeca[0] -> tip) kraj(cv);
		}
		if(varijable[ime].size()) kraj(cv); // TODO dodano, provjeriti, valjda sprjecava vanjsku deklaraciju varijable i funkcije istog imena
		definirana_funkcija[ime] = 1;
		deklarirana_funkcija[ime] = 1;
		deklaracija_funkcije[ime] = make_pair(vector <int>(), cv -> djeca[0] -> tip);
		tipovi_povratnih_vrijednosti.push_back(cv -> djeca[0] -> tip);
		slozena_naredba(cv -> djeca[5]);
	}
	else if(dj[3] == "<lista_parametara>") {
		ime_tipa(cv -> djeca[0]);
		if(cv -> djeca[0] -> tip == 3 || cv -> djeca[0] -> tip == 4) kraj(cv);
		string ime = cv -> djeca[1] -> jedinka; // ovo bi trebalo bit ime, ali svojstvo ime je zapravo jednika
		cout << "F_" << veliko(ime) << "\n";
		if(definirana_funkcija[ime]) kraj(cv);
		lista_parametara(cv -> djeca[3]);
		cout << " SUB R7, %D " << 4 * (int)(cv -> djeca[3] -> tipovi).size() << ", R7\n";
		if(deklarirana_funkcija[ime]) {
			if(!vektori_jednaki(cv -> djeca[3] -> tipovi, deklaracija_funkcije[ime].first) || cv -> djeca[0] -> tip != deklaracija_funkcije[ime].second) kraj(cv);
		}
		if(varijable[ime].size()) kraj(cv); // TODO dodano, provjeriti, valjda sprjecava vanjsku deklaraciju varijable i funkcije istog imena
		definirana_funkcija[ime] = 1;
		deklarirana_funkcija[ime] = 1;
		deklaracija_funkcije[ime] = make_pair(cv -> djeca[3] -> tipovi, cv -> djeca[0] -> tip);
		tipovi_povratnih_vrijednosti.push_back(cv -> djeca[0] -> tip);
		REP(i, (int)(cv -> djeca[3] -> tipovi).size()) { // dodavanje parametara u mapu varijabli za sljedeci blok
			varijable[(cv -> djeca[3] -> imena)[i]].push_back(make_pair(br_bloka + 1, (cv -> djeca[3] -> tipovi)[i]));
			odmak_na_stogu_lok[(cv -> djeca[3] -> imena)[i]].push_back(tr_odmak);
			tr_odmak++;
		}
		slozena_naredba(cv -> djeca[5]);
	}
	else greska(cv);
	tipovi_povratnih_vrijednosti.pop_back();
}

void lista_parametara(cvor *cv) {
	vector <string> dj = daj_uniformne_znakove_djece(cv);
	if(dj[0] == "<deklaracija_parametra>") {
		deklaracija_parametra(cv -> djeca[0]);
		(cv -> tipovi).push_back(cv -> djeca[0] -> tip);
		(cv -> imena).push_back(cv -> djeca[0] -> ime);
	}
	else if(dj[0] == "<lista_parametara>") {
		lista_parametara(cv -> djeca[0]);
		deklaracija_parametra(cv -> djeca[2]);
		cv -> tipovi = cv -> djeca[0] -> tipovi;
		cv -> imena = cv -> djeca[0] -> imena;
		REP(i, (int)(cv -> imena).size()) if((cv -> imena)[i] == cv -> djeca[2] -> ime) kraj(cv);
		(cv -> tipovi).push_back(cv -> djeca[2] -> tip);
		(cv -> imena).push_back(cv -> djeca[2] -> ime);
	}
	else greska(cv);
}

void deklaracija_parametra(cvor *cv) {
	vector <string> dj = daj_uniformne_znakove_djece(cv);
	if((int)dj.size() == 2) {
		ime_tipa(cv -> djeca[0]);
		if(cv -> djeca[0] -> tip == 12) kraj(cv);
		cv -> tip = cv -> djeca[0] -> tip;
		cv -> ime = cv -> djeca[1] -> jedinka; // odn. ime
	}
	else if((int)dj.size() == 4) {
		ime_tipa(cv -> djeca[0]);
		if(cv -> djeca[0] -> tip == 12) kraj(cv);
		cv -> tip = cv -> djeca[0] -> tip + 4; // pretvaranje X u niz(X)
		cv -> ime = cv -> djeca[1] -> jedinka; // odn. ime
	}
	else greska(cv);
}

void lista_deklaracija(cvor *cv) {
	vector <string> dj = daj_uniformne_znakove_djece(cv);
	if(dj[0] == "<deklaracija>") {
		deklaracija(cv -> djeca[0]);
	}
	else if(dj[0] == "<lista_deklaracija>") {
		lista_deklaracija(cv -> djeca[0]);
		deklaracija(cv -> djeca[1]);
	}
	else greska(cv);
}

void deklaracija(cvor *cv) {
	vector <string> dj = daj_uniformne_znakove_djece(cv);
	if(dj[0] == "<ime_tipa>") {
		ime_tipa(cv -> djeca[0]);
		cv -> djeca[1] -> ntip = cv -> djeca[0] -> tip;
		lista_init_deklaratora(cv -> djeca[1]);
	}
	else greska(cv);
}

void lista_init_deklaratora(cvor *cv) {
	vector <string> dj = daj_uniformne_znakove_djece(cv);
	if(dj[0] == "<init_deklarator>") {
		cv -> djeca[0] -> ntip = cv -> ntip;
		init_deklarator(cv -> djeca[0]);
	}
	else if(dj[0] == "<lista_init_deklaratora>") {
		cv -> djeca[0] -> ntip = cv -> ntip;
		lista_init_deklaratora(cv -> djeca[0]);
		cv -> djeca[2] -> ntip = cv -> ntip;
		init_deklarator(cv -> djeca[2]);
	}
	else greska(cv);
}

void init_deklarator(cvor *cv) {
	vector <string> dj = daj_uniformne_znakove_djece(cv);
	if((int)dj.size() == 1) {
		cv -> djeca[0] -> ntip = cv -> ntip;
		izravni_deklarator(cv -> djeca[0]);
		if(cv -> djeca[0] -> tip == 3 || cv -> djeca[0] -> tip == 4 || cv -> djeca[0] -> tip == 7 || cv -> djeca[0] -> tip == 8) kraj(cv);
	}
	else if((int)dj.size() == 3) {
		cv -> djeca[0] -> ntip = cv -> ntip;
		izravni_deklarator(cv -> djeca[0]);
		if(br_bloka) { // dodan if da ne ispisuje komande izvan funkcija
			inicijalizator(cv -> djeca[2]);
			cout << " POP R0\n";
			cout << " POP R1\n";
			cout << " PUSH R0\n";
		}
		else {
			cvor *tr = cv -> djeca[2];
			if((int)(tr -> djeca).size() == 1) {
				while(tr -> uniformni_znak != "<primarni_izraz>") tr = tr -> djeca[0];
				if(tr -> djeca[0] -> uniformni_znak != "BROJ") {
					cout << "globalna varijabla definirana s ne konstantom... Joj\n";
				}
				else {
					string sbroj = tr -> djeca[0] -> jedinka;
					int vr;
					try {
						vr = stoi(sbroj, nullptr, (sbroj.size() >= 2 && (sbroj[1] == 'x' || sbroj[1] == 'X')) ? 0 : 10);
					}
					catch(...) {
						kraj(cv);
					}
					glob_vr.back().second[0] = vr;
				}
			}
			else {
				// nizovi...
			}
		}
		/*if(cv -> djeca[0] -> tip >= 1 && cv -> djeca[0] -> tip <= 4) {
			if(!ide_implicitna_pretvorba(cv -> djeca[2] -> tip, cv -> djeca[0] -> tip)) kraj(cv); // u uputama forsiraju provjeru u T, ali je to identicno ko i u const(T)
		}
		else if(cv -> djeca[0] -> tip >= 5 && cv -> djeca[0] -> tip <= 8) {
			if(cv -> djeca[2] -> br_elem == -1) kraj(cv); // ako u inicijalizatoru nisam postavio br_elem jer nisam nasao niz_znakova
			if(cv -> djeca[2] -> br_elem > cv -> djeca[0] -> br_elem) kraj(cv);
			int T = cv -> djeca[0] -> tip - 4;
			REP(i, (int)(cv -> djeca[2] -> tipovi).size()) {
				if(!ide_implicitna_pretvorba((cv -> djeca[2] -> tipovi)[i], T)) kraj(cv);
			}
		}
		else kraj(cv);*/ // zakomentirano da se nes ne zezne
	}
	else greska(cv);
}

void izravni_deklarator(cvor *cv) {
	vector <string> dj = daj_uniformne_znakove_djece(cv);
	if((int)dj.size() == 1) {
		cv -> tip = cv -> ntip;
		if(cv -> ntip == 12) kraj(cv);
		string ime = cv -> djeca[0] -> jedinka;
		if(varijable[ime].size() && varijable[ime].back().first == br_bloka) kraj(cv);
		if(deklaracije[ime].size() && deklaracije[ime].back().first == br_bloka) kraj(cv); // dodano
		varijable[ime].push_back(make_pair(br_bloka, cv -> tip));
		if(!br_bloka) {
			glob_vr.push_back(make_pair(ime, vector <int>(1, 0)));
			postoji_glob[ime] = 1;
		}
		else {
			cout << " PUSH R0\n";
			odmak_na_stogu_lok[ime].push_back(tr_odmak);
			tr_odmak++;
		}
	}
	else if(dj[2] == "BROJ") {
		cv -> tip = cv -> ntip + 4;
		if(cv -> ntip == 12) kraj(cv);
		string ime = cv -> djeca[0] -> jedinka;
		if(varijable[ime].size() && varijable[ime].back().first == br_bloka) kraj(cv);
		string sbroj = cv -> djeca[2] -> jedinka;
		int broj;
		try {
			broj = stoi(sbroj, nullptr, (sbroj.size() >= 2 && (sbroj[1] == 'x' || sbroj[1] == 'X')) ? 0 : 10);
			if(broj <= 0 || broj > 1024) kraj(cv);
		}
		catch(...) {
			kraj(cv);
		}
		cv -> br_elem = broj;
		varijable[ime].push_back(make_pair(br_bloka, cv -> tip));
		// TODO slicno za nizove
	}
	else if(dj[2] == "KR_VOID") {
		cv -> tip = 10; // skroz nepotrebno
		string ime = cv -> djeca[0] -> jedinka;
		if(deklaracije[ime].size() && deklaracije[ime].back().first == br_bloka) {
			if(deklaracije[ime].back().second.first.size() || deklaracije[ime].back().second.second != cv -> ntip) kraj(cv);
		}
		else if(varijable[ime].size() && varijable[ime].back().first == br_bloka) kraj(cv); //dodano
		else {
			deklaracije[ime].push_back(make_pair(br_bloka, make_pair(vector <int>(), cv -> ntip)));
			deklarirane_funkcije.push_back(make_pair(ime, make_pair(vector <int>(), cv -> ntip)));
		}
	}
	else if(dj[2] == "<lista_parametara>") {
		cv -> tip = 11; // skroz nepotrebno
		lista_parametara(cv -> djeca[2]);
		string ime = cv -> djeca[0] -> jedinka;
		vector <int> tipovi = cv -> djeca[2] -> tipovi;
		if(deklaracije[ime].size() && deklaracije[ime].back().first == br_bloka) {
			if(!vektori_jednaki(tipovi, deklaracije[ime].back().second.first) || deklaracije[ime].back().second.second != cv -> ntip) kraj(cv);
		}
		else if(varijable[ime].size() && varijable[ime].back().first == br_bloka) kraj(cv); //dodano
		else {
			deklaracije[ime].push_back(make_pair(br_bloka, make_pair(tipovi, cv -> ntip)));
			deklarirane_funkcije.push_back(make_pair(ime, make_pair(tipovi, cv -> ntip)));
		}
	}
	else greska(cv);
}

void inicijalizator(cvor *cv) {
	vector <string> dj = daj_uniformne_znakove_djece(cv);
	if(dj[0] == "<izraz_pridruzivanja>") {
		izraz_pridruzivanja(cv -> djeca[0]);
		bool ok = 1;
		cvor *tr = cv -> djeca[0];
		REP(i, 14) {
			if((int)(tr -> djeca).size() != 1) {
				ok = 0;
				break;
			}
			tr = tr -> djeca[0];
		}
		if(ok && tr -> uniformni_znak == "NIZ_ZNAKOVA") {
			string niz_znakova = tr -> jedinka;
			int br = 0;
			for(int i = 1; i < (int)niz_znakova.size() - 1; i++) { // racunanje duljine niza znakova, provjerit
				br++;
				if(niz_znakova[i] == '\\') i++;
			}
			cv -> br_elem = br + 1; // br_elem je za 1 veci od tipovi zbog \0
			cv -> tipovi = vector <int>(br, 2);
		}
		else {
			cv -> tip = cv -> djeca[0] -> tip;
		}
	}
	else if(dj[0] == "L_VIT_ZAGRADA") {
		lista_izraza_pridruzivanja(cv -> djeca[1]);
		cv -> br_elem = cv -> djeca[1] -> br_elem;
		cv -> tipovi = cv -> djeca[1] -> tipovi;
	}
	else greska(cv);
}

void lista_izraza_pridruzivanja(cvor *cv) {
	vector <string> dj = daj_uniformne_znakove_djece(cv);
	if(dj[0] == "<izraz_pridruzivanja>") {
		izraz_pridruzivanja(cv -> djeca[0]);
		vector <int> tipovi = {cv -> djeca[0] -> tip};
		cv -> tipovi = tipovi;
		cv -> br_elem = 1;
	}
	else if(dj[0] == "<lista_izraza_pridruzivanja>") {
		lista_izraza_pridruzivanja(cv -> djeca[0]);
		izraz_pridruzivanja(cv -> djeca[2]);
		cv -> tipovi = cv -> djeca[0] -> tipovi;
		(cv -> tipovi).push_back(cv -> djeca[2] -> tip);
		cv -> br_elem = cv -> djeca[0] -> br_elem + 1;
	}
	else greska(cv);
}
//
// DEKLARACIJE I DEFINICIJE
//
// end
//

void zavrsi(cvor *root) {
	unisti_stablo(root);
	exit(0);
}

int main() { //TODO povecaj brojac petlji na pravom mjestu za break i continue
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
	cout << " MOVE 40000, R7\n";
	cout << " SUB R7, 4, R5\n"; // ostavljam mjesto za povratnu vrijednost
	cout << " CALL F_MAIN\n";
	cout << " HALT\n";
	prijevodna_jedinica(root);
	if(!deklarirana_funkcija["main"] || deklaracija_funkcije["main"].first.size() || deklaracija_funkcije["main"].second != 1) {
		cout << "main\n";
		zavrsi(root);
	}
	REP(i, (int)deklarirane_funkcije.size()) {
		if(!definirana_funkcija[deklarirane_funkcije[i].first] ||
			!vektori_jednaki(deklaracija_funkcije[deklarirane_funkcije[i].first].first, deklarirane_funkcije[i].second.first) ||
			deklaracija_funkcije[deklarirane_funkcije[i].first].second != deklarirane_funkcije[i].second.second) {
			cout << "funkcija\n";
			zavrsi(root);
		}
	}
	cout << "PUNI_R0 MOVE 1, R0\n";
	cout << " RET\n";
	REP(i, (int)konstante.size()) {
		cout << "KONST_" << i << " DW %D " << konstante[i] << "\n";
	}
	REP(i, (int)glob_vr.size()) {
		cout << "KONST_" << veliko(glob_vr[i].first) << " DW %D " << glob_vr[i].second[0] << "\n";
		for(int j = 1; j < (int)glob_vr[i].second.size(); j++) cout << " DW %D " << glob_vr[i].second[j] << "\n";
	}
	zavrsi(root);
	return 0;
}
