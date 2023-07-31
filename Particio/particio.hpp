
#ifndef _PARTICIO_HPP
#define _PARTICIO_HPP

#include <esin/error>
#include <esin/util>

using util::nat;

template <typename T>
class particio {
public:

  // Construeix una particio amb n elements com a màxim.
  explicit particio(nat n) throw(error);

  // Constructora per còpia, assignació i destructora.
  particio(const particio & p) throw(error);
  particio & operator=(const particio & p) throw(error);
  ~particio() throw();

  // Afegeix un nou element a la particio. Aquest nou element formarà ell sol
  // un nou grup. En cas que l'element ja estigués present a la partició llavors
  // no es modifica la partició. Es produeix un error si la particio ja té el
  // número màxim d'elements abans d'afegir aquest nou.
  void afegir(const T &x) throw(error);

  // Uneix els dos grups als quals pertanyen aquests dos elements. Si tots dos
  // elements ja pertanyien al mateix grup no fa res.
  // Es produeix un error si algun dels elements no pertany a la partició.
  void unir(const T & x, const T & y) throw(error);

  // Retorna si els elements x i y pertanyen al mateix grup.
  // Es produeix un error si algun dels dos elements no pertany a la partició.
  bool mateix_grup(const T & x, const T & y) const throw(error);

  // Retorna el número de grups que té la particio.
  nat size() const throw();

  // Retorna el número d'elements que té la particio.
  nat num_elements() const throw();

  // Retorna el número màxim d'elements que pot tenir la particio.
  nat num_maxim() const throw();

  // Gestió d'errors.
  static const int ElemInexistent = 40;
  static const int ParticioPlena = 41;

private:
  #include "particio.rep"
};

/* Degut a la impossibilitat de compilar de manera separada les classes
   genèriques (templates) en el compilador de C++ , la implementació s'ha
   d'incloure en el fitxer particio.t (que no es compila independentment). */
#include "particio.t"

#endif
