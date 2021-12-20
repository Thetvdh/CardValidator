#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define BUFFER_SIZE 256
//#define NELEMS(x) (sizeof(x) / sizeof(x[0])
#define TRUE 1
#define FALSE 0

//the card number - a 16 digit character array
//        the issue date and the expiry date - both in the format mmyy
//        the security code - a 3 digit integer
//the cardholders name - a string of not more than 24 characters

typedef struct card {

    char cardNumber[16];
    int issueDate;
    int expiryDate;
    int securityCode;
    char name[24];

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
    char number[16];
    int isValid = FALSE;

    while(!isValid) {
        printf("Enter card number (16 digits): ");
        if (fgets(buffer, sizeof(buffer), stdin)) {
            fflush(stdin);
            printf("Buffer: %s\n", buffer);
            buffer[strcspn(buffer,"\n")] = 0;
            printf("Buffer after \\n removed: %s\n", buffer);
            if (1 == sscanf(buffer, "%[^\n]s", number)) {
                printf("Number: %s\n", number);
                if (validateCardNumber(number) && strlen(number) <= 16) {
                    strncpy(custCard->cardNumber, number, 16);
                    printf("From struct = %s\n", custCard->cardNumber);
                    isValid = TRUE;
                } else {
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
            printf("Enter issue date in mm/yy: ");
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


int main() {

    card custCard;

    card *pCard = &custCard;
    getCardNumber(pCard);
    // Type 10 = issue date
    // Type 20 = expiry date
//    getDate(pCard, 10);
//    getDate(pCard, 20);
//    getCVV(pCard);
//    getHolderName(pCard);

    printf("%s\n", pCard->cardNumber);
//    printf("%d\n", pCard->issueDate);
//    printf("%d\n", pCard->expiryDate);
//    printf("%d\n", pCard->securityCode);
//    printf("%s\n", pCard->name);


    return 0;


}

