#include "payment.h"

EN_transStat transState = APPROVED;
static int transctionNum = 0;
static ST_cardData cardData;
static ST_terminalData terminalData;
static ST_accountBalance accountBalance[SRVER_DATABSE_LEN] = {
    {"0100.00","123456789"},
    {"6000.00","234567891"},
    {"3250.25","567891234"},
    {"1500.12","456789123"},
    {"0500.00","258649173"},
    {"2100.00","654823719"},
    {"0000.00","971362485"},
    {"0001.00","793148625"},
    {"0010.12","123123456"},
    {"0000.55","456789321"},
};
static ST_serverData serverData[MAX_SERVER_DATA];

void fillCardData(ST_transaction *transaction){
    printf("Please entre card data:\n");
    printf("Please Entre the card holder name:\n");
    fflush(stdin);
    fgets(cardData.cardHolderName,CARD_HOLDER_LEN,stdin);
    printf("Please Enter the Primary Account Number:\n");
    fflush(stdin);
    fgets(cardData.primaryAccountNumber,PRIMARY_ACC_NUM_LEN,stdin);
    cardData.primaryAccountNumber[OFFSET_BY_9] = FALSE;
    printf("Please card Expiry Date:\n");
    fflush(stdin);
    fgets(cardData.cardExpirationDate,CARD_EXP_DATE_LEN,stdin);
    cardData.cardExpirationDate[OFFSET_BY_5] = FALSE;
    transaction->cardHolderData = cardData;
}

void fillTerminalData(ST_transaction *transaction){
    printf("Please Enter Terminal Data:\n" 
    "Please Enter the transaction Amount:\n");

    fflush(stdin);
    fgets(terminalData.transAmount,TRANS_AMOUNT_LEN,stdin);
    terminalData.transAmount[OFFSET_BY_7] = FALSE;
    printf("Please Enter transaction Date:\n");
    fflush(stdin);
    fgets(terminalData.transactionDate,TRANS_DATE_LEN,stdin);
    terminalData.transactionDate[OFFSET_BY_10] = FALSE;

    /*fixed number*/
    strcpy(terminalData.maxTransAmount,MAX_TRANSATION_AMOUNT);

    transaction->transData = terminalData;
}

void checkAmount(ST_transaction *transaction){
int i ;


for(i = LOOPST ; terminalData.maxTransAmount[i] != FALSE ; ++i)
    {
        if(terminalData.maxTransAmount[i] < terminalData.transAmount[i]){
            printf("Transaction is DECLINED!\n");
            transState = DECLINED;
            return;
        }
        else if(terminalData.maxTransAmount[i] == terminalData.transAmount[i])
            continue;
        else
            break;    
    }

    return;
}



void checkExpiryDate(ST_transaction *transaction){
    char currentMonth[MONTH_LEN];
    char currentYear[YEAR_LEN];
    char cardExpiryMonth[MONTH_LEN];
    char cardExpiryYear[YEAR_LEN];

    currentMonth[FIRST_DIGIT]       = terminalData.transactionDate[OFFSET_BY_3];
    currentMonth[SECOND_DIGIT]      = terminalData.transactionDate[OFFSET_BY_4];
    currentYear[FIRST_DIGIT]        = terminalData.transactionDate[OFFSET_BY_8];
    currentYear[SECOND_DIGIT]       = terminalData.transactionDate[OFFSET_BY_9];

    cardExpiryMonth[FIRST_DIGIT]    = cardData.cardExpirationDate[FIRST_DIGIT];
    cardExpiryMonth[SECOND_DIGIT]   = cardData.cardExpirationDate[SECOND_DIGIT];
    cardExpiryYear[FIRST_DIGIT]     = cardData.cardExpirationDate[OFFSET_BY_3];
    cardExpiryYear[SECOND_DIGIT]    = cardData.cardExpirationDate[OFFSET_BY_4]; 
    
    int i ;
    char sameYearFlag = TRUE;
    for(i = LOOPST ; i < YEAR_LEN ; ++i){
        if(cardExpiryYear[i] < currentYear[i]){
            printf("Transaction is DECLINED!\n");
            transState = DECLINED;
            return;
        }
        else if(cardExpiryYear[i] == currentYear[i])
            continue;
        else{
            sameYearFlag = FALSE;
            break;
        }
    }

    if(sameYearFlag){
        for(i = LOOPST ; i < MONTH_LEN ; ++i){
            if(cardExpiryMonth[i] < currentMonth[i]){
                printf("Transaction is DECLINED!\n");
                transState = DECLINED;
                return;
            }
            else if(cardExpiryMonth[i] == currentMonth[i])
                continue;
            else
                break;      
         }
    }

    return;
 
}


void checkBalance(ST_transaction *transaction){
    printf("Verifying Data from the server...\n");
    int i ;
    char userBalance[MAX_TRANS_LEN] = NULL_BALANCE;
    for(i = LOOPST ; i < SRVER_DATABSE_LEN ; ++i){
        
        unsigned char *currentAccountNumber =  transaction->cardHolderData.primaryAccountNumber;
        unsigned char *PAN = accountBalance[i].primaryAccountNumber;
        
        if(strcmp(transaction->cardHolderData.primaryAccountNumber,
                  accountBalance[i].primaryAccountNumber) == 0)
                  {
                    strcpy(userBalance,accountBalance[i].balance);
                  }
    }

    if(strcmp(userBalance,NULL_BALANCE) == 0 )
    {
        printf("Transaction is DECLINED!\n");
        transState = DECLINED;
        return;
    }
    
    for(i = LOOPST ; userBalance[i] != FALSE ; ++i)
    {
        if(userBalance[i] < transaction->transData.transAmount[i]){
            printf("Transaction is DECLINED!\n");
            transState = DECLINED;
            return;
        }
        else if(userBalance[i] == transaction->transData.transAmount[i])
            continue;
        else
            break;    
    }
    printf("The Transaction is APPROVED.\n");
    return;
}

void saveTransactionIntoServer(ST_transaction *transaction){
    serverData[transctionNum].receiptReferenceNumber = transctionNum;
    serverData[transctionNum].transactionStatus = transState;
    ++transctionNum;

}