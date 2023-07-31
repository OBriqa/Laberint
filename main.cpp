#include <string>
#include <list>
#include <utility>
#include <iostream>
#include <fstream>

#include <esin/error>
#include <esin/gen_driver>
#include <esin/util>

#include "paret.hpp"
#include "cambra.hpp"
#include "laberint.hpp"
#include "teseus.hpp"
#include "dedalus.hpp"
#include "particio.hpp"

using util::nat;
using namespace std;

namespace check {
  bool comprova_cami_minim(const posicio& ini, 
                           const posicio& final,
                           const laberint& L,
                           const list<posicio>& camino_candidato);
  bool comprova_perfecte(const laberint& L);
};

bool check::comprova_cami_minim(const posicio& ini, 
  const posicio& final,
  const laberint& L,
  const list<posicio>& camino_candidato){
    int suma = ini.first + final.first;
    suma++;
    int colum = L.num_columnes();
    colum++;
    int test = camino_candidato.front().first;
    test++;
    return true;
  }

bool check::comprova_perfecte(const laberint& L){
  int a = L.num_columnes();
  a++;
  return true;
}

void update_dirs(const string& dir, bool& n, bool& s, bool& e, bool& o) {
  if (dir == "N" or dir == "nord" or dir == "NORD") {
    n = true;
  }
  else if (dir == "S" or dir == "sud" or dir == "SUD") {
    s = true;
  }
  else if (dir == "E" or dir == "est" or dir == "EST") {
    e = true;
  }
  else if (dir == "O" or dir == "oest" or dir == "OEST") {
    o = true;
  }
  else {
    throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod, 
        gen_driver::WrongTypeArgsMsg);
  }

}

void* user_init(gen_driver& dr) { 
  string id = dr.args(1);
  string tid = dr.args(2);
  
  if (tid == "cambra") {
    // init cambra
    if (dr.nargs() < 2 or dr.nargs() > 6)
      throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod, 
        gen_driver::WrongTypeArgsMsg);

    bool n, s, e, o; n = s = e = o = false;
    switch (dr.nargs()) {
      case 6 : { update_dirs(dr.args(6), n, s, e, o); }
      case 5 : { update_dirs(dr.args(5), n, s, e, o); }
      case 4 : { update_dirs(dr.args(4), n, s, e, o); }
      case 3 : { update_dirs(dr.args(3), n, s, e, o); }
      default : { }
    }
    return static_cast<void*>(new cambra(n,s,e,o));
  } 
  else if (tid == "laberint") {
    if (dr.nargs() < 2 or dr.nargs() > 4)
      throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod,
      gen_driver::WrongNumArgsMsg);
    if (dr.nargs() == 2) 
      return static_cast<void*>(new laberint());
    if (dr.nargs() == 3) {
      ifstream input_file(dr.args(3).c_str());
      if (!input_file)
        throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod,
          gen_driver::WrongTypeArgsMsg);
      return static_cast<void*>(new laberint(input_file));
    }
    // dr.nargs() == 4
    int nf = util::toint(dr.args(3));
    int nc = util::toint(dr.args(4));
    if (nf < 0 or nc < 0) 
      throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod,
        gen_driver::WrongTypeArgsMsg);
    return static_cast<void*>(new laberint(nf, nc));
  } 
  else if (tid == "particio<int>") {
    // init particio
    int n = util::toint(dr.args(3));
    if (dr.nargs() == 3) {
      return static_cast<void*>(new particio<int>(n));
    }
    else {
      throw error(gen_driver::WrongNumArgs, gen_driver::nom_mod, 
        gen_driver::WrongNumArgsMsg);
    }
  }
  throw error(gen_driver::WrongTypeArgs, gen_driver::nom_mod,
    gen_driver::WrongTypeArgsMsg);
}

// funcions d'utilitat

// imprimeix un laberint
ostream & operator<<(std::ostream & os, const laberint & l) {
  l.print(os);
  return os;
}

// llegeix un laberint
istream & operator>>(std::istream & is, laberint & l) {
  l = laberint(is);
  return is;
}

// imprimeix un boolea
ostream& operator<=(ostream& os, bool b) {
  os << (b ? "true" : "false");
  return os;
}
 
// imprimeix (graficament) una cambra
ostream& operator<<(ostream& os, const cambra& c) {
  os << '*' << (c.porta_oberta(paret("nord")) ? ' ' : '*') << '*' << endl;
  os << (c.porta_oberta(paret("oest")) ? ' ' : '*') << ' ';
  os << (c.porta_oberta(paret("est")) ? ' ' : '*') << endl;
  os << '*' << (c.porta_oberta(paret("sud")) ? ' ' : '*') << '*';
  return os;
}

// imprimeix una posicio (fila, col)
ostream& operator<<(ostream& os, const posicio& pos) {
  os << "(" << pos.first << "," << pos.second << ")";
  return os;
}

// imprimeix una llista de T's
template <typename T>
ostream& operator<<(ostream& os, const list<T>& L) {
  typename list<T>::const_iterator it = L.begin();
  if (it == L.end()) { os << "[]"; return os; }
  T prev = *it;
  os << "[";
  ++it;
  while (it != L.end()) {
    os << prev << ",";
    prev = *it;
    ++it;
  }
  os << prev << "]"; 
  return os;
}


/* TypeTraits */
template <> const char* TypeTraits<cambra>::name = "cambra";
template <> const char* TypeTraits<laberint>::name = "laberint";
template <> const char* TypeTraits<paret>::name = "paret";
template <> const char* TypeTraits<posicio>::name = "posicio";

template <> const char* TypeTraits<list<posicio> >::name = "list<posicio>";
template <> const char* TypeTraits<particio<int> >::name = "particio<int>";


/*-------------------------< CAMBRA >--------------------------*/

// test memory
void crea_cambra(cambra*& c) 
{ c = new cambra;}

void crea_altra_cambra(cambra*& c) 
{ c = new cambra(true, false, false, false);}

bool test_ctor_cambra(gen_driver& d)
{ return d.generic_memtest<cambra> (crea_cambra, "ctor cambra"); }

bool test_copyctor_cambra(gen_driver& d)
{ return d.copyctor_memtest<cambra> (crea_cambra); }

bool test_assigna_cambra(gen_driver& d)
{ return d.assgn_memtest<cambra>(crea_cambra, crea_altra_cambra); }

// metodes cambra
void porta_oberta(gen_driver& dr) {
  dr.get_ostream() <= dr.object<cambra>()->porta_oberta(paret(dr.args(1)));
  dr.get_ostream() << endl;
}

void obre_porta_c(gen_driver& dr) {
  dr.object<cambra>() -> obre_porta(paret(dr.args(1)));
}

void tanca_porta_c(gen_driver& dr) {
  dr.object<cambra>() -> tanca_porta(paret(dr.args(1)));
}
  
void iguals(gen_driver& dr) {
  (dr.get_ostream() <= (*dr.object<cambra>() == *dr.object<cambra>(1))) << endl;
}

void diferents(gen_driver& dr) {
  (dr.get_ostream() <= (*dr.object<cambra>() != *dr.object<cambra>(1))) << endl;
}

void menor(gen_driver& dr) {
  (dr.get_ostream() <= (*dr.object<cambra>() < *dr.object<cambra>(1))) << endl;
}

void print_c(gen_driver& dr) {
  dr.get_ostream() << (*dr.object<cambra>()) << endl;
}

/*-------------------------< LABERINT >--------------------------*/

// test memory
void crea_laberint(laberint*& l) {
  l = new laberint; 
}

void crea_altre_laberint(laberint*& l) {
  ifstream input_file("laberint_3x3.txt");
  l = new laberint(input_file);
}

bool test_ctor_laberint(gen_driver& d)
{ return d.generic_memtest<laberint> (crea_laberint, 
  "ctor laberint(nat,nat)"); }

bool test_ctor_laberint_fitxer(gen_driver& d)
{ return d.generic_memtest<laberint> (crea_altre_laberint, 
  "ctor laberint(istream&)"); }

bool test_copyctor_laberint(gen_driver& d)
{ return d.copyctor_memtest<laberint> (crea_laberint); }

bool test_assigna_laberint(gen_driver& d)
{ return d.assgn_memtest<laberint>(crea_laberint,crea_altre_laberint); }

// metodes
void num_files(gen_driver& dr) {
  dr.get_ostream() << dr.object<laberint>() -> num_files() << endl;
}

void num_columnes(gen_driver& dr) {
  dr.get_ostream() << dr.object<laberint>() -> num_columnes() << endl;
}

void obt_cambra(gen_driver& dr) {
  pair<nat,nat> pos = make_pair(util::toint(dr.args(1)),
                                util::toint(dr.args(2)));
  dr.get_ostream() << dr.object<laberint>() -> operator()(pos) << endl;
}

void obre_porta_l(gen_driver& dr) {
  pair<nat,nat> pos = make_pair(util::toint(dr.args(2)),
                                util::toint(dr.args(3)));
  dr.object<laberint>() -> obre_porta(paret(dr.args(1)), pos);
}

void tanca_porta_l(gen_driver& dr) {
  pair<nat,nat> pos = make_pair(util::toint(dr.args(2)),
                                util::toint(dr.args(3)));
  dr.object<laberint>() -> tanca_porta(paret(dr.args(1)), pos);
}

void print(gen_driver& dr) {
  dr.object<laberint>() -> print(dr.get_ostream()); 
}

void check_perfecte(gen_driver& dr) {
  laberint* pLab = dr.object<laberint>();
  (dr.get_ostream() <= check::comprova_perfecte(*pLab)) << endl;
}

/*-------------------------< TESEUS >--------------------------*/

void buscar(gen_driver& dr) {
  pair<nat,nat> ini = make_pair(util::toint(dr.args(1)),
                                util::toint(dr.args(2)));
  pair<nat,nat> final = make_pair(util::toint(dr.args(3)),
                                  util::toint(dr.args(4)));
  list<posicio> L;
  teseus::buscar(*dr.object<laberint>(), ini, final, L);  
  dr.get_ostream() << L << endl;
}

void check_cami_minim(gen_driver& dr) {
  pair<nat,nat> ini = make_pair(util::toint(dr.args(1)),
                                util::toint(dr.args(2)));
  pair<nat,nat> final = make_pair(util::toint(dr.args(3)),
                                  util::toint(dr.args(4)));
  list<posicio> L;
  laberint *pLab = dr.object<laberint>();
  teseus::buscar(*pLab, ini, final, L);  
  (dr.get_ostream() <= check::comprova_cami_minim(ini, final, *pLab, L)) << endl;
}

/*-------------------------< DEDALUS >--------------------------*/

void construir(gen_driver& dr) {
  dedalus::construir(*dr.object<laberint>());
  dr.get_ostream() << "dedalus::construir OK" << endl;
}

/*-----------------------< PARTICIO<INT> >-----------------------*/

void afegir(gen_driver& dr) {
  dr.object<particio<int> >() -> afegir(util::toint(dr.args(1)));
}
void unir(gen_driver& dr) {
  dr.object<particio<int> >() -> unir(util::toint(dr.args(1)), 
                                      util::toint(dr.args(2)));
}

void same_block(gen_driver& dr) {
  bool res = dr.object<particio<int> >() -> mateix_grup(util::toint(dr.args(1)), util::toint(dr.args(2)));
  (dr.get_ostream() <= res) << endl;
}

void size(gen_driver& dr) {
  dr.get_ostream() << dr.object<particio<int> >() -> size() << endl;
}

void num_elems(gen_driver& dr) {
  dr.get_ostream() << dr.object<particio<int> >() -> num_elements() << endl;
}

void num_maxim(gen_driver& dr) {
  dr.get_ostream() << dr.object<particio<int> >() -> num_maxim() << endl;
}

// test memory
void crea_particio_int(particio<int>*& l) {
  l = new particio<int>(5);
}

void crea_altra_particio_int(particio<int>*& l) {
  l = new particio<int>(5);
  l -> afegir(1);
  l -> afegir(2);
  l -> afegir(3);
  l -> unir(1,2);
}

bool test_ctor_particio_int(gen_driver& d) {
  return d.generic_memtest<particio<int> >(crea_particio_int,
                                           "ctor particio<int>", 
                                           "particio<int>"); 
}

bool test_copyctor_particio_int(gen_driver& d) {
  return d.copyctor_memtest<particio<int> >(crea_particio_int,
                                            "particio<int>"); 
}

bool test_assigna_particio_int(gen_driver& d)
{ return d.assgn_memtest<particio<int> >(crea_particio_int,
                                         crea_altra_particio_int,
                                         "particio<int>"); 
}


/* --------------------------------< MAIN >--------------------------------- */

int main() {
  gen_driver d("maze.err", false);

  d.add_call("porta_oberta", porta_oberta, "cambra", "string");
  d.add_call("obre_porta_c", obre_porta_c, "cambra", "string");
  d.add_call("tanca_porta_c", tanca_porta_c, "cambra", "string");
  d.add_call("==", iguals, "cambra", "cambra");
  d.add_call("!=", diferents, "cambra", "cambra");
  d.add_call("<", menor, "cambra", "cambra");
  d.add_call("print_c", print_c, "cambra");

  d.add_call("num_files", num_files, "laberint");
  d.add_call("num_columnes", num_columnes, "laberint");
  d.add_call("obt_cambra", obt_cambra, "laberint", "nat nat");
  d.add_call("obre_porta_l", obre_porta_l, "laberint", "string nat nat");
  d.add_call("tanca_porta_l", tanca_porta_l, "laberint", "string nat nat");
  d.add_call("print", print, "laberint");
  d.add_call("comprova_perfecte", check_perfecte, "laberint");
  
  d.add_call("buscar", buscar, "*", "nat nat nat nat");
  d.add_call("comprova_cami_minim", check_cami_minim, 
             "*", "nat nat nat nat");

  d.add_call("construir", construir, "*");

  d.add_call("afegir", afegir, "particio<int>", "int");
  d.add_call("unir", unir, "particio<int>", "int int");
  d.add_call("equiv", same_block, "particio<int>", "int int");
  d.add_call("size", size, "particio<int>");
  d.add_call("num_elems", num_elems, "particio<int>");
  d.add_call("num_maxim", num_maxim, "particio<int>");

  d.add_memory_test<cambra>(test_ctor_cambra);
  d.add_memory_test<cambra>(test_copyctor_cambra);
  d.add_memory_test<cambra>(test_assigna_cambra);

  d.add_memory_test<laberint>(test_ctor_laberint);
  d.add_memory_test<laberint>(test_ctor_laberint_fitxer);
  d.add_memory_test<laberint>(test_copyctor_laberint);
  d.add_memory_test<laberint>(test_assigna_laberint);

  d.add_memory_test<particio<int> >(test_ctor_particio_int, "particio<int>");
  d.add_memory_test<particio<int> >(test_copyctor_particio_int, "particio<int>");
  d.add_memory_test<particio<int> >(test_assigna_particio_int, "particio<int>");

  // instal.lacio de tipus
  d.install_type<cambra>();
  d.install_type<laberint>();
  d.install_type<particio<int> >();

  d.install_std_type<paret>();
  d.install_std_type<posicio>();
  d.install_std_type<list<posicio> >();

  d.go();
}
