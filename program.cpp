#include <bits/stdc++.h>
using namespace std;

#define REP(i, n) for(int i = 0; i < (n); i++)

struct cvor {
	int tip = 0;
	// 0 - undefined, 1 - int, 2 - char, 3 - const(int), 4 - const(char)
	// 5 - niz(int), 6 - niz(char), 7 - niz(const(int)), 8 - niz(const(char))
	// 9 - pov, 10 - funkcija(void -> pov), 11 - funkcija(params -> pov), 12 - void
	bool l_izraz = 0; // TODO postavi l_izraz za identifikatore koji ga trebaju imat
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
	// bla bla
	exit(0);
}

int br_bloka = 0;
map <string, vector <pair <int, int> > > varijable; // za svako ime varijable pamtim brojeve blokova deklaracija i tip

map <string, vector <pair <int, pair <vector <int>, int> > > > deklaracije;

map <string, bool> definirana_funkcija;
map <string, bool> deklarirana_funkcija;
map <string, pair <vector <int>, int> > deklaracija_funkcije; // TODO provjeri defaultnu vrijednost za cvor*

int br_petlji = 0;
vector <int> tipovi_povratnih_vrijednosti;

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

bool vektori_jednaki(vecotor <int> &prvi, vector <int> &drugi) {
	if(prvi.size() != drugi.size()) return 0;
	REP(i, (int)prvi.size()) if(prvi[i] != drugi[i]) return 0;
	return 1;
}

void greska(cvor *cv) {
	cout << "nije prepoznat ni jedan oblik za unifomni znak " << cv -> uniformni_znak << endl;
	exit(1);
}

{
// deklaracije svih provjernih funkcija
void slozena_naredba(cvor *cv);
void lista_naredbi(cvor *cv);
void naredba(cvor *cv);
void izraz_naredba(cvor *cv);
void naredba_grananja(cvor *cv);
void naredba_petlje(cvor *cv);
void naredba_skoka(cvor *cv);
void prijevodna_jedinica(cvor *cv);
//
void definicija_funkcije(cvor *cv);
void lista_parametara(cvor *cv);
void deklaracija_parametara(cvor *cv);
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
	else if(dj[1] == "lista_deklaracija") {
		lista_deklaracija(cv -> djeca[1]);
		lista_naredbi(cv -> djeca[2]);
	}
	else greska(cv);
	 //ovo se moze optimizirat tako da kad deklariramo novu varijablu stavimo to u globalni vektor i onda tu micemo
	map <string, vector <pair <int, cvor*> > >::iterator it = varijable.begin();
	while(it != varijable.end()) {
		while((it -> second).size() && (it -> second).back().first == br_bloka) (it -> second).pop_back();
		it++;
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
		if(!ide_implicitna_pretvorba(cv -> djeca[2] -> tip, 1)) kraj(cv); // provjeri da se tip moze implicitno pretvorit u int
		naredba(cv -> djeca[4]);
	}
	else if((int)dj.size() == 7) {
		izraz(cv -> djeca[2]);
		if(!ide_implicitna_pretvorba(cv -> djeca[2] -> tip, 1)) kraj(cv); // provjeri da se tip moze implicitno pretvorit u int
		naredba(cv -> djeca[4]);
		naredba(cv -> djeca[6]);
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
	}
	else if(dj[1] == "<izraz>") {
		izraz(cv -> djeca[1]);
		if(!tipovi_povratnih_vrijednosti.size() || !ide_implicitna_pretvorba(cv -> djeca[1] -> tip, tipovi_povratnih_vrijednosti.back())) kraj(cv);
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

//
// NAREDBENA STRUKTURA PROGRAMA
//
// end
//
}
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
		if(definirana_funkcija[ime]) kraj(cv);
		if(deklarirana_funkcija[ime]) {
			if(deklaracija_funkcije[ime].first.size() || deklaracija_funkcije[ime].second != cv -> djeca[0] -> tip) kraj(cv);
		}
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
		if(definirana_funkcija[ime]) kraj(cv);
		lista_parametara(cv -> djeca[3]);
		if(deklarirana_funkcija[ime]) {
			if(!vektori_jednaki(cv -> djeca[3] -> tipovi, deklaracija_funkcije[ime].first) || cv -> djeca[0] -> tip != deklaracija_funkcije[ime].second) kraj(cv);
		}
		definirana_funkcija[ime] = 1;
		deklarirana_funkcija[ime] = 1;
		deklaracija_funkcije[ime] = make_pair(cv -> djeca[3] -> tipovi, cv -> djeca[0] -> tip);
		tipovi_povratnih_vrijednosti.push_back(cv -> djeca[0] -> tip);
		REP(i, (int)(cv -> djeca[3] -> tipovi).size()) { // dodavanje parametara u mapu varijabli za sljedeci blok
			varijable[(cv -> djeca[3] -> imena)[i]].push_back(make_pair(br_bloka + 1, (cv -> djeca[3] -> tipovi)[i]));
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
		if()
	}
	else if((int)dj.size() == 3) {
		cv -> djeca[0] -> ntip = cv -> ntip;
		lista_init_deklaratora(cv -> djeca[0]);
		cv -> djeca[2] -> ntip = cv -> ntip;
		init_deklarator(cv -> djeca[2]);
	}
	else greska(cv);
}
//
// DEKLARACIJE I DEFINICIJE
//
// end
//
void rek(cvor *rod) {
	//
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
	unisti_stablo(root);
	return 0;
}
