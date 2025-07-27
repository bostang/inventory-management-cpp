#include "mainwindow.h"
#include "ui_mainwindow.h" // Header yang dihasilkan dari file .ui
#include <QMessageBox>     // Untuk menampilkan pesan pop-up
#include <QDebug>          // Untuk debugging, mencetak ke konsol Qt Creator
#include <QTableWidgetItem> // Untuk item di QTableWidget

// Menggunakan namespace std agar tidak perlu menulis 'std::' berulang kali
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this); // Ini adalah baris penting yang memuat UI dari mainwindow.ui

    // --- Inisialisasi DatabaseManager ---
    // Ganti dengan string koneksi PostgreSQL kamu yang sebenarnya!
    // Contoh: "dbname=inventaris_db user=postgres password=your_password host=localhost port=5432"
    // Pastikan database 'inventaris_db' sudah dibuat dan user 'postgres' (atau user lain) memiliki akses.
    string conn_str = "dbname=inventaris_db user=postgres password=password host=localhost port=5432";
    dbManager = make_unique<DatabaseManager>(conn_str);

    if (!dbManager->connect()) {
        QMessageBox::critical(this, "Kesalahan Koneksi Database",
                              "Gagal terkoneksi ke database. Pastikan PostgreSQL berjalan dan string koneksi benar.\n"
                              "Aplikasi akan keluar.");
        qApp->quit(); // Keluar dari aplikasi jika koneksi gagal total
        return;
    }

    // --- Pengaturan QTableWidget ---
    // Atur jumlah kolom dan header
    ui->tableWidget->setColumnCount(4); // ID, Nama, Kuantitas, Harga
    QStringList headers;
    headers << "ID" << "Nama" << "Kuantitas" << "Harga";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true); // Kolom terakhir mengisi sisa ruang
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); // Pilih seluruh baris
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // Nonaktifkan edit langsung di tabel

    // --- Koneksi Signals & Slots ---
    // Menghubungkan tombol-tombol UI ke slot fungsi kita
    connect(ui->btnAddItem, &QPushButton::clicked, this, &MainWindow::handleAddItemButtonClicked);
    connect(ui->btnUpdateItem, &QPushButton::clicked, this, &MainWindow::handleUpdateItemButtonClicked);
    connect(ui->btnDeleteItem, &QPushButton::clicked, this, &MainWindow::handleDeleteItemButtonClicked);
    connect(ui->btnSearchItem, &QPushButton::clicked, this, &MainWindow::handleSearchItemButtonClicked);
    connect(ui->btnClearFields, &QPushButton::clicked, this, &MainWindow::handleClearFieldsButtonClicked);

    // Opsional: Jika ingin mengisi field saat baris di tabel diklik
    connect(ui->tableWidget, &QTableWidget::itemClicked, this, [this](QTableWidgetItem* item){
        if (item) {
            int row = item->row();
            // Ambil ID dari kolom pertama (indeks 0)
            QString id = ui->tableWidget->item(row, 0)->text();
            // Isi lineEditId dengan ID yang dipilih
            ui->lineEditId->setText(id);
            // Panggil slot pencarian untuk mengisi field lainnya
            handleSearchItemButtonClicked();
        }
    });


    // Muat data inventaris saat aplikasi dimulai
    loadItemsToTable();
}

MainWindow::~MainWindow()
{
    // unique_ptr akan otomatis memanggil destructor DatabaseManager, yang akan memutus koneksi.
    // Jadi, tidak perlu memanggil dbManager->disconnect() secara eksplisit di sini.
    delete ui;
}

// --- Implementasi Fungsi Helper ---

void MainWindow::loadItemsToTable() {
    ui->tableWidget->setRowCount(0); // Hapus semua baris yang ada
    vector<ItemInventaris> items = dbManager->getAllItemsFromDB();

    if (items.empty()) {
        qDebug() << "Tidak ada item di database.";
        return;
    }

    ui->tableWidget->setRowCount(items.size()); // Atur jumlah baris sesuai data
    for (int i = 0; i < items.size(); ++i) {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(items[i].id)));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(items[i].nama)));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(items[i].kuantitas)));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(items[i].harga, 'f', 2)));
    }
    qDebug() << "Tabel inventaris dimuat ulang. Jumlah item:" << items.size();
}

void MainWindow::clearInputFields() {
    // Blokir signals sementara untuk mencegah pemanggilan slot yang tidak diinginkan
    ui->lineEditId->blockSignals(true);
    ui->lineEditName->blockSignals(true);
    ui->lineEditQuantity->blockSignals(true);
    ui->lineEditPrice->blockSignals(true); // Pastikan ini ada
    ui->lineEditSearchId->blockSignals(true);

    ui->lineEditId->clear();
    ui->lineEditName->clear();
    ui->lineEditQuantity->clear();
    ui->lineEditPrice->clear(); // Baris ini harus ada untuk harga
    ui->lineEditSearchId->clear();

    ui->lineEditId->blockSignals(false);
    ui->lineEditName->blockSignals(false);
    ui->lineEditQuantity->blockSignals(false);
    ui->lineEditPrice->blockSignals(false);
    ui->lineEditSearchId->blockSignals(false);

    ui->lineEditId->setFocus();
}

void MainWindow::populateInputFields(const ItemInventaris& item) {
    ui->lineEditId->setText(QString::fromStdString(item.id));
    ui->lineEditName->setText(QString::fromStdString(item.nama));
    ui->lineEditQuantity->setText(QString::number(item.kuantitas));
    ui->lineEditPrice->setText(QString::number(item.harga, 'f', 2));
}


// --- Implementasi Slots untuk Interaksi Pengguna ---

void MainWindow::handleAddItemButtonClicked() {
    qDebug() << "Tombol Tambah Item diklik.";
    ItemInventaris newItem;
    // Konversi QString dari QLineEdit ke std::string atau tipe data lain
    newItem.id = ui->lineEditId->text().toStdString();
    newItem.nama = ui->lineEditName->text().toStdString();
    newItem.kuantitas = ui->lineEditQuantity->text().toInt(); // Otomatis konversi ke int
    newItem.harga = ui->lineEditPrice->text().toDouble();     // Otomatis konversi ke double

    // Validasi input dasar di GUI
    if (newItem.id.empty() || newItem.nama.empty() || newItem.kuantitas < 0 || newItem.harga < 0) {
        QMessageBox::warning(this, "Input Tidak Valid", "ID, Nama tidak boleh kosong, dan Kuantitas/Harga harus positif.");
        return;
    }

    // Coba tambahkan item ke database
    if (dbManager->addItemToDB(newItem)) {
        QMessageBox::information(this, "Berhasil", "Item berhasil ditambahkan!");
        loadItemsToTable(); // Muat ulang tabel untuk menampilkan item baru
        clearInputFields(); // Bersihkan field input
    } else {
        // Pesan error sudah ditangani di DatabaseManager, tapi bisa tambahkan detail di sini
        QMessageBox::critical(this, "Gagal", "Gagal menambahkan item. Mungkin ID sudah ada atau terjadi kesalahan database.");
    }
}

void MainWindow::handleUpdateItemButtonClicked() {
    QString idToUpdate = ui->lineEditId->text(); // ID diambil dari field ID utama
    if (idToUpdate.isEmpty()) {
        QMessageBox::warning(this, "Peringatan", "Masukkan ID item yang ingin diperbarui di field ID.");
        return;
    }

    ItemInventaris updatedItem;
    updatedItem.id = idToUpdate.toStdString();
    updatedItem.nama = ui->lineEditName->text().toStdString();
    updatedItem.kuantitas = ui->lineEditQuantity->text().toInt();
    updatedItem.harga = ui->lineEditPrice->text().toDouble();

    // Validasi input
    if (updatedItem.nama.empty() || updatedItem.kuantitas < 0 || updatedItem.harga < 0) {
        QMessageBox::warning(this, "Input Tidak Valid", "Nama tidak boleh kosong, Kuantitas/Harga harus positif.");
        return;
    }

    if (dbManager->updateItemInDB(updatedItem)) {
        QMessageBox::information(this, "Berhasil", "Item berhasil diperbarui!");
        loadItemsToTable();
        clearInputFields();
    } else {
        QMessageBox::critical(this, "Gagal", "Gagal memperbarui item. Mungkin ID tidak ditemukan atau terjadi kesalahan database.");
    }
}

void MainWindow::handleDeleteItemButtonClicked() {
    QString idToDelete = ui->lineEditId->text(); // Ambil ID dari field ID utama
    if (idToDelete.isEmpty()) {
        QMessageBox::warning(this, "Peringatan", "Masukkan ID item yang ingin dihapus di field ID.");
        return;
    }

    // Konfirmasi penghapusan
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Konfirmasi Hapus",
                                  "Anda yakin ingin menghapus item dengan ID '" + idToDelete + "'?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No) {
        return;
    }

    if (dbManager->deleteItemFromDB(idToDelete.toStdString())) {
        QMessageBox::information(this, "Berhasil", "Item berhasil dihapus!");
        loadItemsToTable();
        clearInputFields();
    } else {
        QMessageBox::critical(this, "Gagal", "Gagal menghapus item. Mungkin ID tidak ditemukan atau terjadi kesalahan database.");
    }
}

void MainWindow::handleSearchItemButtonClicked() {
    QString searchId = ui->lineEditSearchId->text(); // Ambil ID dari field pencarian
    if (searchId.isEmpty()) {
        QMessageBox::warning(this, "Peringatan", "Masukkan ID item yang ingin dicari di field pencarian.");
        return;
    }

    ItemInventaris foundItem = dbManager->getItemFromDB(searchId.toStdString());

    if (!foundItem.id.empty()) { // Jika ID tidak kosong, berarti item ditemukan
        QMessageBox::information(this, "Ditemukan", "Item ditemukan: " + QString::fromStdString(foundItem.nama));
        populateInputFields(foundItem); // Isi field input dengan data item yang ditemukan
    } else {
        QMessageBox::information(this, "Tidak Ditemukan", "Item dengan ID '" + searchId + "' tidak ditemukan.");
        clearInputFields(); // Bersihkan field jika tidak ditemukan
    }
}

void MainWindow::handleClearFieldsButtonClicked() {
    clearInputFields();
    QMessageBox::information(this, "Bersih", "Semua field input telah dibersihkan.");
}