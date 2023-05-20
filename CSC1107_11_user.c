// Filename: CSC1107_Group_Number_user.c

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <openssl/evp.h>

#define DEVICE "/dev/CSC1107device"
#define TIME_BUFF_SIZE 256

char* getDataTime();
char* get_hash(const EVP_MD *type, char *msg);

int main() {
    int i, fd;
    char ch, write_buf[100], read_buf[100];
    int hash_type = 0;
    char *hash_string;

    //call getDataTime() and print
    char *dateTime = getDataTime();
    printf("%s\n", dateTime);

    //prompt for type of hash to be used
    printf("Enter\n1) 512(SHA-512)\n2) SHA-284\n3) SHA-256\n4) SHA-1\n5) MD5\n ");
    scanf(" %1d", &hash_type);

    switch (hash_type)
    {
        case 1:
            printf("You have selected 512(SHA-512)\n");
            hash_string = get_hash(EVP_sha512(), dateTime);
            break;
        case 2:
            printf("You have selected SHA-384\n");
            hash_string = get_hash(EVP_sha384(), dateTime);
            break;
        case 3:
            printf("You have selected SHA-256\n");
            hash_string = get_hash(EVP_sha256(), dateTime);
            break;
        case 4:
            printf("You have selected SHA-1\n");
            hash_string = get_hash(EVP_sha1(), dateTime);
            break;
        case 5:
            printf("You have selected MD5\n");
            hash_string = get_hash(EVP_md5(), dateTime);
            break;
        default:
            printf("Invalid option\n");
            break;
    }

    printf("The hash string is:\n%s\n", hash_string);

    // Acquiring data
    printf("Enter data: ");
    scanf(" %[^\n]", write_buf);

    fd = open(DEVICE, O_RDWR);

    if(fd == -1){
        printf("file %s either does not exist or has been locked by another process\n", DEVICE);
        exit(-1);
    }

    // Writing data to the device
    write(fd, write_buf, strlen(write_buf));

    // Reading data from the device
    read(fd, read_buf, sizeof(read_buf));

    printf("The device sent: %s\n", read_buf);

    close(fd);

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
