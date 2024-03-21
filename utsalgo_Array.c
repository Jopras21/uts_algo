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
};

struct akun {
    char username[30];
    char password[30];
    struct akun *next;
};

bool cekAkun(struct akun *head, const char *username, const char *password);

void login(int kondisi, char username[30], char password[30], struct akun **akunHead) {
    readDatabase(akunHead);
    printf("1. Belum punya akun? buat sekarang!\n");
    printf("2. Sudah punya akun? login sekarang!\n");
    printf("pilihan :"); scanf("%d", &kondisi);

    if (kondisi == 1) {
        printf("================Register==============\n");
        printf("Username : ");
        scanf("%s", username);
        printf("\nPassword : ");
        scanf("%s", password);
        addAcc(akunHead, username, password);
    } else if (kondisi == 2) {
        printf("================Login==============\n");
        printf("Username : ");
        scanf("%s", username);
        printf("\nPassword : ");
        scanf("%s", password);
        if (!cekAkun(*akunHead, username, password)) {
            printf("Username atau password salah.\n");
            return;
        }
    }
}

void createData(FILE *file, char baris[1000]) {
    char karakter;
    while (fgets(baris, sizeof(baris), file) != NULL) {
        printf("%s", baris);
    }
}

void createCustomPlaylist() {
    char playlistName[50];
    printf("Masukkan nama playlist baru: ");
    scanf("%s", playlistName);

    FILE *file = fopen(strcat(playlistName, ".txt"), "a");
    if (file == NULL) {
        printf("Gagal membuat playlist.\n");
        return;
    }

    printf("Playlist '%s' berhasil dibuat.\n", playlistName);
    fclose(file);
}

void createPlaylist() {
    int numSongs;
    const int titleLength = 50;
    const int artistLength = 50;
    const int songLength = 100; // Deklarasi variabel lokal

    struct playlist playlist[songLength];

    printf("1. Buat playlist baru\n");
    printf("2. Tambahkan lagu ke playlist yang ada\n");
    printf("Pilihan: ");
    int choice;
    scanf("%d", &choice);

    if (choice == 1) {
        createCustomPlaylist();
    }

    printf("Masukkan jumlah lagu yang ingin ditambahkan ke playlist: ");
    scanf("%d", &numSongs);
    getchar();

    FILE *file = fopen("playlist.txt", "a");

    if (file == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    for (int i = 0; i < numSongs; i++) {
        printf("Masukkan judul lagu ke-%d: ", i + 1);
        fgets(playlist[i].judul, titleLength, stdin);
        strtok(playlist[i].judul, "\n");

        printf("Masukkan nama penyanyi untuk lagu ke-%d: ", i + 1);
        fgets(playlist[i].penyanyi, artistLength, stdin);
        strtok(playlist[i].penyanyi, "\n");

        printf("Masukkan tahun rilis untuk lagu ke-%d: ", i + 1);
        scanf("%d", &playlist[i].tahun);
        getchar();

        fprintf(file, "%s#%s#(%d)\n", playlist[i].judul, playlist[i].penyanyi, playlist[i].tahun);
    }

    printf("Playlist berhasil ditambahkan ke file.\n");

    fclose(file);
}

void searchSong(struct playlist songs[], int numSongs, char keyword[50]) {
    printf("Enter keyword to search: ");
    scanf("%s", keyword);

    bool found = false;

    printf("=========================================\n");
    printf("      Search Results for \"%s\"       \n", keyword);
    printf("=========================================\n");

    for (int i = 0; i < numSongs; i++) {
        if (strstr(songs[i].judul, keyword) != NULL) {
            printf("Title: %s\n", songs[i].judul);
            printf("Artist: %s\n", songs[i].penyanyi);
            printf("Album: %s\n", songs[i].album);
            printf("Year: %d\n", songs[i].tahun);
            printf("-----------------------------------------\n");
            found = true;
        }
    }

    if (!found) {
        printf("No songs found with \"%s\" in the judul.\n", keyword);
    }
}

void readDatabase(struct akun **head) {
    FILE *file = fopen("database.txt", "r");
    if (file == NULL) {
        printf("Error membuka file database.\n");
        return;
    }

    while (!feof(file)) {
        struct akun *node = (struct akun*)malloc(sizeof(struct akun));
        if (fscanf(file, "%s %s\n", node->username, node->password) != 2) {
            free(node);
            break;
        }

        node->next = *head;
        *head = node;
    }
    fclose(file);
}

// Implementasi lainnya sesuai kebutuhan

int main() {
    char username[30];
    char password[30];
    char keyword[50];
    int kondisi;
    int pilihHome;
    int pilihPlaylist;
    struct akun *akunHead = NULL;

    srand(time(NULL));

    struct playlist songs[100]; // Deklarasi variabel lokal
    int numSongs = 0;

    FILE *file = fopen("listlagu.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    while (numSongs < 100 && !feof(file)) { // Mengganti #define MAX_SONGS dengan angka 100
        if (fscanf(file, "%49[^#]#%49[^#]#%49[^#]#%d\n", songs[numSongs].judul, songs[numSongs].penyanyi, songs[numSongs].album, &songs[numSongs].tahun) == 4) {
            numSongs++;
        }
    }
    fclose(file);

    login(kondisi, username, password, &akunHead);

    // Sisanya implementasi program seperti sebelumnya

    return 0;
}
