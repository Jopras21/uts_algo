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
    printf("================Daftarr==============\n");
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
        char barisArray[1000]; // Define a new array to store each line of data
        while (fgets(barisArray, sizeof(barisArray), file) != NULL) { // Use the new array to read data
            printf("%s", barisArray); // Print each line from the new array
        }
    }

void buatPlaylistKustom(char namaPlaylist[][50], int jumlahPlaylist) {
    for (int i = 0; i < jumlahPlaylist; i++) {
        FILE *file = fopen(strcat(namaPlaylist[i], ".txt"), "a");
        if (file == NULL) {
            printf("Gagal membuat playlist '%s'.\n", namaPlaylist[i]);
        } else {
            printf("Playlist '%s' berhasil dibuat.\n", namaPlaylist[i]);
            fclose(file);
        }
    }
}

void buatPlaylist(char namaPlaylist[][50], int jumlahPlaylist) {
    int panjangLagu = 100;
    char panjangJudul = 50;
    char panjangPenyanyi = 50;
    int jumlahLagu;
    FILE *file;

    struct playlist playlist[panjangLagu];

    printf("1. Buat playlist baru\n");
    printf("2. Tambahkan lagu ke playlist yang ada\n");
    printf("Pilihan: ");
    int pilihan;
    scanf("%d", &pilihan);

    if (pilihan == 1) {
        buatPlaylistKustom(namaPlaylist, jumlahPlaylist);
    }

    printf("Masukkan jumlah lagu yang ingin ditambahkan ke playlist: ");
    scanf("%d", &jumlahLagu);
    getchar();

    file = fopen("playlist.txt", "a");

    file = fopen("playlist.txt", "a");

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

        printf("Masukkan tahun rilis untuk lagu ke-%d: ", i + 1);
        scanf("%d", &playlist[i].tahun);
        getchar();

        fprintf(file, "%s#%s#(%d)\n", playlist[i].judul, playlist[i].penyanyi, playlist[i].tahun);
    }

    printf("Playlist berhasil ditambahkan ke file.\n");

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
    FILE *file = fopen("database.txt", "r");
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

// Fungsi untuk menambahkan akun baru
void tambahAkun(struct akun **head, const char *username, const char *password) {
    struct akun node = (struct akun)malloc(sizeof(struct akun));
    strcpy(node->username, username);
    strcpy(node->password, password);
    node->next = *head;
    *head = node;

    FILE *file = fopen("database.txt", "a");
    if (file == NULL) {
        printf("Error membuka file database.\n");
        return;
    }
    fprintf(file, "%s %s\n", username, password);
    fclose(file);
}

// Fungsi untuk memeriksa keberadaan akun
bool cekAkunArray(struct akun *akun[], int jumlahAkun, const char *username, const char *password) {
    for (int i = 0; i < jumlahAkun; i++) {
        if (strcmp(akun[i]->username, username) == 0 && strcmp(akun[i]->password, password) == 0) {
            return true;
        }
    }
    return false;
}


void tampilkanPlaylistArray(char *daftarPlaylist[], int *jumlahPlaylist) {
    DIR *dir;
    struct dirent *direktori;
    if ((dir = opendir(".")) != NULL) {
        *jumlahPlaylist = 0; // Initialize the count of playlists
        while ((direktori = readdir(dir)) != NULL) {
            if (strstr(direktori->d_name, ".txt") != NULL && strcmp(direktori->d_name, "database.txt") != 0 && strcmp(direktori->d_name, "logo.txt") != 0 ) {
                // Store the playlist name in the array
                daftarPlaylist[*jumlahPlaylist] = direktori->d_name;
                (*jumlahPlaylist)++; // Increment the count of playlists
            }
        }
        closedir(dir);
    } else {
        perror("Direktori tidak tersedia");
    }
}


void hapusPlaylistDariFile(const char *namaFile) {
    if (strcmp(namaFile, "database.txt") == 0 || strcmp(namaFile, "logo.txt") == 0) {
        printf("Tidak ada playlist dengan nama '%s' .\n", namaFile);
    } else {
        if (remove(namaFile) == 0) {
            printf("Playlist '%s' berhasil dihapus.\n", namaFile);
        } else {
            printf("Gagal menghapus playlist.\n");
        }
    }
}

void tambahLaguKePlaylist(const char *namaFilePlaylist) {
    FILE *file = fopen(namaFilePlaylist, "a");
    if (file == NULL) {
        printf("Gagal membuka playlist.\n");
        return;
    }

    struct playlist laguBaru;

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
    getchar(); // Consume newline character left in input buffer

    fprintf(file,"%s#%s#%s#(%d)\n", laguBaru.judul, laguBaru.penyanyi, laguBaru.album, laguBaru.tahun);

    fclose(file);

    printf("Lagu berhasil ditambahkan ke playlist.\n");
}

void hapusLaguDariPlaylist(const char *namaFilePlaylist, int nomorLagu) {
    FILE *file = fopen(namaFilePlaylist, "r");
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

    if (remove(namaFilePlaylist) == 0) {
        if (rename(namaFileSementara, namaFilePlaylist) == 0) {
            printf("Lagu berhasil dihapus dari playlist.\n");
        } else {
            printf("Gagal mengubah nama file.\n");
        }
    } else {
        printf("Gagal menghapus playlist.\n");
    }
}

void tampilkanPlaylist(const char *namaFilePlaylist) {
    FILE *file = fopen(namaFilePlaylist, "r");
    if (file != NULL) {
        char buffer[100];
        printf("================================================================================================\n");
        printf("| %-30s | %-30s | %-35s | %-5s |\n", "Judul", "Penyanyi", "Album", "Tahun");
        printf("================================================================================================\n");
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            if (strstr(buffer, "database.txt") != NULL || strstr(buffer, "logo.txt") != NULL) {
                continue;
            }
            char *judul = strtok(buffer, "#");
            char *penyanyi = strtok(NULL, "#");
            char *album = strtok(NULL, "#");
            char *tahun_str = strtok(NULL, "()");
            int tahun = atoi(tahun_str);
            printf("| %-30s | %-30s | %-35s | %-5d |\n", judul, penyanyi, album, tahun);
        }
        printf("===============================================================================================\n");
        fclose(file);
    } else {
        printf("Gagal membuka playlist.\n");
    }
}


void playlist(int pilihhome, int *pilihPlaylist, struct playlist *head, struct playlist *tail) {
    char namaPlaylist[50][50];
    if (pilihhome == 2) {
        system("cls");
        printf("--------------------Pilih Playlist---------------------\n");
        tampilkanPlaylistArray();
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
            char namaPlaylist[50][50];
            int hitung = 0;
            if ((dir = opendir(".")) != NULL) {
                while ((direktori = readdir(dir)) != NULL) {
                    if (strstr(direktori->d_name, ".txt") != NULL) {
                        strcpy(namaPlaylist[hitung], direktori->d_name);
                        hitung++;
                    }
                }
                closedir(dir);
            } else {
                perror("Direktori tidak tersedia");
                return;
            }

            if (*pilihPlaylist > 0 && *pilihPlaylist <= hitung) {
                tampilkanPlaylist(namaPlaylist[*pilihPlaylist - 1]); // Tampilkan isi playlist yang dipilih
            } else {
                printf("Pilihan tidak valid.\n");
            }
        }

                int opsiPlaylist;
                printf("Pilih opsi:\n");
                printf("1. Putar Lagu\n");
                printf("2. Tambah Lagu ke Playlist\n");
                printf("3. Hapus Lagu dari Playlist\n");
                printf("4. Kembali ke Home\n");
                printf("Pilihan: ");
                scanf("%d", &opsiPlaylist);

                switch (opsiPlaylist) {
                    case 1: {
                    int nomorLagu;
                    printf("Masukkan nomor lagu yang ingin diputar: ");
                    scanf("%d", &nomorLagu);

                    FILE *file = fopen(namaPlaylist[*pilihPlaylist - 1], "r");
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
                                putarLagu(current);
                                sleep(1);
                                printf("\n[Menu Player]\n");
                                printf("1. Lagu Berikutnya\n");
                                printf("2. Lagu Sebelumnya\n");
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
                                    playSong(current);
                                    sleep(1);
                                } else if (kontrol == 2) {
                                    current = current->prev;
                                    if (current == NULL) {
                                        current = tail;
                                    }
                                    putarLagu(current);
                                    sleep(1);
                                } else if (kontrol == 3) {
                                    printf("Pemutaran lagu dihentikan.\n");
                                    break;
                                } else if (kontrol == 4) {
                                    // Acak playlist
                                    // You can implement shuffle functionality here
                                    // or call a function to handle shuffling
                                    printf("Mengacak playlist...\n");
                                } else {
                                    printf("Pilihan tidak valid.\n");
                                }
                                break;
                            }
                            current = current->next;
                        }
                        fclose(file);
                    } else {
                        printf("Gagal membuka playlist.\n");
                    }
                    break;
                }
                    case 2: {
                        // Add Song to Playlist
                        tambahLaguKePlaylist(namaPlaylist[*pilihPlaylist - 1]);
                        break;
                    }
                    case 3: {
                        // Remove Song from Playlist
                        int nomorLagu;
                        printf("Masukkan nomor lagu yang ingin dihapus dari playlist: ");
                        scanf("%d", &nomorLagu);
                        removeSongFromPlaylist(namaPlaylist[*pilihPlaylist - 1], nomorLagu);
                        break;
                    }
                    case 4: {
                        // Kembali ke Home
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




void putarLagu(struct playlist *lagu) {
    printf("\nSedang Diputar: %s\n", lagu->judul);
    printf("Penyanyi: %s\n", lagu->penyanyi);
    printf("Album: %s\n", lagu->album);
}

int main() {
    char username[30];
    char password[30];
    char kataKunci[50];
    int kondisi;
    int pilihHome;
    int pilihPlaylist;

    struct akun *akunHead = NULL;

    srand(time(NULL));

    FILE *file = fopen("listlagu.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
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
    char logo[] = "logo.txt";
    char baris[1000];

    file = fopen(logo, "r");
    createData(file, baris);
    printf("\n");
    fclose(file);

    login(kondisi, username, password);

    do {
        printf("\n===================Home==================\n");
        printf(" Hello, %s\n", username       );
        printf("1. Cari berdasarkan Kata Kunci\n");
        printf("2. Pilih Playlist  \n");
        printf("3. Tampilkan Semua Lagu \n");
        printf("4. Putar Lagu        \n");
        printf("5. Keluar\n");
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
            struct playlist *current = head;
            if (current != NULL) {
                putarLagu(current);
                sleep(1);
            }

            while (current != NULL) {
                printf("\n[Menu Player]\n");
                printf("1. Lagu Berikutnya\n");
                printf("2. Lagu Sebelumnya\n");
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
                    putarLagu(current);
                    sleep(1);
                } else if (kontrol == 2) {
                    current = current->prev;
                    if (current == NULL) {
                        current = tail;
                    }
                    putarLagu(current);
                    sleep(1);
                } else if (kontrol == 3) {
                    printf("Pemutaran lagu dihentikan.\n");
                    break;
                } else if (kontrol == 4) {
                    // Shuffle playlist
                    // You can implement shuffle functionality here
                    // or call a function to handle shuffling
                    // For now, let's just print a message
                    printf("Mengacak playlist...\n");
                } else {
                    printf("Pilihan tidak valid.\n");
                }
            }
        } else if (pilihHome == 5) {
            return;
        }
    } while (pilihHome != 5);

    // Free allocated memory
    struct playlist *current = head;
    while (current != NULL) {
        struct playlist *temp = current;
        current = current->next;
        free(temp);
    }

    return 0;
}