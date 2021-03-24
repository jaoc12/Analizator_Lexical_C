#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

struct token{
    int tip;
    int referinta_valoare;
};

struct tabela_valori{
    int nr = 0;
    vector<int> referinta_valoare;
    vector<string> valoare;
    vector<bool> utilizat;
}tabelaValori;

class AnalizatorLexical{
    private:

        struct transition{
            int start;
            int end;
            string argument_tranzitie;
        };

        transition transitions[54] = {{0, 1, "litera"},
                                      {1, 1, "litera"},
                                      {1, 1, "cifra"},
                                      {0, 2, "cifra"},
                                      {2, 2, "cifra"},
                                      {0, 3, "."},
                                      {2, 3, "."},
                                      {3, 3, "cifra"},
                                      {0, 4, "\""},
                                      {4, 4, "comentariu"},
                                      {4, 5, "\\"},
                                      {4, 6, "\""},
                                      {5, 4, "orice"},
                                      {0, 7, ";"},
                                      {7, 7, ";"},
                                      {0, 8, " "},
                                      {8, 8, " "},
                                      {0, 8, "\n"},
                                      {8, 8, "\n"},
                                      {0, 9, "/"},
                                      {9, 10, "/"},
                                      {10, 10, "neterminal"},
                                      {10, 11, "\n"},
                                      {9, 12, "*"},
                                      {12, 12, "notstar"},
                                      {12, 13, "*"},
                                      {13, 12, "notslash"},
                                      {13, 14, "/"},
                                      {0, 15, "+"}, // +
                                      {15, 16, "+"}, // ++
                                      {15, 17, "="}, // +=
                                      {0, 18, "-"}, // -
                                      {18, 19, "-"}, // --
                                      {18, 19, "="}, // -=
                                      {0, 21, "nerepetabil"}, // *
                                      {21, 22, "="}, // *=
                                      {0, 23, "="}, // =
                                      {23, 24, "="}, // ==
                                      {0, 25, ">"}, // >
                                      {25, 26, "="}, // >=
                                      {25, 27, ">"}, // >>
                                      {27, 28, "="}, // >>=
                                      {0, 29, "<"}, // <
                                      {29, 30, "="}, // <=
                                      {29, 31, "<"}, // <<
                                      {31, 32, "="}, // <<=
                                      {0, 33, "&"}, // &
                                      {33, 34, "&"}, // &&
                                      {33, 35, "="}, // &=
                                      {0, 36, "|"}, // |
                                      {36, 37, "|"}, // ||
                                      {36, 38, "="}, // |=
                                      {0, 39, "~"}, // ~
                                      {9, 40, "="}}; // /=

        string lista_keywords[32] = { "auto", "double", "int", "struct", "break", "else",	"long",	"switch",
                                    "case", "enum", "register",	"typedef", "char", "extern", "return", "union",
                                    "const", "float", "short", "unsigned", "continue", "for", "signed", "void",
                                    "default", "goto", "sizeof", "volatile", "do", "if", "static", "while"};

        int pozitie_text = 0;
        ifstream fisier_intrare;

        bool checkArgument(char argument_test, string argument_tranzitie) {
            if(argument_tranzitie == "litera"){
                if(isalpha(argument_test) || argument_test == '_'){
                    return true;
                }
            }
            if(argument_tranzitie == "cifra"){
                if(isdigit(argument_test)){
                    return true;
                }
            }
            if(argument_tranzitie == "comentariu"){
                if(argument_test != '\"' && argument_test != '\\'){
                    return true;
                }
            }
            if(argument_tranzitie == "orice"){
                return true;
            }
            if(argument_tranzitie == "neterminal"){
                if(argument_test != '\n'){
                    return true;
                }
            }
            if(argument_tranzitie == "notstar"){
                if(argument_test != '*'){
                    return true;
                }
            }
            if(argument_tranzitie == "notslash"){
                if(argument_test != '/'){
                    return true;
                }
            }
            if(argument_tranzitie == "nerepetabil"){
                if(argument_test == '*' || argument_test == '%'
                    || argument_test == '!' || argument_test == '^'){
                    return true;
                }
            }
            if(argument_tranzitie.length() == 1){
                if(argument_tranzitie[0] == argument_test){
                    return true;
                }
            }
            return false;
        }

        int getTransition(int poz_start, char argument){
            for(int i = 0; i <= 54; i++){
                if(poz_start == transitions[i].start){
                    if(checkArgument(argument, transitions[i].argument_tranzitie)){
                        return transitions[i].end;
                    }
                }
            }
            return -1;
        }

    bool eKeyword(string cuvant) {
        for(int i=0; i<32;i++){
            if(cuvant == lista_keywords[i]){
                return true;
            }
        }
        return false;
    }

/*
         * 1 = identificator
         * 2 = keyword
         * 3 = constanta intreaga
         * 4 = constanta flotanta
         * 5 = constanta literara
         * 6 = separator
         * 7 = spatiu/sfarsit de linie
         * 8 = comentariu
         * 9 = operator
         */
        token procesareStareFinala(int pozitie, string cuvant){
            token tok;
            if(pozitie >= 15){
                tok = {9, -1};
            }
            else {
                switch (pozitie) {
                    case 1:
                        if (!eKeyword(cuvant)) {
                            tok = {1, -1};
                        } else {
                            tok = {2, -1};
                        }
                        break;
                    case 2:
                        tok = {3, -1};
                        break;
                    case 3:
                        tok = {4, -1};
                        break;
                    case 6:
                        tok = {5, -1};
                        break;
                    case 7:
                        tok = {6, -1};
                        break;
                    case 8:
                        tok = {7, -1};
                        break;
                    case 9:
                        tok = {9, -1};
                        break;
                    case 11:
                        tok = {8, -1};
                        break;
                    case 14:
                        tok = {8, -1};
                        break;
                    default:
                        tok = {-1, -1};
                        break;
                }
            }
            bool gasit = false;
            for(int i=0; i<tabelaValori.nr; i++){
                if(tabelaValori.valoare[i] == cuvant){
                    tok.referinta_valoare = tabelaValori.referinta_valoare[i];
                    gasit = true;
                }
            }
            if(!gasit){
                tabelaValori.referinta_valoare.push_back(tabelaValori.nr);
                tabelaValori.valoare.push_back(cuvant);
                tabelaValori.utilizat.push_back(false);
                tok.referinta_valoare = tabelaValori.nr;
                tabelaValori.nr++;
            }
            return tok;
        }

    public:
        AnalizatorLexical(string intrare){
            this->fisier_intrare.open(intrare, fstream::in);
            if(!this->fisier_intrare.is_open()){
                cout << "eroare la citire fisier intrare";
            }
        }

        token getToken(){
            int poz_curenta = 0;
            int poz_noua = 0;
            string cuvant;
            char c;
            this->fisier_intrare.seekg(this->pozitie_text);
            while (this->fisier_intrare.get(c)) {
                this->pozitie_text += 1;
                poz_noua = getTransition(poz_curenta, c);
                if (poz_noua == -1) {
                    token tok = procesareStareFinala(poz_curenta, cuvant);
                    if (tok.tip == -1) {
                        return tok;
                    } else {
                        poz_curenta = 0;
                        cuvant = "";
                        this->pozitie_text -= 1;
                        this->fisier_intrare.seekg(this->pozitie_text);
                        if (tok.tip != 7 && tok.tip != 8) {
                            return tok;
                        }
                    }
                } else {
                    cuvant += c;
                    poz_curenta = poz_noua;
                }
            }
            token tok = procesareStareFinala(poz_curenta, cuvant);
            if (tok.tip == -1) {
                return tok;
            } else {
                if (tok.tip != 7 && tok.tip != 8) {
                    return tok;
                } else {
                    tok = {0, 0};
                    return tok;
                }
            }
        }
};

void writeToken(token tok, ostream& fisier_iesire){
    string valoare;
    string tip;
    for(int i=0; i<tabelaValori.nr; i++){
        if(tabelaValori.referinta_valoare[i] == tok.referinta_valoare){
            if(tabelaValori.utilizat[i]){
                return;
            }
            else {
                valoare = tabelaValori.valoare[i];
                tabelaValori.utilizat[i] = true;
                break;
            }
        }
    }
    switch(tok.tip) {
        case 1:
            tip = "identificator";
            break;
        case 2:
            tip = "keyword";
            break;
        case 3:
            tip = "constanta intreaga";
            break;
        case 4:
            tip = "constanta flotanta";
            break;
        case 5:
            tip = "constanta literara";
            break;
        case 6:
            tip = "separator";
            break;
        case 9:
            tip = "operator";
            break;
        default:
            return;
    }
    fisier_iesire << tip << " - " << valoare << endl;
}

int main() {
    string intrare = R"(C:\Users\chitu\Desktop\Facultate\Anul 3\Sem2\Tehnici de compilare\Lab\Tema 1\intrare.txt)";
    string iesire = R"(C:\Users\chitu\Desktop\Facultate\Anul 3\Sem2\Tehnici de compilare\Lab\Tema 1\iesire.txt)";
    ofstream fisier_iesire;
    fisier_iesire.open(iesire, iostream::trunc);
    if(!fisier_iesire.is_open()){
        cout << "eroare la citire fisier iesire" << endl;
        cout << iesire << endl;
        return -1;
    }

    AnalizatorLexical dfa = AnalizatorLexical(intrare);
    token tok;
    do{
        tok = dfa.getToken();
        writeToken(tok, fisier_iesire);
    }while(tok.tip > 0);
    return 0;
}
