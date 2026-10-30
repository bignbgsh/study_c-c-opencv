#include <stdio.h>

int findMAX(int *arr,int size){
    int max = arr[0];
    for(int i=1;i<size;i++){
        if(arr[i]>max){
            max = arr[i];
        }
    }
    return max;
}


int main(){
    int nums[] = {5,9,8,6,3};
    int size = sizeof(nums)/sizeof(nums[0]);

    int max = findMAX(nums,size);
    printf("最大值为：%d\n",max);

    return 0;
}