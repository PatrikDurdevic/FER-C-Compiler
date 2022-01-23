#include <bits/stdc++.h>
//#include "/Users/patrik/Documents/stdc++.h"
using namespace std;

#define REP(i, n) for(int i = 0; i < (n); i++)
typedef long long int ll;

//ofstream out("a.frisc");
#define out cout

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
	out << zadnji -> uniformni_znak << " ::=";
	for(cvor *c : zadnji -> djeca) {
		out << " " << c -> uniformni_znak;
		if(c -> red != -1) {
			out << "(" << c -> red << "," << c -> jedinka << ")";
		}
	}
	out << "\n";
	exit(0);
}

int br_bloka = 0;
map <string, vector <pair <int, int> > > varijable; // za svako ime varijable pamtim brojeve blokova deklaracija i tip
map <string, vector <int> > odmak_na_stogu_lok;
int tr_odmak = 1;
//map <string, vector <int> > odmak_na_stogu_glob;
map <string, bool> postoji_glob;
vector <pair <string, vector <int> > > glob_vr;
//map <string, int> vel_niza;

string ime_fje;

map <string, vector <pair <int, pair <vector <int>, int> > > > deklaracije;

map <string, bool> definirana_funkcija;
map <string, bool> deklarirana_funkcija;
map <string, pair <vector <int>, int> > deklaracija_funkcije;
vector <pair <string, pair <vector <int>, int> > > deklarirane_funkcije;

int br_petlji = 0;
vector <int> tipovi_povratnih_vrijednosti;

int if_label_counter = 0;
int for_check_label_counter = 0;
int for_end_label_counter = 0;
int multipl_label_counter = 0;

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
	out << "nije prepoznat ni jedan oblik za unifomni znak " << cv -> uniformni_znak << endl;
	exit(1);
}

void spremi_kontekst() { // realno treba samo R5
	//out << " PUSH R0\n";
	//out << " PUSH R1\n";
	//out << " PUSH R2\n";
	//out << " PUSH R3\n";
	//out << " PUSH R4\n";
	out << " PUSH R5\n";
}

void obnovi_kontekst() {
	out << " POP R5\n";
	//out << " POP R4\n";
	//out << " POP R3\n";
	//out << " POP R2\n";
	//out << " POP R1\n";
	//out << " POP R0\n";
}

string veliko(string ime) {
	REP(i, (int)ime.size()) {
		if(ime[i] >= 'a' && ime[i] <= 'z') ime[i] += 'A' - 'a';
	}
	return ime;
}

vector <int> konstante;
vector <pair <pair <string, int>, bool> > post_varijable;

bool ne_ispisuj = 0;
map <string, int> je_polje;
map <string, vector <string> > imena_parametara_fje;

void povecaj_post_varijable() {
	for(pair <pair <string, int>, bool> p : post_varijable) {
		if(p.first.second == -1) {
			if(odmak_na_stogu_lok[p.first.first].size()) {
				out << " MOVE %D " << 4 * odmak_na_stogu_lok[p.first.first].back() << ", R0\n";
				out << " SUB R5, R0, R0\n";
				out << " LOAD R1, (R0)\n";
				if(p.second) out << " SUB R1, 1, R1\n";
				else out << " ADD R1, 1, R1\n";
				out << " STORE R1, (R0)\n";
			}
			else if(postoji_glob[p.first.first]) {
				out << " LOAD R0, (KONST_" << veliko(p.first.first) << ")\n";
				if(p.second) out << " SUB R0, 1, R0\n";
				else out << " ADD R0, 1, R0\n";
				out << " STORE R0, (KONST_" << veliko(p.first.first) << ")\n";
			}
		}
		else {
			if(odmak_na_stogu_lok[p.first.first].size()) {
				out << " MOVE %D " << 4 * odmak_na_stogu_lok[p.first.first].back() << ", R0\n";
				out << " SUB R5, R0, R0\n";
				out << " SUB R0, %D " << 4 * p.first.second << ", R0\n";
				out << " LOAD R1, (R0)\n";
				if(p.second) out << " SUB R1, 1, R1\n";
				else out << " ADD R1, 1, R1\n";
				out << " STORE R1, (R0)\n";
			}
			else if(postoji_glob[p.first.first]) {
				out << " MOVE KONST_" << veliko(p.first.first) << ", R0\n";
				out << " ADD R0, %D " << 4 * p.first.second << ", R0\n";
				out << " LOAD R1, (R0)\n";
				if(p.second) out << " SUB R1, 1, R1\n";
				else out << " ADD R1, 1, R1\n";
				out << " STORE R1, (R0)\n";
			}
		}
	}
	post_varijable.clear();
	return;
}

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
		if(!ne_ispisuj) {
			int kolko = je_polje[ime] ? je_polje[ime] : 1;
			if(odmak_na_stogu_lok[ime].size()) {
				out << " MOVE %D " << 4 * odmak_na_stogu_lok[ime].back() << ", R0\n";
				out << " SUB R5, R0, R0\n";
				out << " LOAD R0, (R0)\n";
				out << " PUSH R0\n";
				if(kolko > 1) {
					out << " MOVE %D " << 4 * odmak_na_stogu_lok[ime].back() << ", R0\n";
					out << " SUB R5, R0, R0\n";
					for(int i = 1; i < kolko; i++) {
						out << " SUB R0, 4, R0\n";
						out << " LOAD R1, (R0)\n";
						out << " PUSH R1\n";
					}
				}
			}
			else if(postoji_glob[ime]) {
				out << " LOAD R0, (KONST_" << veliko(ime) << ")\n";
				out << " PUSH R0\n";
				if(kolko > 1) {
					out << " MOVE KONST_" << veliko(ime) << ", R0\n";
					for(int i = 1; i < kolko; i++) {
						out << " ADD R0, 4, R0\n";
						out << " LOAD R1, (R0)\n";
						out << " PUSH R1\n";
					}
				}
			}
			else {
				//out << "varijabla " << ime << " nije deklarirana ili je ime funkcije?\n";
				ime_fje = ime;
			}
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
		//out << " MOVE %D " << vr << ", R0\n";
		out << " LOAD R0, (KONST_" << konstante.size() << ")\n";
		out << " PUSH R0\n";
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
		//int vr;
		//if((int)znak.size() == 2) vr = 0;
		//else if((int)znak.size() == 3) vr = znak[1];
		//else if(znak[2] == 't') vr = '\t';
		//else if(znak[2] == 'n') vr = '\n';
		//else if(znak[2] == '0') vr = '\0';
		//else if(znak[2] == '\'') vr = '\'';
		//else if(znak[2] == '\"') vr = '\"';
		//else if(znak[2] == '\\') vr = '\\';
		//out << " MOVE %D " << (int)vr << ", R0\n";
		//out << " PUSH R0\n";
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
		ne_ispisuj = 1;
		postfiks_izraz(cv->djeca[0]);
		ne_ispisuj = 0;
		if (!(cv->djeca[0]->tip >= 5 && cv->djeca[0]->tip <= 8)) {
			kraj(cv);
		}
		izraz(cv->djeca[2]);
		//
		out << " POP R0\n";
		//out << " POP R1\n";
		string ime = cv -> djeca[0] -> djeca[0] -> djeca[0] -> jedinka;
		if(odmak_na_stogu_lok[ime].size()) {
			out << " MOVE %D " << 4 * odmak_na_stogu_lok[ime].back() << ", R1\n";
			out << " SUB R5, R1, R1\n";
			out << " SUB R1, R0, R1\n";
			out << " SUB R1, R0, R1\n";
			out << " SUB R1, R0, R1\n";
			out << " SUB R1, R0, R1\n";
			out << " LOAD R0, (R1)\n";
			out << " PUSH R0\n";
		}
		else if(postoji_glob[ime]) {
			out << " MOVE KONST_" << veliko(ime) << ", R1\n";
			out << " ADD R1, R0, R1\n";
			out << " ADD R1, R0, R1\n";
			out << " ADD R1, R0, R1\n";
			out << " ADD R1, R0, R1\n";
			out << " LOAD R0, (R1)\n";
			out << " PUSH R0\n";
		}
		//
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
		out << " SUB R7, 4, R5\n";
		out << " CALL F_" << veliko(ime) << "\n";
		obnovi_kontekst();
		//out << (cv -> tip) << endl;
		if(cv -> tip != 12) out << " PUSH R6\n";
	}
	if (cv->djeca.size() == 4 &&
		cv->djeca[0]->uniformni_znak == "<postfiks_izraz>" &&
		cv->djeca[1]->uniformni_znak == "L_ZAGRADA" &&
		cv->djeca[2]->uniformni_znak == "<lista_argumenata>" &&
		cv->djeca[3]->uniformni_znak == "D_ZAGRADA") {
		spremi_kontekst(); // da ne zezne parametre
		postfiks_izraz(cv->djeca[0]);
		out << " SUB R7, 4, R7\n"; //
		lista_argumenata(cv->djeca[2]);
		cvor *tr = cv;
		while(tr -> uniformni_znak == "<postfiks_izraz>") tr = tr -> djeca[0];
		string ime;
		int dalje = 0;
		if(tr -> djeca[0] -> uniformni_znak == "IDN") { // trebalo bi uvijek vrijedit
			ime = tr -> djeca[0] -> jedinka;
			//out << " MOVE " << (int)(cv -> djeca[2] -> tipovi).size() << ", R0\n"; // broj parametara
			//out << " MOVE R0, R1\n";
			if(deklaracije[ime].size()) { // zamijenjen redoslijed tako da prvo provjerim lokalne deklaracije
				if(!(deklaracije[ime].back().second.first.size() == (cv -> djeca[2] -> tipovi).size())) kraj(cv);
				REP(i, (int)(cv -> djeca[2] -> tipovi).size()) {
					if(!ide_implicitna_pretvorba((cv -> djeca[2] -> tipovi)[i], deklaracije[ime].back().second.first[i])) kraj(cv);
					// kopiranje argumenata nakon povratne vrijednosti
					//out << " ADD R7, %D " << i * 4 << ", R0\n";
					//out << " LOAD R1, (R0)\n";
					//out << " SUB R7, %D " << ((int)(cv -> djeca[2] -> tipovi).size() - i + 1) * 4 << ", R0\n";
					//out << " STORE R1, (R0)\n";
				}
				cv -> tip = deklaracije[ime].back().second.second;
			}
			else if(deklarirana_funkcija[ime]) { // dodano
				if(!(deklaracija_funkcije[ime].first.size() == (cv -> djeca[2] -> tipovi).size())) kraj(cv);
				REP(i, (int)(cv -> djeca[2] -> tipovi).size()) {
					if(!ide_implicitna_pretvorba((cv -> djeca[2] -> tipovi)[i], deklaracija_funkcije[ime].first[i])) kraj(cv);
					// kopiranje argumenata nakon povratne vrijednosti
					//out << " ADD R7, %D " << i * 4 << ", R0\n";
					//out << " LOAD R1, (R0)\n";
					//out << " SUB R7, %D " << ((int)(cv -> djeca[2] -> tipovi).size() - i + 1) * 4 << ", R0\n";
					//out << " STORE R1, (R0)\n";
				}
				cv -> tip = deklaracija_funkcije[ime].second;
			}
			else kraj(cv);
			for(string s : imena_parametara_fje[ime]) {
				//odmak_na_stogu_lok[s].back() += dalje;
				if(je_polje[s] > 1) dalje += je_polje[s] - 1;
			}
		}
		else kraj(cv);
		cv->l_izraz = 0;
		out << " ADD R7, %D " << ((int)(cv -> djeca[2] -> tipovi).size() + 1 + dalje) * 4 << ", R7\n"; //
		out << " SUB R7, 4, R5\n";
		out << " CALL F_" << veliko(ime) << "\n";
		//REP(i, (int)(cv -> djeca[2] -> tipovi).size()) out << " POP R0\n";
		obnovi_kontekst();
		//out << (cv -> tip) << endl;
		if(cv -> tip != 12) out << " PUSH R6\n";
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
			//out << " POP R0\n";
			//out << " ADD R0, 1, R0\n";
			out << " PUSH R0\n";
			post_varijable.push_back(make_pair(make_pair("", 0), 0));
		}
		else {
			//out << " POP R0\n";
			//out << " SUB R0, 1, R0\n";
			out << " PUSH R0\n";
			post_varijable.push_back(make_pair(make_pair("", 0), 1));
		}
		cvor *tr = cv -> djeca[0] -> djeca[0];
		if(tr -> uniformni_znak == "<primarni_izraz>") {
			post_varijable.back().first.first = tr -> djeca[0] -> jedinka;
			post_varijable.back().first.second = -1;
		}
		else if(tr -> uniformni_znak == "<postfiks_izraz>") {
			post_varijable.back().first.first = tr -> djeca[0] -> djeca[0] -> jedinka;
			cvor *jos = cv -> djeca[0] -> djeca[2];
			while(jos -> uniformni_znak != "<primarni_izraz>") jos = jos -> djeca[0];
			string sbroj = jos -> djeca[0] -> jedinka;
			int odmak;
			try {
				odmak = stoi(sbroj, nullptr, (sbroj.size() >= 2 && (sbroj[1] == 'x' || sbroj[1] == 'X')) ? 0 : 10);
			}
			catch(...) {
				kraj(cv);
			}
			post_varijable.back().first.second = odmak;
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
			out << " POP R0\n";
			out << " ADD R0, 1, R0\n";
			out << " PUSH R0\n";
		}
		else {
			out << " POP R0\n";
			out << " SUB R0, 1, R0\n";
			out << " PUSH R0\n";
		}
		cvor *tr = cv -> djeca[1] -> djeca[0] -> djeca[0];
		if(tr -> uniformni_znak == "<primarni_izraz>") {
			string ime = tr -> djeca[0] -> jedinka;
			if(odmak_na_stogu_lok[ime].size()) {
				out << " MOVE %D " << 4 * odmak_na_stogu_lok[ime].back() << ", R0\n";
				out << " SUB R5, R0, R0\n";
				out << " POP R1\n";
				out << " STORE R1, (R0)\n";
				out << " PUSH R1\n";
			}
			else if(postoji_glob[ime]) {
				out << " POP R0\n";
				out << " STORE R0, (KONST_" << veliko(ime) << ")\n";
				out << " PUSH R0\n";
			}
		}
		else if(tr -> uniformni_znak == "<postfiks_izraz>") {
			string ime = tr -> djeca[0] -> djeca[0] -> jedinka;
			cvor *jos = cv -> djeca[0] -> djeca[2];
			while(jos -> uniformni_znak != "<primarni_izraz>") jos = jos -> djeca[0];
			string sbroj = jos -> djeca[0] -> jedinka;
			int odmak;
			try {
				odmak = stoi(sbroj, nullptr, (sbroj.size() >= 2 && (sbroj[1] == 'x' || sbroj[1] == 'X')) ? 0 : 10);
			}
			catch(...) {
				kraj(cv);
			}
			//out << "Eto " << odmak << endl;
			if(odmak_na_stogu_lok[ime].size()) {
				out << " MOVE %D " << 4 * odmak_na_stogu_lok[ime].back() << ", R0\n";
				out << " SUB R5, R0, R0\n";
				out << " SUB R0, %D " << 4 * odmak << ", R0\n";
				out << " POP R1\n";
				out << " STORE R1, (R0)\n";
				out << " PUSH R1\n";
			}
			else if(postoji_glob[ime]) {
				out << " MOVE KONST_" << veliko(ime) << ", R0\n";
				out << " ADD R0, %D " << 4 * odmak << ", R0\n";
				out << " POP R1\n";
				out << " STORE R1, (R0)\n";
				out << " PUSH R1\n";
			}
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
			out << " POP R0\n";
			out << " MOVE 0, R1\n";
			out << " SUB R1, R0, R0\n";
			out << " PUSH R0\n";
		}
		else if(cv -> djeca[0] -> djeca[0] -> uniformni_znak == "OP_TILDA") {
			out << " POP R0\n";
			out << " MOVE 0, R1\n";
			out << " SUB R1, 1, R1\n";
			out << " XOR R0, R1, R0\n";
			out << " PUSH R0\n";
		}
		else if(cv -> djeca[0] -> djeca[0] -> uniformni_znak == "OP_NEG") {
			out << " POP R0\n";
			out << " CMP R0, 0\n";
			out << " MOVE 0, R0\n";
			out << " CALL_EQ PUNI_R0\n";
			out << " PUSH R0\n";
		}
		else if(cv -> djeca[0] -> djeca[0] -> uniformni_znak != "PLUS") out << "YOOOOOO, ne valja tu nes\n";
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

		out << " POP R1\n"; // zamijenjen redoslijed popanja
		out << " POP R0\n";
		if(cv -> djeca[1] -> uniformni_znak == "OP_PUTA") {
			out << " MOVE 0, R4\n";
			out << " CMP R0, 0\n";
			out << " CALL_SLT DODAJ_R4\n";
			out << " CMP R0, 0\n";
			out << " CALL_SLT NEG_R0\n";
			out << " CMP R1, 0\n";
			out << " CALL_SLT DODAJ_R4\n";
			out << " CMP R1, 0\n";
			out << " CALL_SLT NEG_R1\n";

			out << " MOVE R0, R3\n";
			out << " MOVE 0, R0\n";
			out << "MNOZI" << multipl_label_counter << " SUB R1, 1, R1\n";
			out << " CMP R1, 0\n";
			out << " JP_SLT KRAJ_MNOZI" << multipl_label_counter << "\n";
			out << " ADD R0, R3, R0\n";
			out << " JP MNOZI" << multipl_label_counter << "\n";
			out << "KRAJ_MNOZI" << multipl_label_counter << "\n";
			out << " CMP R4, 1\n";
			out << " CALL_EQ NEG_R0\n";
		} else if(cv -> djeca[1] -> uniformni_znak == "OP_DIJELI") {
			out << " MOVE 0, R4\n";
			out << " CMP R0, 0\n";
			out << " CALL_SLT DODAJ_R4\n";
			out << " CMP R0, 0\n";
			out << " CALL_SLT NEG_R0\n";
			out << " CMP R1, 0\n";
			out << " CALL_SLT DODAJ_R4\n";
			out << " CMP R1, 0\n";
			out << " CALL_SLT NEG_R1\n";

			out << " MOVE 0, R3\n";
			out << "DIJELI" << multipl_label_counter << "\n";
			out << " SUB R0, R1, R0\n";
			out << " CMP R0, 0\n";
			out << " JP_SLT KRAJ_DIJELI" << multipl_label_counter << "\n";
			out << " ADD R3, 1, R3\n";
			out << " JP DIJELI" << multipl_label_counter << "\n";
			out << "KRAJ_DIJELI" << multipl_label_counter << " MOVE R3, R0\n";

			out << " CMP R4, 1\n";
			out << " CALL_EQ NEG_R0\n";
		} else if(cv -> djeca[1] -> uniformni_znak == "OP_MOD") {
			out << " MOVE 0, R4\n";
			out << " CMP R0, 0\n";
			out << " CALL_SLT DODAJ_R4\n";
			out << " CMP R0, 0\n";
			out << " CALL_SLT NEG_R0\n";
			out << " CMP R1, 0\n";
			out << " CALL_SLT DODAJ_R4\n";
			out << " CMP R1, 0\n";
			out << " CALL_SLT NEG_R1\n";

			out << "MOD" << multipl_label_counter << "\n";
			out << " SUB R0, R1, R0\n";

			out << " CMP R0, 0\n";
			out << " JP_SLT KRAJ1_MOD" << multipl_label_counter << "\n";
			out << " CMP R0, 0\n";
			out << " JP_EQ KRAJ_MOD" << multipl_label_counter << "\n";

			out << " JP MOD" << multipl_label_counter << "\n";
			out << "KRAJ1_MOD" << multipl_label_counter << " ADD R0, R1, R0\n";
			out << "KRAJ_MOD" << multipl_label_counter << "\n";

			//out << " CMP R4, 1\n";
			//out << " CALL_EQ NEG_R0\n";
		}
		multipl_label_counter++;
		out << " PUSH R0\n";
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
		out << " POP R1\n"; // zamijenjen redoslijed popanja
		out << " POP R0\n";
		if(cv -> djeca[1] -> uniformni_znak == "PLUS") out << " ADD R0, R1, R0\n";
		else out << " SUB R0, R1, R0\n";
		out << " PUSH R0\n";
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
		out << " POP R1\n"; // zamijenjen redoslijed popanja
		out << " POP R0\n";
		out << " CMP R0, R1\n";
		out << " MOVE 0, R0\n";
		if(cv -> djeca[1] -> uniformni_znak == "OP_LT") out << " CALL_SLT PUNI_R0\n";
		else if(cv -> djeca[1] -> uniformni_znak == "OP_GT") out << " CALL_SGT PUNI_R0\n";
		else if(cv -> djeca[1] -> uniformni_znak == "OP_LTE") out << " CALL_SLE PUNI_R0\n";
		else out << " CALL_SGE PUNI_R0\n";
		out << " PUSH R0\n";
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
		out << " POP R0\n";
		out << " POP R1\n";
		out << " CMP R0, R1\n";
		out << " MOVE 0, R0\n";
		if(cv -> djeca[1] -> uniformni_znak == "OP_EQ") out << " CALL_EQ PUNI_R0\n";
		else out << " CALL_NE PUNI_R0\n";
		out << " PUSH R0\n";
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
		out << " POP R0\n";
		out << " POP R1\n";
		out << " AND R0, R1, R0\n";
		out << " PUSH R0\n";
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
		out << " POP R0\n";
		out << " POP R1\n";
		out << " XOR R0, R1, R0\n";
		out << " PUSH R0\n";
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
		out << " POP R0\n";
		out << " POP R1\n";
		out << " OR R0, R1, R0\n";
		out << " PUSH R0\n";
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
		out << " POP R0\n";
		out << " CMP R0, 0\n";
		out << " CALL_NE PUNI_R0\n";
		out << " MOVE R0, R1\n";
		out << " POP R0\n";
		out << " CMP R0, 0\n";
		out << " CALL_NE PUNI_R0\n";
		out << " AND R0, R1, R0\n";
		out << " PUSH R0\n";
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
		out << " POP R0\n";
		out << " POP R1\n";
		out << " OR R0, R1, R0\n";
		out << " CMP R0, 0\n";
		out << " CALL_NE PUNI_R0\n";
		out << " PUSH R0\n";
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
		cvor *tr = cv -> djeca[0] -> djeca[0];
		if(tr -> uniformni_znak == "<primarni_izraz>") {
			string ime = tr -> djeca[0] -> jedinka;
			if(odmak_na_stogu_lok[ime].size()) {
				out << " MOVE %D " << 4 * odmak_na_stogu_lok[ime].back() << ", R0\n";
				out << " SUB R5, R0, R0\n";
				out << " POP R1\n";
				out << " STORE R1, (R0)\n";
				out << " PUSH R1\n";
			}
			else if(postoji_glob[ime]) {
				out << " POP R0\n";
				out << " STORE R0, (KONST_" << veliko(ime) << ")\n";
				out << " PUSH R0\n";
			}
		}
		else if(tr -> uniformni_znak == "<postfiks_izraz>") {
			string ime = tr -> djeca[0] -> djeca[0] -> jedinka;
			cvor *jos = cv -> djeca[0] -> djeca[2];
			while(jos -> uniformni_znak != "<primarni_izraz>") jos = jos -> djeca[0];
			string sbroj = jos -> djeca[0] -> jedinka;
			int odmak;
			try {
				odmak = stoi(sbroj, nullptr, (sbroj.size() >= 2 && (sbroj[1] == 'x' || sbroj[1] == 'X')) ? 0 : 10);
			}
			catch(...) {
				kraj(cv);
			}
			//out << "Eto " << odmak << endl;
			if(odmak_na_stogu_lok[ime].size()) {
				out << " MOVE %D " << 4 * odmak_na_stogu_lok[ime].back() << ", R0\n";
				out << " SUB R5, R0, R0\n";
				out << " SUB R0, %D " << 4 * odmak << ", R0\n";
				out << " POP R1\n";
				out << " STORE R1, (R0)\n";
				out << " PUSH R1\n";
			}
			else if(postoji_glob[ime]) {
				out << " MOVE KONST_" << veliko(ime) << ", R0\n";
				out << " ADD R0, %D " << 4 * odmak << ", R0\n";
				out << " POP R1\n";
				out << " STORE R1, (R0)\n";
				out << " PUSH R1\n";
			}
		}
		else {
			out << "zas je tu?\n"; // TODO obrisi ovo
		}
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
			else out << "ne postoji varijabla " << ime << " na odmak_na_stogu_lok\n";
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
	povecaj_post_varijable();
}

void naredba_grananja(cvor *cv) {
	vector <string> dj = daj_uniformne_znakove_djece(cv);
	if((int)dj.size() == 5) {
		izraz(cv -> djeca[2]);
		povecaj_post_varijable();
		out << " POP R0\n";

		/* cast u bool */
		out << " CMP R0, 0\n";
		out << " CALL_NE PUNI_R0\n";

		out << " CMP R0, 0" << "\n";
		out << " JP_EQ IF" << if_label_counter << "\n";
		int this_if_label_counter = if_label_counter++;
		if(!ide_implicitna_pretvorba(cv -> djeca[2] -> tip, 1)) kraj(cv); // provjeri da se tip moze implicitno pretvorit u int
		naredba(cv -> djeca[4]);
		out << "IF" << this_if_label_counter << "\n";
	}
	else if((int)dj.size() == 7) {
		izraz(cv -> djeca[2]);
		if(!ide_implicitna_pretvorba(cv -> djeca[2] -> tip, 1)) kraj(cv); // provjeri da se tip moze implicitno pretvorit u int

		int end_if_label_counter = if_label_counter++;
		int else_label_counter = if_label_counter++;
		povecaj_post_varijable();
		out << " POP R0\n";

		/* cast u bool */
		out << " CMP R0, 0\n";
		out << " CALL_NE PUNI_R0\n";

		out << " CMP R0, 0" << "\n";
		out << " JP_EQ IF" << else_label_counter << "\n";
		naredba(cv -> djeca[4]);
		out << " JP IF" << end_if_label_counter << "\n";
		out << "IF" << else_label_counter << "\n";
		naredba(cv -> djeca[6]);
		out << "IF" << end_if_label_counter << "\n";
	}
	else greska(cv);
}

void naredba_petlje(cvor *cv) {
	vector <string> dj = daj_uniformne_znakove_djece(cv);
	if(dj[4] == "<naredba>") {
		int l_for_check_label_counter = for_check_label_counter++;
		int l_for_end_label_counter = for_end_label_counter++;
		out << "FORCHECK" << l_for_check_label_counter << "\n";
		izraz(cv -> djeca[2]);
		povecaj_post_varijable();
		out << " POP R0\n";
		
		/* cast u bool */
		out << " CMP R0, 0\n";
		out << " CALL_NE PUNI_R0\n";
		out << " CMP R0, 0\n";
		out << " JP_EQ FOREND" << l_for_end_label_counter << "\n";

		if(!ide_implicitna_pretvorba(cv -> djeca[2] -> tip, 1)) kraj(cv); // provjeri da se tip moze implicitno pretvorit u int
		br_petlji++;
		naredba(cv -> djeca[4]);

		out << " JP FORCHECK" << l_for_check_label_counter << "\n";
		out << "FOREND" << l_for_end_label_counter << "\n";
	}
	else if(dj[4] == "D_ZAGRADA") {
		izraz_naredba(cv -> djeca[2]);

		int l_for_check_label_counter = for_check_label_counter++;
		int l_for_end_label_counter = for_end_label_counter++;
		out << "FORCHECK" << l_for_check_label_counter << "\n";
		izraz_naredba(cv -> djeca[3]);
		out << " POP R0\n";

		/* cast u bool */
		out << " CMP R0, 0\n";
		out << " CALL_NE PUNI_R0\n";
		out << " CMP R0, 0\n";
		out << " JP_EQ FOREND" << l_for_end_label_counter << "\n";
		if(!ide_implicitna_pretvorba(cv -> djeca[3] -> tip, 1)) kraj(cv); // provjeri da se tip moze implicitno pretvorit u int
		br_petlji++;
		naredba(cv -> djeca[5]);
		out << " JP FORCHECK" << l_for_check_label_counter << "\n";
		out << "FOREND" << l_for_end_label_counter << "\n";
	}
	else if(dj[4] == "<izraz>") {
		izraz_naredba(cv -> djeca[2]);

		int l_for_check_label_counter = for_check_label_counter++;
		int l_for_end_label_counter = for_end_label_counter++;
		out << "FORCHECK" << l_for_check_label_counter << "\n";
		izraz_naredba(cv -> djeca[3]);
		out << " POP R0\n";

		/* cast u bool */
		out << " CMP R0, 0\n";
		out << " CALL_NE PUNI_R0\n";
		out << " CMP R0, 0\n";
		out << " JP_EQ FOREND" << l_for_end_label_counter << "\n";
		if(!ide_implicitna_pretvorba(cv -> djeca[3] -> tip, 1)) kraj(cv); // provjeri da se tip moze implicitno pretvorit u int
		izraz(cv -> djeca[4]);
		povecaj_post_varijable();
		br_petlji++;
		naredba(cv -> djeca[6]);
		out << " JP FORCHECK" << l_for_check_label_counter << "\n";
		out << "FOREND" << l_for_end_label_counter << "\n";
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
		out << " MOVE R5, R7\n"; // brisem  lokalne varijable sa stoga
		out << " RET\n";
	}
	else if(dj[1] == "<izraz>") {
		izraz(cv -> djeca[1]);
		if(!tipovi_povratnih_vrijednosti.size() || !ide_implicitna_pretvorba(cv -> djeca[1] -> tip, tipovi_povratnih_vrijednosti.back())) kraj(cv);
		//
		out << " POP R0\n";
		out << " MOVE R0, R6\n";
		out << " MOVE R5, R7\n";
		out << " RET\n";
	}
	else greska(cv);
	povecaj_post_varijable();
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
		imena_parametara_fje[ime] = vector <string>();
		out << "F_" << veliko(ime) << "\n";
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
		imena_parametara_fje[ime] = vector <string>();
		out << "F_" << veliko(ime) << "\n";
		if(definirana_funkcija[ime]) kraj(cv);
		lista_parametara(cv -> djeca[3]);
		out << " SUB R7, %D " << 4 * ((int)(cv -> djeca[3] -> tipovi).size() + 10) << ", R7\n"; // hmm
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
			imena_parametara_fje[ime].push_back((cv -> djeca[3] -> imena)[i]);
			odmak_na_stogu_lok[(cv -> djeca[3] -> imena)[i]].push_back(tr_odmak);
			tr_odmak++;
		}
		slozena_naredba(cv -> djeca[5]);
	}
	else greska(cv);
	out << " MOVE R5, R7\n"; // ako void nema rijec return
	out << " RET\n";
	tipovi_povratnih_vrijednosti.pop_back();
	tr_odmak = 1; // dodano da se resetira odmak
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
	povecaj_post_varijable();
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
			cvor *tr = cv -> djeca[2];
			if((int)(tr -> djeca).size() == 1) {
				while(tr -> uniformni_znak != "<primarni_izraz>") tr = tr -> djeca[0];
				if(tr -> djeca[0] -> uniformni_znak != "BROJ" && tr -> djeca[0] -> uniformni_znak != "ZNAK") {
					string niz_znakova = tr -> djeca[0] -> jedinka;
					int br = 0, vr = 0;
					out << " MOVE %D " << 4 * odmak_na_stogu_lok[cv -> djeca[0] -> djeca[0] -> jedinka].back() << ", R0\n";
					out << " SUB R5, R0, R0\n";
					for(int i = 1; i < (int)niz_znakova.size() - 1; i++) {
						if(niz_znakova[i] != '\\') vr = niz_znakova[i];
						else if(niz_znakova[i + 1] == 't') vr = '\t';
						else if(niz_znakova[i + 1] == 'n') vr = '\n';
						else if(niz_znakova[i + 1] == '0') vr = '\0';
						else if(niz_znakova[i + 1] == '\'') vr = '\'';
						else if(niz_znakova[i + 1] == '\"') vr = '\"';
						else if(niz_znakova[i + 1] == '\\') vr = '\\';
						out << " MOVE %D " << (int)vr << ", R1\n";
						out << " STORE R1, (R0)\n";
						out << " SUB R0, 4, R0\n";
						br++;
					}
					vr = (int)'\0';
					out << " MOVE %D " << (int)vr << ", R1\n";
					out << " STORE R1, (R0)\n";
					
				}
				else {
					inicijalizator(cv -> djeca[2]);
					out << " POP R0\n";
					out << " POP R1\n";
					out << " PUSH R0\n";
				}
			}
			else {
				string ime = cv -> djeca[0] -> djeca[0] -> jedinka;
				tr = tr -> djeca[1];
				int brojac = 0;
				vector <char> ini;
				do {
					cvor *tr_izraz;
					if((int)(tr -> djeca).size() == 1) tr_izraz = tr -> djeca[0];
					else tr_izraz = tr -> djeca[2];
					while(tr_izraz -> uniformni_znak != "<primarni_izraz>") tr_izraz = tr_izraz -> djeca[0];
					int vr = 0;
					if(tr_izraz -> djeca[0] -> uniformni_znak == "BROJ") {
						string sbroj = tr_izraz -> djeca[0] -> jedinka;
						try {
							vr = stoi(sbroj, nullptr, (sbroj.size() >= 2 && (sbroj[1] == 'x' || sbroj[1] == 'X')) ? 0 : 10);
						}
						catch(...) {
							kraj(cv);
						}
					}
					else if(tr_izraz -> djeca[0] -> uniformni_znak == "ZNAK") {
						string znak = tr_izraz -> djeca[0] -> jedinka;
						if((int)znak.size() == 2) vr = 0;
						else if((int)znak.size() == 3) vr = znak[1];
						else if(znak[2] == 't') vr = '\t';
						else if(znak[2] == 'n') vr = '\n';
						else if(znak[2] == '0') vr = '\0';
						else if(znak[2] == '\'') vr = '\'';
						else if(znak[2] == '\"') vr = '\"';
						else if(znak[2] == '\\') vr = '\\';
					}
					ini.push_back(vr);
					tr = tr -> djeca[0];
					brojac++;
				} while(tr -> uniformni_znak != "<izraz_pridruzivanja>");
				reverse(ini.begin(), ini.end());
				out << " MOVE %D " << 4 * odmak_na_stogu_lok[ime].back() << ", R0\n";
				out << " SUB R5, R0, R0\n";
				REP(i, (int)ini.size()) {
					out << " MOVE %D " << (int)ini[i] << ", R1\n";
					out << " STORE R1, (R0)\n";
					out << " SUB R0, 4, R0\n";
				}
			}
			// TODO ifaj i pitaj ak je niz pa prekopiraj
		}
		else {
			cvor *tr = cv -> djeca[2];
			if((int)(tr -> djeca).size() == 1) {
				while(tr -> uniformni_znak != "<primarni_izraz>") tr = tr -> djeca[0];
				if(tr -> djeca[0] -> uniformni_znak != "BROJ" && tr -> djeca[0] -> uniformni_znak != "ZNAK") {
					string niz_znakova = tr -> djeca[0] -> jedinka;
					int br = 0, vr = 0;
					for(int i = 1; i < (int)niz_znakova.size() - 1; i++) {
						if(niz_znakova[i] != '\\') vr = niz_znakova[i];
						else if(niz_znakova[i + 1] == 't') vr = '\t';
						else if(niz_znakova[i + 1] == 'n') vr = '\n';
						else if(niz_znakova[i + 1] == '0') vr = '\0';
						else if(niz_znakova[i + 1] == '\'') vr = '\'';
						else if(niz_znakova[i + 1] == '\"') vr = '\"';
						else if(niz_znakova[i + 1] == '\\') vr = '\\';
						glob_vr.back().second[br] = vr;
						br++;
					}
				}
				else if(tr -> djeca[0] -> uniformni_znak == "BROJ"){
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
				else {
					int vr = 0;
					string znak = tr-> djeca[0] -> jedinka;
					if((int)znak.size() == 2) vr = 0;
					else if((int)znak.size() == 3) vr = znak[1];
					else if(znak[2] == 't') vr = '\t';
					else if(znak[2] == 'n') vr = '\n';
					else if(znak[2] == '0') vr = '\0';
					else if(znak[2] == '\'') vr = '\'';
					else if(znak[2] == '\"') vr = '\"';
					else if(znak[2] == '\\') vr = '\\';
					glob_vr.back().second[0] = (int)vr;
				}
			}
			else {
				string ime = cv -> djeca[0] -> djeca[0] -> jedinka;
				tr = tr -> djeca[1];
				int brojac = 0;
				vector <char> ini;
				do {
					cvor *tr_izraz;
					if((int)(tr -> djeca).size() == 1) tr_izraz = tr -> djeca[0];
					else tr_izraz = tr -> djeca[2];
					while(tr_izraz -> uniformni_znak != "<primarni_izraz>") tr_izraz = tr_izraz -> djeca[0];
					int vr = 0;
					if(tr_izraz -> djeca[0] -> uniformni_znak == "BROJ") {
						string sbroj = tr_izraz -> djeca[0] -> jedinka;
						try {
							vr = stoi(sbroj, nullptr, (sbroj.size() >= 2 && (sbroj[1] == 'x' || sbroj[1] == 'X')) ? 0 : 10);
						}
						catch(...) {
							kraj(cv);
						}
					}
					else if(tr_izraz -> djeca[0] -> uniformni_znak == "ZNAK") {
						string znak = tr_izraz -> djeca[0] -> jedinka;
						if((int)znak.size() == 2) vr = 0;
						else if((int)znak.size() == 3) vr = znak[1];
						else if(znak[2] == 't') vr = '\t';
						else if(znak[2] == 'n') vr = '\n';
						else if(znak[2] == '0') vr = '\0';
						else if(znak[2] == '\'') vr = '\'';
						else if(znak[2] == '\"') vr = '\"';
						else if(znak[2] == '\\') vr = '\\';
					}
					//out << " MOVE %D " << (int)vr << ", R0\n";
					//out << " MOVE KONST_" << veliko(ime) << ", R1\n";
					//out << " ADD R1, %D " << 4 * brojac << "\n";
					//out << " STORE R0, (R1)\n";
					//
					//glob_vr.back().second[(int)glob_vr.back().second.size() - 1 - brojac] = vr;
					ini.push_back(vr);
					tr = tr -> djeca[0];
					brojac++;
				} while(tr -> uniformni_znak != "<izraz_pridruzivanja>");
				reverse(ini.begin(), ini.end());
				REP(i, (int)ini.size()) {
					glob_vr.back().second[i] = ini[i];
				}
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
			out << " PUSH R0\n";
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
		if(!br_bloka) {
			glob_vr.push_back(make_pair(ime, vector <int>(broj, 0)));
			postoji_glob[ime] = 1;
		}
		else {
			REP(i, broj) out << " PUSH R0\n";
			odmak_na_stogu_lok[ime].push_back(tr_odmak);
			tr_odmak += broj; // TODO problem je kad je niz u scopeu, resetiraj na izlazu iz definicije funkcije
		}
		je_polje[ime] = broj;
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
		//out << tr -> rod -> uniformni_znak << " -> " << tr -> uniformni_znak << endl;
	}
	out << " MOVE 40000, R7\n";
	out << " SUB R7, 4, R5\n"; // ostavljam mjesto za povratnu vrijednost
	out << " CALL F_MAIN\n";
	out << " HALT\n";
	prijevodna_jedinica(root);
	if(!deklarirana_funkcija["main"] || deklaracija_funkcije["main"].first.size() || deklaracija_funkcije["main"].second != 1) {
		out << "main\n";
		zavrsi(root);
	}
	REP(i, (int)deklarirane_funkcije.size()) {
		if(!definirana_funkcija[deklarirane_funkcije[i].first] ||
			!vektori_jednaki(deklaracija_funkcije[deklarirane_funkcije[i].first].first, deklarirane_funkcije[i].second.first) ||
			deklaracija_funkcije[deklarirane_funkcije[i].first].second != deklarirane_funkcije[i].second.second) {
			out << "funkcija\n";
			zavrsi(root);
		}
	}
	out << "PUNI_R0 MOVE 1, R0\n";
	out << " RET\n";
	out << "DODAJ_R4 ADD R4, 1, R4\n";
	out << " RET\n";
	out << "NEG_R0 MOVE 0, R3\n";
	out << " SUB R3, R0, R0\n";
	out << " RET\n";
	out << "NEG_R1 MOVE 0, R3\n";
	out << " SUB R3, R1, R1\n";
	out << " RET\n";
	REP(i, (int)konstante.size()) {
		out << "KONST_" << i << " DW %D " << konstante[i] << "\n";
	}
	REP(i, (int)glob_vr.size()) {
		out << "KONST_" << veliko(glob_vr[i].first) << " DW %D " << glob_vr[i].second[0] << "\n";
		for(int j = 1; j < (int)glob_vr[i].second.size(); j++) out << " DW %D " << glob_vr[i].second[j] << "\n";
	}
	zavrsi(root);
	return 0;
}
