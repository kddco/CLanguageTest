//mmap 標準讀取之比較
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

#define SIZE 1*1024
int standard_RW() {
    FILE* fp;
    struct timeval t1, t2;
    char c = 'a';
    long spend_us;

    

    //open
    gettimeofday(&t1, NULL);
    if ((fp = fopen("disk_file", "w")) == NULL) { //標準io開檔
        printf("fopen error!\n");
        return -1;
    }
    

    //standard write
    int i = 0;
    for (i = 0; i < SIZE; i++) { //標準io寫入
        if (fwrite(&c, sizeof(char), 1, fp) != 1) {
            printf("file write error!\n");
        }
    }

    gettimeofday(&t2, NULL);
    spend_us = 1000000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec);
    printf("write disk file,spend time:%ld us \n ", spend_us); //標準io寫檔時間
    fclose(fp);

    // standard read
    char buffer[100];
    gettimeofday(&t1, NULL);
    if ((fp = fopen("disk_file", "r")) == NULL) { //標準io寫檔案指標
        printf("fopen error!\n");
        return -1;
    }
    


    while (!feof(fp)) // to read file 
    {
        fread(buffer, sizeof(buffer), 1, fp);
        
        
    }
    //printf("%s", buffer);
    gettimeofday(&t2, NULL);
    spend_us = 1000000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec);
    printf("read disk file,spend time:%ld us \n ", spend_us); //標準io讀檔時間
    fclose(fp);
    return 0;
    
}
int main(int argc, char** argv)
{
    FILE* fp;
    char c = 'a';
    int i;
    char* pmap;
    int fd;
    char* mem;
    struct timeval t1, t2;
    long spend_us;
    printf("file size: %dKB \n", SIZE  / 1024);
    //printf("file size: %dMB \n", SIZE / 1024 / 1024);

    

    standard_RW(fp);
    //standard_write
    //gettimeofday(&t1, NULL);

    //for (i = 0; i < SIZE; i++) { //標準io寫入
    //    if (fwrite(&c, sizeof(char), 1, fp) != 1) {
    //        printf("file write error!\n");
    //    }
    //}

    //gettimeofday(&t2, NULL);
    //spend_us = 1000000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec);
    //printf("write disk file,spend time:%ld us \n ", spend_us); //計算標準io寫檔花費時間
    //fclose(fp);
    //standard_write end

    gettimeofday(&t1, NULL);
    fd = open("disk_file", O_RDWR);
    if (fd < 0) {
        perror("open mmap_file");
        return -1;
    }
    pmap = (char*)mmap(NULL, sizeof(char) * SIZE, PROT_READ | PROT_WRITE,
        MAP_SHARED, fd, 0);

    if (pmap == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    for (i = 0; i < SIZE; i++) {
        memcpy(pmap + i, &c, 1);
    }

    gettimeofday(&t2, NULL);
    spend_us = 1000000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec);
    printf("write mmap file,spend time:%ld us\n", spend_us);
    
    //mmap read
    gettimeofday(&t1, NULL);
    char buffer[1];
    for (i = 0; i < SIZE; i++) {
      
        buffer[0] = pmap[i];
    }
    printf("\n");
    gettimeofday(&t2, NULL);

    spend_us = 1000000 * (t2.tv_sec - t1.tv_sec) + (t2.tv_usec - t1.tv_usec);
    printf("read mmap file,spend time:%ld us\n", spend_us);

    close(fd);
    munmap(pmap, sizeof(char) * SIZE);

    return 0;
}
