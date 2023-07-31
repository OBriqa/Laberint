#include "particio.hpp"

using util::hash;

// Cost = 1 = O(1), 
// Constructura d'un node que conté la informació d'un element de la partició
template <typename T>
particio<T>::node_hash::node_hash(const T &k, nat n, node_hash* seg, node_hash* presi) 
    throw(error) : _k(k), _n(n), _seg(seg), _presi(presi) {}


// Cost = 1.25*n = O(n), n = nombre màxim d'elements de la partició
// Construeix una particio amb n elements com a màxim.
template <typename T>
particio<T>::particio(nat n) throw(error){
    
    _maxim = n;
    _grups = 0;
    _elements = 0;

    _mida = n*1.25 + 1; // evitem que la mida sigui 0 i es produeixi una excepció.

    _taula = new node_hash*[_mida];
    for(nat i = 0; i < _mida; i++) _taula[i] = NULL;

}


// Cost = n + n*log(n) = n*log(n) = O(n*log(n))
// n = nombre màxim d'elements de la partició

// Constructora per còpia, assignació i destructora.
template <typename T>
particio<T>::particio(const particio & p) throw(error){
        
    _mida = p._mida;
    _grups = p._grups;
    _maxim = p._maxim;
    _elements = p._elements;

    _taula = new node_hash*[_mida];
    for(nat z = 0; z < _mida; z++) _taula[z] = NULL; 
    // Inicialitzem el vector de punters per evitar accesos a posicions sense inicialitzar.

    copia_nodes(p._taula,p._mida);
}


// Cost = n + n + n*log(n) = n*log(n) = O(n*log(n))
// n = nombre màxim d'elements de la partició
template <typename T>
particio<T> & particio<T>::operator=(const particio & p) throw(error){
    
    if(this != &p){

        destruir_taula(_taula,_mida);
        _taula = new node_hash*[p._mida];
        for(nat z = 0; z < p._mida; z++) _taula[z] = NULL; 
        copia_nodes(p._taula,p._mida);

        _mida = p._mida;
        _grups = p._grups;
        _maxim = p._maxim;
        _elements = p._elements;      
    }

    return (*this);
}

// Cost = n + n*log(n) = O(n*log(n)), n = nombre d'elements  
// log(n) és el cost de buscar el representant de cada element de la partició.
template <typename T>
void particio<T>::copia_nodes(node_hash** taula, nat mida) throw(){

    // Copiem tots els nodes a la taula del p.i
    for(nat i = 0; i < mida; i++){

        node_hash *q = taula[i];
        node_hash *n = NULL;
        node_hash *ant = NULL;

        if(q != NULL){

            while(q != NULL){ 
                n = new node_hash(q->_k,q->_n,NULL);
                if(ant != NULL) ant->_seg = n;
                else _taula[i] = n;

                ant = n;
                q = q->_seg;
            }

        }

    }

    // Cerquem el representant (_presi) de cada element de la taula i l'enllaçem
    for(nat i = 0; i < mida; i++){

        node_hash *q = taula[i];
        node_hash *n = _taula[i];

        while(q != NULL and n != NULL){

            node_hash *t = q;
            while(t != NULL and t->_presi != t) t = t->_presi;

            long j = -1;
            if(t != NULL) j = hash<T>(t->_k) % mida;

            node_hash *s = _taula[j];
            while(s != NULL and s->_k != t->_k) s = s->_seg;

            if(s != NULL) n->_presi = s;

            q = q->_seg;
            n = n->_seg;

        }

    }

}


// Cost = n = O(n), n = nombre màxim d'elements de la partició
template <typename T>
void particio<T>::destruir_taula(node_hash** taula, nat mida) throw(){
   
    for(nat i = 0; i < mida; i++){
        node_hash* p = taula[i];
        while(p != NULL){
            node_hash* q = p;
            p = p->_seg;
            delete q;
        }
    }
    delete[] _taula;
    
}

// Cost = 1.25*n = O(n), n = nombre màxim d'elements de la partició
template <typename T>
particio<T>::~particio() throw(){
    destruir_taula(_taula,_mida);
}


// Cost = constant (cas mig) = O(1)

// PRE: L'element k no hi és a la partició
// POST: S'ha afegit l'element k a la partició, el nombre d'elements i el nombre de grups s'incrementen en 1
template <typename T>
void particio<T>::insereix(const T& k){

    long i = hash<T>(k) % _mida;
    ++_elements;
    ++_grups;

    node_hash* p = _taula[i];
    if(p == NULL){
        _taula[i] = new node_hash(k,1,NULL);
        _taula[i]->_presi = _taula[i];
    }
    else{
        node_hash* q = NULL;
        while(p != NULL and p->_k < k){
            q = p;
            p = p->_seg;
        }
        if(p == NULL){
            q->_seg = new node_hash(k,1,NULL);
            q->_seg->_presi = q->_seg;
        }
        else{
            if(q != NULL){
                q->_seg = new node_hash(k,1,p);
                q->_seg->_presi = q->_seg;
            }
            else{
                _taula[i] = new node_hash(k,1,p);
                _taula[i]->_presi = _taula[i];
            }
        }     
    }
}


// Cost = constant (cas mig) = O(1)

// PRE:  Cert
// POST: Retorna cert si l'element k és a la partició
template <typename T>
bool particio<T>::buscar(const T& k) const{

    long i = hash<T>(k) % _mida;
    node_hash* p = _taula[i];

    bool trobat = false;
    while(p != NULL and p->_k <= k and not trobat){
        trobat = (p->_k == k);
        p = p->_seg;
    }

    return trobat;

}

// Cost = max{buscar,insereix} = constant (cas mig) = O(1)
// n = nombre màxim d'elements de la partició

// Afegeix un nou element a la particio. Aquest nou element formarà ell sol
// un nou grup. En cas que l'element ja estigués present a la partició llavors
// no es modifica la partició. Es produeix un error si la particio ja té el
// número màxim d'elements abans d'afegir aquest nou.
template <typename T>
void particio<T>::afegir(const T &x) throw(error){
    
    if(not buscar(x)){

        if(_elements == _maxim) throw error(ParticioPlena);
        else insereix(x);

    }

}


// Cost = O(1)*log(n) = O(log(n))
// n = nombre màxim d'elements de la partició
// log(n) és el cost de buscar el representant de cada element de la partició.

// Uneix els dos grups als quals pertanyen aquests dos elements. Si tots dos
// elements ja pertanyien al mateix grup no fa res.
// Es produeix un error si algun dels elements no pertany a la partició.
template <typename T>
void particio<T>::unir(const T & x, const T & y) throw(error){
    
    if(not mateix_grup(x,y)){

        _grups--;

        long i = hash<T>(x) % _mida;
        node_hash* p = _taula[i];
        while(p != NULL and p->_k != x) p = p->_seg;

        while(p != NULL and p->_presi != p) p = p->_presi;

        long j = hash<T>(y) % _mida;
        node_hash* q = _taula[j];
        while(q != NULL and q->_k != y) q = q->_seg;

        while(q != NULL and q->_presi != q) q = q->_presi;

        if(p != NULL and q != NULL){

            // Unió per pes.
            if(p->_n >= q->_n){
                p->_n++;
                q->_n--;
                q->_presi = p;
            }
            else{
                q->_n++;
                p->_n--;
                p->_presi = q;
            }

        }

    }
 
}


// Cost = O(1) + (1) + log(n) = O(log(n))
// n = nombre màxim d'elements de la partició
// log(n) és el cost de buscar el representant de cada element de la partició.

// Retorna si els elements x i y pertanyen al mateix grup.
// Es produeix un error si algun dels dos elements no pertany a la partició.
template <typename T>
bool particio<T>::mateix_grup(const T & x, const T & y) const throw(error){
    
    if(buscar(x) and buscar(y)){

        long i = hash<T>(x) % _mida;
        node_hash* p = _taula[i];
        while(p != NULL and p->_k != x) p = p->_seg; // busquem l'element a la taula de dispersió

        while(p != NULL and p->_presi != p) p = p->_presi; // busquem el seu representant

        long j = hash<T>(y) % _mida;
        node_hash* q = _taula[j];
        while(q != NULL and q->_k != y) q = q->_seg;

        while(q != NULL and q->_presi != q) q = q->_presi;
    
        return (p == q);

    }
    else throw error(ElemInexistent);
    
}

// Cost = constant = O(1)
// Retorna el número de grups que té la particio.
template <typename T>
nat particio<T>::size() const throw(){
    return _grups;
}

// Cost = constant = O(1)
// Retorna el número d'elements que té la particio.
template <typename T>
nat particio<T>::num_elements() const throw(){
    return _elements;
}

// Cost = constant = O(1)
// Retorna el número màxim d'elements que pot tenir la particio.
template <typename T>
nat particio<T>::num_maxim() const throw(){
    return _maxim;
}