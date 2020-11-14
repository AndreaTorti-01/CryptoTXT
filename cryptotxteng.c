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
        printf("Enter the filename without extension: ");
        scanf("%s", &filename);
        strcat(filename, ".txt");
        fileptr = fopen(filename, "r");
        if (fileptr == NULL) {
            system("cls");
            printf("Cryptotxt v2.0\n--------------\n\n");
            printf("Invalid filename: the file does not exist\n\n");
            fclose(fileptr);
        }
    } while (fileptr == NULL);

    tempfileptr = fopen("temp.txt", "w+");
    if (tempfileptr == NULL) {
        printf("Cannot run the software: contact the developer\n\n");
        fclose(fileptr);
        return 0;
    }

    printf("\nEnter the password: ");
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
        printf("Select the desired action:\n\n0 :Insert new credentials\n1 :Delete existing credentials\n2 :Search existing credentials\n3: Print existing credentials list\n4: Quit\n");
        scanf("%d", &azione);
        system("cls");
        printf("Cryptotxt v2.0\n--------------\n\n");
        switch (azione) {

            case 0:
                fileptr = fopen(filename, "a");
                printf("Enter info 1 of 3 of the new credential: ");
                scanf("%s", &info1);
                printf("Enter info 2 of 3 of the new credential: ");
                scanf("%s", &info2);
                printf("Enter info 3 of 3 of the new credential: ");
                scanf("%s", &info3);
                r =  - 1;
                do {
                    r = r + 1; /*scrive su una nuova linea i dati inseriti*/
                }
                 while ((fgets(buffer, 92, fileptr)) != NULL && strstr(buffer, info1) != NULL);
                fprintf(fileptr, "%s %s %s\n", info1, info2, info3);
                fclose(fileptr);
                printf("\nNew credential registered successfully\n");
                break;

            case 1:
                fileptr = fopen(filename, "r+");
                printf("Insert one info of the credential to delete: ");
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
                    printf("\nCredential deleted successfully\n");
                }
                 else {
                    printf("\nCredential not found\n");
                    fclose(fileptr);
                }
                break;

            case 2:
                fileptr = fopen(filename, "r");
                printf("Insert one info of the credential to search for: ");
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
                    printf("\nCredential not found\n");
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
                printf("Please enter a valid value");
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
