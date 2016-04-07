#ifndef RANDOM_H
#define RANDOM_H

int nahodne(int max);
int nahodne(int min, int max);
double nahodne_d(double max);
double nahodne_d(double min, double max);
int exp_rand(int xp);
int exp_rand(int xp, int max);
int signed_xp_rand(int xp);
int signed_xp_rand(int xp, int max);
int nahodne_vyber(const double* pole, int pocet);

#endif // RANDOM_H
