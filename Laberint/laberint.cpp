#include "laberint.hpp"

static nat fil = 5;
static nat col = 5;

// Constructora d'un laberint buit sense excavar (sense cap porta oberta)
// de la mida indicada. Totes les cambres del laberint no tenen cap porta.
// Es produeix un error si el número de files o de columnes del laberint és 0.
laberint::laberint(nat num_files, nat num_columnes) throw(error){ // O(n,m) = n*m

    fil = num_files;
    col = num_columnes;

    if(num_files*num_columnes > 0){

        L = new cambra[num_files*num_columnes];
        for(nat i = 0; i < (num_files*num_columnes); i++) L[i] = cambra();

    }
    else throw error(FilsColsIncorrecte);

}

// Constructora d'un laberint a partir d'un istream (canal d'entrada) is.
// El format de l'istream seguirà l'exposat a l'apartat 2.3. Es presuposa que
// el laberint és correcte.
laberint::laberint(std::istream & is) throw(error){

    nat fila = 0, columna = 0;
    is >> fila; is >> columna;

    L = new cambra[fila*columna];

}

// Constructora per còpia, assignació i destructora.
laberint::laberint(const laberint & l) throw(error){

    nat mida = l.num_files()*l.num_columnes();

    L = new cambra[mida];

    for(nat i = 0; i < mida; i++) L[i] = l.L[i];
}

laberint & laberint::operator=(const laberint & l) throw(error){

    if(this != &l){
        /* laberint T(l);
        cambra* aux = L;
        L = T.L;
        T.L = aux;  */
    }

    return *this;
}

laberint::~laberint() throw() {
    delete[] L;
}

// Retornen el número de files i columnes que té el laberint, respectivament.
nat laberint::num_files() const throw(){
    return fil;
}

nat laberint::num_columnes() const throw(){
    return col;
}

// Retorna la cambra situada a la posició especificada per pos.
// Aquest mètode permet utilitzar el laberint d'aquesta manera:
//   laberint l(3,4);
//   posicio pos(2,2);
//   cambra c = l(pos);
// Es produeix un error si la posició donada no existeix al laberint.
cambra laberint::operator()(const posicio & pos) const throw(error){

    cambra C;
    
    if(pos.first > 0 and pos.first <= fil and pos.second > 0 and pos.second <= col)
        C = L[(pos.first - 1)*col + (pos.second - 1)];

    else throw error(PosicioInexistent);

    return C;
}

// Obre la porta en la paret donada de la cambra que està especificada per pos.
// També obre la porta corresponent en la cambra adjacent. Es produeix un error
// si la posició no existeix o no es pot obrir una porta en la direcció
// indicada perquè dóna a l'exterior.
void laberint::obre_porta(paret p, const posicio & pos) throw(error){

    if(pos.first > 0 and pos.first <= fil and pos.second > 0 and pos.second <= col){

        nat fila = pos.first - 1;
        nat columna = pos.second - 1;

        if(p == paret::NORD){
            if((pos.first - 1) > 0){
                L[fila*col + columna].obre_porta(paret::NORD);
                L[(fila - 1)*col + columna].obre_porta(paret::SUD);
            }
            else throw error(PortaExterior);
        }
        else if(p == paret::SUD){
            if((pos.first + 1) <= fil){
                L[fila*col + columna].obre_porta(paret::SUD);
                L[(fila + 1)*col + columna].obre_porta(paret::NORD);
            }
            else throw error(PortaExterior);
        }
        else if(p == paret::EST){
            if((pos.second + 1) <= col){
                L[fila*col + columna].obre_porta(paret::EST);
                L[fila*col + (columna + 1)].obre_porta(paret::OEST);
            }
            else throw error(PortaExterior);
        }
        else if(p == paret::OEST){
            if((pos.second - 1) > 0){
                L[fila*col + columna].obre_porta(paret::OEST);
                L[fila*col + (columna - 1)].obre_porta(paret::EST);
            }
            else throw error(PortaExterior);
        }
        else if(p == paret::NO_DIR){}

    }
    else throw error(PosicioInexistent);

}

// Tanca la porta en la paret donada de la cambra que està especificada per pos.
// També tanca la porta corresponent en la cambra adjacent. Es produeix un error
// si la posició no existeix.
void laberint::tanca_porta(paret p, const posicio & pos) throw(error){

    if(pos.first > 0 and pos.first <= fil and pos.second > 0 and pos.second <= col){

        nat fila = pos.first - 1;
        nat columna = pos.second - 1;

        if(p == paret::NORD and ((pos.first - 1) > 0)){
            L[fila*col + columna].tanca_porta(paret::NORD);
            L[(fila - 1)*col + columna].tanca_porta(paret::SUD);
        }
        else if(p == paret::SUD and ((pos.first + 1) <= fil)){
            L[fila*col + columna].tanca_porta(paret::SUD);
            L[(fila + 1)*col + columna].tanca_porta(paret::NORD);
        }
        else if(p == paret::EST and ((pos.second + 1) <= col)){
            L[fila*col + columna].tanca_porta(paret::EST);
            L[fila*col + (columna + 1)].tanca_porta(paret::OEST);
        }
        else if(p == paret::OEST and ((pos.second - 1) > 0)){
            L[fila*col + columna].tanca_porta(paret::OEST);
            L[fila*col + (columna - 1)].tanca_porta(paret::EST);
        }
        else if(p == paret::NO_DIR){}

    }
    
    else throw error(PosicioInexistent);
}

// Escriu el laberint a l'ostream (canal de sortida) os. El format per escriure
// el laberint seguirà l'exposat a l'apartat 2.3.
void laberint::print(std::ostream & os) const throw(){

    string s1 = "", s2 = "";

    os << fil << " " << col << "\n";
    for(nat i = 0; i < (2*col + 1); i++) (i == 2*col) ? os << "*\n": os << "* ";

    for(nat j = 0; j < fil; j++){

        for(nat i = 0; i < col; i++){

            s1 = L[j*col + i].porta_oberta(paret::OEST) ? s1 + "    "  : s1 + "*   ";
            s2 = L[j*col + i].porta_oberta(paret::SUD)  ? s2 + "*   " : s2 + "* * ";

        }
        os << s1 + "*\n";
        os << s2 + "*\n";
        s1.clear(); s2.clear(); // O(n) = 1

    }


}
