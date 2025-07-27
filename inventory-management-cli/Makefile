# Kompiler C++ yang digunakan
CXX = g++

# Direktori
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Opsi kompilasi:
# -c: Kompilasi file .cpp ke file objek (.o) tanpa linking
# -Wall: Mengaktifkan semua peringatan (sangat direkomendasikan!)
# -std=c++17: Menggunakan standar C++17 (libpqxx seringkali memerlukan C++14 atau lebih baru)
# -I$(SRC_DIR): Menambahkan direktori src/ ke jalur pencarian include header
# -I$(SRC_DIR)/inventory: Menambahkan src/inventory ke jalur pencarian include header
# -I$(SRC_DIR)/utils: Menambahkan src/utils ke jalur pencarian include header
# -I$(SRC_DIR)/database: Menambahkan src/database ke jalur pencarian include header
# -I/usr/include/postgresql/pqxx: Contoh jalur include spesifik libpqxx di Linux (sesuaikan!)
# -I/usr/local/include: Contoh jalur include standar untuk Homebrew di macOS (sesuaikan!)
CXXFLAGS = -c -Wall -std=c++17 -I$(SRC_DIR) -I$(SRC_DIR)/inventory -I$(SRC_DIR)/utils -I$(SRC_DIR)/database -I/usr/include/postgresql/pqxx

# Opsi linker:
# -L/usr/lib/x86_64-linux-gnu: Contoh jalur library di Linux (sesuaikan!)
# -L/usr/local/lib: Contoh jalur library standar untuk Homebrew di macOS (sesuaikan!)
# -lpqxx: Link dengan library libpqxx
# -lpq: Link dengan library libpq (dependencies libpqxx)
LDFLAGS = -L/usr/lib/x86_64-linux-gnu -lpqxx -lpq

# Nama file executable yang akan dihasilkan
TARGET = inventaris_app

# Daftar semua file objek yang akan dibuat
OBJECTS = $(OBJ_DIR)/main.o \
          $(OBJ_DIR)/inventory/ItemInventaris.o \
          $(OBJ_DIR)/utils/Utils.o \
          $(OBJ_DIR)/database/DatabaseManager.o # Tambahkan DatabaseManager.o

# Aturan utama (default): membangun TARGET
.PHONY: all
all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D) # Pastikan direktori bin ada
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "Program $(TARGET) berhasil dikompilasi ke $(BIN_DIR)/!"

# Aturan untuk membuat file objek dari masing-masing file .cpp
# Pastikan direktori obj/subfolder ada sebelum membuat .o
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/inventory/ItemInventaris.h $(SRC_DIR)/utils/Utils.h $(SRC_DIR)/database/DatabaseManager.h
	@mkdir -p $(@D) # Pastikan direktori obj/ ada
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ_DIR)/inventory/ItemInventaris.o: $(SRC_DIR)/inventory/ItemInventaris.cpp $(SRC_DIR)/inventory/ItemInventaris.h $(SRC_DIR)/utils/Utils.h
	@mkdir -p $(@D) # Pastikan direktori obj/inventory ada
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ_DIR)/utils/Utils.o: $(SRC_DIR)/utils/Utils.cpp $(SRC_DIR)/utils/Utils.h
	@mkdir -p $(@D) # Pastikan direktori obj/utils ada
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ_DIR)/database/DatabaseManager.o: $(SRC_DIR)/database/DatabaseManager.cpp $(SRC_DIR)/database/DatabaseManager.h $(SRC_DIR)/inventory/ItemInventaris.h
	@mkdir -p $(@D) # Pastikan direktori obj/database ada
	$(CXX) $(CXXFLAGS) $< -o $@

# Aturan 'clean': menghapus semua file objek dan executable
.PHONY: clean
clean:
	@echo "Membersihkan file objek dan executable..."
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Pembersihan selesai."