#include <stdio.h>
#include <string.h>
unsigned long hash(char*);
void clr_brand ();
void crypt(char*, unsigned long);
void decrypt(char*, unsigned long);

int main() {
    FILE *fileptr;
    FILE *tempfileptr;
    char info1[30], info2[30], info3[30], buffer[92], filename[20], psw[20], ch;
    int action, r, temp;
    unsigned long pswhash;

	tempfileptr = fopen("temp.crp", "w");	/*checks if the program has the permission to create temp file*/
    if (tempfileptr == NULL) {
        printf("Cannot run the software: contact the developer\n\n");
        fclose(tempfileptr);
        return 0;
    }
    fclose(tempfileptr);
    remove("temp.crp");

    do{
    	clr_brand ();
    	printf("Select the desired action:\n\n0 :Create new file\n1 :Open existing file\n\n");
    	scanf("%d", &action);
    	clr_brand ();
		switch (action){
			case 0:
				printf("Enter the new file name without extension: ");	/*creates the new file, writing the password at the beginning before the first cryptation*/
		        scanf("%s", filename);
		        strcat(filename, ".crp");
		        fileptr = fopen(filename, "w");
				printf("\nEnter the new password: ");
				scanf("%s", psw);
				pswhash = hash(psw);
		    	fprintf(fileptr, "%s\n", psw);
		    	fclose(fileptr);
		    	crypt(filename, pswhash);
				break;

			case 1:
				do {	/*asks the user the file name to find until a valid one is entered*/
			        printf("Enter the file name without extension: ");
			        scanf("%s", filename);
			        strcat(filename, ".crp");
			        fileptr = fopen(filename, "r");
			        if (fileptr == NULL) {
			            clr_brand();
			            printf("Invalid file name: the file does not exist\n\n");
			        }
			        fclose(fileptr);
			    } while (fileptr == NULL);

			    temp=0;		/*asks the user the password until the first line of the file corresponds to it*/
			    printf("\nEnter the password: ");
			    do {
				    scanf("%s", psw);
				    pswhash = hash(psw);
					decrypt(filename, pswhash);
					fileptr = fopen(filename, "r");
					fgets(buffer, 20, fileptr);
					strcat(psw, "\n");
					if (strcmp(buffer, psw)==0) temp=1;
					fclose(fileptr);
					crypt(filename, pswhash);
					if (temp==0) printf ("\nIncorrect password, try again: ");
				} while (temp==0);
			    break;

			default:
	            printf("Please enter a valid value\n\n");
		}
	} while (action!=0 && action!=1);

	clr_brand();

    do {
        printf("Select the desired action:\n\n0 :Insert new credentials\n1 :Delete existing credentials\n2 :Search existing credentials\n3: Print existing credentials list\n4: Quit\n\n");
        scanf("%d", &action);
        clr_brand();
        switch (action) {

            case 0:
                printf("Enter info 1 of 3 of the new credential: ");	/*asks for the new infos*/
                scanf("%s", info1);
                printf("Enter info 2 of 3 of the new credential: ");
                scanf("%s", info2);
                printf("Enter info 3 of 3 of the new credential: ");
                scanf("%s", info3);

                decrypt(filename, pswhash);
                fileptr = fopen(filename, "a");			/*appends the new infos to the file*/
                fprintf (fileptr, "%s %s %s\n", info1, info2, info3);
                fclose (fileptr);
                crypt(filename, pswhash);
                printf("\nNew credential registered successfully\n");
                break;

            case 1:
                printf("Insert one info of the credential to delete: ");
                do {
	                scanf("%s", info1);
	                if (strcmp(info1, psw)==0) printf ("\nCannot delete the password, enter the info again: ");
	        	} while (strcmp(info1, psw)==0);	/*asks for info to find and compares the line with the password*/
	        	decrypt(filename, pswhash);
	        	fileptr = fopen(filename, "r");
                rewind(fileptr);
                r = -1;
                temp = 1;
                while ((fgets(buffer, 92, fileptr)) != NULL) {
                    r = r + 1;
                    if (strstr(buffer, info1) != NULL) {
                        temp = 0;				/*finds the line to delete in the database, if found sets temp=0*/
                        break;
                    }
                }
	            if (temp == 0) {				/*creates temp.crp with all chars copied except the selected line*/
                    rewind(fileptr);
                    tempfileptr = fopen("temp.crp", "w");
                    rewind (tempfileptr);
                    ch = 'a';
                    while (ch != EOF) {
                        ch = getc(fileptr);
                        if (temp != r && ch != EOF) putc(ch, tempfileptr);
                        if (ch == '\n') temp++;
                    }
                    fclose(fileptr);
                    fclose(tempfileptr);
                    remove(filename);			/*deletes the original file and renames temp.crp*/
                    rename("temp.crp", filename);
                    printf("\nCredential deleted successfully\n");
                }
                else {
                	printf("\nCredential not found\n");
					fclose(fileptr);
				}
				crypt(filename, pswhash);
                break;

            case 2:
                printf("Insert one info of the credential to find: ");
                do {
	                scanf("%s", info1);
	                if (strcmp(info1, psw)==0) printf ("\nCannot search for the password, enter the info again: ");
	        	} while (strcmp(info1, psw)==0);	/*asks for info to find and compares the line with the password*/
                temp = 1;
                r =  - 1;
                decrypt(filename, pswhash);
                fileptr = fopen(filename, "r");
                rewind (fileptr);
                while ((fgets(buffer, 92, fileptr)) != NULL) {
                    r = r + 1;
                    if (strstr(buffer, info1) != NULL) {
                        printf("\n%s", buffer);			/*finds the line in the database, if found it prints it and sets temp=0*/
                        temp=0;
                        break;
                    }
                }
                fclose(fileptr);
                if (temp==1) printf("\nCredential not found\n");
                crypt(filename, pswhash);
                break;

            case 3:
            	decrypt(filename, pswhash);
                fileptr = fopen(filename, "r");		/*prints all the lines except the first one, containing the password*/
                rewind (fileptr);
                fgets(buffer, 92, fileptr);
                while (!feof(fileptr)) {
                    if (fgets(buffer, 92, fileptr) != NULL) {
                        printf("%s", buffer);
                    }
                }
                fclose(fileptr);
                crypt(filename, pswhash);
                break;

            case 4:
                break;

            default:
                printf("Please enter a valid value");
        }
        printf("\n\n\n");
    } while (action != 4);
    return 0;
}

unsigned long hash(char*str) {
    unsigned long hash = 5381;
    int c;
    while (c =  *str++) {
        hash = ((hash << 5) + hash) + c;
    } return hash;
}

void clr_brand (){
	system("cls");
    printf("Cryptotxt\n---------\n\n");
}

void crypt (char*filename, unsigned long pswhash){
	FILE *fileptr;
    FILE *tempfileptr;
	char ch;

	fileptr = fopen(filename, "r");
	tempfileptr = fopen("temp.crp", "w");
	rewind(fileptr);
	rewind(tempfileptr);

	ch = fgetc(fileptr);
    while (ch != EOF) {
        ch += pswhash;
        fputc(ch, tempfileptr);
        ch = fgetc(fileptr);
    }

	fclose(fileptr);
    fclose(tempfileptr);
    remove(filename);
    rename("temp.crp", filename);
}

void decrypt (char*filename, unsigned long pswhash){
	FILE *fileptr;
    FILE *tempfileptr;
	char ch;
	
	fileptr = fopen(filename, "r");
	tempfileptr = fopen("temp.crp", "w");
	rewind(fileptr);
	rewind(tempfileptr);
	
	ch = fgetc(fileptr);
    while (ch != EOF) {
        ch -= pswhash;
        fputc(ch, tempfileptr);
        ch = fgetc(fileptr);
    }

	fclose(fileptr);
    fclose(tempfileptr);
    remove(filename);
    rename("temp.crp", filename);
}
