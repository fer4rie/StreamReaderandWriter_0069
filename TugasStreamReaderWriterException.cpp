#include <iostream>
#include <string>
#include <array>
#include <stdexcept>
#include <fstream>
#include <vector>

using namespace std;

struct Barang {
    string nama;
    int harga;
};

class TokoElektronik {
private:
    // Enkapsulasi Array berkapasitas 3 elemen menggunakan tipe struct
    array<Barang, 3> etalase;

public:
    TokoElektronik() {
        // Mengisi data awal ke array beserta harganya
        etalase = {
            Barang{"Kamera Sony", 7500000}, 
            Barang{"Laptop Lenovo", 15000000}, 
            Barang{"Smart TV LG", 6000000}
        };
    }

    string ambilBarang(size_t nomorRak) {
        try {
            Barang b = etalase.at(nomorRak);
            // Mengembalikan string gabungan nama dan harga
            return b.nama + " (Rp" + to_string(b.harga) + ")";
        } 
        catch (const out_of_range& e) {
            string errorMsg = "Gagal Mengambil Barang : Rak nomor " + to_string(nomorRak) + " kosong atau tidak tersedia!";
            throw errorMsg; 
        }
    }
};

//class ManajemenGudang (FILE I/O CRUD)
class ManajemenGudang {
private:
    string namaFile = "gudang.txt";

public:
    // READ
    void tampilkanBarang() {
        ifstream fileIn(namaFile);
        string nama, hargaStr;
        int nomor = 1;

        cout << "\n=== DAFTAR BARANG DI GUDANG ===\n";
        if (!fileIn.is_open() || fileIn.peek() == ifstream::traits_type::eof()) {
            cout << "(Gudang masih kosong atau file belum dibuat)\n";
            return;
        }

        // Membaca dua baris sekaligus (nama dan harga)
        while (getline(fileIn, nama) && getline(fileIn, hargaStr)) {
            cout << nomor << ". " << nama << " - Rp" << hargaStr << "\n";
            nomor++;
        }
        fileIn.close();
    }

    // CREATE
    void tambahBarang() {
        ofstream fileOut(namaFile, ios::app);
        Barang bBaru;
        
        cout << "Masukkan nama barang baru: ";
        cin.ignore();
        getline(cin, bBaru.nama);
        cout << "Masukkan harga (Rp): ";
        cin >> bBaru.harga;
        
        // Disimpan dalam dua baris terpisah agar mudah dibaca tanpa library tambahan
        fileOut << bBaru.nama << "\n" << bBaru.harga << "\n";
        cout << "Barang berhasil ditambahkan ke gudang!\n";
        fileOut.close();
    }

    // UPDATE
    void updateBarang() {
        ifstream fileIn(namaFile);
        vector<Barang> daftarBarang;
        string nama, hargaStr;
        
        while (getline(fileIn, nama) && getline(fileIn, hargaStr)) {
            daftarBarang.push_back({nama, stoi(hargaStr)});
        }
        fileIn.close();

        if (daftarBarang.empty()) {
            cout << "Gudang kosong, tidak ada barang yang bisa diupdate.\n";
            return;
        }

        int indeks;
        cout << "Pilih nomor barang yang ingin diupdate: ";
        cin >> indeks;

        if (indeks > 0 && indeks <= daftarBarang.size()) {
            cout << "Masukkan nama barang pengganti: ";
            cin.ignore();
            getline(cin, daftarBarang[indeks - 1].nama);
            cout << "Masukkan harga pengganti (Rp): ";
            cin >> daftarBarang[indeks - 1].harga;
            
            ofstream fileOut(namaFile);
            for (const Barang& b : daftarBarang) {
                fileOut << b.nama << "\n" << b.harga << "\n";
            }
            fileOut.close();
            cout << "Data barang berhasil diperbarui!\n";
        } else {
            cout << "Error: Nomor barang tidak valid!\n";
        }
    }

    // DELETE
    void hapusBarang() {
        ifstream fileIn(namaFile);
        vector<Barang> daftarBarang;
        string nama, hargaStr;
        
        while (getline(fileIn, nama) && getline(fileIn, hargaStr)) {
            daftarBarang.push_back({nama, stoi(hargaStr)});
        }
        fileIn.close();

        if (daftarBarang.empty()) {
            cout << "Gudang kosong, tidak ada barang yang bisa dihapus.\n";
            return;
        }

        int indeks;
        cout << "Pilih nomor barang yang ingin dihapus: ";
        cin >> indeks;

        if (indeks > 0 && indeks <= daftarBarang.size()) {
            daftarBarang.erase(daftarBarang.begin() + indeks - 1);

            ofstream fileOut(namaFile);
            for (const Barang& b : daftarBarang) {
                fileOut << b.nama << "\n" << b.harga << "\n";
            }
            fileOut.close();
            cout << "Data barang berhasil dihapus!\n";
        } else {
            cout << "Error! Nomor barang tidak valid.\n";
        }
    }
};

int main() {
    TokoElektronik etalaseToko;
    ManajemenGudang gudang;
    int pilihan = -1;

    cout << "Sistem Digitalisasi Toko Elektronik Gibran Jaya Berhasil Dinyalakan.\n";

    // Looping Menu Utama Command Line Interface (CLI)
    while (pilihan != 0) {
        gudang.tampilkanBarang();

        cout << "\n=== MENU UTAMA GIBRAN JAYA ===\n";
        cout << "1. Tambah Barang Baru \n";
        cout << "2. Update Data Barang \n";
        cout << "3. Hapus Data Barang \n";
        cout << "4. Jalankan Simulasi Etalase \n";
        cout << "0. Keluar\n";
        cout << "Pilih menu (0-4): ";
        cin >> pilihan;

        if (pilihan == 1) {
            gudang.tambahBarang();
        } 
        else if (pilihan == 2) {
            gudang.updateBarang();
        } 
        else if (pilihan == 3) {
            gudang.hapusBarang();
        } 
        else if (pilihan == 4) {
            cout << "\n--- MEMULAI SIMULASI ETALASE ---\n";
            
            cout << "[Skenario 1] Pengambilan barang di rak indeks ke-1...\n";
            try {
                cout << "-> Berhasil mengambil: " << etalaseToko.ambilBarang(1) << "\n";
            } 
            catch (const string& pesanError) {
                cout << "-> " << pesanError << "\n";
            }

            cout << "\n[Skenario 2] Pengambilan barang di rak indeks ke-5...\n";
            try {
                cout << "-> Berhasil mengambil: " << etalaseToko.ambilBarang(5) << "\n";
            } 
            catch (const string& pesanError) {
                cout << "-> " << pesanError << "\n";
            }
            cout << "--------------------------------\n";
        } 
        else if (pilihan == 0) {
            cout << "Keluar. Terima kasih!\n";
        } 
        else {
            cout << "Pilihan tidak valid, silakan coba lagi.\n";
        }
    }

    return 0;
}