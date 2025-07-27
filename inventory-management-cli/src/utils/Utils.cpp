#include "Utils.h"
#include <iostream>

using namespace std;

void clearInputBuffer() {
    // Membuang semua karakter dari buffer input hingga karakter newline ditemukan
    // atau hingga batas maksimum stream size tercapai.
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getValidatedIntInput(const string& prompt, int min, int max) {
    int pilihan;
    while (true) {
        cout << prompt;
        // Coba baca input integer
        if (cin >> pilihan) {
            // Jika pembacaan berhasil, bersihkan sisa newline di buffer
            clearInputBuffer();
            // Lakukan validasi rentang nilai
            if (pilihan >= min && pilihan <= max) {
                return pilihan; // Input valid, kembalikan nilai
            } else {
                cout << "Nilai di luar rentang. Masukkan angka antara " << min << " dan " << max << ": ";
            }
        } else {
            // Jika pembacaan gagal (misal, user input teks), bersihkan status error cin
            cout << "Input tidak valid. Masukkan angka: ";
            cin.clear();
            // Buang sisa input yang salah dari buffer
            clearInputBuffer();
        }
    }
}

double getValidatedDoubleInput(const string& prompt, double min) {
    double nilai;
    while (true) {
        cout << prompt;
        if (cin >> nilai) {
            clearInputBuffer(); // Bersihkan buffer setelah pembacaan berhasil
            if (nilai >= min) {
                return nilai;
            } else {
                cout << "Nilai harus " << min << " atau lebih tinggi. Masukkan angka positif: ";
            }
        } else {
            cout << "Input tidak valid. Masukkan angka: ";
            cin.clear();
            clearInputBuffer();
        }
    }
}