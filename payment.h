#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CARD_HOLDER_LEN         25
#define PRIMARY_ACC_NUM_LEN     19
#define CARD_EXP_DATE_LEN       6
#define TRANS_AMOUNT_LEN        8
#define MAX_TRANS_LEN           8
#define TRANS_DATE_LEN          11
#define MAX_TRANSATION_AMOUNT   "5000.00"
#define LOOPST                  0 
#define MONTH_LEN               2
#define YEAR_LEN                2
#define FIRST_DIGIT             0
#define SECOND_DIGIT            1
#define OFFSET_BY_3             3
#define OFFSET_BY_4             4
#define OFFSET_BY_8             8
#define OFFSET_BY_9             9
#define OFFSET_BY_5             5
#define OFFSET_BY_10            10
#define OFFSET_BY_7             7
#define TRUE                    1
#define FALSE                   0
#define SRVER_DATABSE_LEN       10
#define NULL_BALANCE            "NONE"
#define MAX_SERVER_DATA         255

/*****
 * enum to hold the declined and approved state feed back 
 * this can be either the feed back from the server to the ATM terminal or from terminal to user
 * conditio for declined are
 *      1-card experiy date < current date
 *      2-tran amount > max trans amount     
 *      3-trans amoumt > user banck account
 *      
 */
typedef enum EN_transStat
{
    DECLINED,
    APPROVED
}EN_transStat;


typedef enum EN_state{
    INPUTCAPTURE,
    VALIDATION,
    SAVE_TO_SERVER,
    FINALIZE,
    TERMINATE
}EN_state;



typedef struct ST_cardData
{
    unsigned char cardHolderName[CARD_HOLDER_LEN];
    unsigned char primaryAccountNumber[PRIMARY_ACC_NUM_LEN];
    unsigned char cardExpirationDate[CARD_EXP_DATE_LEN];
}ST_cardData;



typedef struct ST_terminalData
{
    unsigned char transAmount[TRANS_AMOUNT_LEN];
    unsigned char maxTransAmount[MAX_TRANS_LEN];
    unsigned char transactionDate[TRANS_DATE_LEN];
}ST_terminalData;

typedef struct ST_serverData
{
    EN_transStat transactionStatus;
    unsigned int receiptReferenceNumber;
}ST_serverData;

typedef struct ST_transaction
{
    ST_cardData  cardHolderData;
    ST_terminalData  transData;
    ST_serverData transResponse;
}ST_transaction;


typedef struct ST_accountBalance
{
    unsigned char balance[8];

    unsigned char primaryAccountNumber[19];

}ST_accountBalance;



/*............Card-side APIs...........*/
void fillCardData(ST_transaction *transaction);
void checkAmount(ST_transaction *transaction);

/*  Terminal-Side APIs */
void fillTerminalData(ST_transaction *transaction);
void checkExpiryDate(ST_transaction *transaction);

/*  Server-Side APIs  */
void saveTransactionIntoServer(ST_transaction *transaction);
void checkBalance(ST_transaction *transaction);

