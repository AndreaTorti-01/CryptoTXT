#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sha256.c"

#define MAX_INFO_LEN 30
#define PSWHASH_LEN 64

typedef struct { //credentials structure
	char info1[MAX_INFO_LEN + 1];
	char info2[MAX_INFO_LEN + 1];
	char info3[MAX_INFO_LEN + 1];
} credential;

void hash(char[], char[]); //functions
void clr_brand ();
void crypt();
void decrypt();

FILE* fileptr; //global variables
FILE* tempfileptr;
char filename[20];
credential credential_one;

int main() {
	char buffer[3 * MAX_INFO_LEN + 3], ch, psw[20], pswhash[PSWHASH_LEN];
	int action, r, temp;

	do{
		clr_brand ();
		printf("Select the desired action:\n\n0 :Create new file\n1 :Open existing file\n\n>> ");
		scanf("%d", &action); //the user can create a new file or open an existing one
		clr_brand ();
		switch (action) {
			
			case 0:
				printf("Enter the new file name without extension: "); //asks for new filename
				scanf("%s", filename);
				strcat(filename, ".crp"); //adds the extension
				fileptr = fopen(filename, "w"); //tries to create the file
				if (fileptr == NULL) { //checks that the file was created
					printf ("Critical error: cannot run the software");
					exit(1);
				}

				printf("\nEnter the new password: "); //asks for password
				scanf("%s", psw);
				hash(psw, pswhash); //generates the hash
				fprintf(fileptr, "%s", pswhash); //writes the password hash at the beginning of the new file
				fprintf(fileptr, "\n");
				fclose(fileptr);
				crypt(); //crypts the new file
				break;

			case 1:
				do {
					printf("Enter the file name without extension: "); //asks for filename to open
					scanf("%s", filename);
					strcat(filename, ".crp"); //adds the extension
					fileptr = fopen(filename, "r"); //tries to open the file
					if (fileptr == NULL) { //checks that the file was opened
						printf ("The file %s does not exist\n", filename);
					}
					fclose(fileptr);
				} while (fileptr == NULL); //asks again until a valid filename is entered

				temp=0;
				printf("\nEnter the password: ");
				do {
					scanf("%s", psw); //asks for the password
					hash(psw, pswhash); //generates the hash
					decrypt(); //decrypts the file
					fileptr = fopen(filename, "r"); //opens the file
					if (fileptr == NULL) {
						printf ("Critical error: cannot run the software");
						exit(1);
					}
					fgets(buffer, PSWHASH_LEN+1, fileptr); //fills the buffer with the first line of the file
					if (strcmp(buffer, pswhash)==0) temp=1; //if the buffer corresponds to the password hash, the entered one was correct
					fclose(fileptr);
					crypt(); //crypts the file again
					if (temp==0) printf ("\nIncorrect password, try again: ");
				} while (temp==0); //re-tries the process until the correct password is entered
				break;

			default:
				printf("Please enter a valid value\n\n");
		}
	} while (action!=0 && action!=1);

	clr_brand();

	do {
		printf("Select the desired action:\n\n0 :Insert new credentials\n1 :Delete existing credentials\n2 :Search existing credentials\n3: Print existing credentials list\n4: Quit\n\n>> ");
		scanf("%d", &action); //the user can insert new credentials, delete existing ones, search for some, print the whole file or quit
		clr_brand();
		switch (action) {

			case 0:
				printf("Enter info 1 of 3 of the new credential: "); //asks for the new infos
				scanf("%s", credential_one.info1);
				printf("Enter info 2 of 3 of the new credential: ");
				scanf("%s", credential_one.info2);
				printf("Enter info 3 of 3 of the new credential: ");
				scanf("%s", credential_one.info3);

				decrypt();
				fileptr = fopen(filename, "a"); //opens the file placing the file pointer at the end of it
				if (fileptr == NULL) { //checks for correct opening
					printf ("Critical error: cannot run the software");
					exit(1);
				}
				fprintf (fileptr, "%s %s %s\n", credential_one.info1, credential_one.info2, credential_one.info3); //prints the new credentials and a newline char at the end of the file
				fclose (fileptr);
				printf("\nNew credential registered successfully\n");
				crypt();
				break;

			case 1:
				printf("Insert one info of the credential to delete: ");
				scanf("%s", credential_one.info1); //asks for an info of the line to delete
				decrypt();
				fileptr = fopen(filename, "r"); //opens the file in read-only mode
				if (fileptr == NULL) { //checks for correct opening
					printf ("Critical error: cannot run the software");
					exit(1);
				}
				r = -1; //sets the "found" line to -1
				temp = 1; //sets temp as "line not found"

				while ((fgets(buffer, 92, fileptr)) != NULL) { //writes the whole line in the buffer and sets the pointer to the next one
					r = r + 1; //r is the number of the line where it finds the info
					if (strstr(buffer, credential_one.info1) != NULL) { //tries to find the info in the current line
						temp = 0; //if found, informs the software about it
						break;
					}
				}

				if (temp == 0) { //if the line is found starts the second section
					rewind(fileptr); //rewinds the file pointer, since the file is still open
					tempfileptr = fopen("temp.crp", "w"); //creates a temp file
					if (tempfileptr == NULL) { //checks for correct creation
						printf ("Critical error: cannot run the software");
						exit(1);
					}
					ch = 'a';
					while (ch != EOF) { //copies the whole file to temp...
						ch = getc(fileptr);
						if (temp != r && ch != EOF) putc(ch, tempfileptr); //...except the line to delete (r)
						if (ch == '\n') temp++;
					}
					fclose(fileptr);
					fclose(tempfileptr);
					remove(filename);
					rename("temp.crp", filename); //closes the files, deletes the original one and renames temp to filetemp
					printf("\nCredential deleted successfully\n");
				}
				else {
					printf("\nCredential not found\n");
					fclose(fileptr); //takes care of closing the file anyway in case of failure to find the info
				}
				crypt();
				break;

			case 2:
				printf("Insert one info of the credential to find: ");
				scanf("%s", credential_one.info1); //asks for the info to find
				decrypt();
				fileptr = fopen(filename, "r"); //opens the file in read mode
				if (fileptr == NULL) { //checks for correct opening
					printf ("Critical error: cannot run the software");
					exit(1);
				}

				temp = 1; //"line not found!"
				while ((fgets(buffer, 92, fileptr)) != NULL) { //finds the line
					if (strstr(buffer, credential_one.info1) != NULL) {
						printf("\n%s", buffer); //literally prints the line
						temp = 0; //"line found!"
						break;
					}
				}
				fclose(fileptr);
				if (temp == 1) printf("\nCredential not found\n");
				crypt();
				break;

			case 3:
				decrypt();
				fileptr = fopen(filename, "r"); //opens the file in read mode
				if (fileptr == NULL) { //checks for correct opening
					printf ("Critical error: cannot run the software");
					exit(1);
				}

				fgets(buffer, 92, fileptr); //fills the buffer with the first line to avoid printing the password
				while (!feof(fileptr)) {
					if (fgets(buffer, 92, fileptr) != NULL) { //and then the other lines...
						printf("%s", buffer); //...before printing them
					}
				}
				fclose(fileptr);
				crypt();
				break;

			case 4:
				break; //does nothing, prepares to quit

			default:
				printf("Please enter a valid value");
		}
		printf("\n\n\n");
	} while (action != 4); //repeats until 4 is entered
	system("cls");
	return 0;
}

void hash(char psw[], char pswhash[]) {

	BYTE buf[SHA256_BLOCK_SIZE];
	SHA256_CTX ctx;
	int i;
	sha256_init(&ctx);
	sha256_update(&ctx, psw, strlen(psw));
	sha256_final(&ctx, buf);
    for(i=0; i<SHA256_BLOCK_SIZE; i++)
		sprintf(&pswhash[i*2], "%02X", buf[i]);
}

void clr_brand (){ //branding to put on top of the terminal
	system("cls");
	printf("Cryptotxt \n---------\n\n");
}

void crypt (){ //crypting function
	/*char ch;

	fileptr = fopen(filename, "r"); //opens the file in read mode
	tempfileptr = fopen("temp.crp", "w"); //creates a temp file

	if (tempfileptr == NULL || fileptr == NULL) { //checks for correct openings
		printf ("Critical error: cannot run the software");
		exit(1);
	}

	ch = fgetc(fileptr);
	while (!feof(fileptr)) { //repeats until the pointer reaches the end of the file (ch != eof does not work because eof is actually a char that can appear when the file is crypted)
		ch -= pswhash; //subtracts the hash from every single char in the file
		fputc(ch, tempfileptr);
		ch = fgetc(fileptr);
	}

	fclose(fileptr);
	fclose(tempfileptr);
	remove(filename);
	rename("temp.crp", filename); //deletes original file, renames temp file.*/
}

void decrypt (){ //same as crypting but with the plus (+) instead of the minus (-)
	/*char ch;
	
	fileptr = fopen(filename, "r");
	tempfileptr = fopen("temp.crp", "w");

	if (tempfileptr == NULL || fileptr == NULL) {
		printf ("Critical error: cannot run the software");
		exit(1);
	}
	
	ch = fgetc(fileptr);
	while (!feof(fileptr)) {
		ch += pswhash;
		fputc(ch, tempfileptr);
		ch = fgetc(fileptr);
	}

	fclose(fileptr);
	fclose(tempfileptr);
	remove(filename);
	rename("temp.crp", filename);*/
}