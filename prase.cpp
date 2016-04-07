#include <math.h>

#include "nahoda.h"
#include "prase.h"

Prase::Prase(std::string text_) :
  je_inklud(),
  chce_mezeru(),
  nechce_odradkovat(false),
  museni_odradkovat(false),
  je_sprasitelne(false),
  text(text_)
{
  je_inklud = text[0] == '#';
  chce_mezeru = !je_znak_specialni(text[0]);
  je_sprasitelne = !je_inklud && chce_mezeru && !(text[0] >= '0' && text[0] <= '9');
}

bool Prase::je_znak_specialni(char zn) {
  return !((zn >= '0' && zn <= '9') || (zn >= 'A' && zn <= 'Z') ||
         (zn >= 'a' && zn <= 'z') || zn == '$' || zn == '_' || zn == '#');
}

std::string nahodny_nazev(int velikost) {
  std::string result = "";
  char c;
  int id = nahodne(52);
  if (id < 26) {
    c = 'A' + id;
  } else {
    c = 'a' + id - 26;
  }
  result.push_back(c);
  float prodluzovak = log2(velikost)/log2(26);
  for (int i = 2 + exp_rand(4) + prodluzovak; i; i--) {
    id = nahodne(64);
    if (id < 26) {
      c = 'A' + id;
    } else if (id < 52) {
      c = 'a' + id - 26;
    } else if (id < 62) {
      c = '0' + id - 52;
    } else {
      c = '_';
    }
    result.push_back(c);
  }
  return result;
}
