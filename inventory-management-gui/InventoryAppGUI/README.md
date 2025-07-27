# InventoryAppGUI

## Cara Menjalankan

## Cara Pembuatan

```bash
# langkah 0 : instalasi library
# pada linux:
sudo apt update
# Instal Qt base dan development tools
sudo apt install qtbase5-dev qt5-qmake cmake
# Instal Qt Creator (IDE) - sangat direkomendasikan
sudo apt install qtcreator
# Instal modul SQL untuk PostgreSQL (penting untuk database)
sudo apt install libqt5sql5-psql
# Instal modul dev
sudo apt install qttools5-dev

# modifikasi CMakeLists.txt

# build
cmake . && make

# modifikasi main.cpp dan mainwindow.cpp

# modifikasi mainwindow.ui di Qt Designer

cmake . && make


# jalankan aplikasi
./InventoryAppGUI
```
