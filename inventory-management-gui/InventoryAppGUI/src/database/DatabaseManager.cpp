#include "database/DatabaseManager.h" // Include header kustom kita
#include <iostream>
#include <stdexcept> // Untuk std::runtime_error
#include <iomanip>   // Untuk std::fixed, std::setprecision

// Menggunakan namespace std agar tidak perlu menulis 'std::' berulang kali
using namespace std;

// Konstruktor
DatabaseManager::DatabaseManager(const string& conn_str) : connectionString(conn_str), conn(nullptr) {}

// Fungsi untuk koneksi ke database
bool DatabaseManager::connect() {
    try {
        // Inisialisasi objek koneksi
        conn = make_unique<pqxx::connection>(connectionString);
        if (conn->is_open()) {
            cout << "Koneksi ke database berhasil!" << endl;
            // Pastikan tabel ada setelah koneksi berhasil
            return createTableIfNotExists();
        } else {
            cerr << "Koneksi ke database gagal." << endl;
            return false;
        }
    } catch (const pqxx::sql_error &e) {
        cerr << "SQL error saat koneksi: " << e.what() << " (Query: " << e.query() << ")" << endl;
        return false;
    } catch (const exception &e) {
        cerr << "Error lain saat koneksi: " << e.what() << endl;
        return false;
    }
}

// Fungsi untuk membuat tabel jika belum ada
bool DatabaseManager::createTableIfNotExists() {
    if (!conn || !conn->is_open()) {
        cerr << "Tidak terkoneksi ke database. Tidak bisa membuat tabel." << endl;
        return false;
    }
    try {
        pqxx::work W(*conn); // Objek transaksi
        // Perintah SQL untuk membuat tabel jika tidak ada
        W.exec("CREATE TABLE IF NOT EXISTS inventaris ("
               "id VARCHAR(50) PRIMARY KEY,"
               "nama VARCHAR(100) NOT NULL,"
               "kuantitas INT NOT NULL,"
               "harga DECIMAL(10, 2) NOT NULL"
               ");");
        W.commit(); // Commit transaksi
        cout << "Tabel 'inventaris' berhasil diperiksa/dibuat." << endl;
        return true;
    } catch (const pqxx::sql_error &e) {
        cerr << "SQL error saat membuat tabel: " << e.what() << " (Query: " << e.query() << ")" << endl;
        return false;
    } catch (const exception &e) {
        cerr << "Error lain saat membuat tabel: " << e.what() << endl;
        return false;
    }
}

// Fungsi untuk memutus koneksi
void DatabaseManager::disconnect() {
    // Dengan std::unique_ptr, koneksi akan otomatis ditutup saat 'conn' dihancurkan
    // atau ketika DatabaseManager object keluar dari scope.
    // Tidak perlu memanggil conn->disconnect() secara eksplisit di sini.
    if (conn) { // Cek apakah unique_ptr tidak null
        cout << "Koneksi database akan diputus secara otomatis." << endl;
        // Opsional: reset unique_ptr untuk secara eksplisit memutus koneksi lebih awal
        conn.reset();
    }
}

// Fungsi untuk menambahkan item
bool DatabaseManager::addItemToDB(const ItemInventaris& item) {
    if (!conn || !conn->is_open()) {
        cerr << "Tidak terkoneksi ke database." << endl;
        return false;
    }
    try {
        pqxx::work W(*conn);
        // Menggunakan W.quote() untuk mencegah SQL injection dan menangani string dengan benar
        W.exec("INSERT INTO inventaris (id, nama, kuantitas, harga) VALUES ("
               + W.quote(item.id) + ", "
               + W.quote(item.nama) + ", "
               + W.quote(item.kuantitas) + ", " // int akan otomatis di-quote dengan benar
               + W.quote(item.harga) + ");");   // double akan otomatis di-quote dengan benar
        W.commit();
        cout << "Item '" << item.nama << "' berhasil ditambahkan ke database." << endl;
        return true;
    } catch (const pqxx::unique_violation &e) {
        cerr << "Error: ID Item '" << item.id << "' sudah ada di database." << endl;
        return false;
    } catch (const pqxx::sql_error &e) {
        cerr << "SQL error saat menambah item: " << e.what() << " (Query: " << e.query() << ")" << endl;
        return false;
    } catch (const exception &e) {
        cerr << "Error lain saat menambah item: " << e.what() << endl;
        return false;
    }
}

// Fungsi untuk mengambil semua item
vector<ItemInventaris> DatabaseManager::getAllItemsFromDB() {
    vector<ItemInventaris> items;
    if (!conn || !conn->is_open()) {
        cerr << "Tidak terkoneksi ke database." << endl;
        return items;
    }
    try {
        pqxx::nontransaction N(*conn); // Nontransaction untuk operasi SELECT
        pqxx::result R = N.exec("SELECT id, nama, kuantitas, harga FROM inventaris ORDER BY id;");

        for (pqxx::row row : R) {
            ItemInventaris item;
            item.id = row["id"].as<string>();
            item.nama = row["nama"].as<string>();
            item.kuantitas = row["kuantitas"].as<int>();
            item.harga = row["harga"].as<double>();
            items.push_back(item);
        }
    } catch (const pqxx::sql_error &e) {
        cerr << "SQL error saat mengambil semua item: " << e.what() << " (Query: " << e.query() << ")" << endl;
    } catch (const exception &e) {
        cerr << "Error lain saat mengambil semua item: " << e.what() << endl;
    }
    return items;
}

// Fungsi untuk mengambil item berdasarkan ID
ItemInventaris DatabaseManager::getItemFromDB(const string& id) {
    ItemInventaris item; // Item kosong sebagai default
    if (!conn || !conn->is_open()) {
        cerr << "Tidak terkoneksi ke database." << endl;
        return item;
    }
    try {
        pqxx::nontransaction N(*conn);
        pqxx::result R = N.exec("SELECT id, nama, kuantitas, harga FROM inventaris WHERE id = " + N.quote(id) + ";");

        if (!R.empty()) {
            pqxx::row row = R[0];
            item.id = row["id"].as<string>();
            item.nama = row["nama"].as<string>();
            item.kuantitas = row["kuantitas"].as<int>();
            item.harga = row["harga"].as<double>();
        } else {
            // Item tidak ditemukan, kembalikan item kosong
        }
    } catch (const pqxx::sql_error &e) {
        cerr << "SQL error saat mencari item: " << e.what() << " (Query: " << e.query() << ")" << endl;
    } catch (const exception &e) {
        cerr << "Error lain saat mencari item: " << e.what() << endl;
    }
    return item;
}

// Fungsi untuk memperbarui item
bool DatabaseManager::updateItemInDB(const ItemInventaris& item) {
    if (!conn || !conn->is_open()) {
        cerr << "Tidak terkoneksi ke database." << endl;
        return false;
    }
    try {
        pqxx::work W(*conn);
        // Perbarui kuantitas dan harga berdasarkan ID
        pqxx::result R = W.exec("UPDATE inventaris SET "
                                "nama = " + W.quote(item.nama) + ", " // Nama juga bisa diupdate
                                "kuantitas = " + W.quote(item.kuantitas) + ", "
                                "harga = " + W.quote(item.harga) + " "
                                "WHERE id = " + W.quote(item.id) + ";");
        W.commit();

        if (R.affected_rows() > 0) {
            cout << "Item dengan ID '" << item.id << "' berhasil diperbarui di database." << endl;
            return true;
        } else {
            cout << "Item dengan ID '" << item.id << "' tidak ditemukan untuk diperbarui." << endl;
            return false;
        }
    } catch (const pqxx::sql_error &e) {
        cerr << "SQL error saat memperbarui item: " << e.what() << " (Query: " << e.query() << ")" << endl;
        return false;
    } catch (const exception &e) {
        cerr << "Error lain saat memperbarui item: " << e.what() << endl;
        return false;
    }
}

// Fungsi untuk menghapus item
bool DatabaseManager::deleteItemFromDB(const string& id) {
    if (!conn || !conn->is_open()) {
        cerr << "Tidak terkoneksi ke database." << endl;
        return false;
    }
    try {
        pqxx::work W(*conn);
        pqxx::result R = W.exec("DELETE FROM inventaris WHERE id = " + W.quote(id) + ";");
        W.commit();

        if (R.affected_rows() > 0) {
            cout << "Item dengan ID '" << id << "' berhasil dihapus dari database." << endl;
            return true;
        } else {
            cout << "Item dengan ID '" << id << "' tidak ditemukan untuk dihapus." << endl;
            return false;
        }
    } catch (const pqxx::sql_error &e) {
        cerr << "SQL error saat menghapus item: " << e.what() << " (Query: " << e.query() << ")" << endl;
        return false;
    } catch (const exception &e) {
        cerr << "Error lain saat menghapus item: " << e.what() << endl;
        return false;
    }
}