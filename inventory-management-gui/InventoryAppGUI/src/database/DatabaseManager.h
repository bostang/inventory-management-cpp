#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <string>
#include <vector>
#include <memory> // Untuk std::unique_ptr
#include <pqxx/pqxx> // Pustaka utama libpqxx

// Sertakan header ItemInventaris karena DatabaseManager akan bekerja dengan struct ini
#include "inventory/ItemInventaris.h"

// Menggunakan namespace std agar tidak perlu menulis 'std::' berulang kali
using namespace std;

/**
 * @brief Kelas untuk mengelola koneksi dan operasi database PostgreSQL.
 */
class DatabaseManager {
public:
    /**
     * @brief Konstruktor untuk DatabaseManager.
     * @param conn_str String koneksi ke database PostgreSQL.
     * Contoh: "dbname=inventaris_db user=postgres password=your_password host=localhost port=5432"
     */
    DatabaseManager(const string& conn_str);

    /**
     * @brief Mencoba untuk membuat koneksi ke database.
     * @return true jika koneksi berhasil, false jika gagal.
     */
    bool connect();

    /**
     * @brief Memutus koneksi dari database.
     */
    void disconnect();

    /**
     * @brief Menambahkan item baru ke database.
     * @param item Objek ItemInventaris yang akan ditambahkan.
     * @return true jika berhasil, false jika gagal.
     */
    bool addItemToDB(const ItemInventaris& item);

    /**
     * @brief Mengambil semua item dari database.
     * @return Vektor berisi semua objek ItemInventaris.
     */
    vector<ItemInventaris> getAllItemsFromDB();

    /**
     * @brief Mengambil item berdasarkan ID dari database.
     * @param id ID item yang ingin dicari.
     * @return Objek ItemInventaris jika ditemukan, atau ItemInventaris kosong jika tidak.
     */
    ItemInventaris getItemFromDB(const string& id);

    /**
     * @brief Memperbarui kuantitas dan harga item yang sudah ada di database.
     * @param item Objek ItemInventaris dengan ID yang sudah ada dan nilai baru.
     * @return true jika berhasil, false jika gagal.
     */
    bool updateItemInDB(const ItemInventaris& item);

    /**
     * @brief Menghapus item dari database berdasarkan ID.
     * @param id ID item yang ingin dihapus.
     * @return true jika berhasil, false jika gagal.
     */
    bool deleteItemFromDB(const string& id);

private:
    string connectionString;
    // unique_ptr digunakan untuk mengelola lifetime objek koneksi pqxx::connection
    // Ini memastikan koneksi ditutup secara otomatis saat objek DatabaseManager dihancurkan
    unique_ptr<pqxx::connection> conn;

    /**
     * @brief Membuat tabel inventaris jika belum ada.
     * @return true jika tabel berhasil dibuat atau sudah ada, false jika gagal.
     */
    bool createTableIfNotExists();
};

#endif // DATABASE_MANAGER_H