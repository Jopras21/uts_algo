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
    struct playlist *left, *right;
};

struct akun {
    char username[30];
    char password[30];
    struct akun *next;
};

bool cekAkun(struct akun *head, const char *username, const char *password);

void readDatabase(struct akun **head);
void addAcc(struct akun **head, const char *username, const char *password);
void insertBST(struct playlist **root, struct playlist *newNode);
void inOrderTraversal(struct playlist *root);
void searchSong(struct playlist *root, char keyword[50]);
void displayExistingPlaylists();
void deletePlaylistFromFile(const char *filename);
void addSongToPlaylist(const char *playlistFilename);
void removeSongFromPlaylist(const char *playlistFilename, int songNumber);
void displayPlaylist(const char *playlistFilename);
void playlist(int pilihhome, int *pilihPlaylist, struct playlist *root);
void playSong(struct playlist *song);
void playSongFromPlaylist(char *playlistName, int songNumber);
void quickSort(struct playlist *arr[], int left, int right);
void bubbleSort(struct playlist *arr[], int n);
void buildArrayFromBST(struct playlist *root, struct playlist *arr[], int *index);
void displaySortedPlaylist(struct playlist *root, int sortOption);
void displaySongs(struct playlist *root);

struct playlist* binarySearch(struct playlist *arr[], int low, int high, const char *keyword);
struct playlist* interpolationSearch(struct playlist *arr[], int n, const char *keyword);

void createData(FILE *file, char baris[1000]) {
    while (fgets(baris, sizeof(baris), file) != NULL) {
        printf("%s", baris);
    }
}

login(int kondisi, char username[30], char password[30]) {
    struct akun *akunHead = NULL;
    readDatabase(&akunHead);
    printf("1. Belum punya akun? buat sekarang!\n");
    printf("2. Sudah punya akun? login sekarang!\n");
    printf("pilihan :");
    scanf("%d", &kondisi);

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

void createCustomPlaylist() {
    char playlistName[50];
    printf("Masukkan nama playlist baru: ");
    scanf("%s", playlistName);

    FILE *file = fopen(strcat(playlistName, ""), "a");
    if (file == NULL) {
        printf("Gagal membuat playlist.\n");
        return;
    }

    printf("Playlist '%s' berhasil dibuat.\n", playlistName);
    fclose(file);
}

void createPlayList() {
    int numSongs;
    FILE *file;

    struct playlist *root = NULL;

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
        fgets(node->judul, sizeof(node->judul), stdin);
        strtok(node->judul, "\n");

        printf("Masukkan nama penyanyi untuk lagu ke-%d: ", i + 1);
        fgets(node->penyanyi, sizeof(node->penyanyi), stdin);
        strtok(node->penyanyi, "\n");

        printf("Masukkan nama album untuk lagu ke-%d: ", i + 1);
        fgets(node->album, sizeof(node->album), stdin);
        strtok(node->album, "\n");

        printf("Masukkan tahun rilis untuk lagu ke-%d: ", i + 1);
        scanf("%d", &node->tahun);
        getchar();

        node->left = NULL;
        node->right = NULL;

        insertBST(&root, node);

        fprintf(file, "%s#%s#%s#%d\n", node->judul, node->penyanyi, node->album, node->tahun);
    }

    printf("Playlist berhasil ditambahkan ke file.\n");

    fclose(file);
}

void insertBST(struct playlist **root, struct playlist *newNode) {
    if (*root == NULL) {
        *root = newNode;
    } else {
        if (strcmp(newNode->judul, (*root)->judul) < 0) {
            insertBST(&(*root)->left, newNode);
        } else {
            insertBST(&(*root)->right, newNode);
        }
    }
}

void inOrderTraversal(struct playlist *root) {
    if (root != NULL) {
        inOrderTraversal(root->left);
        printf("Title   : %s\n", root->judul);
        printf("Artist  : %s\n", root->penyanyi);
        printf("Album   : %s\n", root->album);
        printf("Year    : %d\n", root->tahun);
        printf("-----------------------------------------\n");
        inOrderTraversal(root->right);
    }
}

void searchSong(struct playlist *root, char keyword[50]) {
    struct playlist *arr[100];
    int count = 0;

    buildArrayFromBST(root, arr, &count);

    int searchOption;
    printf("Choose searching option:\n");
    printf("1. Binary Search\n");
    printf("2. Interpolation Search\n");
    printf("Pilihan: ");
    scanf("%d", &searchOption);
    getchar();

    struct playlist *result = NULL;

    printf("Enter keyword to search: ");
    scanf("%s", keyword);

    if (searchOption == 1) {
        result = binarySearch(arr, 0, count - 1, keyword);
    } else if (searchOption == 2) {
        result = interpolationSearch(arr, count, keyword);
    }

    if (result != NULL) {
        printf("=========================================\n");
        printf("      Search Results for \"%s\"       \n", keyword);
        printf("=========================================\n");
        printf("Title: %s\n", result->judul);
        printf("Artist: %s\n", result->penyanyi);
        printf("Album: %s\n", result->album);
        printf("Year: %d\n", result->tahun);
        printf("-----------------------------------------\n");
    } else {
        printf("No songs found with \"%s\" in the title.\n", keyword);
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
    FILE *file = fopen(playlistFilename, "a");
    if (file == NULL) {
        printf("Gagal membuka playlist.\n");
        return;
    }

    struct playlist newSong;

    printf("Masukkan judul lagu: ");
    fflush(stdin);
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

    fprintf(file, "%s#%s#%s#%d\n", newSong.judul, newSong.penyanyi, newSong.album, newSong.tahun);

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
            printf("| %-2d | %-30s | %-30s | %-43s | %-7d |\n", no, judul, penyanyi, album, tahun);
            no++;
        }
        printf("===============================================================================================================================\n");
        fclose(file);
    } else {
        printf("Gagal membuka playlist '%s'.\n", playlistFilename);
        return;
    }
}

void playlist(int pilihhome, int *pilihPlaylist, struct playlist *root) {
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
                    struct playlist *current = root;
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
                        current = (count < songNumber) ? current->right : current->left;
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
                            current = current->right;
                            if (current == NULL) {
                                current = root;
                                while (current->left) current = current->left;
                            }
                            playSong(current);
                            sleep(1);
                        } else if (kontrol == 2) {
                            current = current->left;
                            if (current == NULL) {
                                current = root;
                                while (current->right) current = current->right;
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

void quickSort(struct playlist *arr[], int left, int right) {
    int i = left, j = right;
    struct playlist *tmp;
    char *pivot = arr[(left + right) / 2]->judul;

    while (i <= j) {
        while (strcmp(arr[i]->judul, pivot) > 0) // Descending order
            i++;
        while (strcmp(arr[j]->judul, pivot) < 0) // Descending order
            j--;
        if (i <= j) {
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
            j--;
        }
    }

    if (left < j)
        quickSort(arr, left, j);
    if (i < right)
        quickSort(arr, i, right);
}

void bubbleSort(struct playlist *arr[], int n) {
    int i, j;
    struct playlist *tmp;
    for (i = 0; i < n-1; i++) {
        for (j = 0; j < n-i-1; j++) {
            if (strcmp(arr[j]->judul, arr[j+1]->judul) > 0) {
                tmp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = tmp;
            }
        }
    }
}

void buildArrayFromBST(struct playlist *root, struct playlist *arr[], int *index) {
    if (root == NULL) return;
    buildArrayFromBST(root->left, arr, index);
    arr[(*index)++] = root;
    buildArrayFromBST(root->right, arr, index);
}

void displaySortedPlaylist(struct playlist *root, int sortOption) {
    struct playlist *arr[100];
    int count = 0;

    buildArrayFromBST(root, arr, &count);

    if (sortOption == 1) {
        quickSort(arr, 0, count - 1);
    } else if (sortOption == 2) {
        bubbleSort(arr, count);
    }

    for (int i = 0; i < count; i++) {
        printf("Title   : %s\n", arr[i]->judul);
        printf("Artist  : %s\n", arr[i]->penyanyi);
        printf("Album   : %s\n", arr[i]->album);
        printf("Year    : %d\n", arr[i]->tahun);
        printf("-----------------------------------------\n");
    }
}

void displaySongs(struct playlist *root) {
    printf("=========================================\n");
    printf("        Displaying All Songs (Unsorted)         \n");
    printf("=========================================\n");
    inOrderTraversal(root);

    int sortOption;
    printf("\n=========================================\n");
    printf("        Displaying All Songs (Sorted)         \n");
    printf("=========================================\n");
    printf("\nChoose sorting option:\n");
    printf("1. Sort Z-A\n");
    printf("2. Bubble Sort A-Z\n");
    printf("Pilihan: ");
    scanf("%d", &sortOption);
    printf("\n=========================================\n");
    printf("        Displaying All Songs (Sorted)         \n");
    printf("=========================================\n");
    displaySortedPlaylist(root, sortOption);
}

struct playlist* binarySearch(struct playlist *arr[], int low, int high, const char *keyword) {
    if (high >= low) {
        int mid = low + (high - low) / 2;

        if (strstr(arr[mid]->judul, keyword) != NULL)
            return arr[mid];

        if (strcmp(arr[mid]->judul, keyword) > 0)
            return binarySearch(arr, low, mid - 1, keyword);

        return binarySearch(arr, mid + 1, high, keyword);
    }
    return NULL;
}

struct playlist* interpolationSearch(struct playlist *arr[], int n, const char *keyword) {
    int low = 0, high = n - 1;

    while (low <= high && strcmp(keyword, arr[low]->judul) >= 0 && strcmp(keyword, arr[high]->judul) <= 0) {
        if (low == high) {
            if (strstr(arr[low]->judul, keyword) != NULL) return arr[low];
            return NULL;
        }

        int pos = low + (((double)(high - low) /
              (strcmp(arr[high]->judul, arr[low]->judul))) *
              (strcmp(keyword, arr[low]->judul)));

        if (strstr(arr[pos]->judul, keyword) != NULL)
            return arr[pos];

        if (strcmp(arr[pos]->judul, keyword) < 0)
            low = pos + 1;
        else
            high = pos - 1;
    }
    return NULL;
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
        printf("Error membuka file.\n");
        return 1;
    }

    struct playlist *root = NULL;

    while (!feof(file)) {
        struct playlist *node = (struct playlist*)malloc(sizeof(struct playlist));
        if (fscanf(file, "%49[^#]#%49[^#]#%49[^#]#%d\n", node->judul, node->penyanyi, node->album, &node->tahun) != 4) {
            free(node);
            break;
        }

        node->left = NULL;
        node->right = NULL;
        insertBST(&root, node);
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
        printf(" Hello, %s\n", username);
        printf("1. Search by Keyword\n");
        printf("2. Choose Playlist  \n");
        printf("3. Display All Song \n");
        printf("4. Create playlist \n");
        printf("5. Play Song        \n");
        printf("6. Logout\n");
        printf("Pilihan : ");
        scanf("%d", &pilihHome);

        if (pilihHome == 1) {
            searchSong(root, keyword);
        } else if (pilihHome == 2) {
            playlist(pilihHome, &pilihPlaylist, root);
        } else if (pilihHome == 3) {
            system("cls");;
            displaySongs(root);
        } else if (pilihHome == 4) {
            createPlayList();
        } else if (pilihHome == 5) {
            struct playlist *current = root;
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
                    current = current->right;
                    if (current == NULL) {
                        current = root;
                        while (current->left) current = current->left;
                    }
                    playSong(current);
                    sleep(1);
                } else if (kontrol == 2) {
                    current = current->left;
                    if (current == NULL) {
                        current = root;
                        while (current->right) current = current->right;
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
        } else if (pilihHome == 6) {
            return 0;
        }
    } while (pilihHome != 6);

    return 0;
}
