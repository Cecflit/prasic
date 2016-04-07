#include <math.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <ctime>
#include <map>

#include "nahoda.h"
#include "prase.h"

using namespace std;

map<string, string> slovnik;
vector<Prase> slova;
string nazev_souboru;

void nacti_slovnik() {
  FILE *f = fopen("const_str.txt", "r");

  char znak = ' ';
  string slovo;
  while (!feof(f)) {
    slovo = "";

    // Přeskočí mezery
    while (znak <= ' ' && !feof(f)) {
      znak = fgetc(f);
    }
    if (feof(f)) {
      break;
    }

    do {
      slovo.push_back(znak);
      znak = fgetc(f);
    } while (znak > ' ' && !feof(f));
    slovnik[slovo] = slovo;
  }
  fclose(f);
}

void uloz_slovnik() {
  FILE *f = fopen("out/slovnik.txt", "w");
  FILE *g = fopen("out/const_str.txt", "w");

  for (map<string,string>::iterator it=slovnik.begin(); it!=slovnik.end(); ++it) {
    if (it->first == it->second) {
      string str = it->first + '\n';
      fputs(str.c_str(), g);
    } else {
      string str = it->second + " = " + it->first + '\n';
      fputs(str.c_str(), f);
    }
  }

  fclose(f);
  fclose(g);
}

bool je_obsazeno(string s) {
  for (map<string,string>::iterator it=slovnik.begin(); it!=slovnik.end(); ++it) {
    if (it->second == s) {
      return true;
    }
  }
  return false;
}

string spras_slovo(string slovo) {
  if (slovnik[slovo].size()) {
    return slovnik[slovo];
  } else {
    string novy;
    do {
      novy = nahodny_nazev(slovnik.size());
    } while (je_obsazeno(novy));
    slovnik[slovo] = novy;
    return novy;
  }
}

void nacti_usek(FILE* f, string& slovo, char& znak, char ukoncovaci_znak) {
  do {
    slovo.push_back(znak);
    znak = fgetc(f);
    if (znak == '\\') {
      slovo.push_back(znak);
      znak = fgetc(f);
    }
  } while (znak != ukoncovaci_znak && !feof(f));
  slovo.push_back(znak);
  znak = fgetc(f);
}

void nacti_zdroj() {
  string zdroj = "in/" + nazev_souboru;
  FILE *f = fopen(zdroj.c_str(), "r");

  char znak = ' ';
  string slovo;
  bool mod_inc;
  bool je_include;
  bool je_cislo;
  while (!feof(f)) {
    slovo = "";

    // Přeskočí mezery
    while (znak <= ' ' && !feof(f)) {
      if (znak == 0x0D || znak == 0x0A) {
        if (mod_inc) {
          (slova.end()-1)->museni_odradkovat = true;
        }
        mod_inc = false;
        je_include = false;
      }
      znak = fgetc(f);
    }
    if (feof(f)) {
      break;
    }

    if (Prase::je_znak_specialni(znak)) {
      if (znak == '/') {
        slovo.push_back(znak);
        znak = fgetc(f);
        if (znak == '*') {
          // komentář /**/
          bool pokracuj = true;
          bool byla_hvezda = false;
          while (pokracuj && !feof(f)) {
            znak = fgetc(f);
            pokracuj = !(byla_hvezda && (znak == '/'));
            byla_hvezda = znak == '*';
          }
          continue;
        } else if (znak == '/') {
          // komentář //
          while (znak != 0x0D && znak != 0x0A && !feof(f)) {
            znak = fgetc(f);
          }
          continue;
        } else if (znak == '=') {
          slovo.push_back(znak);
          znak = fgetc(f);
        }
      } else if (znak == '+' || znak == '-' || znak == '|' || znak == '&' ||
                 znak == ':' || znak == '=' || znak == '>' || znak == '<' ||
                 znak == '^' || znak == '*' || znak == '%' || znak == '!') {
        if (je_include && znak == '<') {
          nacti_usek(f, slovo, znak, '>');
        } else {
          slovo.push_back(znak);
          znak = fgetc(f);
          if ((znak == slovo[0] && znak != '*' && znak != '*') || znak == '=' ||
              (slovo[0] == '-' && znak == '>')) {
            slovo.push_back(znak);
            znak = fgetc(f);
          }
        }
      } else if (znak == '"') {
        nacti_usek(f, slovo, znak, '"');
      } else if (znak == '\'') {
        nacti_usek(f, slovo, znak, '\'');
      } else {
        slovo.push_back(znak);
        znak = fgetc(f);
      }
    } else {
      je_cislo = znak >= '0' && znak <= '9';
      do {
        slovo.push_back(znak);
        znak = fgetc(f);
      } while (znak > ' ' && !feof(f) && (!Prase::je_znak_specialni(znak) || (znak == '.' && je_cislo)));
    }
    Prase p(slovo);
    p.nechce_odradkovat = mod_inc;
    if (p.je_inklud) {
      mod_inc = true;
      je_include = slovo == "#include";
    }
    if (p.je_sprasitelne) {
      p.text = spras_slovo(p.text);
    }
    slova.push_back(p);
  }

  fclose(f);
}

void uloz_zdroj() {
  string zdroj = "out/" + nazev_souboru;
  FILE *f = fopen(zdroj.c_str(), "w");

  int odsazeni = 0;
  for (vector<Prase>::iterator it = slova.begin(); it != slova.end(); ++it) {
    fputs(it->text.c_str(), f);
    if (it+1 != slova.end()) {
      if (it->museni_odradkovat || (it+1)->je_inklud || (exp_rand(6) > 3 && !(it+1)->nechce_odradkovat)) {
        fprintf(f, "\n");
        if (nahodne(3) == 0 && odsazeni < 10) {
          odsazeni++;
        }
        if (nahodne(3) == 0 && odsazeni > 0) {
          odsazeni--;
        }
        for (int j = odsazeni; j; j--) {
          fprintf(f, "  ");
        }
      }
      if ((it->chce_mezeru && (it+1)->chce_mezeru) || nahodne(6) == 0) {
        fprintf(f, " ");
      }
    }
  }

  fclose(f);
}

int main(int argc, char** arg) {
  srandom(time(0));

  slovnik.clear();
  nacti_slovnik();

  nazev_souboru = "main.cpp";
  nacti_zdroj();
  uloz_zdroj();

  uloz_slovnik();

  return 0;
}
