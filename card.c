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
//#define NELEMS(x) (sizeof(x) / sizeof(x[0])
#define TRUE 1
#define FALSE 0
#define CARDNUMBER_CAP 16+1
#define NAME_CAP 24+1


//the card number - a 16 digit character array
//        the issue date and the expiry date - both in the format mmyy
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
//            printf("Buffer = %s\n", buffer);
            buffer[strcspn(buffer, "\n")] = 0;
//            printf("Buffer without \\n = %s\n", buffer);
            if(1 == sscanf(buffer, "%[^\n]s", name)) {
                printf("name = %s\n", name);
                if(strlen(name) <= 24) {
                    strncpy(custCard->name, name, 24);
//                    printf("From struct = %s\n", custCard->name);
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
        printf("Valid date\n");
        custCard->issueDate = (mm * 100) + yy;
        return TRUE;
    }
    else {
        return FALSE;
    }
}

int issueExpiryAssign(card *custCard, int mm, int yy) {
    if ((mm >=1 && mm <= 12) && (yy >= 21)) {
        printf("Valid date\n");
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
                    // This could and probably should be a switch case statement but i'm lazy
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
                    printf("Invalid issue date. Please enter in format mmyy\n");
                }
            }
        }



}

void createNewList(card *pFirstNode, card *pLastNode) {

    card *pNewCard = (card *) malloc(sizeof(card));

    pNewCard->next = NULL;

    getCardNumber(pNewCard);
    getDate(pNewCard, 10);
    getDate(pNewCard, 20);
    getCVV(pNewCard);
    getHolderName(pNewCard);

    pFirstNode = pLastNode = pNewCard;
}

void createNewCard(card *pFirstNode, card *pLastNode) {

    if(pFirstNode == NULL) {
        createNewList(pFirstNode, pLastNode);
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


    }

}

void printCards(card *pFirstNode) {
    card *pCards = pFirstNode;

    printf("All cards are as follows\n");
    while(pCards != NULL) {
        printf("Card Number: %s", pCards->cardNumber);
        printf("Expiry Date: %01d", pCards->expiryDate);
        printf("Issue Date: %d", pCards->issueDate);
        printf("CVV: %d", pCards->securityCode);
        printf("Card Holder: %s", pCards->name);
        printf("--------------------------------");
        pCards = pCards->next;
    }
}


int printMenu() {

    char buffer[BUFFER_SIZE];
    int choice;
    CLEAR_SCREEN;
    printf("|-------Menu-------|\n");
    printf("| 1) Create Card   |\n");
    printf("| 2) View Card     |\n");
    printf("| 3) Exit          |\n");
    printf("|------------------|\n");

    while(TRUE) {
        printf("Enter a choice: ");
        if (fgets(buffer, sizeof(buffer), stdin)) {
            if (1 == sscanf(buffer, "%d", &choice)) {
                if(choice <= 3 && choice >= 1) {
                    return choice;
                }
                else {
                    printf("Invalid option\n");
                }

            }
        }
    }

    return 0;
}




int main() {

    card *pFirstNode = NULL;
    card *pLastNode = NULL;

    createNewCard(pFirstNode, pLastNode);
    printf("Value in first node %d", pFirstNode->issueDate);
//    createNewCard(pFirstNode, pLastNode);
//    createNewCard(pFirstNode, pLastNode);

    printCards(pFirstNode);


    return 0;


}

