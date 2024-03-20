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

void cetakData(FILE *file, char baris[1000]) {
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

void displayExistingPlaylists() {
    system("cls");
    printf("--------------------Pilih Playlist---------------------\n");
    printf("Daftar Playlist:\n");

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(".")) != NULL) {
        int count = 0;
        while ((ent = readdir(dir)) != NULL) {
            if (strstr(ent->d_name, ".txt") != NULL) {
                printf("%d. %s\n", ++count, ent->d_name);
            }
        }
        closedir(dir);
    } else {
        perror("Direktori tidak tersedia");
    }
    
}

void deletePlaylistFromFile(const char *filename) {
    if (remove(filename) == 0) {
        printf("Playlist '%s' berhasil dihapus.\n", filename);
    } else {
        printf("Gagal menghapus playlist.\n");
    }
}

void deletePlaylistFromLinkedList(struct playlist **head, int playlistNumber) {
    struct playlist *current = *head;
    struct playlist *prev = NULL;

    int count = 1;
    while (current != NULL) {
        if (count == playlistNumber) {
            if (prev == NULL) {
                *head = current->next;
            } else {
                prev->next = current->next;
                if (current->next != NULL) {
                    current->next->prev = prev;
                }
            }
            free(current);
            printf("Playlist %d berhasil dihapus.\n", playlistNumber);
            return;
        }
        prev = current;
        current = current->next;
        count++;
    }

    printf("Playlist tidak ditemukan.\n");
}

void playlist(int pilihhome, int *pilihPlaylist) {
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
            struct dirent *ent;
            char playlistNames[50][50]; 
            int count = 0;
            if ((dir = opendir(".")) != NULL) {
                while ((ent = readdir(dir)) != NULL) {
                    if (strstr(ent->d_name, ".txt") != NULL) {
                        strcpy(playlistNames[count], ent->d_name);
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
                    char buffer[100];
                    printf("=========================================================================\n");
                    printf("| %-30s | %-30s | %-30s | %-5s |\n", "Judul", "Penyanyi", "Album", "Tahun");
                    printf("=========================================================================\n");
                    while (fgets(buffer, sizeof(buffer), file) != NULL) {
                        char *judul = strtok(buffer, "#");
                        char *penyanyi = strtok(NULL, "#");
                        char *album = strtok(NULL, "#");
                        char *tahun_str = strtok(NULL, "()");
                        int tahun = atoi(tahun_str);
                        printf("| %-30s | %-30s | %-30s | %-5d |\n", judul, penyanyi, album, tahun);
                    }
                    printf("=========================================================================\n");
                    fclose(file);
                } else {
                    printf("Gagal membuka playlist.\n");
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
    int pilihHome;
    int pilihPlaylist;

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
    cetakData(file, baris);
    printf("\n");
    fclose(file);

    printf("===================Login=================\n");
    printf("Username : ");
    scanf("%s", username);
    printf("\nPassword : ");
    scanf("%s", password);

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
            playlist(pilihHome, &pilihPlaylist);
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