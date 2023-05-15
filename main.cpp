//  main.cpp
//  Proyecto Polinomios C++
//  Created by Josue Ponce on 14/5/23.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cctype>
using namespace std;

struct tPolinomio {
    int grado = 0;
    vector < double > coef;
};
//Funcion para separar el polinomio en terminos
vector<string> separar_terminos( string polinomio );

//Funcion para encontrar el coeficiente y el grado de un termino
pair<int, double> encontrar_grado_y_coeficiente( string termino );

//Funcion para validar
bool termino_valido(string &termino);

//Funcion para encontrar grado y coeficientes de los resultados
tPolinomio encontrar_resultado (vector<tPolinomio>&polinomios);

// Funcion para imprimir el resultado
void imprimir_polinomio( vector<tPolinomio>&polinomios );

//Aqui va el codigo principal
int main () {

    int cant_polinomios {0};
    string polinomio;
    tPolinomio resultado;
    string entrada;
    cout << "Ingrese la cantidad de polinomios (entero positivo): ";
    while (true) {
            cin >> entrada;
            bool es_entero = true;
        for (char c : entrada) {
                    if (!isdigit(c)) {
                        es_entero = false;
                        break;
                    }
        }
        if (!es_entero or entrada=="0") {
            cout << "Por favor, ingrese una cantidad de polinomios valida: " << std::endl;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else {
            cant_polinomios = stoi(entrada);
            break;
        }
    }
    cin.ignore();
    cout<<endl;
    vector < tPolinomio > polinomios ( cant_polinomios );
    for ( int i = 1; i <= cant_polinomios; i++ ) {
        cout << "Polinomio " << i << ": ";
        getline( cin, polinomio );
        if(polinomio.empty()){
            cout<<"Vuelva a ingresar"<<endl;
            i=i-1;
            continue;
        }
        vector<string> terminos = separar_terminos( polinomio );
        for (int j=0; j<terminos.size(); j++){
            while (!(termino_valido(terminos[j]))){
                cout<<"Polinomio invalido"<<endl;
                cout<<"Polinomio "<<i<<": ";
                getline(cin,polinomio);
                terminos=separar_terminos(polinomio);
            }
        }
        for ( int j = 0; j < terminos.size(); j++ ) {
            pair<int, double> grado_y_coef = encontrar_grado_y_coeficiente( terminos[ j ] );
            int grado = grado_y_coef.first;
            double coeficiente = grado_y_coef.second;
            if ( grado >= polinomios[ i - 1 ].grado ) {
                polinomios[ i - 1 ].grado = grado;
                polinomios[ i - 1 ].coef.resize( grado + 1 );
            }
            polinomios[ i - 1 ].coef[ grado ] += coeficiente;
        }
    }
    imprimir_polinomio(polinomios);
    return 0;
}

//Funcion para separar el polinomio en terminos
vector<string> separar_terminos(string polinomio) {
    vector<string> terminos;
    polinomio.erase(remove_if(polinomio.begin(), polinomio.end(), ::isspace), polinomio.end()); //ayuda de ChatGPT
    size_t pos = 0; //ayuda de ChatGPT
    while (pos < polinomio.length()) { //ayuda de ChatGPT
        size_t next_pos = polinomio.find_first_of("+-", pos + 1); //ayuda de ChatGPT
        if (next_pos == string::npos) { //ayuda de ChatGPT
            next_pos = polinomio.length(); //ayuda de ChatGPT
        }
        terminos.push_back(polinomio.substr(pos, next_pos - pos)); //ayuda de ChatGPT
        pos = next_pos; //ayuda de ChatGPT
    }
    return terminos;
}

//Funcion para encontrar el coeficiente y el grado de un termino
pair<int, double> encontrar_grado_y_coeficiente( string termino ){
    pair<int, double> res;
    size_t pos_x = termino.find_first_of( "xX" );
    size_t pos_asterisco = termino.find_first_of( "*" );
    if(pos_asterisco == string::npos and (pos_x==0 or pos_x==1) and pos_x !=termino.length()-1) {
        res.first=stoi(termino.substr(pos_x +1));
        if (termino[0] == '-')
            res.second= -1.0;
        else
            res.second=1;
        
    }
   else  if ( pos_x == string::npos and pos_asterisco==string::npos ) {
        res.first = 0;
        res.second = stod( termino );
    }
    else {
        if ( pos_asterisco == string::npos and (pos_x==termino.length()-1 or termino[termino.length()-1]=='1')) {
            res.first = 1;
            res.second = ( termino[ 0 ] == '-' ) ? -1.0 : 1.0;
        }
        else {
            if ( (pos_x==termino.length()-1 or termino[termino.length()-1]=='1') and pos_asterisco==pos_x-1) {
                res.first = 1;
                res.second = stod( termino.substr(0, pos_asterisco) );
            }
            else {
                res.first = stoi( termino.substr( pos_x + 1 ) );
                res.second = stod(termino.substr( 0, pos_asterisco) );
            }
        }
    }
    return res;
}

//Funcion para validar
bool termino_valido(string &termino) {
    size_t pos_x = termino.find_first_of("xX");
    size_t pos_asterisco = termino.find_first_of("*");
    for (int i=0; i<termino.length(); i++){
        if (!isdigit(termino[i]) and termino[i]!='.' and termino[i]!='x' and termino[i]!='X' and termino[i]!='*' and termino[i]!='+' and termino [i] !='-')
            return false;
    }
    if (pos_x==string::npos and pos_asterisco!=string::npos){
        return false;
    }
    if (pos_x == string::npos) {
        try {
            stod(termino);
            return true;
        }
        catch (...) {
            return false;
        }
    }
    else {
        if (pos_asterisco == string::npos and (pos_x==termino.length()-1 or termino[termino.length()-1]=='1') and  (termino[pos_x-1]=='+' or termino[pos_x-1]=='-' or pos_x==0)) {
            return true;
        }
        else if (pos_asterisco == string::npos and (pos_x==0 or pos_x==1) and pos_x !=termino.length()-1) {
            return true;
        }
    
        else if (pos_x==termino.length()-1 or termino[termino.length()-1]=='1') {
            if (pos_asterisco==string::npos)
                return false;
            else{
                try{
                    stod(termino.substr(0, pos_asterisco));
                    return true;
                }
                catch(...){
                    return false;
                }
            }
        }
        else if (pos_asterisco==string::npos and pos_x!=termino.length()-1)
            return false;
        try {
            stod(termino.substr(0, pos_asterisco));
        }
        catch (...) {
            return false;
        }
        try {
            stoi(termino.substr(pos_x + 1));
        }
        catch (...) {
            return false;
        }
        return true;
    }
}

//Funcion para encontrar grado y coeficientes de los resultados
tPolinomio encontrar_resultado (vector<tPolinomio>&polinomios){
    tPolinomio resultado;
    for (int i=0; i<polinomios.size(); i++){
        if (resultado.grado<polinomios[i].grado)
            resultado.grado=polinomios[i].grado;
    }
    for (int i = 0; i <= resultado.grado; i++) { //ayuda de ChatGPT //suma de coeficientes
            double suma = 0; //ayuda de ChatGPT
            for (int j = 0; j<polinomios.size(); j++) { //ayuda de ChatGPT
                if (i <= polinomios[j].grado) { //ayuda de ChatGPT
                    suma= suma+polinomios[j].coef[i]; //ayuda de ChatGPT
                }
            }
            resultado.coef.push_back(suma); //ayuda de ChatGPT
        }
    for (int i=resultado.grado; i>=0; i--){
        if (i==resultado.grado and resultado.coef[i]==0 and resultado.grado>0)
            resultado.grado-=1;
    }
    return resultado;
}

// Funcion para imprimir el resultado
void imprimir_polinomio( vector<tPolinomio>&polinomios) {
    tPolinomio p=encontrar_resultado(polinomios);
    cout<<endl;
    cout<<"El polinomio suma es de grado "<<p.grado<<":"<<endl;
    cout<<fixed<<setprecision(1);
    bool nada;
    for (int i=0; i<=p.grado; i++){
        if (p.coef[i]==0)
            nada=true;
        else{
            nada=false;
            break;
        }
    }
    if(!nada){
        for (int i = 0; i <= p.grado; i++) {
            if (p.coef[i]==0)
                continue;
            else if (i==0 and p.coef[i]<0)
                cout<<"- "<<-(p.coef[i]);
            else if (i==0 and p.coef[i]>0)
                cout<<p.coef[i];
            else if (p.coef[i]>0 and i==1)
                cout<<" + "<<p.coef[i]<<"*x";
            else if (p.coef[i]<0 and i==1)
                cout<<" - "<<-(p.coef[i])<<"*x";
            else if (p.coef[i]>0 and i!=0 and i!=1)
                cout<<" + "<<p.coef[i]<<"*x"<<i;
            else if (p.coef[i]<0 and i!=0 and i!=1)
                cout<<" - "<<-(p.coef[i])<<"*x"<<i;
        }
        cout << endl;
    }
    else{
        cout<<p.coef[0]<<endl;
    }
}
