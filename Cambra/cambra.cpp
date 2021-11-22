#include "cambra.hpp"

cambra::cambra(bool n, bool s, bool e, bool o) throw(error){
    C[0] = n; C[1] = s; C[2] = e; C[3] = o;
}

// Constructor per còpia, assignació i destructora.
cambra::cambra(const cambra & c) throw(error){
    for(int i = 0; i < 4; i++) C[i] = c.C[i];
}

cambra & cambra::operator=(const cambra & c) throw(error){
    
    if(this != &c) for(int i = 0; i < 4; i++) C[i] = c.C[i];

    return *this;
}

cambra::~cambra() throw(){};

// Retorna si l'habitació té una porta oberta a la paret indicada.
bool cambra::porta_oberta(paret p) const throw(){
    return C[p];
}

// Obre una nova porta a la paret indicada. Si la porta ja està oberta no fa res.
// Es produeix un error si la paret es NO_DIR.
void cambra::obre_porta(paret p) throw(error){
    if(p == paret::NO_DIR) throw error(ParetInexistent);
    else if(not C[p]) C[p] = true;
}

// Tanca la porta a la paret indicada. Si la porta ja estava tancada no fa res.
// Es produeix un error si la paret és NO_DIR.
void cambra::tanca_porta(paret p) throw(error){
    if(p == paret::NO_DIR) throw error(ParetInexistent);
    else if(C[p]) C[p] = false;
}

// Igualtat i desigualtat entre cambres. 
// Dues cambres es consideren iguals si tenen les mateixes portes obertes a les mateixes parets.
bool cambra::operator==(const cambra & c) const throw(){

    return (C[0] == c.C[0] and 
            C[1] == c.C[1] and 
            C[2] == c.C[2] and 
            C[3] == c.C[3]);

}

bool cambra::operator!=(const cambra & c) const throw(){
    return not (*this == c);
}


// Operador "menor que" entre cambres. Una cambra és més petita que una
// altra si té més portes tancades que l'altra. Tenint en compte que una
// porta tancada és més petita que una porta oberta, en cas que tinguin el
// mateix número de portes tancades, per decidir qui és més petit es miraran
// les portes en aquest ordre NORD, EST, SUD i OEST. Per exemple:
//   cambra c1, c2(true,true), c3(true, false, true);
//   cout << (c1 < c2); // escriu 'true'
//   cout << (c2 < c3); // escriu 'true'
bool cambra::operator<(const cambra & c) const throw(){

    bool petit = false;
    int c1 = 0, c2 = 0; 

    for(int i = 0; i < 4; i++){
        if(C[i]) c1++;
        if(c.C[i]) c2++;
    }

    if(c1 == c2){
        bool trobat = false;
        int i = 0;
        while(i < 4 and not trobat){
            if(not C[i] and c.C[i]) trobat = petit = true;
            else if(C[i] and not c.C[i]){trobat = true; petit = false;}
            else i++;
        }
    }
    else petit = (c1 < c2);

    return petit;
}
