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


    void createData(FILE *file, char baris[1000]) {
        char barisArray[1000]; // Define a new array to store each line of data
        while (fgets(barisArray, sizeof(barisArray), file) != NULL) { // Use the new array to read data
            printf("%s", barisArray); // Print each line from the new array
        }
    }

void createCustomPlaylist(char playlistNames[][50], int numPlaylists) {
    for (int i = 0; i < numPlaylists; i++) {
        FILE *file = fopen(strcat(playlistNames[i], ".txt"), "a");
        if (file == NULL) {
            printf("Gagal membuat playlist '%s'.\n", playlistNames[i]);
        } else {
            printf("Playlist '%s' berhasil dibuat.\n", playlistNames[i]);
            fclose(file);
        }
    }
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
bool cekAkunArray(struct akun *accounts[], int numAccounts, const char *username, const char *password) {
    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i]->username, username) == 0 && strcmp(accounts[i]->password, password) == 0) {
            return true;
        }
    }
    return false;
}


void displayExistingPlaylistsArray(char *existingPlaylists[], int *playlistCount) {
    DIR *dir;
    struct dirent *direct;
    if ((dir = opendir(".")) != NULL) {
        *playlistCount = 0; // Initialize the count of playlists
        while ((direct = readdir(dir)) != NULL) {
            if (strstr(direct->d_name, ".txt") != NULL && strcmp(direct->d_name, "database.txt") != 0 && strcmp(direct->d_name, "logo.txt") != 0 ) {
                // Store the playlist name in the array
                existingPlaylists[*playlistCount] = direct->d_name;
                (*playlistCount)++; // Increment the count of playlists
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

void removeSongFromPlaylist(const char *playlistName, int songNumber) {
    FILE *rem = fopen(playlistName, "r");
    if (rem == NULL) {
        printf("Gagal membuka playlist.\n");
        return;
    }

    const char tempFilename[] = "temp.txt"; // Define tempFilename as a constant string
    FILE *tempFile = fopen(tempFilename, "w");
    if (tempFile == NULL) {
        fclose(rem);
        printf("Gagal membuat file sementara.\n");
        return;
    }

    char buffer[1000];
    int count = 0;

    // Define an array to store lines from the playlist
    char lines[1000][1000]; // Assuming maximum number of lines and maximum line length

    while (fgets(buffer, sizeof(buffer), rem) != NULL) {
        count++;
        if (count != songNumber) {
            // Copy line to the array if it's not the song to be removed
            strcpy(lines[count - 1], buffer);
        }
    }

    fclose(rem);

    // Rewrite the playlist with the lines from the array
    for (int i = 0; i < count - 1; i++) {
        fputs(lines[i], tempFile);
    }

    fclose(tempFile);

    // Remove the original playlist and rename the temporary file
    if (remove(playlistName) == 0) {
        if (rename(tempFilename, playlistName) == 0) {
            printf("Lagu berhasil dihapus dari playlist.\n");
        } else {
            printf("Gagal mengubah nama file.\n");
        }
    } else {
        printf("Gagal menghapus playlist.\n");
    }
}


void displayPlaylist(const char *playlistName) {
    FILE *display = fopen(playlistName, "r");
    if (display != NULL) {
        char buffer[100];
        printf("=============================================================================================================\n");
        printf("| %-30s | %-30s | %-35s | %-5s |\n", "Judul", "Penyanyi", "Album", "Tahun");
        printf("=============================================================================================================\n");

        // Define arrays to store data
        char judul[50][100];  // Assuming maximum length of judul is 100 characters
        char penyanyi[50][100];  // Assuming maximum length of penyanyi is 100 characters
        char album[50][100];  // Assuming maximum length of album is 100 characters
        int tahun[50];  // Array to store years
        int index = 0;  // Index for arrays

        while (fgets(buffer, sizeof(buffer), display) != NULL) {
            if (strstr(buffer, "zdatabase.txt") != NULL || strstr(buffer, "zlogo.txt") != NULL) {
                continue;
            }
            // Use sscanf to parse the buffer
            sscanf(buffer, "%[^#]#%[^#]#%[^#]#(%d)", judul[index], penyanyi[index], album[index], &tahun[index]);
            printf("| %-30s | %-30s | %-35s | %-5d |\n", judul[index], penyanyi[index], album[index], tahun[index]);
            index++;
        }

        printf("============================================================================================================\n");
        fclose(display);
    } else {
        printf("Gagal membuka playlist.\n");
    }
}


void playlist(int pilihhome, int *pilihPlaylist, struct playlist *head, struct playlist *tail) {
    char playlistNames[50][50];
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
                displayPlaylist(playlistNames[*pilihPlaylist - 1]); // Tampilkan isi playlist yang dipilih
            } else {
                printf("Pilihan tidak valid.\n");
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
