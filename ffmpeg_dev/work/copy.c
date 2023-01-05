//
// Created by kk on 2023/1/5.
//
#include <stdio.h>
#include <string.h>

#define AUDIO_FILE_PREFIX "audio/7be17688-1ae2-11e8-b7f2-000c29ffef9b_"

char* Int2String(int num,char *str);

const char* getAudioFileName(int index);

int main(){

    int ret = 0;
    FILE *f_src = NULL;
    FILE *f_des = NULL;
    const char* audio_file_prefix = "audio/7be17688-1ae2-11e8-b7f2-000c29ffef9b_";
    char file_src_path[40];
    char file_des_path[40];

    strcpy(file_src_path, "0.aac");
    strcat(file_src_path, audio_file_prefix);

    strcpy(file_des_path, "0_copy.aac");
    strcat(file_des_path, audio_file_prefix);

    f_src = fopen(file_src_path, "rb");
    f_des = fopen(file_des_path, "wb");
    if(f_src == NULL) {
        printf("open src file failed!\n");
        return -1;
    }
    if(f_des == NULL) {
        printf("open des file failed!\n");
        return -1;
    }
    while ((ret = fgetc(f_src)) != EOF) {
        fputc(ret, f_des);
    }

    fclose(f_src);
    fclose(f_des);

    printf("success!\n");
    return 0;
}

const char* getAudioFileName(int index){
    char str[4];
    return strcat(Int2String(index, str), ".aac");
}

char* Int2String(int num,char *str)//10进制
{
    int i = 0;//指示填充str
    if(num<0)//如果num为负数，将num变正
    {
        num = -num;
        str[i++] = '-';
    }
    //转换
    do
    {
        str[i++] = num%10+48;//取num最低位 字符0~9的ASCII码是48~57；简单来说数字0+48=48，ASCII码对应字符'0'
        num /= 10;//去掉最低位
    }while(num);//num不为0继续循环

    str[i] = '\0';

    //确定开始调整的位置
    int j = 0;
    if(str[0]=='-')//如果有负号，负号不用调整
    {
        j = 1;//从第二位开始调整
        ++i;//由于有负号，所以交换的对称轴也要后移1位
    }
    //对称交换
    for(;j<i/2;j++)
    {
        //对称交换两端的值 其实就是省下中间变量交换a+b的值：a=a+b;b=a-b;a=a-b;
        str[j] = str[j] + str[i-1-j];
        str[i-1-j] = str[j] - str[i-1-j];
        str[j] = str[j] - str[i-1-j];
    }

    return str;//返回转换后的值
}