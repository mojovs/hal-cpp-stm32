//
// Created by meng on 2024/5/27.
//

#include "tools.h"

void shell_sort(int *data, int len) {
    int step=len/2; //分组
    int i,j,tmp;
    //分组大小
    for(step;step>=1;step=step/2){
        //分组内插入排序
        for(i=step;i<len;i++){
            tmp=data[i];        //组内序列最大成员
            j=i;
            //组内插入排序，将数放到最小的位置
            for(j;j>=step && tmp<data[j-step];j=j-step){
                data[j]=data[j-step];
            }
            data[j]=tmp;
        }

    }
}

void bubble_sort(int *data,int len){
    for(int i=0;i<len-1;i++)
        for(int j=i+1;j<len;j++){
            if(data[j]<data[i]) //序列号大的比小的值小
            {
                int tmp=data[j];
                data[j]=data[i];
                data[i]=data[j];
            }
        }
}