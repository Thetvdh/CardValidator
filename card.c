#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


#ifdef _WIN32
#define CLEAR_SCREEN system("cls")
#endif

#ifdef linux
#define CLEAR_SCREEN system("clear")
#endif

#define BUFFER_SIZE 256
#define TRUE 1
#define FALSE 0
#define CARDNUMBER_CAP (16+1)
#define NAME_CAP (24+1)


//the card number - a 16 digit character array
//        the issue date and the expiry date - both in the format mm/yy
//        the security code - a 3 digit integer
//the cardholders name - a string of not more than 24 characters


typedef struct card {
    char cardNumber[CARDNUMBER_CAP];
    int issueDate;
    int expiryDate;
    int securityCode;
    char name[NAME_CAP];

    struct card *next;

} card;



void getHolderName(card *custCard) {

    char buffer[BUFFER_SIZE];
    char name[24];
    int isValid = FALSE;

    while(!isValid) {

        printf("Enter account holders name (24 Characters): ");
        if(fgets(buffer, sizeof(buffer), stdin)) {
            fflush(stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            if(1 == sscanf(buffer, "%[^\n]s", name)) {
                if(strlen(name) <= 24) {
                    strncpy(custCard->name, name, 24);
                    isValid = TRUE;
                }
                else {
                    printf("Invalid name\n");
                }

            }
            else {
                printf("Invalid name\n");
            }

        }
    }


}

int getNumberDigits(int number) {

    int count = 0;
    while(number != 0) {
        number = number / 10;
        count++;
    }

    return count;

}

void getCVV(card *custCard) {

    char buffer[BUFFER_SIZE];
    int cvv;
    int isValid = FALSE;
    while(!isValid) {
        printf("Input CVV: ");
        if (fgets(buffer, sizeof(buffer), stdin)) {
            fflush(stdin);
            if (1 == sscanf(buffer, "%d", &cvv)) {
                if (getNumberDigits(cvv) == 3) {
                    custCard->securityCode = cvv;
                    isValid = TRUE;
                }
                else {
                    printf("Invalid CVV\n");
                }
            }
            else {
                printf("Invalid CVV\n");
            }
        }
    }
}

int validateCardNumber(char *number) {
    int i;
    for(i=0; i < 16; i++){
        if(!isdigit(number[i])){
            return FALSE;
        }
    }
    return TRUE;

}

void getCardNumber(card *custCard) {
    char buffer[BUFFER_SIZE];
    char number[CARDNUMBER_CAP];
    int isValid = FALSE;

    while(!isValid) {
        printf("Enter card number (16 digits): ");

        if (fgets(buffer, sizeof(buffer), stdin)) {
            fflush(stdin);
            buffer[strcspn(buffer,"\n")] = 0;

            if (1 == sscanf(buffer, "%[^\n]s", number)) {
                if (validateCardNumber(number) && strlen(number) <= 16) {
		            number[CARDNUMBER_CAP] = '\0';

                    strncpy(custCard->cardNumber, number, CARDNUMBER_CAP);

                    isValid = TRUE;
                }
                else {
                    printf("Invalid card number.\n");
                }
            }
        }
    }
}

int issueDateAssign(card *custCard, int mm, int yy) {
    if ((mm >=1 && mm <= 12) && (yy <= 21)) {
        custCard->issueDate = (mm * 100) + yy;
        return TRUE;
    }
    else {
        return FALSE;
    }
}

int issueExpiryAssign(card *custCard, int mm, int yy) {
    if ((mm >=1 && mm <= 12) && (yy >= 21)) {
        custCard->expiryDate = (mm * 100) + yy;
        return TRUE;
    }
    else {
        return FALSE;
    }
}



void getDate(card *custCard, int type) {

        int isValid = FALSE;
        char buffer[BUFFER_SIZE];
        int mm, yy;

        while(!isValid) {
            printf("Enter %s date in mm/yy: ", type == 10  ? "issue" : "expiry");
            if (fgets(buffer, sizeof(buffer), stdin)) {
                fflush(stdin);
                if (2 == sscanf(buffer, "%d/%d", &mm, &yy)) {
                    // This could and probably should be a switch case statement, but I'm lazy
                    if(type == 10) {
                        if(issueDateAssign(custCard, mm, yy)) {
                            isValid = TRUE;
                        }

                    }
                    else if(type == 20) {
                        if(issueExpiryAssign(custCard, mm, yy)) {
                            isValid = TRUE;
                        }

                    }

                }
                else {
                    printf("Invalid issue date. Please enter in format mm/yy\n");
                }
            }
        }



}


static card *pFirstNode = NULL;
static card *pLastNode = NULL;

void createNewList() {

    card *pNewCard = (card *) malloc(sizeof(card));

    pNewCard->next = NULL;

    getCardNumber(pNewCard);
    getDate(pNewCard, 10);
    getDate(pNewCard, 20);
    getCVV(pNewCard);
    getHolderName(pNewCard);

    pFirstNode = pLastNode = pNewCard;
    printf("Card successfully created\n");
    printf("Press ENTER to continue: ");
    getchar();
}

void createNewCard() {

    if(pFirstNode == NULL) {
        createNewList();
    }
    else {
        card *pNewCard = (card *) malloc(sizeof(card));
        getCardNumber(pNewCard);
        getDate(pNewCard, 10);
        getDate(pNewCard, 20);
        getCVV(pNewCard);
        getHolderName(pNewCard);
        if(pFirstNode == pLastNode) {
            pFirstNode->next = pNewCard;
            pLastNode = pNewCard;
        }
        else {
            pLastNode->next = pNewCard;
            pNewCard->next = NULL;
            pLastNode = pNewCard;
        }
        printf("Card successfully created\n");
        printf("Press ENTER to continue: ");
        getchar();


    }
}

void printCard(card *pCardIterator) {
    printf("Card Number: %s\n", pCardIterator->cardNumber);
    printf("Expiry Date: %03d\n", pCardIterator->expiryDate); // TODO fix number padding here
    printf("Issue Date: %03d\n", pCardIterator->issueDate); // TODO fix number padding here
    printf("CVV: %d\n", pCardIterator->securityCode);
    printf("Card Holder: %s\n", pCardIterator->name);
    printf("--------------------------------\n");
}

void printCards() {
    card *pCards = pFirstNode;
    if(pCards == NULL) {
        printf("Sorry. There are no cards stored\n");
        printf("Press ENTER to continue: ");
        getchar();
    }
    else {
        printf("All cards are as follows\n");
        printf("--------------------------------\n");
        while (pCards != NULL) {
            printCard(pCards);
            pCards = pCards->next;
        }
        printf("Press ENTER to continue: ");
        getchar();
    }
}



void searchCardNum(char * number) {
    int numFound = 0;
    card *pCardIterator = pFirstNode;
    while(pCardIterator != NULL) {
        int equality = strncmp(pCardIterator->cardNumber, number, sizeof(pCardIterator->cardNumber));
        if(!equality) {
            printf("Card with the number %s was found\n", number);
            printCard(pCardIterator);
            numFound++;
        }
        pCardIterator = pCardIterator->next;
    }
    if(numFound == 0) {
        printf("Sorry, no people with that name could be found.\n");
    }
    getchar();
}




void searchName(char * name) {
    card *pCardIterator = pFirstNode;
    int numFound = 0;
    while (pCardIterator != NULL) {
        int equality = strncmp(pCardIterator->name, name, sizeof(pCardIterator->name));
        if (!equality) {
            printf("Card with the name %s was found!\n", name);
            printCard(pCardIterator);
            numFound++;
        }
        pCardIterator = pCardIterator->next;
    }
    if(numFound == 0) {
        printf("Sorry, no people with that name could be found.\n");
    }
    getchar();
}

int printMenu() {

    char buffer[BUFFER_SIZE];
    int choice;
    int menuOptions = 4; // Change this value based on how many options are available in the menu
    CLEAR_SCREEN;
    // INFO menu does not get reprinted in CLion run. No clue why.
    printf("|-------Menu-------|\n");
    printf("| 1) Create Card   |\n");
    printf("| 2) Search Card   |\n");
    printf("| 3) Show all Cards|\n");
    printf("| 4) Exit          |\n");
    printf("|------------------|\n");

    while(TRUE) {
        printf("Enter a choice: ");
        if (fgets(buffer, sizeof(buffer), stdin)) {
            fflush(stdin);
            if (1 == sscanf(buffer, "%d", &choice)) {
                if(choice <= menuOptions && choice >= 1) {
                    return choice;
                }
                else {
                    printf("Invalid option\n");
                }

            }
        }
    }
}
/*
// TODO make this shite work
void searchMenu() {

    char buffer[BUFFER_SIZE];
    int choice;

    printf("Enter a search option: ");
    if(fgets(buffer, sizeof(buffer), stdin)) {
        fflush(stdin);
        if(1 == sscanf(buffer, "%d", &choice)) {

            }
        }
}
*/

int main() {


    int menu_choice;

    while(TRUE) {

        menu_choice = printMenu();

        switch(menu_choice) {
            case 1:
                createNewCard();
                break;
            case 2:
                searchName("Jack");
                searchCardNum("1111111111111111");
                break;
            case 3:
                printCards();
                break;
            case 4:
                CLEAR_SCREEN;
                printf("Thank you for using card validator!\n");
                exit(EXIT_SUCCESS);
            default:
                printf("Incorrect value has been entered. Please try again.\n");
        }


    }




}


