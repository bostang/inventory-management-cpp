# Inventory Management (Modularized)

## Struktur file

```tree
.
├── Makefile
├── README.md
├── sql
│   └── db_init.sql
└── src
    ├── database
    │   ├── DatabaseManager.cpp
    │   └── DatabaseManager.h
    ├── inventory
    │   ├── ItemInventaris.cpp
    │   └── ItemInventaris.h
    ├── main.cpp
    └── utils
        ├── Utils.cpp
        └── Utils.h
```

## Cara Menjalankan

```bash
# inisialisasi DB
psql -U postgres -f sql/db_init.sql

# melakukan kompilasi
make        

# menjalankan aplikasi
./bin/inventaris_app

# untuk membersihkan aplikasi (setelah selesai)
make clean
```

## Library yang dipakai

### `libpqxx`

- `libpqxx` : library C++ modern yang menyediakan interface objek-relasional (object-relational interface) ke database PostgreSQL. Ini adalah wrapper C++ yang aman dan mudah digunakan di atas library C bawaan PostgreSQL, yaitu `libpq`.

Instalasi pada LINUX:

```bash
sudo apt update
# sudo apt install postgresql postgresql-client libpq-dev libpqxx-dev
sudo apt install libpqxx-dev
```

### `Qt`

> Qt baca : _cute😽_

Instalasi pada LINUX:

```bash
sudo apt update
# Instal Qt base dan development tools
sudo apt install qtbase5-dev qt5-qmake cmake
# Instal Qt Creator (IDE) - sangat direkomendasikan
sudo apt install qtcreator
# Instal modul SQL untuk PostgreSQL (penting untuk database)
sudo apt install libqt5sql5-psql

sudo apt install qttools5-dev
```
