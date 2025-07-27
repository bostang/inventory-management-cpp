#include "ItemInventaris.h" // Include header kustom kita
#include "Utils.h"          // Include header Utils untuk fungsi helper
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cctype>

// Menggunakan namespace std agar tidak perlu menulis 'std::' berulang kali
using namespace std;

// --- Implementasi Fungsi-fungsi Aplikasi Utama ---

void tambahItem(vector<ItemInventaris>& inventaris) {
    ItemInventaris newItem;
    cout << "\n--- Tambah Item Baru ---" << endl;

    cout << "Masukkan ID Item (misal: A001): ";
    // clearInputBuffer();
    getline(cin, newItem.id);

    // Cek apakah ID sudah ada
    for (const auto& item : inventaris) {
        if (item.id == newItem.id) {
            cout << "Error: ID Item '" << newItem.id << "' sudah ada. Gagal menambahkan item." << endl;
            return;
        }
    }

    cout << "Masukkan Nama Item: ";
    getline(cin, newItem.nama);

    newItem.kuantitas = getValidatedIntInput("Masukkan Kuantitas: ", 0, 99999);
    newItem.harga = getValidatedDoubleInput("Masukkan Harga per Unit: ", 0.0);

    inventaris.push_back(newItem);
    cout << "Item berhasil ditambahkan!" << endl;
}

void tampilkanSemuaItem(const vector<ItemInventaris>& inventaris) {
    cout << "\n--- Daftar Inventaris ---" << endl;
    if (inventaris.empty()) {
        cout << "Inventaris kosong." << endl;
        return;
    }

    cout << left << setw(10) << "ID"
         << left << setw(25) << "Nama"
         << right << setw(12) << "Kuantitas"
         << right << setw(15) << "Harga/Unit" << endl;
    cout << string(62, '-') << endl;

    for (const auto& item : inventaris) {
        cout << left << setw(10) << item.id
             << left << setw(25) << item.nama
             << right << setw(12) << item.kuantitas
             << right << setw(15) << fixed << setprecision(2) << item.harga << endl;
    }
    cout << string(62, '-') << endl;
}

void cariItem(const vector<ItemInventaris>& inventaris) {
    cout << "\n--- Cari Item ---" << endl;
    if (inventaris.empty()) {
        cout << "Inventaris kosong. Tidak ada yang bisa dicari." << endl;
        return;
    }

    string keyword;
    cout << "Masukkan ID atau Nama Item yang ingin dicari: ";
    // clearInputBuffer();
    getline(cin, keyword);

    string keywordLower;
    for (char c : keyword) {
        keywordLower += tolower(c);
    }

    bool ditemukan = false;
    cout << "\nHasil Pencarian:" << endl;
    for (const auto& item : inventaris) {
        string itemNamaLower;
        for (char c : item.nama) {
            itemNamaLower += tolower(c);
        }
        string itemIdLower;
        for (char c : item.id) {
            itemIdLower += tolower(c);
        }

        if (itemIdLower.find(keywordLower) != string::npos ||
            itemNamaLower.find(keywordLower) != string::npos) {
            cout << "ID: " << item.id
                 << ", Nama: " << item.nama
                 << ", Kuantitas: " << item.kuantitas
                 << ", Harga: " << fixed << setprecision(2) << item.harga << endl;
            ditemukan = true;
        }
    }

    if (!ditemukan) {
        cout << "Item dengan '" << keyword << "' tidak ditemukan." << endl;
    }
}

void perbaruiItem(vector<ItemInventaris>& inventaris) {
    cout << "\n--- Perbarui Item ---" << endl;
    if (inventaris.empty()) {
        cout << "Inventaris kosong. Tidak ada item untuk diperbarui." << endl;
        return;
    }

    string idToUpdate;
    cout << "Masukkan ID Item yang ingin diperbarui: ";
    // clearInputBuffer();
    getline(cin, idToUpdate);

    bool ditemukan = false;
    for (auto& item : inventaris) {
        if (item.id == idToUpdate) {
            ditemukan = true;
            cout << "Item ditemukan: " << item.nama << " (Kuantitas: " << item.kuantitas << ", Harga: " << item.harga << ")" << endl;

            cout << "Pilih apa yang ingin diperbarui:" << endl;
            cout << "1. Kuantitas" << endl;
            cout << "2. Harga" << endl;
            int pilihanUpdate = getValidatedIntInput("Pilihan (1-2): ", 1, 2);

            if (pilihanUpdate == 1) {
                item.kuantitas = getValidatedIntInput("Masukkan Kuantitas Baru: ", 0, 99999);
                cout << "Kuantitas item '" << item.nama << "' berhasil diperbarui." << endl;
            } else if (pilihanUpdate == 2) {
                item.harga = getValidatedDoubleInput("Masukkan Harga Baru: ", 0.0);
                cout << "Harga item '" << item.nama << "' berhasil diperbarui." << endl;
            }
            return;
        }
    }

    if (!ditemukan) {
        cout << "Item dengan ID '" << idToUpdate << "' tidak ditemukan." << endl;
    }
}

void hapusItem(vector<ItemInventaris>& inventaris) {
    cout << "\n--- Hapus Item ---" << endl;
    if (inventaris.empty()) {
        cout << "Inventaris kosong. Tidak ada item untuk dihapus." << endl;
        return;
    }

    string idToDelete;
    cout << "Masukkan ID Item yang ingin dihapus: ";
    // clearInputBuffer();
    getline(cin, idToDelete);

    auto it = remove_if(inventaris.begin(), inventaris.end(),
                        [&](const ItemInventaris& item) {
                            return item.id == idToDelete;
                        });

    if (it != inventaris.end()) {
        inventaris.erase(it, inventaris.end());
        cout << "Item dengan ID '" << idToDelete << "' berhasil dihapus." << endl;
    } else {
        cout << "Item dengan ID '" << idToDelete << "' tidak ditemukan." << endl;
    }
}