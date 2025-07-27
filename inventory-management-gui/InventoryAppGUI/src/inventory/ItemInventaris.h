#ifndef ITEM_INVENTARIS_H
#define ITEM_INVENTARIS_H

#include <vector>
#include <string>

// Struktur untuk merepresentasikan sebuah Item Inventaris
struct ItemInventaris {
    std::string id;
    std::string nama;
    int kuantitas;
    double harga;
};

// Deklarasi fungsi-fungsi terkait manajemen inventaris
// (Implementasinya ada di ItemInventaris.cpp)
void tambahItem(std::vector<ItemInventaris>& inventaris);
void tampilkanSemuaItem(const std::vector<ItemInventaris>& inventaris);
void cariItem(const std::vector<ItemInventaris>& inventaris);
void perbaruiItem(std::vector<ItemInventaris>& inventaris);
void hapusItem(std::vector<ItemInventaris>& inventaris);

#endif // ITEM_INVENTARIS_H