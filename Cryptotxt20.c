#include <stdio.h>
#include <string.h>
unsigned long hash(unsigned char *str);

int main() {
    FILE *fileptr;
    FILE *tempfileptr;
    char info1[30], info2[30], info3[30], buffer[92], filename[20], psw[20];
    int azione, r, temp;
    unsigned long pswhash;
    char ch;

    printf("Cryptotxt v2.0\n--------------\n\n");

    do {
        printf("inserire il nome del file senza estensione: ");
        scanf("%s", &filename);
        strcat(filename, ".txt");
        fileptr = fopen(filename, "r");
        if (fileptr == NULL) {
            system("cls");
            printf("Cryptotxt v2.0\n--------------\n\n");
            printf("Il file che si vuole aprire non esiste\n\n");
            fclose(fileptr);
        }
    } while (fileptr == NULL);

    tempfileptr = fopen("temp.txt", "w+");
    if (tempfileptr == NULL) {
        printf("Impossibile eseguire il programma: contattare lo sviluppatore\n\n");
        fclose(fileptr);
        return 0;
    }

    printf("\ninserire la password: ");
    scanf("%s", &psw);
    pswhash = hash(psw);
    ch = fgetc(fileptr);
    while (ch != EOF) {
        ch = ch - pswhash;
        fputc(ch, tempfileptr);
        ch = fgetc(fileptr);
    }
    system("cls");
    printf("Cryptotxt v2.0\n--------------\n\n");

    rewind(tempfileptr);
    while (!feof(tempfileptr)) {
        if (fgets(buffer, 92, tempfileptr) != NULL) {
            printf("%s", buffer);
        }
    }
    printf("\n\n");
    fclose(fileptr);
    fclose(tempfileptr);
    remove(filename);
    rename("temp.txt", filename);
    system("pause");
    system("cls");
    printf("Cryptotxt v2.0\n--------------\n\n");

    do {
        printf("scegliere l'azione desiderata:\n\n0 :inserire nuove credenziali\n1 :eliminare credenziali esistenti\n2 :ricercare credenziali esistenti\n3: stampa la lista di credenziali presenti\n4: esci\n");
        scanf("%d", &azione);
        system("cls");
        printf("Cryptotxt v2.0\n--------------\n\n");
        switch (azione) {

            case 0:
                fileptr = fopen(filename, "a");
                printf("inserire informazione 1 della nuova credenziale: ");
                scanf("%s", &info1);
                printf("inserire informazione 2 della nuova credenziale: ");
                scanf("%s", &info2);
                printf("inserire informazione 3 della nuova credenziale: ");
                scanf("%s", &info3);
                r =  - 1;
                do {
                    r = r + 1; /*scrive su una nuova linea i dati inseriti*/
                }
                 while ((fgets(buffer, 92, fileptr)) != NULL && strstr(buffer, info1) != NULL);
                fprintf(fileptr, "%s %s %s\n", info1, info2, info3);
                fclose(fileptr);
                printf("\ncredenziale registrata correttamente\n");
                break;

            case 1:
                fileptr = fopen(filename, "r+");
                printf("inserire una informazione della credenziale da eliminare: ");
                scanf("%s", &info1);
                rewind(fileptr);
                r =  - 1;
                temp = 1;
                while ((fgets(buffer, 92, fileptr)) != NULL) {
                    r = r + 1; /*trova nel database il dato richiesto*/
                    if (strstr(buffer, info1) != NULL) {
                        temp = 0;
                        break;
                    }
                }
                if (temp == 0) {
                     /*se è stato trovato crea un file temporaneo che contiene tutte le linee copiate eccetto quella incriminata*/
                    rewind(fileptr);
                    tempfileptr = fopen("tempfileptr.txt", "w");
                    ch = 'a';
                    while (ch != EOF) {
                        ch = getc(fileptr);
                        if (temp != r && ch != EOF) {
                            putc(ch, tempfileptr);
                        }
                        if (ch == '\n') {
                            temp++;
                        }
                    }
                    fclose(fileptr);
                    fclose(tempfileptr);
                    remove(filename); /*elimina l'originale e rinomina il nuovo file*/
                    rename("tempfileptr.txt", filename);
                    printf("\ncredenziale eliminata correttamente\n");
                }
                 else {
                    printf("\ncredenziale non trovata\n");
                    fclose(fileptr);
                }
                break;

            case 2:
                fileptr = fopen(filename, "r");
                printf("inserire una informazione della credenziale da ricercare: ");
                scanf("%s", &info1);
                temp = 1;
                r =  - 1;
                while ((fgets(buffer, 92, fileptr)) != NULL) {
                    r = r + 1;
                    if (strstr(buffer, info1) != NULL) {
                        temp = 0;
                        break;
                    }
                }
                if (temp == 0) {
                    printf("\n%s", buffer);
                }
                 else {
                    printf("\ncredenziale non trovata\n");
                }
                fclose(fileptr);
                break;

            case 3:
                fileptr = fopen(filename, "r");
                while (!feof(fileptr)) {
                    if (fgets(buffer, 92, fileptr) != NULL) {
                        printf("%s", buffer);
                    }
                }
                fclose(fileptr);
                break;

            case 4:
                break;

            default:
                printf("inserire un valore valido");
        }
        printf("\n\n\n");
    }
    while (azione != 4)
        ;
    system("cls");
    printf("Cryptotxt v2.0\n--------------\n\n");
    fileptr = fopen(filename, "r");
    tempfileptr = fopen("temp.txt", "w");
    ch = fgetc(fileptr);
    while (ch != EOF) {
        ch = ch + pswhash;
        fputc(ch, tempfileptr);
        ch = fgetc(fileptr);
    }
    fclose(fileptr);
    fclose(tempfileptr);
    remove(filename);
    rename("temp.txt", filename);
    return 0;
}

unsigned long hash(unsigned char *str) {
    unsigned long hash = 5381;
    unsigned long c;
    while (c =  *str++) {
        hash = ((hash << 5) + hash) + c;
    } return hash;
}
