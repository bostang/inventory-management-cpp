#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <limits> // Diperlukan untuk std::numeric_limits

// Deklarasi fungsi-fungsi helper umum
// (Implementasinya ada di Utils.cpp)
void clearInputBuffer();
int getValidatedIntInput(const std::string& prompt, int min, int max);
double getValidatedDoubleInput(const std::string& prompt, double min);

#endif // UTILS_H