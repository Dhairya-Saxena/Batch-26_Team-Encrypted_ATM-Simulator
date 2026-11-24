/* ATM Simulator using functions, loops and conditionals */
#include <stdio.h>
#define MAX_TRIES 2  /* You get two more tries in casee your first PIN was incorrect before the card gets blocked */
#define MIN_BAL 500  /* Your savings account must keep this amount as a minimum balance */
#define OVERDRAFT 5000 /* Your account can go upto this amount while below 0 */
#define MAX_WITHDRAWL 10000 /* You cannot withdraw cash exceeding this amount */
#define MAX_DEPOSIT 50000 /* You cannot deposit cash exceeding this amount */
/* Account Details */
char cardno[17]="5975486021978644"; /* A 16 digit card no. */
int cPIN=2787;
int savings=75084; /* Money in Savings Account */
int current=2000; /* Money in Current ACcount */
int cdLimit=35000; /* Maximum Money you can borrow */
int cdOwed=3000; /* Money you haven't paid back yet */
/* Transaction Number */
int txNo=1000;
/* Statement: Remembers last 5 transactions */
char stAcc[5]; /* 'S' savings, 'C' current, 'R' credit */
char stType[5];   /* 'W' withdrawal, 'D' deposit          */
int  stAmount[5]; /* Transaction Amount */
int  stBal[5]; /* Balance (or amount owed) afterwards  */
int  stNo[5];     /* Transaction number */
int  stCount=0; /* Recorded Transactions */
int readInt(void) /* Accepts only numeric value */
 {
    int x;
    if (scanf("%d", &x)!=1) {
        while (getchar()!='\n') {
        }
        return -1;
    }
    return x;
}
void addStatement(char acc, char type, int amt, int bal) /* Stores last 5 transactions */
 {
    if (stCount==5) {
        for (int i=1; i<5; i++) {
            stAcc[i-1]=stAcc[i];
            stType[i-1]=stType[i];
            stAmount[i-1]=stAmount[i];
            stBal[i-1]=stBal[i];
            stNo[i-1]=stNo[i];
        }
        stCount=4;
    }
    stAcc[stCount]=acc;
    stType[stCount]=type;
    stAmount[stCount]=amt;
    stBal[stCount]=bal;
    stNo[stCount]=txNo;
    stCount++;
    txNo++;
}
int checkPin(void) /* Checks if PIN is valid */
 {
    int pin;
    printf("Card No: %s\n", cardno);
    printf("Enter PIN: ");
    pin=readInt();
    if (pin==cPIN) return 1;
    for (int i=1; i<=MAX_TRIES; i++) {
        printf("Incorrect PIN. Try again (%d/%d): ", i, MAX_TRIES);
        pin=readInt();
        if (pin==cPIN) return 1;
    }
    printf("Too many incorrect attempts. Exiting.\n");
    return 0;
}
void balanceEnquiry(void) {
    int acc;
    printf("\n1. Savings  2. Current  3. Credit\nChoose account: ");
    acc=readInt();
    if (acc==1) {
        int avail=(savings>MIN_BAL)? savings-MIN_BAL:0;
        printf("Savings Balance: %d | Available to Withdraw: %d\n", savings, avail);
    } else if (acc==2) {
        printf("Current Balance: %d | Available (with overdraft): %d\n", current, current+OVERDRAFT);
    } else if (acc==3) {
        printf("Credit Owed: %d | Credit Limit: %d | Available Credit: %d\n", cdOwed, cdLimit, cdLimit-cdOwed);
    } else {
        printf("Invalid account choice.\n");
    }
}
void cashDeposit(void) {
    int acc, amt;
    printf("\n1. Savings  2. Current  3. Credit Card\nChoose account: ");
    acc=readInt();
    if (acc<1 || acc>3) {
        printf("Invalid account choice.\n");
        return;
    }
    printf("Enter amount (Max %d): ", MAX_DEPOSIT);
    amt = readInt();
    if (amt<=0 || amt>MAX_DEPOSIT) {
        printf("Invalid amount.\n");
        return;
    }
    if (acc==1) {
        savings+=amt;
        printf("Transaction #%d: Deposited %d into Savings. Updated Balance: %d\n", txNo, amt, savings);
        addStatement('S', 'D', amt, savings);
    } else if (acc==2) {
        current+=amt;
        printf("Transaction #%d: Deposited %d into Current. Updated Balance: %d\n", txNo, amt, current);
        addStatement('C', 'D', amt, current);
    } else {
        if (amt>cdOwed) {
            printf("Invalid amount: exceeds credit owed (%d).\n", cdOwed);
            return;
        }
        cdOwed-=amt;
        printf("Transaction #%d: Paid %d toward Credit Card. Updated Credit Owed: %d\n", txNo, amt, cdOwed);
        addStatement('R', 'D', amt, cdOwed);
    }
}
void cashWithdrawal(void) {
    int acc, amt;
    printf("\n1. Savings  2. Current  3. Credit Card\nChoose account: ");
    acc=readInt();
    if (acc<1 || acc>3) {
        printf("Invalid account choice.\n");
        return;
    }
    printf("Enter amount (Max %d): ", MAX_WITHDRAWL);
    amt=readInt();
    if (amt<=0 || amt>MAX_WITHDRAWL) {
        printf("Invalid amount.\n");
        return;
    }
    if (acc==1) {
        if (savings-amt<MIN_BAL) {
            printf("Withdrawal rejected: Savings balance cannot go below %d. Current Balance: %d\n", MIN_BAL, savings);
            return;
        }
        savings-=amt;
        printf("Transaction #%d: Withdrew %d from Savings. Updated Balance: %d\n", txNo, amt, savings);
        addStatement('S', 'W', amt, savings);
    } else if (acc==2) {
        if (current-amt < -OVERDRAFT) {
            printf("Withdrawal rejected: Exceeds overdraft limit (%d). Current Balance: %d\n", OVERDRAFT, current);
            return;
        }
        current-=amt;
        printf("Transaction #%d: Withdrew %d from Current. Updated Balance: %d\n", txNo, amt, current);
        addStatement('C', 'W', amt, current);
    } else {
        if (cdOwed+amt>cdLimit) {
            printf("Withdrawal rejected: Exceeds credit limit (%d). Current Credit Owed: %d\n", cdLimit, cdOwed);
            return;
        }
        cdOwed+=amt;
        printf("Transaction #%d: Withdrew/Charged %d on Credit Card. Updated Credit Owed: %d\n", txNo, amt, cdOwed);
        addStatement('R', 'W', amt, cdOwed);
    }
}
void showStatement(void) {
    printf("\n--- Mini Statement ---\n");
    if (stCount==0) {
        printf("No transactions recorded.\n");
        return;
    }
    for (int i=0; i<stCount; i++) {
        printf("Txn #%d | Acc: %c | Type: %c | Amount: %d | Balance: %d\n",
               stNo[i], stAcc[i], stType[i], stAmount[i], stBal[i]);
    }
}
int main(void) {
    if (!checkPin()) return 0;
    printf("================================ \n");
    printf("				    \n");
    printf("   Welcome to ATM Simulator\n");
    printf("				    \n");
    printf("================================ \n");
    int choice=0;
    while (choice!=5) {
        printf("\n1. Balance Enquiry\n");
        printf("2. Cash Deposit\n");
        printf("3. Cash Withdrawal\n");
        printf("4. Statement\n");
        printf("5. Exit\n");
        printf("\n ");
        choice=readInt();
   switch (choice) {
        case 1:
            balanceEnquiry();
            break;
        case 2:
            cashDeposit();
            break;
        case 3:
            cashWithdrawal();
            break;
        case 4:
            showStatement();
            break;
        case 5:
            printf("Thank you for using the ATM.\n");
            break;
        default:
            printf("Invalid option.\n");
            break;
         }
    }
    return 0;
}
