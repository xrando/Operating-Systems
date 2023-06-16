// Filename: CSC1107_Group_Number_user.c

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <openssl/evp.h>

#define DEVICE "/dev/CSC1107_11_kernel"
#define TIME_BUFF_SIZE 256
#define BUF_SIZE 4096

char* getDataTime();
char* get_hash(const EVP_MD *type, char *msg);
void get_output_from_kernel();

int main() {
    int i, fd;
    char ch, write_buf[BUF_SIZE], read_buf[BUF_SIZE];
    int hash_type = 0;
    char *hash_string;
    const char *type_string;
    const EVP_MD *type;

    //call getDataTime() and print
    char *dateTime = getDataTime();
    printf("%s\n", dateTime);

    //prompt for type of hash to be used
    printf("Enter\n1) SHA-512\n2) SHA-384\n3) SHA-256\n4) SHA-1\n5) MD5\n ");
    scanf(" %1d", &hash_type);

    switch (hash_type)
    {
        case 1:
            type = EVP_sha512();
            break;
        case 2:
            type = EVP_sha384();
            break;
        case 3:
            type = EVP_sha256();
            break;
        case 4:
            type = EVP_sha1();
            break;
        case 5:
            type = EVP_md5();
            break;
        default:
            break;
    }
    type_string = EVP_MD_name(type);
    printf("You have selected %s\n", type_string);
    hash_string = get_hash(type, dateTime);
    printf("The hash string is:\n%s\n", hash_string);

    //format the data to be send to kernel
    // 1. original sentence
    // 2. hashed sentence
    // 3. type of hash
    snprintf(write_buf, BUF_SIZE, "%s;%s;%s", dateTime, type_string, hash_string);

    free(hash_string);
    fd = open(DEVICE, O_RDWR);

    if(fd == -1){
        printf("file %s either does not exist or has been locked by another process\n", DEVICE);
        exit(-1);
    }

    // Writing data to the device
    write(fd, write_buf, strlen(write_buf));

    // Reading data from the device
    read(fd, read_buf, sizeof(read_buf));

    close(fd);

    // Print the read data from the Device
    get_output_from_kernel();

    return 0;
}

char* getDataTime() {
    time_t currentTime;
    struct tm *localTime;

    // Allocate memory for dateTime. Don't forget to free it later.
    char *dateTime = malloc(TIME_BUFF_SIZE);

    // Get the current time
    currentTime = time(NULL);

    // Convert the current time to the local time
    localTime = localtime(&currentTime);

    // Format the date and time
    strftime(dateTime, TIME_BUFF_SIZE, "It is %H:%M:%S on %dth %B %Y now.", localTime);

    // Return local time
    return dateTime;
}

char* get_hash(const EVP_MD *type, char *msg) {
    EVP_MD_CTX *mdctx;
    unsigned char output[EVP_MAX_MD_SIZE];
    unsigned int output_len;
    int i;

    mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, type, NULL);
    EVP_DigestUpdate(mdctx, msg, strlen(msg));
    EVP_DigestFinal_ex(mdctx, output, &output_len);
    EVP_MD_CTX_free(mdctx);

    char* hash_string = (char*) malloc(output_len * 2 + 1);
    for(i = 0; i < output_len; i++) {
        sprintf(hash_string + (i * 2), "%02x", output[i]);
    }
    hash_string[output_len * 2] = 0;

    return hash_string;
}

void get_output_from_kernel() {
    // Read from the Device and get the hashed string and comparison result
    int fd = open(DEVICE, O_RDWR);
    char read_buf[BUF_SIZE];
    read(fd, read_buf, sizeof(read_buf));
    close(fd);

    // Split the read_buf into 3 parts with \n as the delimiter 
    // (First one is the hashed string, second one is the comparison result)
    char *hash_string = strtok(read_buf, "\n");
    char *result = strtok(NULL, "\n");

    // Print the hashed string and comparison result
    printf("The hashed string from kernel is:\n%s\n", hash_string);
    printf("The comparison result from kernel is:\n%s\n", result);
}
