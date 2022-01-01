#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


#ifdef _WIN32
#define CLEAR_SCREEN system("cls")
#else
#define CLEAR_SCREEN system("clear")
#endif

#define BUFFER_SIZE 256
#define TRUE 1
#define FALSE 0
#define CARDNUMBER_CAP (19+1)
#define NAME_CAP (24+1)





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



int luhnValid(char *number) {

    int numDigits = (int) strlen(number);
    int finSum = 0;
    int isSecond = FALSE;
    int i;

    for(i=numDigits - 1; i >= 0; i--) {
        int d = number[i] - '0';
        if(isSecond) {
            d = d * 2;
        }

        finSum += d / 10;
        finSum += d % 10;

        isSecond = !isSecond;
    }
    return (finSum % 10 == 0);


}


void getCardNumber(card *custCard) {
    char buffer[BUFFER_SIZE];
    char number[CARDNUMBER_CAP];
    int isValid = FALSE;

    while(!isValid) {
        printf("Enter card number: ");

        if (fgets(buffer, sizeof(buffer), stdin)) {
            fflush(stdin);
            buffer[strcspn(buffer,"\n")] = 0;

            if (1 == sscanf(buffer, "%[^\n]s", number)) {
                if (validateCardNumber(number) && ((strlen(number) == 13) || (strlen(number) == 16) || (strlen(number) == 19))) {
		            number[CARDNUMBER_CAP] = '\0';
                    if(luhnValid(number)) {
                        strncpy(custCard->cardNumber, number, CARDNUMBER_CAP);
                        isValid = TRUE;
                    }
                    else {
                        printf("Invalid card number.\n");
                    }


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
// This could actually Y2K, but it's a personal project so I don't care.
    if ((mm >=1 && mm <= 12) && (yy >= 21) && (yy <= 99)) {
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

void createDeveloperCard() {
    card *pNewCard = (card *) malloc(sizeof(card));

    pNewCard->next = NULL;
    strncpy(pNewCard->cardNumber, "4929055935563696\0", 17);
    pNewCard->issueDate = 718;
    pNewCard->expiryDate = 825;
    pNewCard->securityCode = 666;
    strncpy(pNewCard->name, "Developer\0", 10);

    pFirstNode = pLastNode = pNewCard;
    printf("Added developer card\n");
    printf("Press ENTER to continue: ");
    getchar();
}


void createNewCard() {

    if(pFirstNode == NULL) {
        createNewList(); // First element of linked list
    }
    else {
        card *pNewCard = (card *) malloc(sizeof(card));
        getCardNumber(pNewCard);
        getDate(pNewCard, 10);
        getDate(pNewCard, 20);
        getCVV(pNewCard);
        getHolderName(pNewCard);
        // This is true when we are inserting the second list item
        if(pFirstNode == pLastNode) {
            pFirstNode->next = pNewCard;
            pNewCard->next = NULL;
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

void convertDateToStr(int date, char *buff) {
    int i;
    int write = sprintf(buff, "%d", date);
    
    // Create a string that is in format: XXxx
    if(write == 3) {
        for(i = 3; i >= 1; --i)
            buff[i] = buff[i - 1];

        buff[0] = '0';
    }
    
    // Simply shift digits in cells [2] and [3] along one, so we can add a '/'
    buff[4] = buff[3];
    buff[3] = buff[2];
    buff[2] = '/';
    
    buff[5] = '\0';
}

void printCard(card *pCardIterator) {
    char issStr[6];
    char expStr[6];

    convertDateToStr(pCardIterator->issueDate, issStr);
    convertDateToStr(pCardIterator->expiryDate, expStr);

    printf("Card Number: %s\n", pCardIterator->cardNumber);
    printf("Issue Date: %s\n", issStr);
    printf("Expiry Date: %s\n", expStr);
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
        int counter = 0;
        printf("All cards are as follows\n");
        printf("--------------------------------\n");
        while (pCards != NULL) {
            printCard(pCards);
            pCards = pCards->next;
            counter++;
        }
        printf("Press ENTER to continue: ");
        getchar();
    }
}



void searchCardNum() {

    char buffer[BUFFER_SIZE];
    char number[CARDNUMBER_CAP];
    int isValid = FALSE;
    while(!isValid) {
        printf("Enter the card number to search for: ");
        if(fgets(buffer, sizeof(buffer), stdin)) {
            buffer[strcspn(buffer, "\n")] = 0;
            if(strlen(buffer) > 19) {
                printf("Invalid card number\n");
            }
            else {
                if(1 == sscanf(buffer, "%s", number)) {
                    number[CARDNUMBER_CAP] = '\0';
                    isValid = TRUE;
                }
                else {
                    printf("Error getting number input\n");
                }
            }

        }
        else {
            printf("Error getting number input\n");
        }
    }
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
        printf("Sorry, no people with that card number could be found.\n");
    }
    printf("Press ENTER to continue: ");
    getchar();
}




void searchName() {

    char buffer[BUFFER_SIZE];
    char name[NAME_CAP];
    int isValid = FALSE;
    while(!isValid) {
        printf("Enter the name to search for: ");
        if (fgets(buffer, sizeof(buffer), stdin)) {
            buffer[strcspn(buffer, "\n")] = 0;
            if (strlen(buffer) > 24) {
                printf("Invalid input\n");
            }
            else {
                if(1 == sscanf(buffer, "%s", name)) {
                    name[NAME_CAP] = '\0';
                    isValid = TRUE;
                }

            }
        }
    }



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
    printf("Press ENTER to continue: ");
    getchar();
}

void searchExpiryDate() {
    char buffer[BUFFER_SIZE];
    char expStr[6];
    int mm, yy;
    int date;
    int isValid = FALSE;
    int numFound = 0;

    while(!isValid) {
        printf("Enter the expiry date to search for (mm/yy): ");
        if (fgets(buffer, sizeof(buffer), stdin)) {
            if(2 == sscanf(buffer, "%d/%d", &mm, &yy)) {
                if ((mm >=1 && mm <= 12) && (yy >= 21) && (yy <= 99)) {
                    isValid = TRUE;
                }
                else {
                    printf("Invalid date\n");
                }
            }

        }
    }

    date = (mm * 100) + yy;
    card *pCardIterator = pFirstNode;

    while (pCardIterator != NULL) {
        int equality = (date == pCardIterator->expiryDate);
        if (equality) {
            convertDateToStr(date, expStr);
            printf("Card with the expiry date %s was found!\n", expStr);
            printCard(pCardIterator);
            numFound++;
        }
        pCardIterator = pCardIterator->next;
    }
    if(numFound == 0) {
        printf("Sorry, no people with that expiry date could be found.\n");
    }
    printf("Press ENTER to continue: ");
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
                if(choice <= menuOptions && choice >= 0) {
                    return choice;
                }
                else {
                    printf("Invalid option\n");
                }

            }
        }
    }
}


void searchMenu() {

    char buffer[BUFFER_SIZE];
    int choice;

    while (TRUE) {
        CLEAR_SCREEN;
        printf("|----Search Menu----|\n");
        printf("| 1) Card Number    |\n");
        printf("| 2) Search Name    |\n");
        printf("| 3) Expiry Date    |\n");
        printf("| 4) Exit           |\n");
        printf("|-------------------|\n");

        printf("Enter a search option: ");
        if (fgets(buffer, sizeof(buffer), stdin)) {
            fflush(stdin);
            buffer[strcspn(buffer, "\n")] = 0;

            if (1 == sscanf(buffer, "%d", &choice)) {
                switch (choice) {
                    case 1:
                        searchCardNum();
                        break;
                    case 2:
                        searchName();
                        break;
                    case 3:
                        searchExpiryDate();
                    case 4:
                        return;
                    default:
                        printf("Invalid option\n");
                        break;

                }
            }
        }
    }
}


int main() {

    // Manually change this line for debug mode
    // PROD: remove this line and IF statement
    int DEBUG = FALSE;

    if(DEBUG) {
        createDeveloperCard();
    }

    int menu_choice;

    while(TRUE) {

        menu_choice = printMenu();

        switch(menu_choice) {
            case 0:
                createDeveloperCard();
                break;
            case 1:
                createNewCard();
                break;
            case 2:
                searchMenu();
//                searchName("Jack");
//                searchCardNum("1111111111111111");
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


