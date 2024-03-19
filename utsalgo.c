#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct playlist {
    char judul[50];
    char penyanyi[50];
    char album[50];
    int tahun;
    struct playlist *next, *prev;
};

void createAccount(char *username, char *password) {
    system("cls");
    printf("----------------------Create Account---------------------\n");
    printf("Username: "); scanf("%s", username);
    printf("Password: "); scanf("%s", password);
}

int login(const char *savedUsername, const char *savedPassword) {
    char username[50];
    char password[50];
    system("cls");
    printf("----------------------Login---------------------\n");
    printf("Username: "); scanf("%s", username);
    printf("Password: "); scanf("%s", password);
    if(strcmp(username, savedUsername) == 0 && strcmp(password, savedPassword) == 0) {
        return 1; // Login berhasil-
    }
    return 0; // Login gagal
}

void menu(int *pilihhome) {
    system("cls");
    printf("----------------------Home---------------------\n");
    printf("2. Search         :\n");
    printf("1. Choose Playlist:\n");
    printf("3. Create Playlist:\n");
    printf("4. Delete Playlist:\n");
    printf("Pilihan : "); scanf("%d", pilihhome);
}

void playlist(int pilihhome, int *pilihplaylist) {
    if(pilihhome == 1) {
        system("cls");
        printf("--------------------Choose Playlist---------------------\n");
        printf("1. Playlist 1\n");
        printf("2. Playlist 2\n");
        printf("3. Playlist 3\n");
        printf("4. Delete Playlist \n");
        printf("Pilihan : ");
        scanf("%d", pilihplaylist);

        if(*pilihplaylist == 1 || *pilihplaylist == 2 || *pilihplaylist == 3){
            system("cls");
            printf("--------------------Playlist %d---------------------\n", *pilihplaylist);
            printf("1. Play Song  :\n");
            printf("2. Add Song   :\n");
            printf("3. Delete Song:\n");
        }
    }
}

int main() {
    int pilihhome;
    int pilihplaylist;
    char savedUsername[50] = "";
    char savedPassword[50] = "";

    int loggedIn = 0;
    while (!loggedIn) {
        system("cls");
        printf("----------------------Home---------------------\n");
        printf("1. Create Account\n");
        printf("2. Login\n");
        printf("Pilihan : "); scanf("%d", &pilihhome);
        if (pilihhome == 1) {
            createAccount(savedUsername, savedPassword);
            loggedIn = 1; // Setelah membuat akun, langsung masuk ke mode login
        } else if (pilihhome == 2) {
            loggedIn = login(savedUsername, savedPassword);
            if (!loggedIn) {
                printf("Username atau password salah. Silakan coba lagi.\n");
                system("pause");
            }
        }
    }

    while (loggedIn) {
        menu(&pilihhome);
        if (pilihhome == 1) {
            playlist(pilihhome, &pilihplaylist);
        }
    }

    return 0;
}
