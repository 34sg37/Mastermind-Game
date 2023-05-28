#include <iostream>
#include <ctime>
#include <cstring>
using namespace std;

//generate random number, N holds length of number, digs is the number array that holds digits
void generateRandomNumber(int N,int* digs){
    bool used;//holds 1 if the digit has already been used
    digs[0] = rand() % 9 + 1; //high-most digit cant be 0, so we generate a random number between 1 and 9

    //remaining digits
    for (int i = 1; i < N; i++) {
        int digit = rand() % 10; //random digit between 0 and 9

        //check if the digit is used before
        used = false;
        for (int j = 0; j < i; j++) {
            if (digit == digs[j]) { //if used set used=true
                used = true;
                break;
            }
        }

        //generate a new digit if the digit is used before
        while (used) {
            digit = rand() % 10; //try new
            used = false;
            for (int j = 0; j < i; j++) {
                if (digit == digs[j]) { //if still used, set used=true and try another
                    used = true;
                    break;
                }
            }
        }
        digs[i] = digit; // add the digit to number
    }
}

//check if the input is valid, N holds length of number, digs is the number array that holds digits
void checkUserDefinedNumber(int N,const int* digs){
    for(int i=0; i<N;i++){
        int digit = digs[i]; //int variable to hold i. digit in digs array

        //if first digit is zero print E0 error and exit
        while(i==0 && digit==0){
            cerr<<"E0"<<endl;
            exit(0);
        }

        for(int j=0; j<i;j++){
            if(digit==digs[j]){ //if used before, print error E0 and exit
                cerr<<"E0"<<endl;
                exit(0);
            }
        }
    }
}

//function to play the game, N holds number length, number holds the number array that holds digits
void playGame(int N, int* number){
    //exact: the count of digits of the proposed number that match in place of the secretnumber
    //misplaced: the count of digits of the proposed number which do exist in the secretnumber but are not in place
    //count: holds the iteration counter
    int exact,misplaced,count=0;
    bool found=false; //holds 1 if the number is found

    //do the following while the number is found or until the user uses all their chances 
    while (!found && count<100){
        count++;//++ for each iteration
        exact=0;        //reset exact and misplaced for each iteration
        misplaced=0;    //
        found=true;

        //enter the number into a string first
        string enterChar; 
        cin>>enterChar;

        int size=enterChar.length(); //size of proposed number 
        //if size of proposed number does not match with secret number then print error E1 and exit
        if (size!=N){
            cerr<<"E1"<<endl;
            exit(1);
        }

        //check if all chars of proposed number is a digit, if not then give error E2 and exit
        for(int i=0; i<N; i++){
            if (enterChar[i]>'9' || enterChar[i]<'0'){
                cerr<<"E2"<<endl;
                exit(2);
            }
        }
        //transform the number string into number array
        int enterInt[size];
        for(int i=0; i<N; i++){
            enterInt[i]=enterChar[i]-'0';
        }

        checkUserDefinedNumber(size,enterInt);//check if entered number is valid

        //if a digit of proposed number matches with secret number's digit in right place exact++
        for(int i=0; i<N; i++){
            if (enterInt[i]==number[i]) exact++;
        }

        //if a digit of proposed number matches with secret number's digit in wrong place misplaced++
        for(int i=0; i<N; i++){
            for(int j=0; j<N; j++){
                if(i!=j && number[i]==enterInt[j]){
                    misplaced++;
                    break;
                }
            }
        }

        //check if proposed number is exactly same with secret number; if not, set found=false again
        for (int i=0; i<N;i++) if(enterInt[i]!=number[i]) found=false;

        if(found) cout<<"FOUND "<<count<<endl; //if found print found 
        else if(count==100) cout<<"FAILED"<<endl; //if its last chance and couldnt find print failed
        else cout<<exact<<" "<<misplaced<<endl; //else print the values of exact and misplaced and continue in while loop
    }
}

int main(int argc, char** argv) {
    int N=strlen(argv[2]); //length of 2nd argument

    //check if the 2nd argument contains integers only
    for(int i=0; i<N; i++){
        if (argv[2][i]>'9' || argv[2][i]<'0'){
            cerr<<"E2"<<endl;
            exit(2);
        }
    }

    //check if the number of arguments is correct
    if (argc!=3){
        cerr<<"E0"<<endl;
        exit(0);
    }
    srand(time(0));

    //check if the first argument is -r which means random secret number
    if(argv[1][0]=='-' && argv[1][1]=='r'){
        N = atoi(argv[2]); //number of digits of the number that will be randomly generated
        int number[N]; //array to hold the digits of the number
        generateRandomNumber(N,number);
        playGame(N,number);
    }

    //check if the first argument is -u which means user defined secret number
    else if(argv[1][0]=='-' && argv[1][1]=='u'){
        N= strlen(argv[2]); //number of digits of the entered number

        //convert the string that holds the number into an array
        int number[N];
        for(int i=0; i<N; i++){
            number[i]=argv[2][i]-'0';
        }
        checkUserDefinedNumber(N,number); //check if entered number is valid
        playGame(N,number);
    }

    //if entered operation is invalid give E0 error and exit
    else{ 
        cerr<<"E0"<<endl;
        exit(0);
    }
    
    return 0;
}