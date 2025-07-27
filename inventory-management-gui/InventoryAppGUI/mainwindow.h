#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory> // Untuk std::unique_ptr

// Sertakan header untuk kelas-kelas backend kita
#include "database/DatabaseManager.h"
#include "inventory/ItemInventaris.h" // Diperlukan untuk struct ItemInventaris

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT // Makro ini diperlukan untuk fitur signals & slots Qt

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Slots untuk merespons klik tombol
    // void on_btnAddItem_clicked();
    // void on_btnUpdateItem_clicked();
    // void on_btnDeleteItem_clicked();
    // void on_btnSearchItem_clicked();
    // void on_btnClearFields_clicked(); // Slot tambahan untuk membersihkan input

    void handleAddItemButtonClicked(); // Ganti nama
    void handleUpdateItemButtonClicked();
    void handleDeleteItemButtonClicked();
    void handleSearchItemButtonClicked();
    void handleClearFieldsButtonClicked();
    // Slot untuk merespons perubahan teks di lineEditId (misal untuk fitur cari/update otomatis)
    // void on_lineEditId_textChanged(const QString &arg1); // Opsional, jika ingin fitur auto-fill

private:
    Ui::MainWindow *ui;
    // Gunakan unique_ptr untuk DatabaseManager agar lifetime-nya terkelola otomatis
    std::unique_ptr<DatabaseManager> dbManager;

    // Fungsi helper untuk memuat data dari DB ke QTableWidget
    void loadItemsToTable();
    // Fungsi helper untuk mengosongkan semua field input
    void clearInputFields();
    // Fungsi helper untuk mengisi field input dari objek ItemInventaris
    void populateInputFields(const ItemInventaris& item);
};

#endif // MAINWINDOW_H