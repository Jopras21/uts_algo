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
    readDatabase(&akunHead);
    printf("1. Belum punya akun? buat sekarang!\n");
    printf("2. Sudah punya akun? login sekarang!\n");
    printf("pilihan :"); scanf("%d", &kondisi);

if (kondisi == 1) {
    printf("================Register==============\n");
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
    int songLength = 100;
    char titleLength = 50;
    char artistLength = 50;
    int numSongs;
    FILE *file;

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

    file = fopen("playlist.txt", "a");

    file = fopen("playlist.txt", "a");

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

void searchSong(struct playlist *head, char keyword[50]) {
            printf("Enter keyword to search: ");
            scanf("%s", keyword);

            bool found = false;
            struct playlist *current = head;

            printf("=========================================\n");
            printf("      Search Results for \"%s\"       \n", keyword);
            printf("=========================================\n");

            while (current != NULL) {
                if (strstr(current->judul, keyword) != NULL) {
                    printf("Title: %s\n", current->judul);
                    printf("Artist: %s\n", current->penyanyi);
                    printf("Album: %s\n", current->album);
                    printf("Year: %d\n", current->tahun);
                    printf("-----------------------------------------\n");
                    found = true;
                }
                current = current->next;
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

// Fungsi untuk menambahkan akun baru
void addAcc(struct akun **head, const char *username, const char *password) {
    struct akun *node = (struct akun*)malloc(sizeof(struct akun));
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

void displayExistingPlaylists() {
    system("cls");
    printf("--------------------Pilih Playlist---------------------\n");
    printf("Daftar Playlist:\n");

    DIR *dir;
    struct dirent *direct;
    if ((dir = opendir(".")) != NULL) {
        int count = 0;
        while ((direct = readdir(dir)) != NULL) {
            if (strstr(direct->d_name, ".txt") != NULL && strcmp(direct->d_name, "database.txt") != 0 && strcmp(direct->d_name, "logo.txt") != 0 ) {
                printf("%d. %s\n", ++count, direct->d_name);
            }
        }
        closedir(dir);
    } else {
        perror("Direktori tidak tersedia");
    }
    
}

void deletePlaylistFromFile(const char *filename) {
    if (strcmp(filename, "database.txt") == 0 || strcmp(filename, "logo.txt") == 0) {
        printf("Tidak ada playlist dengan nama '%s' .\n", filename);
    } else {
        if (remove(filename) == 0) {
            printf("Playlist '%s' berhasil dihapus.\n", filename);
        } else {
            printf("Gagal menghapus playlist.\n");
        }
    }
}

void addSongToPlaylist(const char *playlistFilename) {
    FILE *file = fopen(playlistFilename, "a");
    if (file == NULL) {
        printf("Gagal membuka playlist.\n");
        return;
    }

    struct playlist newSong;

    printf("Masukkan judul lagu: ");
    fgets(newSong.judul, sizeof(newSong.judul), stdin);
    strtok(newSong.judul, "\n");

    printf("Masukkan nama penyanyi: ");
    fgets(newSong.penyanyi, sizeof(newSong.penyanyi), stdin);
    strtok(newSong.penyanyi, "\n");

    printf("Masukkan album: ");
    fgets(newSong.album, sizeof(newSong.album), stdin);
    strtok(newSong.album, "\n");

    printf("Masukkan tahun rilis: ");
    scanf("%d", &newSong.tahun);
    getchar(); // Consume newline character left in input buffer

    fprintf(file,"%s#%s#%s#(%d)\n", newSong.judul, newSong.penyanyi, newSong.album, newSong.tahun);

    fclose(file);

    printf("Lagu berhasil ditambahkan ke playlist.\n");
}

void removeSongFromPlaylist(const char *playlistFilename, int songNumber) {
    FILE *file = fopen(playlistFilename, "r");
    if (file == NULL) {
        printf("Gagal membuka playlist.\n");
        return;
    }

    char tempFilename[] = "temp.txt";
    FILE *tempFile = fopen(tempFilename, "w");
    if (tempFile == NULL) {
        fclose(file);
        printf("Gagal membuat file sementara.\n");
        return;
    }

    char buffer[1000];
    int count = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        count++;
        if (count != songNumber) {
            fputs(buffer, tempFile);
        }
    }

    fclose(file);
    fclose(tempFile);

    if (remove(playlistFilename) == 0) {
        if (rename(tempFilename, playlistFilename) == 0) {
            printf("Lagu berhasil dihapus dari playlist.\n");
        } else {
            printf("Gagal mengubah nama file.\n");
        }
    } else {
        printf("Gagal menghapus playlist.\n");
    }
}

void displayPlaylist(const char *playlistFilename) {
    FILE *file = fopen(playlistFilename, "r");
    if (file != NULL) {
        char buffer[100];
        printf("========================================================================================\n");
        printf("| %-30s | %-30s | %-20s | %-5s |\n", "Judul", "Penyanyi", "Album", "Tahun");
        printf("========================================================================================\n");
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            // Memeriksa apakah baris berisi nama file yang tidak diinginkan
            if (strstr(buffer, "database.txt") != NULL || strstr(buffer, "logo.txt") != NULL) {
                continue; // Langsung lanjut ke baris berikutnya jika baris berisi file yang tidak diinginkan
            }

            // Parsing data dari buffer yang dibaca dari file
            char *judul = strtok(buffer, "#");
            char *penyanyi = strtok(NULL, "#");
            char *album = strtok(NULL, "#");
            char *tahun_str = strtok(NULL, "()");
            int tahun = atoi(tahun_str);
            printf("| %-30s | %-30s | %-20s | %-5d |\n", judul, penyanyi, album, tahun);
        }
        printf("=======================================================================================\n");
        fclose(file);
    } else {
        printf("Gagal membuka playlist.\n");
    }
}


void playlist(int pilihhome, int *pilihPlaylist, struct playlist *head, struct playlist *tail) {
    if (pilihhome == 2) {
        system("cls");
        printf("--------------------Pilih Playlist---------------------\n");
        displayExistingPlaylists();
        printf("0. Hapus Playlist \n");
        printf("Pilihan : ");
        scanf("%d", pilihPlaylist);

        if (*pilihPlaylist == 0) {
            char filename[50];
            printf("Masukkan nama file playlist yang ingin dihapus: ");
            scanf("%49s", filename);
            deletePlaylistFromFile(filename);
        } else {
            DIR *dir;
            struct dirent *direct;
            char playlistNames[50][50]; 
            int count = 0;
            if ((dir = opendir(".")) != NULL) {
                while ((direct = readdir(dir)) != NULL) {
                    if (strstr(direct->d_name, ".txt") != NULL) {
                        strcpy(playlistNames[count], direct->d_name);
                        count++;
                    }
                }
                closedir(dir);
            } else {
                perror("Direktori tidak tersedia");
                return;
            }

            if (*pilihPlaylist > 0 && *pilihPlaylist <= count) {
    FILE *file = fopen(playlistNames[*pilihPlaylist - 1], "r");
    if (file != NULL) {
    if (file != NULL) {
        char buffer[100];
        printf("========================================================================================\n");
        printf("| %-30s | %-30s | %-20s | %-5s |\n", "Judul", "Penyanyi", "Album", "Tahun");
        printf("========================================================================================\n");
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            // Memeriksa apakah baris berisi nama file yang tidak diinginkan
            if (strstr(buffer, "database.txt") != NULL || strstr(buffer, "logo.txt") != NULL) {
                continue; // Langsung lanjut ke baris berikutnya jika baris berisi file yang tidak diinginkan
            }

            // Parsing data dari buffer yang dibaca dari file
            char *judul = strtok(buffer, "#");
            char *penyanyi = strtok(NULL, "#");
            char *album = strtok(NULL, "#");
            char *tahun_str = strtok(NULL, "()");
            int tahun = atoi(tahun_str);
            printf("| %-30s | %-30s | %-20s | %-5d |\n", judul, penyanyi, album, tahun);
        }
        printf("=======================================================================================\n");
        fclose(file);
    } else {
        printf("Gagal membuka playlist.\n");
    }
                }
                
                int playlistOption;
                printf("Pilih opsi:\n");
                printf("1. Play Song\n");
                printf("2. Add Song to Playlist\n");
                printf("3. Remove Song from Playlist\n");
                printf("4. Kembali ke Home\n");
                printf("Pilihan: ");
                scanf("%d", &playlistOption);

                switch (playlistOption) {
                    case 1: {
                    int songNumber;
                    printf("Masukkan nomor lagu yang ingin diputar: ");
                    scanf("%d", &songNumber);

                    FILE *file = fopen(playlistNames[*pilihPlaylist - 1], "r");
                    if (file != NULL) {
                        char buffer[1000];
                        int count = 0;
                        struct playlist *current = head;
                        while (current != NULL) {
                            count++;
                            if (count == songNumber) {
                                printf("\nNow Playing: %s\n", current->judul);
                                printf("Artist: %s\n", current->penyanyi);
                                printf("Album: %s\n", current->album);
                                playSong(current);
                                sleep(1);
                                printf("\n[Menu Player]\n");
                                printf("1. Next Song\n");
                                printf("2. Previous Song\n");
                                printf("3. Stop\n");
                                printf("4. Shuffle\n");
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
                                    playSong(current);
                                    sleep(1);
                                } else if (kontrol == 3) {
                                    printf("Song playback stopped.\n");
                                    break;
                                } else if (kontrol == 4) {
                                    // Shuffle playlist
                                    // You can implement shuffle functionality here
                                    // or call a function to handle shuffling
                                    printf("Shuffling playlist...\n");
                                } else {
                                    printf("Invalid choice.\n");
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
                        addSongToPlaylist(playlistNames[*pilihPlaylist - 1]);
                        break;
                    }
                    case 3: {
                        // Remove Song from Playlist
                        int songNumber;
                        printf("Masukkan nomor lagu yang ingin dihapus dari playlist: ");
                        scanf("%d", &songNumber);
                        removeSongFromPlaylist(playlistNames[*pilihPlaylist - 1], songNumber);
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
    }
}




void playSong(struct playlist *song) {
    printf("\nNow Playing: %s\n", song->judul);
    printf("Artist: %s\n", song->penyanyi);
    printf("Album: %s\n", song->album); 
}

int main() {
    char username[30];
    char password[30];
    char keyword[50];
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
        struct playlist *node = (struct playlist*)malloc(sizeof(struct playlist));
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
        printf("1. Search by Keyword\n");
        printf("2. Choose Playlist  \n");
        printf("3. Display All Song \n");
        printf("4. Play Song        \n");
        printf("5. Logout\n");
        printf("Pilihan : ");
        scanf("%d", &pilihHome);

        if (pilihHome == 1) {
            searchSong(head, keyword);
        } else if (pilihHome == 2) {
            playlist(pilihHome, &pilihPlaylist, head, tail);
        } else if (pilihHome == 3) {
            system("cls");
            printf("=========================================\n");
            printf("           Displaying All Songs          \n");
            printf("=========================================\n");
            struct playlist *current = head;
            while (current != NULL) {
                printf("Title   : %s\n", current->judul);
                printf("Artist  : %s\n", current->penyanyi);
                printf("Album   : %s\n", current->album);
                printf("Year    : %d\n", current->tahun);
                printf("-----------------------------------------\n");
                current = current->next;
            }
        } else if (pilihHome == 4) {
            struct playlist *current = head;
            if (current != NULL) {
                playSong(current); 
                sleep(1); 
            }

            while (current != NULL) {
                printf("\n[Menu Player]\n");
                printf("1. Next Song\n");
                printf("2. Previous Song\n");
                printf("3. Stop\n");
                printf("4. Shuffle\n");
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
                    playSong(current);
                    sleep(1); 
                } else if (kontrol == 3) {
                    printf("Song playback stopped.\n");
                    break; 
                } else if (kontrol == 4) {
                    // Shuffle playlist
                    // You can implement shuffle functionality here
                    // or call a function to handle shuffling
                    // For now, let's just print a message
                    printf("Shuffling playlist...\n");
                } else {
                    printf("Invalid choice.\n");
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