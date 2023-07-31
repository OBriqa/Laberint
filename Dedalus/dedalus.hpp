
#ifndef _DEDALUS_HPP
#define _DEDALUS_HPP

#include <esin/error>
#include <esin/util>
#include "laberint.hpp"
#include "particio.hpp"

namespace dedalus {

  // Excava el laberint, fent un laberint perfecte. Un laberint està excavat
  // si i només si alguna de les seves cambres té alguna porta oberta.
  // Es produeix un error si el laberint ja està excavat.
  void construir(laberint & M) throw(error);

  // Gestió d'errors.
  static const int EstaExcavat = 50;
};

#endif
