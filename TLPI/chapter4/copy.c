
#include<unistd.h>
#include<sys/stat.h> 
#include<sys/types.h>
#include<fcntl.h>
#include<stdio.h>

#include<errno.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc, char const *argv[])
{
    printf("\nstart\n");
    int inputFd, outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];
 
    if (argc != 3 || strcmp(argv[1], "--help") == 0)
    {
        printf("usageErr:%s old-file new-file\n", argv[0]);
        exit(-1);
    }
    
    // 如果不是新建文件，第三个参数（mode参数）可以省略
    inputFd = open(argv[1], O_RDONLY);
    if(inputFd == -1)
    {
        printf("opening file %s", argv[1]);
        exit(-1);
    }

    // 打开文件的主标志：O_RDONLY:只读；O_RDWR:读写；O_WRONLY:只写
    // 打开文件的副标志：O_APPEND，读写文件从文件尾部开始移动，所写入的数据追加到文件尾部
    // O_TRUNC，若文件存在并且以可写方式打开时，此标志会将文件长度清0，原有数据消失
    // O_CREAT，若路径中的文件不存在则自动建立该文件
    // 主标志之间是互斥的，可配合使用副标志
    openFlags = O_CREAT | O_WRONLY | O_TRUNC;

    /*文件权限标志：S_IRUSR，所有者拥有读权限；
    *S_IWUSR，所有者拥有写权限；
    *S_IXUSR，所有者拥有执行权限；
    *S_IRGRP，群组拥有读权限；
    *S_IWGRP，群组拥有写权限；
    *S_IXGRP，群组拥有执行权限；
    *S_IROTH，其他用户拥有读权限；
    *S_IWOTH，其他用户拥有写权限；
    *S_IXOTH，其他用户拥有执行权限；
    *正好就是Linux中指定的文件权限
    */
    // 这里的文件权限就是：  rw-rw-rw- 
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |S_IWOTH |S_IROTH ;
    
    outputFd = open(argv[2], openFlags, filePerms);
    if(outputFd == -1)
    {
        printf("opening file %s", argv[1]);
        exit(-1);
    }

    while((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
    {
        // 将实际读到的字节数numRead写到目标文件中
        if(numRead != write(outputFd, buf, numRead))
        {
            printf("could't write whole buffer");
            exit(-1);
        }
    }
    
    // read() 函数在遇到文件结束符时返回0 ； 遇到错误则返回-1。
    if(numRead == -1)
    {
        printf("read");
        exit(-1);
    }
    
    // 关闭输入文件和输出文件
    // 当一进程终止时，将自动关闭其已打开的所有文件描述符, 显式关闭不再需要
    // 的文件描述符往往是良好的编程习惯，会使代码在后续修改时更具可读性，也更可靠。
    if(close(inputFd) == -1)
    {
        printf("close input");
        exit(-1);
    }
    if(close(outputFd == -1))
    {
        printf("close output");
        exit(-1);
    }
    
    printf("\nend\n");
    //exit(0);


}

