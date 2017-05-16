#ifndef PRASE_H
#define PRASE_H

#include <string>

class Prase
{
  public:
    Prase(std::string text_);

    bool je_inklud;
    bool chce_mezeru;
    bool nechce_odradkovat;
    bool museni_odradkovat;
    bool je_sprasitelne;

    std::string text;

    static bool je_znak_specialni(char zn);
};

std::string nahodny_nazev(int velikost);

#endif // PRASE_H
