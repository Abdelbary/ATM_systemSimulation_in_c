#include "payment.h"

extern EN_state transState;
int main(){
    

    EN_state state = INPUTCAPTURE;
    while(state != TERMINATE){
        switch(state){

            case INPUTCAPTURE:
                /*initialize state of transaction as approved*/
                transState = APPROVED;  
                ST_transaction * transaction = (ST_transaction *) malloc(sizeof(ST_transaction));      
                fillCardData(transaction);
                fillTerminalData(transaction);
                state = VALIDATION;
            break;

            
            case VALIDATION:
                checkAmount(transaction);
                checkExpiryDate(transaction);
                if(transState == DECLINED){
                    transState = APPROVED;
                    state = FINALIZE;
                }else
                state = SAVE_TO_SERVER;
            break;

            case SAVE_TO_SERVER:
                checkBalance(transaction);
                saveTransactionIntoServer(transaction);
                state = FINALIZE;
            break;

            case FINALIZE:
                free(transaction);
                transaction = NULL;
                fflush(stdin);
                fflush(stdout);
                printf("Do you want to continue (y/n)?:\n");
                char answer;
                answer = getchar();
                fflush(stdout);
                if(answer == 'Y' || answer == 'y')
                    state = INPUTCAPTURE;
                else
                    state = TERMINATE;
                break;
        
        }
    }
    return 0;
}