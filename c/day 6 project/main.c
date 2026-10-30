#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    srand(time(NULL)); 
    int target = rand() % 100 + 1;
    int guess,count = 0;;

    
    do {
        printf("猜一个1-100之间的数字: ");
        scanf("%d", &guess);
        count++;
        if (guess < target) {
            printf("太小了！再试一次: \n");
        } else if (guess > target) {
            printf("太大了！再试一次:\n ");
        } else {
            printf("恭喜你，猜对了！你总共猜了 %d 次。\n", count);
            break;
        }
    }while (guess != target) ;{
       
    }
    return 0;
}