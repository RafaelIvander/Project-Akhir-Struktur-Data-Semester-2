    #include <iostream>
    #include <string>
    using namespace std;

    struct Pelanggan {
        string nik;
        string nama;
        Pelanggan *next;
    };

    struct antreanSewa {
        string idAntrean;
        string idMobil;
        antreanSewa *next;
    };

    struct tumpukanCuci {
        string idMobil;
        tumpukanCuci *next;
    };

    struct historiSewa {
        string namaPenyewa;
        historiSewa *next;
    };

    struct Mobil {
        int idMobil;
        string merkMobil;
        string nopol;
        int harga;
        bool tersedia;
        historiSewa *headHistori;
        Mobil *left, *right;
    };

    struct Sorting {
        int id;
        int harga;
        string merk;
        string noPolisi;
    };

    struct Rute {
        string kotaTujuan;
        int jarakTempuh;
        Rute *next;
    };

    struct Cabang {
        string namaKotaCabang;
        Rute *headRute;
    };

    antreanSewa *front = NULL;
    antreanSewa *rear = NULL;

    tumpukanCuci *top = NULL;

    Mobil *root = NULL;

    Sorting harga[100];
    int indeksHarga = 0;

    const int ukuranTabel = 10;
    Pelanggan *tabelPelanggan[ukuranTabel];
    void inisialisasiTabel() {
        for(int i = 0; i < ukuranTabel; i++) {
            tabelPelanggan[i] = NULL;
        }
    }

    const int jumlahCabang = 4;
    Cabang petaRental[jumlahCabang];

    void tambahAntrean(string idAntri, string idM) {
        antreanSewa *baru = new antreanSewa;
        baru->idAntrean = idAntri;
        baru->idMobil = idM;
        baru->next = NULL;

        if(front == NULL) {
            front = baru;
            rear = baru;
        } else {
            rear->next = baru;
            rear = baru;
        }
    }

    void lihatAntrean() {
        antreanSewa *bantu = front;
        if(bantu == NULL) {
            cout << "Antrean Masih Kosong\n";
        } else {
            while(bantu != NULL) {
                cout << "Nomor Antrian  : " << bantu->idAntrean << '\n';
                cout << "ID Mobil       : " << bantu->idMobil << '\n';
                bantu = bantu->next;
            }
        }
    }

    void panggilAntrean() {
        if(front == NULL) {
            cout << "Data Kosong\n";
        } else {
            antreanSewa *hapus = front;
            front = front->next;
            delete hapus;

            if(front == NULL) {
                rear = NULL;
            }
        }
    }

    void pushCuciMobil (string idM) {
        tumpukanCuci *baru = new tumpukanCuci;
        baru->idMobil = idM;
        baru->next = top;
        top = baru;
    }

    void lihatTumpukan() {
        tumpukanCuci *bantu = top;
        if(top == NULL) {
            cout << "Belum ada mobil yang dikembalikan\n";
        } else {
            while (bantu != NULL) {
                cout << "ID Mobil   : " << bantu->idMobil << '\n';
                bantu = bantu->next;
            }
        }
    }

    void popCuciMobil() {
        if(top == NULL) {
            cout << "Antrean kosong, tidak ada yang bisa dicuci\n";
        } else {
            tumpukanCuci *hapus = top;
            top = top->next;
            delete hapus;
        }
    }

    Mobil *tambahMobil(Mobil *root, int id, string np, string merk, int hrg) {
        if(root == NULL) {
            Mobil *baru = new Mobil;
            baru->idMobil = id;
            baru->nopol = np;
            baru->merkMobil = merk;
            baru->harga = hrg;
            baru->tersedia = true;
            baru->headHistori = NULL;
            baru->left = NULL;
            baru->right = NULL;
            return baru;
        }

        if(id < root->idMobil) {
            root->left = tambahMobil(root->left, id, np, merk, hrg);
        } else if (id > root->idMobil) {
            root->right = tambahMobil(root->right, id, np, merk, hrg);
        } else {
            cout << "ID duplikat!\n";
        }

        return root;
    }

    void urutkanHarga(Mobil *root) {
        if(root != NULL) {
            urutkanHarga(root->left);
            harga[indeksHarga].id = root->idMobil;
            harga[indeksHarga].harga = root->harga;
            harga[indeksHarga].merk = root->merkMobil;
            harga[indeksHarga].noPolisi = root->nopol;
            indeksHarga++;
            urutkanHarga(root->right);
        }
    }

    void tampilMobil(Mobil *root) {
        indeksHarga = 0;
        urutkanHarga(root);

        for(int i = 0; i < indeksHarga - 1; i++) {
            for(int j = 0; j < indeksHarga - i - 1; j++) {
                if(harga[j].harga > harga[j+1].harga) {
                    Sorting bantu = harga[j];
                    harga[j] = harga[j+1];
                    harga[j+1] = bantu;
                }
            }
        }
        for(int i = 0; i < indeksHarga; i++) {
            cout << harga[i].merk << " | " << harga[i].noPolisi << " | " << harga[i].harga << '\n';
        }
    }

    Mobil *cariMobil(Mobil *root, int id) {
        if(root == NULL) {
            cout << "Mobil yang dicari tidak ada!\n";
            return NULL;
        }

        if(id == root->idMobil) {
            cout << "Mobil ditemukan: " << root->idMobil << " | " << root->merkMobil << " | " << root->nopol << " | " << root->harga << '\n';
            return root;
        } else if(id < root->idMobil) {
            return cariMobil(root->left, id); 
        } else {
            return cariMobil(root->right, id);
        }
    }

    int hashFunction(string nik) {
        int total = 0;
        for(char c : nik) {
            total += c;
        }
        return total % ukuranTabel;
    }

    void tambahPelanggan(string nik, string nama) {
        int index = hashFunction(nik);

        Pelanggan *baru = new Pelanggan;
        baru->nik = nik;    
        baru->nama = nama;
        baru->next = NULL;

        if(tabelPelanggan[index] == NULL) {
            tabelPelanggan[index] = baru;
        } else {
            Pelanggan *bantu = tabelPelanggan[index];
            while(bantu->next != NULL) {
                bantu = bantu->next;
            }
            bantu->next = baru;
        }
    }

    void tampilPelanggan() {
        cout << "--- Data Pelanggan ---\n";
        cout << "NIK     | Nama Pelanggan\n";
        for(int i = 0; i < ukuranTabel; i++) {
            Pelanggan *bantu = tabelPelanggan[i];
            while(bantu != NULL) {
                cout << bantu->nik << " | " << bantu->nama << '\n';
                bantu = bantu->next;
            }
        }
    }

    void cariPelanggan(string nik) {
        int index = hashFunction(nik);
        Pelanggan *bantu = tabelPelanggan[index];
        bool ketemu = false;

        while(bantu != NULL) {
            if(bantu->nik == nik) {
                ketemu = true;
                break;
            }
            bantu = bantu->next;
        }

        if(ketemu == true) {
            cout << "Data ditemukan!\n";
            cout << "Nama : " << bantu->nama << '\n';
            cout << "NIK  : " << bantu->nik << '\n';
        } else {
            cout << "Data Pelanggan Tidak ditemukan!\n";
        }
    }

    void tambahHistori(Mobil *target, string nama) {
        if(target == NULL) return;
        historiSewa *baru = new historiSewa;
        baru->namaPenyewa = nama;
        baru->next = target->headHistori;
        target->headHistori = baru;
    }

    void tampilHistori(Mobil *target) {
        if(target == NULL) {
            return;
        }

        if(target->headHistori == NULL) {
            cout << "Mobil Ini Belum Pernah disewa Oleh Siapapun!\n";
        } else {
            cout << "--- Riwayat Penyewa Mobil " << target->merkMobil << " ---\n";
            historiSewa *bantu = target->headHistori;
            int no = 1;
            while(bantu != NULL) {
                cout << no << ". " << bantu->namaPenyewa << '\n';
                bantu = bantu->next;
                no++;
            }
        }
    }

    void inisialisasiPeta(){
        petaRental[0].namaKotaCabang = "Surabaya";
        petaRental[1].namaKotaCabang = "Sidoarjo";
        petaRental[2].namaKotaCabang = "Gresik";
        petaRental[3].namaKotaCabang = "Malang";

        for(int i = 0; i < jumlahCabang; i++) {
            petaRental[i].headRute = NULL;
        }

        Rute *r1 = new Rute{"Sidoarjo", 14, NULL};
        Rute *r2 = new Rute{"Gresik", 20, NULL};
        r1->next = r2;
        petaRental[0].headRute = r1;

        Rute *r3 = new Rute{"Surabaya", 14, NULL};
        Rute *r4 = new Rute{"Malang", 60, NULL};
        r3->next = r4;
        petaRental[1].headRute = r3;

        Rute *r5 = new Rute{"Surabaya", 20, NULL};
        petaRental[2].headRute = r5;

        Rute *r6 = new Rute{"Sidoarjo", 60, NULL};
        petaRental[3].headRute = r6;
    }

    void tampilPeta() {
        cout << "--- Peta Jaringan Rental Mobil ---\n";
        
        for(int i = 0; i < jumlahCabang; i++) {
            cout << "Jarak Cabang [" << petaRental[i].namaKotaCabang << "] ke : \n";
            Rute *bantu = petaRental[i].headRute;

            if(bantu == NULL) {
                cout << "Belum Memiliki Cabang Lain\n";
            } else {
                while(bantu != NULL) {
                    cout << "Cabang " << bantu->kotaTujuan << " (" << bantu->jarakTempuh << " KM)\n";
                    bantu = bantu->next;
                }
            }
            cout << "-----------------------------------\n";
        }
    }

    int main(){
        inisialisasiTabel();
        inisialisasiPeta();
        int pilihan;
        do {
            cout << "=============================\n";
            cout << "     RENTAL MOBIL RAFAEL\n";
            cout << "=============================\n";
            cout << "1. Tambah/Lihat Armada Mobil\n";
            cout << "2. Data Pelanggan\n";
            cout << "3. Transaksi Sewa Mobil\n";
            cout << "4. Pengembalian Mobil\n";
            cout << "5. Peta Jaringan Rental\n";
            cout << "0. Keluar\n";
            cout << "Masukkan Pilihan [0-5] : ";
            cin >> pilihan;

            switch(pilihan) {
                case 1 : {
                    int pilihanMobil;
                    do {
                        cout << "--- Tambah/Lihat Armada Mobil ---\n";
                        cout << "1. Tambah Armada Mobil\n";
                        cout << "2. Lihat Armada Mobil\n";
                        cout << "3. Lihat Riwayat Sewa Mobil\n";
                        cout << "0. Keluar\n";
                        cout << "Masukkan Pilihan [0-3] : "; cin >> pilihanMobil;

                        switch(pilihanMobil) {
                            case 1 : {
                                int id, hrg;
                                string np, merk;
                                cout << "--- Tambah Armada Mobil ---\n";
                                cout << "Masukkan ID Mobil      : "; cin >> id;
                                cout << "Masukkan Merk Mobil    : "; cin.ignore(); getline(cin, merk);
                                cout << "Masukkan No. Polisi    : "; getline(cin, np);
                                cout << "Masukkan Harga Sewa    : "; cin >> hrg;
                                root = tambahMobil(root, id, np, merk, hrg);
                                cout << "Mobil berhasil ditambahkan!\n";
                                break;
                            }
                            case 2 : {
                                cout << "--- Lihat Armada Mobil ---\n";
                                cout << " Merk  | No. Polisi | Harga Sewa\n";
                                cout << "--------------------------------------\n";
                                tampilMobil(root);
                                break;
                            }
                            case 3: {
                                int id;
                                cout << "--- Lihat Riwayat Sewa Mobil ---\n";
                                cout << "Masukkan ID : "; cin >> id;
                                Mobil *target = cariMobil(root, id);
                                if(target != NULL) {
                                    tampilHistori(target);
                                }
                                break;
                            }
                        }
                    } while(pilihanMobil != 0);
                    break;
                }
                case 2: {
                    int pilihanPelanggan;
                    do {
                        cout << "--- Data Pelanggan ---\n";
                        cout << "1. Tambah Pelanggan\n";
                        cout << "2. Lihat Semua Pelanggan\n";
                        cout << "3. Cari Pelanggan(NIK)\n";
                        cout << "0. Keluar\n";
                        cout << "Masukkan Pilihan [0-3] : "; cin >> pilihanPelanggan; cin.ignore(100, '\n');

                        switch(pilihanPelanggan) {
                            case 1: {
                                string nik, nama;
                                cout << "--- Tambah Pelanggan ---\n";
                                cout << "Masukkan NIK   : "; cin >> nik; cin.ignore();
                                cout << "Masukkan Nama  : "; getline(cin, nama);
                                tambahPelanggan(nik, nama);
                                cout << "Data Pelanggan Telah Disimpan\n";
                                break;
                            }
                            case 2: tampilPelanggan();
                            break;
                            case 3: {
                                string nikCari;
                                cout << "--- Cari Pelanggan(NIK) ---\n";
                                cout << "Masukkan NIK Pelanggan : "; cin >> nikCari;
                                cariPelanggan(nikCari);
                                break;
                            }
                        }
                    } while(pilihanPelanggan != 0);
                    break;
                }
                case 3: {
                    int pilihanSewa;
                    do{
                        string idAntri, idM;
                        cout << "--- Transaksi Sewa Mobil ---\n";
                        cout << "1. Tambah Antrean\n";
                        cout << "2. Lihat Antrean\n";
                        cout << "3. Panggil Antrean\n";
                        cout << "0. Keluar\n";
                        cout << "Masukkan Pilihan [0-3] : ";
                        cin >> pilihanSewa;
                        cin.ignore(100, '\n');

                        switch(pilihanSewa) {
                            case 1: {
                                cout << "--- Tambah Antrean ---\n";
                                cout << "Masukkan Nomor Antrean : "; cin >> idAntri;
                                cout << "Masukkan ID Mobil      : "; cin >> idM;
                                tambahAntrean(idAntri, idM);
                                break;
                            }
                            case 2: lihatAntrean();
                            break;
                            case 3: {
                                if(front == NULL) {
                                    cout << "Antrean Kosong!\n";
                                } else {
                                    int idCari = stoi(front->idMobil);
                                    Mobil *target = cariMobil(root, idCari);
                                    if(target != NULL) {
                                        if(target->tersedia == true) {
                                            string namaPenyewa;
                                            cout << "Masukkan Nama Penyewa  : "; getline(cin, namaPenyewa);
                                            tambahHistori(target, namaPenyewa);
                                            target->tersedia = false;
                                            cout << "Transaksi Sewa Berhasil\n";
                                            panggilAntrean();
                                        } else {
                                            cout << "Maaf!, mobil sedang tidak tersedia\n";
                                        }
                                    } else {
                                        cout << "Data Kosong";
                                        panggilAntrean();
                                    }
                                }
                                break;
                            }
                        }
                    } while(pilihanSewa != 0);
                    break;
                }
                case 4: {
                    int pilihanKembali;
                    do {
                        cout << "--- Pengembalian Mobil ---\n";
                        cout << "1. Konfirmasi Pengembalian Mobil\n";
                        cout << "2. Lihat Antrean Cuci\n";
                        cout << "3. Ambil Mobil untuk dicuci\n";
                        cout << "0. Keluar\n";
                        cout << "Masukkan Pilihan [0-3]: "; cin >> pilihanKembali;

                        switch(pilihanKembali) {
                            case 1 : {
                                int idM;
                                cout << "--- Konfirmasi Pengembalian Mobil ---\n";
                                cout << "Masukkan ID Mobil yang dikembalikan : "; cin >> idM;
                                Mobil *target = cariMobil(root, idM);
                                if(target != NULL && target->tersedia == false) {
                                    pushCuciMobil(to_string(idM));
                                    cout << "Mobil " << target->merkMobil << " diterima. Silahkan proses untuk pencucian\n";
                                } else {
                                    cout << "Mobil tidak ditemukan atau sudah digarasi\n";
                                }
                                break;
                            }
                            case 2: {
                                lihatTumpukan();
                                break;
                            }
                            case 3: {
                                if(top != NULL) {
                                    int id = stoi(top->idMobil);
                                    Mobil *target = cariMobil(root, id);

                                    if(target != NULL) {
                                        target->tersedia = true;
                                        cout << "Mobil dengan Merk " << target->merkMobil << " telah Selesai dicuci dan siap disewa kembali.\n";
                                        popCuciMobil();
                                    }
                                }
                                break;
                            }
                        }
                    } while (pilihanKembali != 0);
                    break;
                }
                case 5: tampilPeta();
                break;
            }
        } while(pilihan != 0);
    }