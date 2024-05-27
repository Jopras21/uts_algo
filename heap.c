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

struct minHeap {
    struct playlist **songs;
    int size;
    int capacity;
};

bool cekAkun(struct akun *head, const char *username, const char *password);

void readDatabase(struct akun **head);
void addAcc(struct akun **head, const char *username, const char *password);
void insertBST(struct playlist **root, struct playlist *newNode);
void inOrderTraversal(struct playlist *root);
void searchSong(struct playlist *root, char keyword[50]);
void displayExistingPlaylists();
void deletePlaylistFromFile(const char *filename);
void addSongToPlaylist(const char *playlistFilename, struct minHeap *heap);
void removeSongFromPlaylist(const char *playlistFilename, int songNumber, struct minHeap *heap);
void displayPlaylist(const char *playlistFilename, struct playlist *displayOrder[], int *songCount);
void playlist(int pilihhome, int *pilihPlaylist, struct playlist *root, struct minHeap *heap);
void playSong(struct playlist *song);
void playSongFromPlaylist(struct playlist *displayOrder[], int songCount, int songNumber);
void quickSort(struct playlist *arr[], int left, int right);
void bubbleSort(struct playlist *arr[], int n);
void buildArrayFromBST(struct playlist *root, struct playlist *arr[], int *index);
void displaySortedPlaylist(struct playlist *root, int sortOption);
void displaySongs(struct minHeap *heap);
void heapify(struct minHeap *heap, int i);
void insertHeap(struct minHeap *heap, struct playlist *song);
struct playlist* deleteMinHeap(struct minHeap *heap);
struct minHeap* createMinHeap(int capacity);

void createData(FILE *file, char baris[1000]) {
    while (fgets(baris, sizeof(baris), file) != NULL) {
        printf("%s", baris);
    }
}

void login(int kondisi, char username[30], char password[30], struct akun *akunHead) {
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
            login(kondisi, username, password, akunHead);
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

void createPlayList(struct minHeap *heap) {
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
        insertHeap(heap, node);

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
    printf("Enter keyword to search: ");
    scanf("%s", keyword);

    bool found = false;
    struct playlist *current = root;

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
        if (strcmp(keyword, current->judul) < 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    if (!found) {
        printf("No songs found with \"%s\" in the judul.\n");
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

void addSongToPlaylist(const char *playlistFilename, struct minHeap *heap) {
    FILE *file = fopen(playlistFilename, "a");
    if (file == NULL) {
        printf("Gagal membuka playlist.\n");
        return;
    }

    struct playlist *newSong = (struct playlist*)malloc(sizeof(struct playlist));

    printf("Masukkan judul lagu: ");
    fflush(stdin);
    fgets(newSong->judul, sizeof(newSong->judul), stdin);
    strtok(newSong->judul, "\n");

    printf("Masukkan nama penyanyi: ");
    fgets(newSong->penyanyi, sizeof(newSong->penyanyi), stdin);
    strtok(newSong->penyanyi, "\n");

    printf("Masukkan album: ");
    fgets(newSong->album, sizeof(newSong->album), stdin);
    strtok(newSong->album, "\n");

    printf("Masukkan tahun rilis: ");
    scanf("%d", &newSong->tahun);

    fprintf(file, "%s#%s#%s#%d\n", newSong->judul, newSong->penyanyi, newSong->album, newSong->tahun);
    insertHeap(heap, newSong);

    fclose(file);

    printf("Lagu berhasil ditambahkan ke playlist.\n");
}

void removeSongFromPlaylist(const char *playlistFilename, int songNumber, struct minHeap *heap) {
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
            deleteMinHeap(heap);
        } else {
            printf("Gagal mengubah nama file.\n");
        }
    } else {
        printf("Gagal menghapus playlist.\n");
    }
}

void displayPlaylist(const char *playlistFilename, struct playlist *displayOrder[], int *songCount) {
    FILE *file = fopen(playlistFilename, "r");
    if (file != NULL) {
        char buffer[100];
        struct minHeap *tempHeap = createMinHeap(100);  // Create a temporary heap to sort songs

        // Read songs from file and insert into heap
        while (fgets(buffer, sizeof(buffer), file)) {
            struct playlist *song = (struct playlist*)malloc(sizeof(struct playlist));
            sscanf(buffer, "%[^#]#%[^#]#%[^#]#%d", song->judul, song->penyanyi, song->album, &song->tahun);
            insertHeap(tempHeap, song);
        }

        // Display songs in ASCII order using min-heap
        printf("===============================================================================================================================\n");
        printf("| %-2s | %-30s | %-30s | %-43s | %-7s |\n", "No", "Judul", "Penyanyi", "Album", "Tahun");
        printf("===============================================================================================================================\n");

        int no = 1;
        *songCount = 0;
        while (tempHeap->size > 0) {
            struct playlist *song = deleteMinHeap(tempHeap);
            printf("| %-2d | %-30s | %-30s | %-43s | %-7d |\n", no++, song->judul, song->penyanyi, song->album, song->tahun);
            displayOrder[*songCount] = song;
            (*songCount)++;
        }

        printf("===============================================================================================================================\n");
        fclose(file);
        free(tempHeap->songs);
        free(tempHeap);
    } else {
        printf("Gagal membuka playlist '%s'.\n", playlistFilename);
    }
}

void playlist(int pilihhome, int *pilihPlaylist, struct playlist *root, struct minHeap *heap) {
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
                struct playlist *displayOrder[100];
                int songCount = 0;
                displayPlaylist(playlistNames[*pilihPlaylist - 1], displayOrder, &songCount);

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
                        playSongFromPlaylist(displayOrder, songCount, songNumber);
                        break;
                    }

                    case 2: {
                        addSongToPlaylist(playlistNames[*pilihPlaylist - 1], heap);
                        break;
                    }

                    case 3: {
                        int songNumber;
                        printf("Masukkan nomor lagu yang ingin dihapus dari playlist: ");
                        scanf("%d", &songNumber);
                        removeSongFromPlaylist(playlistNames[*pilihPlaylist - 1], songNumber, heap);
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
                return;
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

void playSongFromPlaylist(struct playlist *displayOrder[], int songCount, int songNumber) {
    if (songNumber > 0 && songNumber <= songCount) {
        struct playlist *songToPlay = displayOrder[songNumber - 1];
        if (songToPlay) {
            printf("\nLagu Sedang Diputar:\n");
            printf("Title: %s\nArtist: %s\nAlbum: %s\nYear: %d\n", songToPlay->judul, songToPlay->penyanyi, songToPlay->album, songToPlay->tahun);
        } else {
            printf("Nomor lagu tidak ditemukan di playlist.\n");
        }
    } else {
        printf("Nomor lagu tidak valid.\n");
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

void displaySongs(struct minHeap *heap) {
    printf("=========================================\n");
    printf("        Displaying All Songs         \n");
    printf("=========================================\n");
    
    for (int i = 0; i < heap->size; i++) {
        struct playlist *song = heap->songs[i];
        printf("Title   : %s\n", song->judul);
        printf("Artist  : %s\n", song->penyanyi);
        printf("Album   : %s\n", song->album);
        printf("Year    : %d\n", song->tahun);
        printf("-----------------------------------------\n");
    }
}

// Heap functions
struct minHeap* createMinHeap(int capacity) {
    struct minHeap *heap = (struct minHeap*)malloc(sizeof(struct minHeap));
    heap->songs = (struct playlist**)malloc(capacity * sizeof(struct playlist*));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void heapify(struct minHeap *heap, int i) {
    int smallest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < heap->size && strcmp(heap->songs[left]->judul, heap->songs[smallest]->judul) < 0) {
        smallest = left;
    }

    if (right < heap->size && strcmp(heap->songs[right]->judul, heap->songs[smallest]->judul) < 0) {
        smallest = right;
    }

    if (smallest != i) {
        struct playlist *temp = heap->songs[i];
        heap->songs[i] = heap->songs[smallest];
        heap->songs[smallest] = temp;
        heapify(heap, smallest);
    }
}

void insertHeap(struct minHeap *heap, struct playlist *song) {
    if (heap->size == heap->capacity) {
        printf("Heap overflow\n");
        return;
    }

    int i = heap->size++;
    heap->songs[i] = song;

    while (i != 0 && strcmp(heap->songs[i]->judul, heap->songs[(i - 1) / 2]->judul) < 0) {
        struct playlist *temp = heap->songs[i];
        heap->songs[i] = heap->songs[(i - 1) / 2];
        heap->songs[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

struct playlist* deleteMinHeap(struct minHeap *heap) {
    if (heap->size <= 0) {
        return NULL;
    }

    if (heap->size == 1) {
        return heap->songs[--heap->size];
    }

    struct playlist *root = heap->songs[0];
    heap->songs[0] = heap->songs[--heap->size];
    heapify(heap, 0);

    return root;
}

int main() {
    char username[30];
    char password[30];
    char keyword[50];
    int kondisi;
    int pilihHome;
    int pilihPlaylist;

    struct akun *akunHead = NULL;
    struct minHeap *heap = createMinHeap(100);

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
        insertHeap(heap, node);
    }
    fclose(file);
    char logo[] = "zlogo.txt";
    char baris[1000];

    file = fopen(logo, "r");
    createData(file, baris);
    printf("\n");
    fclose(file);

    login(kondisi, username, password, akunHead);

    struct playlist *displayOrder[100];
    int songCount = 0;

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
            playlist(pilihHome, &pilihPlaylist, root, heap);
        } else if (pilihHome == 3) {
            system("cls");
            displaySongs(heap);
        } else if (pilihHome == 4) {
            createPlayList(heap);
        } else if (pilihHome == 5) {
            if (songCount > 0) {
                int songNumber;
                printf("Masukkan nomor lagu yang ingin diputar: ");
                scanf("%d", &songNumber);
                playSongFromPlaylist(displayOrder, songCount, songNumber);
            } else {
                printf("Belum ada lagu yang ditampilkan. Pilih opsi 'Choose Playlist' terlebih dahulu.\n");
            }
        } else if (pilihHome == 6) {
            free(heap->songs);
            free(heap);
            return 0;
        }
    } while (pilihHome != 6);

    free(heap->songs);
    free(heap);
    return 0;
}
