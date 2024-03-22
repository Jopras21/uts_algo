#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <dirent.h>

struct playlist {
    char judul[50];
    char penyanyi[50];
    char album[50];
    int tahun;
    struct playlist *next, *prev;
};

struct akun {
    char username[30];
    char password[30];
    struct akun *next;
};

bool cekAkun(struct akun *head, const char *username, const char *password);

login(int kondisi, char username[30],char password[30]) {
    struct akun *akunHead = NULL;
    bacaDatabase(&akunHead);
    printf("1. Belum punya akun? buat sekarang!\n");
    printf("2. Sudah punya akun? login sekarang!\n");
    printf("pilihan :"); scanf("%d", &kondisi);

if (kondisi == 1) {
    printf("================Daftar==============\n");
    printf("Username : ");
    scanf("%s", username);
    printf("\nPassword : ");
    scanf("%s", password);
    addAcc(&akunHead, username, password);
} else if (kondisi == 2) {
    printf("================Login==============\n");
    printf("Username : ");
    scanf("%s", username);
    printf("\nPassword : ");
    scanf("%s", password);
    if (!cekAkun(akunHead, username, password)) {
        printf("Username atau password salah.\n");
        return main();
    }
}
}

void buatData(FILE *file, char baris[1000]) {
    char karakter;
    while (fgets(baris, sizeof(baris), file) != NULL) {
        printf("%s", baris);
    }
}

void buatPlaylistKustom() {
    char namaPlaylist[50];
    printf("Masukkan nama playlist baru: ");
    scanf("%s", namaPlaylist);

    FILE *file = fopen(strcat(namaPlaylist,""), "a");
    if (file == NULL) {
        printf("Gagal membuat playlist.\n");
        return;
    }

    printf("Playlist '%s' berhasil dibuat.\n", namaPlaylist);
    fclose(file);
}

void buatPlayList() {
    int panjangLagu = 100;
    char panjangJudul = 50;
    char panjangPenyanyi = 50;
    char panjangAlbum = 50;
    int jumlahLagu;
    FILE *file;

    struct playlist playlist[panjangLagu];

    printf("1. Buat playlist baru\n");
    printf("2. Tambahkan lagu ke playlist yang ada\n");
    printf("Pilihan: ");
    int pilihan;
    scanf("%d", &pilihan);

    if (pilihan == 1) {
        buatPlaylistKustom();
    }

    printf("Masukkan jumlah lagu yang ingin ditambahkan ke playlist: ");
    scanf("%d", &jumlahLagu);
    getchar();

    file = fopen("playlist1.txt", "a");

    if (file == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    for (int i = 0; i < jumlahLagu; i++) {
        printf("Masukkan judul lagu ke-%d: ", i + 1);
        fgets(playlist[i].judul, panjangJudul, stdin);
        strtok(playlist[i].judul, "\n");

        printf("Masukkan nama penyanyi untuk lagu ke-%d: ", i + 1);
        fgets(playlist[i].penyanyi, panjangPenyanyi, stdin);
        strtok(playlist[i].penyanyi, "\n");

        printf("Masukkan judul untuk album lagu ke-%d: ", i + 1);
        fgets(playlist[i].album, panjangAlbum, stdin);
        strtok(playlist[i].album, "\n");

        printf("Masukkan tahun rilis untuk lagu ke-%d: ", i + 1);
        scanf("%d", &playlist[i].tahun);
        getchar();

        fprintf(file, "%s#%s#%s#%d\n", playlist[i].judul, playlist[i].penyanyi, playlist[i].album, playlist[i].tahun);
    }

    printf("Playlist berhasil ditambahkan.\n");

    fclose(file);
}

void cariLagu(struct playlist *head, char kataKunci[50]) {
            printf("Masukkan kata kunci untuk mencari: ");
            scanf("%s", kataKunci);

            bool found = false;
            struct playlist *current = head;

            printf("=========================================\n");
            printf("      Hasil Pencarian untuk \"%s\"       \n", kataKunci);
            printf("=========================================\n");

            while (current != NULL) {
                if (strstr(current->judul, kataKunci) != NULL) {
                    printf("Judul: %s\n", current->judul);
                    printf("Penyanyi: %s\n", current->penyanyi);
                    printf("Album: %s\n", current->album);
                    printf("Tahun: %d\n", current->tahun);
                    printf("-----------------------------------------\n");
                    found = true;
                }
                current = current->next;
            }
            if (!found) {
                printf("Tidak ada lagu dengan kata kunci \"%s\" dalam judul.\n", kataKunci);
            }
}

void bacaDatabase(struct akun **head) {
    FILE *file = fopen("zdatabase.txt", "r");
    if (file == NULL) {
        printf("Error membuka file database.\n");
        return;
    }

    while (!feof(file)) {
        struct akun node = (struct akun)malloc(sizeof(struct akun));
        if (fscanf(file, "%s %s\n", node->username, node->password) != 2) {
            free(node);
            break;
        }

        node->next = *head;
        *head = node;
    }
    fclose(file);
}

void tambahAkun(struct akun **head, const char *username, const char *password) {
    struct akun node = (struct akun)malloc(sizeof(struct akun));
    strcpy(node->username, username);
    strcpy(node->password, password);
    node->next = *head;
    *head = node;

    FILE *file = fopen("zdatabase.txt", "a");
    if (file == NULL) {
        printf("Error membuka file database.\n");
        return;
    }
    fprintf(file, "%s %s\n", username, password);
    fclose(file);
}

bool cekAkun(struct akun *head, const char *username, const char *password) {
    struct akun *current = head;
    while (current != NULL) {
        if (strcmp(current->username, username) == 0 && strcmp(current->password, password) == 0) {
            return true;
        }
        current = current->next;
    }
    return false;
}

int akhiranTxt(const char *str) {
    int len = strlen(str);
    return len >= 4 && strcmp(str + len - 4, ".txt") == 0;
}

void tampilkanPlaylistTersedia() {
    char daftarPlaylist[100][50];
    int hitung = 0;

    system("dir /b > files.txt");
    FILE *file = fopen("files.txt", "r");
    if (file != NULL) {
        printf("Daftar Playlist:\n");

        char baris[256];
        while (fgets(baris, sizeof(baris), file)) {
            strtok(baris, "\n");

            if (akhiranTxt(baris) && strcmp(baris, "zdatabase.txt") != 0 && strcmp(baris, "zlogo.txt") != 0 && strcmp(baris, "files.txt") != 0) {
                strncpy(daftarPlaylist[hitung], line, strlen(line) - 4);
                daftarPlaylist[hitung][strlen(baris) - 4] = '\0';
                hitung++;
            }
        }
        fclose(file);
        remove("files.txt");

        for (int i = 0; i < hitung; i++) {
            printf("%d. %s\n", i + 1, daftarPlaylist[i]);
        }
    } else {
        perror("Direktori tidak tersedia");
    }
}

void hapusPlaylistDariFile(const char *namaFile) {
    if (strcmp(namaFile, "zdatabase.txt") == 0 || strcmp(namaFile, "zlogo.txt") == 0) {
        printf("Tidak ada playlist dengan nama '%s' .\n", namaFile);
    } else {
        if (remove(namaFile) == 0) {
            printf("Playlist '%s' berhasil dihapus.\n", namaFile);
        } else {
            printf("Gagal menghapus playlist.\n");
        }
    }
}

void tambahLaguKePlaylist(const char *namaPlaylist) {
    FILE *file = fopen(namaPlaylist, "a");
    if (file == NULL) {
        printf("Gagal membuka playlist.\n");
        return;
    }

    struct lagu laguBaru;

    printf("Masukkan judul lagu: ");
    fgets(laguBaru.judul, sizeof(laguBaru.judul), stdin);
    strtok(laguBaru.judul, "\n");

    printf("Masukkan nama penyanyi: ");
    fgets(laguBaru.penyanyi, sizeof(laguBaru.penyanyi), stdin);
    strtok(laguBaru.penyanyi, "\n");

    printf("Masukkan album: ");
    fgets(laguBaru.album, sizeof(laguBaru.album), stdin);
    strtok(laguBaru.album, "\n");

    printf("Masukkan tahun rilis: ");
    scanf("%d", &laguBaru.tahun);
    getchar();

    fprintf(file,"%s#%s#%s#%d\n", laguBaru.judul, laguBaru.penyanyi, laguBaru.album, laguBaru.tahun);

    fclose(file);

    printf("Lagu berhasil ditambahkan ke playlist.\n");

}

void hapusLaguDariPlaylist(const char *namaPlaylist, int nomorLagu) {
    FILE *file = fopen(namaPlaylist, "r");
    if (file == NULL) {
        printf("Gagal membuka playlist.\n");
        return;
    }

    char namaFileSementara[] = "temp.txt";
    FILE *fileSementara = fopen(namaFileSementara, "w");
    if (fileSementara == NULL) {
        fclose(file);
        printf("Gagal membuat file sementara.\n");
        return;
    }

    char buffer[1000];
    int hitung = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        hitung++;
        if (hitung != nomorLagu) {
            fputs(buffer, fileSementara);
        }
    }

    fclose(file);
    fclose(fileSementara);

    if (remove(namaPlaylist) == 0) {
        if (rename(namaFileSementara, namaPlaylist) == 0) {
            printf("Lagu berhasil dihapus dari playlist.\n");
        } else {
            printf("Gagal mengubah nama file.\n");
        }
    } else {
        printf("Gagal menghapus playlist.\n");
    }
}

void tampilPlaylist(const char *namaPlaylist) {
    FILE *file = fopen(namaPlaylist, "r");
    if (file != NULL) {
        char buffer[100];
        int no = 0;
        printf("===============================================================================================================================\n");
        printf("| %-2s | %-30s | %-30s | %-43s | %-7s |\n", "No", "Judul", "Penyanyi", "Album", "Tahun");
        printf("===============================================================================================================================\n");
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            if (strstr(buffer, "zdatabase.txt") != NULL || strstr(buffer, "zlogo.txt") != NULL) {
                continue;
            }
            char *judul = strtok(buffer, "#");
            char *penyanyi = strtok(NULL, "#");
            char *album = strtok(NULL, "#");
            char *tahun_str = strtok(NULL, "()");
            int tahun = atoi(tahun_str);
            printf("| %-2d | %-30s | %-30s | %-43s | %-7d |\n", no,  judul, penyanyi, album, tahun);
            no++;
        }
        printf("===============================================================================================================================\n");
        fclose(file);
    } else {
        printf("Gagal membuka playlist '%s'.\n", namaPlaylist);
        return;
    }
}

void playlist(int pilihHome, int *pilihPlaylist, struct playlist *head, struct playlist *tail) {
    char namaPlaylist[50][50];
    if (pilihHome == 2) {
        system("cls");
        printf("--------------------Pilih Playlist---------------------\n");
        tampilkanPlaylistYangAda();
        printf("0. Hapus Playlist \n");
        printf("Pilihan : ");
        scanf("%d", pilihPlaylist);

        if (*pilihPlaylist == 0) {
            char namaFile[50];
            printf("Masukkan nama file playlist yang ingin dihapus: ");
            scanf("%49s", namaFile);
            hapusPlaylistDariFile(namaFile);
        } else {
            DIR *dir;
            struct dirent *direktori;
            int hitung = 0;
            if ((dir = opendir(".")) != NULL) {
                while ((direktori = readdir(dir)) != NULL) {
                    if (strstr(direktori->d_name, ".txt") != NULL) {
                        strcpy(playlist[hitung], direktori->d_name);
                        hitung++;
                    }
                }
                closedir(dir);
            } else {
                perror("Direktori tidak tersedia");
                return;
            }

            if (*pilihPlaylist > 0 && *pilihPlaylist <= hitung) {
                tampilPlaylist(playlist[*pilihPlaylist - 1]);
            } else {
                printf("Pilihan tidak valid.\n");
                return;
            }
        }

        int opsiPlaylist;
        printf("Pilih opsi:\n");
        printf("1. Putar Lagu\n");
        printf("2. Tambahkan Lagu ke Playlist\n");
        printf("3. Hapus Lagu dari Playlist\n");
        printf("4. Kembali ke Home\n");
        printf("Pilihan: ");
        scanf("%d", &opsiPlaylist);

        switch (opsiPlaylist) {
            case 1: {
                int nomorLagu;
                printf("Masukkan nomor lagu yang ingin diputar: ");
                scanf("%d", &nomorLagu);

                FILE *file = fopen(playlist[*pilihPlaylist - 1], "r");
                if (file != NULL) {
                    char buffer[1000];
                    int hitung = 0;
                    struct playlist *current = head;
                    while (current != NULL) {
                        hitung++;
                        if (hitung == nomorLagu) {
                            printf("\nSedang Diputar: %s\n", current->judul);
                            printf("Penyanyi: %s\n", current->penyanyi);
                            printf("Album: %s\n", current->album);
                            mainkanLagu(current);
                            sleep(1);
                            break;
                        }
                        current = current->next;
                    }
                        while (current != NULL) {
                            printf("\n[Menu Player]\n");
                            printf("1. Lagu berikutnya\n");
                            printf("2. Lagu sebelumnya\n");
                            printf("3. Berhenti\n");
                            printf("4. Acak\n");
                            printf("Pilihan: ");
                            int kontrol;
                            scanf("%d", &kontrol);

                            if (kontrol == 1) {
                                current = current->next;
                                if (current == NULL) {
                                    current = head;
                                }
                                mainkanLagu(current);
                                sleep(1);
                            } else if (kontrol == 2) {
                                current = current->prev;
                                if (current == NULL) {
                                    current = tail;
                                }
                                mainkanLagu(current);
                                sleep(1);
                            } else if (kontrol == 3) {
                                printf("Pemutaran lagu dihentikan.\n");
                                break;
                            } else if (kontrol == 4) {
                                // Shuffle playlist
                                // Anda bisa implementasikan fungsionalitas pengocokan di sini
                                // atau panggil fungsi untuk menangani pengocokan
                                // Untuk sekarang, mari hanya mencetak pesan
                                printf("Mengocok playlist...\n");
                            } else {
                                printf("Pilihan tidak valid.\n");
                            }
                        }
                        fclose(file);
                } else {
                    printf("Gagal membuka playlist.\n");
                }
                break;
            }

            case 2: {
                tambahLaguKePlaylist(playlist[*pilihPlaylist - 1]);
                break;
            }

            case 3: {
                int nomorLagu;
                printf("Masukkan nomor lagu yang ingin dihapus dari playlist: ");
                scanf("%d", &nomorLagu);
                hapusLaguDariPlaylist(playlist[*pilihPlaylist - 1], nomorLagu);
                break;
            }

            case 4: {
                break;
            }

            default: {
                printf("Pilihan tidak valid.\n");
                break;
            }
        }
    } else {
        printf("Pilihan tidak valid.\n");
    }
}

void mainkanLagu(struct playlist *lagu) {
    printf("\nSedang Diputar: %s\n", lagu->judul);
    printf("Penyanyi: %s\n", lagu->penyanyi);
    printf("Album: %s\n", lagu->album);
}

void mainkanLaguDariPlaylist(char *namaPlaylist, int nomorLagu) {
    FILE *file = fopen(namaPlaylist, "r");
    if (file != NULL) {
        char buffer[1000];
        int hitung = 0;
        while (fgets(buffer, sizeof(buffer), file)) {
            hitung++;
            if (hitung == nomorLagu) {
                printf("\nLagu Sedang Diputar:\n");
                printf("%s", buffer);
                // Tambahkan fungsionalitas pemutaran di sini
                printf("Memutar lagu %s...\n", buffer); // Contoh: Menampilkan informasi lagu yang diputar
                break;
            }
        }
        fclose(file);
    } else {
        printf("Gagal membuka playlist.\n");
    }
}

int main() {
    char username[30];
    char password[30];
    char kataKunci[50];
    int kondisi;
    int pilihHome;
    int pilihPlaylist;

    struct akun *headAkun = NULL;

    srand(time(NULL));

    FILE *file = fopen("listlagu.txt", "r");
    if (file == NULL) {
        printf("Error saat membuka file.\n");
        return 1;
    }

    struct playlist *head = NULL;
    struct playlist *tail = NULL;

    while (!feof(file)) {
        struct playlist node = (struct playlist)malloc(sizeof(struct playlist));
        if (fscanf(file, "%49[^#]#%49[^#]#%49[^#]#%d\n", node->judul, node->penyanyi, node->album, &node->tahun) != 4) {
            free(node);
            break;
        }

        node->next = NULL;
        node->prev = tail;
        if (head == NULL) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
    }
    fclose(file);
    char logo[] = "zlogo.txt";
    char baris[1000];

    file = fopen(logo, "r");
    buatData(file, baris);
    printf("\n");
    fclose(file);

    login(kondisi, username, password);

    do {
        printf("\n===================Beranda==================\n");
        printf(" Hello, %s\n", username);
        printf("1. Cari berdasarkan Kata Kunci\n");
        printf("2. Pilih Playlist  \n");
        printf("3. Tampilkan Semua Lagu \n");
        printf("4. Buat playlist \n");
        printf("5. Putar Lagu        \n");
        printf("6. Keluar\n");
        printf("Pilihan : ");
        scanf("%d", &pilihHome);

        if (pilihHome == 1) {
            cariLagu(head, kataKunci);
        } else if (pilihHome == 2) {
            playlist(pilihHome, &pilihPlaylist, head, tail);
        } else if (pilihHome == 3) {
            system("cls");
            printf("=========================================\n");
            printf("           Menampilkan Semua Lagu          \n");
            printf("=========================================\n");
            struct playlist *current = head;
            while (current != NULL) {
                printf("Judul   : %s\n", current->judul);
                printf("Penyanyi  : %s\n", current->penyanyi);
                printf("Album   : %s\n", current->album);
                printf("Tahun    : %d\n", current->tahun);
                printf("-----------------------------------------\n");
                current = current->next;
            }
        } else if (pilihHome == 4) {
            buatPlaylist();
        } else if (pilihHome == 5) {
            struct playlist *current = head;
            if (current != NULL) {
                mainkanLagu(current);
                sleep(1);
            }

            while (current != NULL) {
                printf("\n[Menu Player]\n");
                printf("1. Lagu berikutnya\n");
                printf("2. Lagu sebelumnya\n");
                printf("3. Berhenti\n");
                printf("4. Shuffle\n");
                printf("Pilihan: ");
                int kontrol;
                scanf("%d", &kontrol);

                if (kontrol == 1) {
                    current = current->next;
                    if (current == NULL) {
                        current = head;
                    }
                    mainkanLagu(current);
                    sleep(1);
                } else if (kontrol == 2) {
                    current = current->prev;
                    if (current == NULL) {
                        current = tail;
                    }
                    mainkanLagu(current);
                    sleep(1);
                } else if (kontrol == 3) {
                    printf("Pemutaran lagu dihentikan.\n");
                    break;
                } else if (kontrol == 4) {
                    // Shuffle playlist
                    // Anda bisa implementasikan fungsionalitas pengocokan di sini
                    // atau panggil fungsi untuk menangani pengocokan
                    // Untuk sekarang, mari hanya mencetak pesan
                    printf("Mengocok playlist...\n");
                } else {
                    printf("Pilihan tidak valid.\n");
                }
            }
        } else if (pilihHome == 6) {
            return;
        }
    } while (pilihHome != 6);

    struct playlist *current = head;
    while (current != NULL) {
        struct playlist *temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}