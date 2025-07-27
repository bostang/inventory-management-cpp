-- db_init.sql
-- Inisialisasi database untuk aplikasi inventaris

-- Pastikan Anda sudah berada di terminal psql
-- jalankan dengan perintah berikut:
-- psql -U postgres -f sql/db_init.sql

-- Buat database baru (misal: inventaris_db)
DROP DATABASE IF EXISTS inventaris_db;
CREATE DATABASE inventaris_db;

-- Hubungkan ke database baru
\c inventaris_db

-- Buat tabel inventaris
DROP TABLE IF EXISTS inventaris CASCADE;
CREATE TABLE inventaris (
    id VARCHAR(50) PRIMARY KEY,
    nama VARCHAR(100) NOT NULL,
    kuantitas INT NOT NULL,
    harga DECIMAL(10, 2) NOT NULL
);

-- Keluar dari psql
-- \q