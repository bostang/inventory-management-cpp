#include <iostream>
#include <vector>
#include <limits> // Untuk std::numeric_limits
#include "inventory/ItemInventaris.h" // Include header untuk ItemInventaris dan fungsinya
#include "utils/Utils.h"          // Include header untuk fungsi helper
#include "database/DatabaseManager.h" // Include header untuk DatabaseManager

// Menggunakan namespace std agar tidak perlu menulis 'std::' berulang kali
using namespace std;

// Fungsi-fungsi manajemen inventaris yang dimodifikasi untuk berinteraksi dengan DB
void tambahItem(DatabaseManager& dbManager);
void tampilkanSemuaItem(DatabaseManager& dbManager);
void cariItem(DatabaseManager& dbManager);
void perbaruiItem(DatabaseManager& dbManager);
void hapusItem(DatabaseManager& dbManager);

int main() {
    // Ganti dengan string koneksi PostgreSQL kamu yang sebenarnya!
    // Contoh: "dbname=inventaris_db user=postgres password=your_password host=localhost port=5432"
    // Pastikan database 'inventaris_db' sudah dibuat dan user 'postgres' (atau user lain) memiliki akses.
    string conn_str = "dbname=inventaris_db user=postgres password=password host=localhost port=5432";
    DatabaseManager dbManager(conn_str);

    if (!dbManager.connect()) {
        cerr << "Gagal terkoneksi ke database. Aplikasi akan keluar." << endl;
        return 1; // Keluar dengan kode error
    }

    int pilihan;

    do {
        cout << "\n=== Manajemen Inventaris ===" << endl;
        cout << "1. Tambah Item" << endl;
        cout << "2. Tampilkan Semua Item" << endl;
        cout << "3. Cari Item" << endl;
        cout << "4. Perbarui Item" << endl;
        cout << "5. Hapus Item" << endl;
        cout << "0. Keluar" << endl;

        pilihan = getValidatedIntInput("Pilih opsi (0-5): ", 0, 5);

        switch (pilihan) {
            case 1:
                tambahItem(dbManager);
                break;
            case 2:
                tampilkanSemuaItem(dbManager);
                break;
            case 3:
                cariItem(dbManager);
                break;
            case 4:
                perbaruiItem(dbManager);
                break;
            case 5:
                hapusItem(dbManager);
                break;
            case 0:
                cout << "Terima kasih telah menggunakan aplikasi manajemen inventaris." << endl;
                break;
            default:
                cout << "Pilihan tidak dikenal. Silakan coba lagi." << endl;
                break;
        }
    } while (pilihan != 0);

    dbManager.disconnect(); // Putuskan koneksi saat aplikasi keluar
    return 0;
}


// --- Implementasi fungsi-fungsi manajemen inventaris yang berinteraksi dengan DB ---

void tambahItem(DatabaseManager& dbManager) {
    ItemInventaris newItem;
    cout << "\n--- Tambah Item Baru ---" << endl;

    cout << "Masukkan ID Item (misal: A001): ";
    getline(cin, newItem.id);

    // Cek apakah ID sudah ada di database
    if (!dbManager.getItemFromDB(newItem.id).id.empty()) {
        cout << "Error: ID Item '" << newItem.id << "' sudah ada. Gagal menambahkan item." << endl;
        return;
    }

    cout << "Masukkan Nama Item: ";
    getline(cin, newItem.nama);

    newItem.kuantitas = getValidatedIntInput("Masukkan Kuantitas: ", 0, 99999);
    newItem.harga = getValidatedDoubleInput("Masukkan Harga per Unit: ", 0.0);

    if (dbManager.addItemToDB(newItem)) {
        cout << "Item berhasil ditambahkan!" << endl;
    } else {
        cout << "Gagal menambahkan item ke database." << endl;
    }
}

void tampilkanSemuaItem(DatabaseManager& dbManager) {
    cout << "\n--- Daftar Inventaris ---" << endl;
    vector<ItemInventaris> inventaris = dbManager.getAllItemsFromDB();

    if (inventaris.empty()) {
        cout << "Inventaris kosong." << endl;
        return;
    }

    // Header tabel
    cout << left << setw(10) << "ID"
         << left << setw(25) << "Nama"
         << right << setw(12) << "Kuantitas"
         << right << setw(15) << "Harga/Unit" << endl;
    cout << string(62, '-') << endl; // Garis pemisah

    // Menampilkan setiap item
    for (const auto& item : inventaris) {
        cout << left << setw(10) << item.id
             << left << setw(25) << item.nama
             << right << setw(12) << item.kuantitas
             << right << setw(15) << fixed << setprecision(2) << item.harga << endl;
    }
    cout << string(62, '-') << endl;
}

void cariItem(DatabaseManager& dbManager) {
    cout << "\n--- Cari Item ---" << endl;
    string keyword;
    cout << "Masukkan ID Item yang ingin dicari: ";
    getline(cin, keyword);

    ItemInventaris foundItem = dbManager.getItemFromDB(keyword);

    if (!foundItem.id.empty()) { // Jika ID tidak kosong, berarti item ditemukan
        cout << "Ditemukan! ID: " << foundItem.id
             << ", Nama: " << foundItem.nama
             << ", Kuantitas: " << foundItem.kuantitas
             << ", Harga: " << fixed << setprecision(2) << foundItem.harga << endl;
    } else {
        cout << "Item dengan ID '" << keyword << "' tidak ditemukan." << endl;
    }
}

void perbaruiItem(DatabaseManager& dbManager) {
    cout << "\n--- Perbarui Item ---" << endl;
    string idToUpdate;
    cout << "Masukkan ID Item yang ingin diperbarui: ";
    getline(cin, idToUpdate);

    ItemInventaris existingItem = dbManager.getItemFromDB(idToUpdate);

    if (existingItem.id.empty()) {
        cout << "Item dengan ID '" << idToUpdate << "' tidak ditemukan." << endl;
        return;
    }

    cout << "Item ditemukan: " << existingItem.nama << " (Kuantitas: " << existingItem.kuantitas << ", Harga: " << existingItem.harga << ")" << endl;

    cout << "Pilih apa yang ingin diperbarui:" << endl;
    cout << "1. Kuantitas" << endl;
    cout << "2. Harga" << endl;
    cout << "3. Nama" << endl; // Tambahkan opsi update nama
    int pilihanUpdate = getValidatedIntInput("Pilihan (1-3): ", 1, 3);

    if (pilihanUpdate == 1) {
        existingItem.kuantitas = getValidatedIntInput("Masukkan Kuantitas Baru: ", 0, 99999);
    } else if (pilihanUpdate == 2) {
        existingItem.harga = getValidatedDoubleInput("Masukkan Harga Baru: ", 0.0);
    } else if (pilihanUpdate == 3) {
        cout << "Masukkan Nama Baru: ";
        getline(cin, existingItem.nama);
    }

    if (dbManager.updateItemInDB(existingItem)) {
        cout << "Item berhasil diperbarui." << endl;
    } else {
        cout << "Gagal memperbarui item di database." << endl;
    }
}

void hapusItem(DatabaseManager& dbManager) {
    cout << "\n--- Hapus Item ---" << endl;
    string idToDelete;
    cout << "Masukkan ID Item yang ingin dihapus: ";
    getline(cin, idToDelete);

    if (dbManager.deleteItemFromDB(idToDelete)) {
        cout << "Item berhasil dihapus." << endl;
    } else {
        cout << "Gagal menghapus item dari database atau item tidak ditemukan." << endl;
    }
}