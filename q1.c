#include<stdio.h>
#include<stdbool.h> 

void display();
bool isValid(int length, int pos);
int main(){
    int length = 5;
    int arr[]={10,20,30,40,50};



    return 0;
}

// print function for my array 1D
void display(int arr[], int length){
    int i;

    for(i=0; i<length; i++){
        printf("%d, ", arr[i]);
    }
bool isValid(int length, int pos){
    if (pos<=length){
        return true;
    }
    else{
        return false;
    }
}
}