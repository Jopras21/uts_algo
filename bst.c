#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


struct playlist {
    char judul[50];
    char penyanyi[50];
    char album[50];
    int tahun;
    struct playlist *left, *right;
};

// Struktur untuk akun
struct akun {
    char username[30];
    char password[30];
    struct akun *next;
};

// Struktur untuk Queue
typedef struct QueueNode {
    struct playlist *song;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue {
    QueueNode *front, *rear;
} Queue;

// Fungsi Queue
Queue* createQueue() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

bool isEmptyQueue(Queue *q) {
    return (q->front == NULL);
}

void enqueue(Queue *q, struct playlist *song) {
    QueueNode *temp = (QueueNode*)malloc(sizeof(QueueNode));
    temp->song = song;
    temp->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }
    q->rear->next = temp;
    q->rear = temp;
}

struct playlist* dequeue(Queue *q) {
    if (isEmptyQueue(q))
        return NULL;
    QueueNode *temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL)
        q->rear = NULL;
    struct playlist *song = temp->song;
    free(temp);
    return song;
}

void playQueue(Queue *q) {
    while (!isEmptyQueue(q)) {
        struct playlist *song = dequeue(q);
        playSong(song);
    }
}

// Fungsi lainnya
bool cekAkun(struct akun *head, const char *username, const char *password);
void login(int *kondisi, char username[30], char password[30]);
void createData(FILE *file, char baris[1000]);
void createCustomPlaylist(char playlistNames[][50], int numPlaylists);
void createPlaylist(struct playlist **playlistRoot);
struct playlist *insertBST(struct playlist *root, char *judul, char *penyanyi, char *album, int tahun);
struct playlist *searchSong(struct playlist *root, char keyword[50]);
void readDatabase(struct akun **head);
void addAcc(struct akun **head, const char *username, const char *password);
bool cekAkun(struct akun *head, const char *username, const char *password);
void displayAllSongs(struct playlist *root);
void playSong(struct playlist *song);

// Implementasi fungsi lainnya
void login(int *kondisi, char username[30], char password[30]) {
    struct akun *akunHead = NULL;
    readDatabase(&akunHead);
    printf("1. Belum punya akun? buat sekarang!\n");
    printf("2. Sudah punya akun? login sekarang!\n");
    printf("pilihan :");
    scanf("%d", kondisi);

    if (*kondisi == 1) {
        printf("================Register==============\n");
        printf("Username : ");
        scanf("%s", username);
        printf("\nPassword : ");
        scanf("%s", password);
        addAcc(&akunHead, username, password);
    } else if (*kondisi == 2) {
        printf("================Login==============\n");
        printf("Username : ");
        scanf("%s", username);
        printf("\nPassword : ");
        scanf("%s", password);
        if (!cekAkun(akunHead, username, password)) {
            printf("Username atau password salah.\n");
            exit(0);
        }
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

void createPlaylist(struct playlist **playlistRoot) {
    int songLength = 100;
    char titleLength = 50;
    char artistLength = 50;
    char playlistNames[50][50];
    int numPlaylists;
    int numSongs;
    FILE *file;

    struct playlist playlist[songLength];

    printf("1. Buat playlist baru\n");
    printf("2. Tambahkan lagu ke playlist yang ada\n");
    printf("Pilihan: ");
    int choice;
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Masukkan jumlah playlist yang ingin dibuat: ");
        scanf("%d", &numPlaylists);
        getchar();
        for (int i = 0; i < numPlaylists; i++) {
            printf("Masukkan nama playlist ke-%d: ", i + 1);
            fgets(playlistNames[i], sizeof(playlistNames[i]), stdin);
            strtok(playlistNames[i], "\n");
        }
        createCustomPlaylist(playlistNames, numPlaylists);
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
        printf("Masukkan judul lagu ke-%d: ", i + 1);
        fgets(playlist[i].judul, titleLength, stdin);
        strtok(playlist[i].judul, "\n");

        printf("Masukkan nama penyanyi untuk lagu ke-%d: ", i + 1);
        fgets(playlist[i].penyanyi, artistLength, stdin);
        strtok(playlist[i].penyanyi, "\n");

        printf("Masukkan nama album untuk lagu ke-%d: ", i + 1);
        fgets(playlist[i].album, titleLength, stdin);
        strtok(playlist[i].album, "\n");

        printf("Masukkan tahun rilis untuk lagu ke-%d: ", i + 1);
        scanf("%d", &playlist[i].tahun);
        getchar();

        fprintf(file, "%s#%s#%s#%d\n", playlist[i].judul, playlist[i].penyanyi, playlist[i].album, playlist[i].tahun);
        *playlistRoot = insertBST(*playlistRoot, playlist[i].judul, playlist[i].penyanyi, playlist[i].album, playlist[i].tahun);
    }

    printf("Playlist berhasil ditambahkan ke file.\n");

    fclose(file);
}

struct playlist *insertBST(struct playlist *root, char *judul, char *penyanyi, char *album, int tahun) {
    if (root == NULL) {
        struct playlist *node = (struct playlist *)malloc(sizeof(struct playlist));
        strcpy(node->judul, judul);
        strcpy(node->penyanyi, penyanyi);
        strcpy(node->album, album);
        node->tahun = tahun;
        node->left = node->right = NULL;
        return node;
    }
    if (strcmp(judul, root->judul) < 0)
        root->left = insertBST(root->left, judul, penyanyi, album, tahun);
    else if (strcmp(judul, root->judul) > 0)
        root->right = insertBST(root->right, judul, penyanyi, album, tahun);
    return root;
}

struct playlist *searchSong(struct playlist *root, char keyword[50]) {
    if (root == NULL)
        return NULL;
    if (strstr(root->judul, keyword) != NULL || strstr(root->penyanyi, keyword) != NULL || strstr(root->album, keyword) != NULL) {
        printf("Title: %s\n", root->judul);
        printf("Artist: %s\n", root->penyanyi);
        printf("Album: %s\n", root->album);
        printf("Year: %d\n", root->tahun);
        printf("-----------------------------------------\n");
        return root;
    }
    struct playlist *leftSearch = searchSong(root->left, keyword);
    if (leftSearch != NULL) return leftSearch;
    return searchSong(root->right, keyword);
}

void readDatabase(struct akun **head) {
    FILE *file = fopen("database.txt", "r");
    if (file == NULL) {
        printf("Error membuka file database.\n");
        return;
    }

    while (!feof(file)) {
        struct akun *node = (struct akun *)malloc(sizeof(struct akun));
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
    struct akun *node = (struct akun *)malloc(sizeof(struct akun));
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

bool cekAkun(struct akun *head, const char *username, const char *password) {
    struct akun *temp = head;
    while (temp != NULL) {
        if (strcmp(temp->username, username) == 0 && strcmp(temp->password, password) == 0)
            return true;
        temp = temp->next;
    }
    return false;
}

void displayAllSongs(struct playlist *root) {
    if (root != NULL) {
        displayAllSongs(root->left);
        printf("Title: %s\n", root->judul);
        printf("Artist: %s\n", root->penyanyi);
        printf("Album: %s\n", root->album);
        printf("Year: %d\n", root->tahun);
        printf("-----------------------------------------\n");
        displayAllSongs(root->right);
    }
}

void playSong(struct playlist *song) {
    if (song != NULL) {
        printf("Playing song: %s by %s from album %s (%d)\n", song->judul, song->penyanyi, song->album, song->tahun);
    }
}

int main() {
    struct playlist *playlistRoot = NULL;
    int pilihHome, kondisi = 0;
    char username[30], password[30], keyword[50];
    Queue *songQueue = createQueue();

    FILE *file = fopen("playlist.txt", "r");
    if (file == NULL) {
        printf("Error membuka file.\n");
        return 1;
    }

    char judul[50], penyanyi[50], album[50];
    int tahun;
    while (fscanf(file, "%49[^#]#%49[^#]#%49[^#]#%d\n", judul, penyanyi, album, &tahun) == 4) {
        playlistRoot = insertBST(playlistRoot, judul, penyanyi, album, tahun);
    }
    fclose(file);

    login(&kondisi, username, password);

    do {
        printf("1. Search Song\n");
        printf("2. Create Playlist\n");
        printf("3. Display All Songs\n");
        printf("4. Add Song to Queue\n");
        printf("5. Play Songs from Queue\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &pilihHome);

        switch (pilihHome) {
            case 1:
                printf("Enter a keyword to search: ");
                scanf("%s", keyword);
                searchSong(playlistRoot, keyword);
                break;
            case 2:
                createPlaylist(&playlistRoot);
                break;
            case 3:
                displayAllSongs(playlistRoot);
                break;
            case 4:
                printf("Enter song title to add to queue: ");
                scanf("%s", keyword);
                struct playlist *songToAdd = searchSong(playlistRoot, keyword);
                if (songToAdd != NULL) {
                    enqueue(songQueue, songToAdd);
                    printf("Song added to queue.\n");
                } else {
                    printf("Song not found.\n");
                }
                break;
            case 5:
                playQueue(songQueue);
                break;
            case 6:
                printf("Thank you for using our service.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (pilihHome != 6);

    return 0;
}