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

    FILE *file = fopen(strcat(playlistName,""), "a");
    if (file == NULL) {
        printf("Gagal membuat playlist.\n");
        return;
    }

    printf("Playlist '%s' berhasil dibuat.\n", playlistName);
    fclose(file);
}

void createPlayList() {
    int songLength = 100;
    char titleLength = 50;
    char artistLength = 50;
    char albumLength = 50;
    int numSongs;
    FILE *file;

    struct playlist *head = NULL;
    struct playlist *tail = NULL;

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

    if (file == NULL) {
        printf("Gagal membuka file.\n");
        return;
    }

    for (int i = 0; i < numSongs; i++) {
        struct playlist *node = (struct playlist*)malloc(sizeof(struct playlist));
        
        printf("Masukkan judul lagu ke-%d: ", i + 1);
        fgets(node->judul, titleLength, stdin);
        strtok(node->judul, "\n");

        printf("Masukkan nama penyanyi untuk lagu ke-%d: ", i + 1);
        fgets(node->penyanyi, artistLength, stdin);
        strtok(node->penyanyi, "\n");

        printf("Masukkan nama penyanyi untuk lagu ke-%d: ", i + 1);
        fgets(node->album, albumLength, stdin);
        strtok(node->album, "\n");

        printf("Masukkan tahun rilis untuk lagu ke-%d: ", i + 1);
        scanf("%d", &node->tahun);
        getchar();

        node->next = NULL;
        node->prev = tail;

        if (head == NULL) {
            head = node;
            tail = node;
        } else {
            tail->next = node;
            tail = node;
        }

        fprintf(file, "%s#%s#%d\n", node->judul, node->penyanyi, node-> album, node->tahun);
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
    FILE *file = fopen("zdatabase.txt", "r");
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

void addAcc(struct akun **head, const char *username, const char *password) {
    struct akun *node = (struct akun*)malloc(sizeof(struct akun));
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

int endsWithTxt(const char *str) {
    int len = strlen(str);
    return len >= 4 && strcmp(str + len - 4, ".txt") == 0;
}

void displayExistingPlaylists() {
    char playlists[100][50];
    int count = 0;

    system("dir /b > files.txt");
    FILE *file = fopen("files.txt", "r");
    if (file != NULL) {
        printf("Daftar Playlist:\n");

        char line[256];
        while (fgets(line, sizeof(line), file)) {
            strtok(line, "\n");
            if (endsWithTxt(line) && strcmp(line, "zdatabase.txt") != 0 && strcmp(line, "zlogo.txt") != 0 && strcmp(line, "files.txt") != 0) {
                strncpy(playlists[count], line, strlen(line) - 4);
                playlists[count][strlen(line) - 4] = '\0';
                count++;
            }
        }
        fclose(file);
        remove("files.txt");

        for (int i = 0; i < count; i++) {
            printf("%d. %s\n", i + 1, playlists[i]);
        }
    } else {
        perror("Direktori tidak tersedia");
    }
}

void deletePlaylistFromFile(const char *filename) {
    if (strcmp(filename, "zdatabase.txt") == 0 || strcmp(filename, "zlogo.txt") == 0) {
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
    int c;
    FILE *file = fopen(playlistFilename, "a");
    if (file == NULL) {
        printf("Gagal membuka playlist.\n");
        return;
    }

    struct playlist newSong;

    printf("Masukkan judul lagu: "); (fflush(stdin));
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

    fprintf(file,"%s#%s#%s#%d\n", newSong.judul, newSong.penyanyi, newSong.album, newSong.tahun);

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
        int no = 1;
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
        printf("Gagal membuka playlist '%s'.\n", playlistFilename);
        return;
    }
}

void playlist(int pilihhome, int *pilihPlaylist, struct playlist *head, struct playlist *tail) {
    char playlistNames[50][50];
    int playlistOption;
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
                displayPlaylist(playlistNames[*pilihPlaylist - 1]);
            } else {
                printf("Pilihan tidak valid.\n");
                return;
            }
        }

        printf("Pilih opsi:\n");
        printf("1. Putar Lagu\n");
        printf("2. Tambahkan Lagu ke Playlist\n");
        printf("3. Hapus Lagu dari Playlist\n");
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
                            printf("\nSedang Diputar: %s\n", current->judul);
                            printf("Artis: %s\n", current->penyanyi);
                            printf("Album: %s\n", current->album);
                            playSong(current);
                            sleep(1);
                            break;
                        }
                        current = current->next;
                    }
                        while (current != NULL) {
                            printf("\n[Menu Player]\n");
                            printf("1. Next Song\n");
                            printf("2. Previous Song\n");
                            printf("3. Stop\n");

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
                            } else {
                                printf("Invalid choice.\n");
                            }
                        }
                        fclose(file);
                } else {
                    printf("Gagal membuka playlist.\n");
                }
                break;
            }

            case 2: {
                addSongToPlaylist(playlistNames[*pilihPlaylist - 1]);
                break;
            }

            case 3: {
                int songNumber;
                printf("Masukkan nomor lagu yang ingin dihapus dari playlist: ");
                scanf("%d", &songNumber);
                removeSongFromPlaylist(playlistNames[*pilihPlaylist - 1], songNumber);
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

void playSong(struct playlist *song) {
    printf("\nNow Playing: %s\n", song->judul);
    printf("Artist: %s\n", song->penyanyi);
    printf("Album: %s\n", song->album);
}

void playSongFromPlaylist(char *playlistName, int songNumber) {
    FILE *file = fopen(playlistName, "r");
    if (file != NULL) {
        char buffer[1000];
        int count = 0;
        while (fgets(buffer, sizeof(buffer), file)) {
            count++;
            if (count == songNumber) {
                printf("\nLagu Sedang Diputar:\n");
                printf("%s", buffer);
                printf("Playing song %s...\n", buffer); 
                break;
            }
        }
        fclose(file);
    } else {
        printf("Gagal membuka playlist.\n");
    }
}

// void shuffle(int *array, int n) {
//     if (n > 1) {
//         int i;
//         for (i = 0; i < n - 1; i++) {
//             int j = i + rand() / (RAND_MAX / (n - i) + 1);
//             int temp = array[j];
//             array[j] = array[i];
//             array[i] = temp;
//         }
//     }
// }

// void shufflePlaylist(struct playlist *head, const char *filename) {
//     FILE *file = fopen("listlagu.txt", "r");
//     if (file == NULL) {
//         printf("Gagal membuka file.\n");
//         return;
//     }

//     char line[100];
//     int count = 0;
//     while (fgets(line, sizeof(line), file) != NULL) {
//         count++;
//     }
//     rewind(file);

//     int *indices = (int *)malloc(count * sizeof(int));
//     if (indices == NULL) {
//         printf("Gagal alokasi memori.\n");
//         fclose(file);
//         return;
//     }

//     int i = 0;
//     while (fgets(line, sizeof(line), file) != NULL) {
//         indices[i] = i + 1;
//         i++;
//     }

//     fclose(file);

//     shuffle(indices, count);

//     struct playlist *current = head;
//     i = 0;
//     while (current != NULL && i < count) {
//         struct playlist *temp = current;
//         current = current->next;
//         struct playlist *swapNode = head;
//         int j;
//         for (j = 0; j < indices[i] - 1; j++) {
//             swapNode = swapNode->next;
//         }
//         if (temp != swapNode) {
//             if (temp->prev != NULL) {
//                 temp->prev->next = temp->next;
//             }
//             if (temp->next != NULL) {
//                 temp->next->prev = temp->prev;
//             }
//             if (swapNode->next != NULL) {
//                 swapNode->next->prev = temp;
//             }
//             temp->next = swapNode->next;
//             temp->prev = swapNode;
//             swapNode->next = temp;
//         }
//         i++;
//     }

//     free(indices);
// }


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
        printf("Error membuka file.\n");
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
    char logo[] = "zlogo.txt";
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
        printf("4. Create playlist \n");
        printf("5. Play Song        \n");
        printf("6. Logout\n");
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
            createPlayList();
        } else if (pilihHome == 5) {
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
                } else {
                    printf("Invalid choice.\n");
                }
            }
        } else if (pilihHome == 5) {
            return 0;
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