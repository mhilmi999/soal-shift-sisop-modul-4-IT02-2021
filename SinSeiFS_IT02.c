#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <sys/xattr.h>
#include <sys/wait.h>
#include <pthread.h>

static const char *dirpath = "/home/lambang/Downloads/fuse";

char command[100][1024], keterangan[100][1024];
int jumlah, jumlahket;
/*
alias cc="sudo umount biasalah && gcc -Wall `pkg-config fuse --cflags` confuse.c -o con `pkg-config fuse --libs` && ./con -f biasalah"
alias ss="gcc -Wall `pkg-config fuse --cflags` confuse.c -o con `pkg-config fuse --libs` && ./con -f biasalah"
*/

char upper_case[] = {'Z', 'Y', 'X', 'W', 'V', 'U',
                     'T', 'S', 'R', 'Q', 'P', 'O',
                     'N', 'M', 'L', 'K', 'J', 'I',
                     'H', 'G', 'F', 'E', 'D', 'C', 'B', 'A'};

// Creating a array of lower case alphabets
char lower_case[] = {'z', 'y', 'x', 'w', 'v', 'u',
                     't', 's', 'r', 'q', 'p', 'o',
                     'n', 'm', 'l', 'k', 'j', 'i',
                     'h', 'g', 'f', 'e', 'd', 'c', 'b', 'a'};

void atbash(char *message)
{
    int len = strlen(message); // Calculating lenght of the string
    // printf("len %d\n", len);
    char cipher[1024];
    int ascii_char;
    char result[1024];
    for (int i = 0; i < len; i++)
    {
        ascii_char = message[i]; // Storing ASCII value of alphabet

        if (ascii_char >= 'A' && ascii_char <= 'Z')
            message[i] = upper_case[ascii_char - 65]; // If character is in upper case(minus 65 from its ASCII value to get reversed character)
        else if (ascii_char >= 'a' && ascii_char <= 'z')
            message[i] = lower_case[ascii_char - 97]; // If character is in upper case(minus 97 from its ASCII value to get reversed character)
        else if (ascii_char == 46)
            break;
        else
            continue;
        // printf("msg %d\n", message[i]);
    }
}

void rot13(char *message)
{
    int len = strlen(message); // Calculating lenght of the string
    // printf("len %d\n", len);
    char cipher[1024];
    int ascii_char;
    char result[1024];
    for (int i = 0; i < len; i++)
    {
        ascii_char = message[i]; // Storing ASCII value of alphabet

        if (ascii_char >= 'A' && ascii_char <= 'Z')
        {
            message[i] = ascii_char + 13;
            if (ascii_char - 'A' + 13 >= 26)
            {
                int key = ((ascii_char - 'A') + 13) % 26;

                message[i] = 'A' + key;
            }
        }
        // If character is in upper case(minus 65 from its ASCII value to get reversed character)
        else if (ascii_char >= 'a' && ascii_char <= 'z')
        {
            message[i] = ascii_char + 13;
            // printf("%d\n", ascii_char - 'a' + 13);
            if (ascii_char - 'a' + 13 >= 26)
            {
                int key = ((ascii_char - 'a') + 13) % 26;
                // printf("key %d\n", key);
                message[i] = 'a' + key;
            }
            // If character is in upper case(minus 97 from its ASCII value to get reversed character)
        }
        else if (ascii_char == 46)
            break;
        else
            continue;
        // printf("msg %d\n", message[i]);
    }
}

void vigenereencode(char *msg, char *res)
{
    char key[5] = "SISOP";
    int msgLen = strlen(msg), keyLen = strlen(key), i, j, k = 0;
    char buf[100];
    bzero(buf, sizeof(buf));

    strcpy(buf, msg); // = msg;
    char newKey[msgLen], encryptedMsg[msgLen];
    // printf("len vig %d\n", msgLen);
    //generating new key
    bzero(newKey, sizeof(newKey));
    for (i = 0, j = 0; i < msgLen; ++i, ++j)
    {
        if (j == keyLen)
            j = 0;

        newKey[i] = key[j];
    }

    newKey[i] = '\0';
    bzero(encryptedMsg, sizeof(encryptedMsg));
    //encryption
    for (i = 0; i < msgLen; i++)
    {
        char ascii_char = buf[i];
        if ((ascii_char >= 'a' && ascii_char <= 'z') || (ascii_char >= 'A' && ascii_char <= 'Z'))
        {
            // printf("own %c\n", ascii_char);
            if (ascii_char >= 'A' && ascii_char <= 'Z')
            {

                encryptedMsg[i] = ((ascii_char - 'A' + newKey[i] - 65) % 26) + 65;
                // encryptedMsg[i] = ((msg[i] + newKey[i]) % 26) + 'A';
            }
            else if (ascii_char >= 'a' && ascii_char <= 'z')
            {
                encryptedMsg[i] = ((ascii_char - 162 + newKey[i]) % 26) + 97;
                // encryptedMsg[i] = ((msg[i] + newKey[i]) % 26) + 'a';
            }
        }
        else if (ascii_char == 46)
        {
            // printf("46 %c\n", ascii_char);
            // sprintf(buffer, "%s%s",encryptedMsg,)
            for (k = i; k < msgLen; k++)
            {
                ascii_char = buf[k];
                encryptedMsg[k] = ascii_char;
            }
            encryptedMsg[k] = "\0";
            break;
        }
        else
        {
            // printf("cnt %c\n", ascii_char);
            encryptedMsg[i] = ascii_char;
            // continue;
        }
    }

    if (k < i)
    {
        encryptedMsg[i] = 0;
    }
    // printf("len vig %d\n", msgLen);

    // puts("");

    // encryptedMsg[i] = '\0';/
    // printf("%s\n", encryptedMsg);
    // bzero(res, sizeof(encryptedMsg));
    printf("strlen %d\n", strlen(encryptedMsg));
    bzero(res, sizeof(res));
    strcpy(res, encryptedMsg);
}

void vigeneredecode(char *msg, char *res)
{
    char key[5] = "SISOP";
    int msgLen = strlen(msg), keyLen = strlen(key), i, j, k = 0;
    char buf[100];
    bzero(buf, sizeof(buf));
    bzero(res, sizeof(res));
    // printf("strlen %d\n", sizeof(res));
    strcpy(buf, msg); // = msg;
                      // printf("buf %s\n", buf);
    char newKey[msgLen], decryptedMsg[msgLen];
    // printf("len vig %d\n", msgLen);
    //generating new key
    bzero(newKey, sizeof(newKey));
    for (i = 0, j = 0; i < msgLen; ++i, ++j)
    {
        if (j == keyLen)
            j = 0;

        newKey[i] = key[j];
    }

    newKey[i] = '\0';
    bzero(decryptedMsg, sizeof(decryptedMsg));

    //decryption
    for (i = 0; i < msgLen; ++i)
    {
        char ascii_char = buf[i];
        if ((ascii_char >= 'a' && ascii_char <= 'z') || (ascii_char >= 'A' && ascii_char <= 'Z'))
        {
            // printf("own %c\n", ascii_char);
            if (ascii_char >= 'A' && ascii_char <= 'Z')
            {
                decryptedMsg[i] = ((ascii_char - newKey[i] + 26) % 26) + 65;
                // printf("nwo %c\n", decryptedMsg[i]);
                // decryptedMsg[i] = (((msg[i] - newKey[i]) + 26) % 26) + 'A';
            }
            else if (ascii_char >= 'a' && ascii_char <= 'z')
            {
                // printf("ascii %d newkey %d hasil 1 %d hasil 2 %d\n", ascii_char, newKey[i], ascii_char - 'a' - newKey[i] - 97 + 26, (ascii_char - 'a' - newKey[i] - 97 + 26) % 26);
                decryptedMsg[i] = ((ascii_char - newKey[i] - 6) % 26) + 97;
                // decryptedMsg[i] = (((msg[i] - newKey[i]) + 26) % 26) + 'a';
                // printf("nwos %c\n", decryptedMsg[i]);
            }
        }
        else if (ascii_char == 46)
        {
            // printf("46 %c\n", ascii_char);
            // sprintf(buffer, "%s%s",encryptedMsg,)
            for (k = i; k < msgLen; k++)
            {
                ascii_char = buf[k];
                decryptedMsg[k] = ascii_char;
            }
            decryptedMsg[k] = 0;
            break;
        }
        else
        {
            // printf("cnt %c\n", ascii_char);
            decryptedMsg[i] = ascii_char;
            // continue;
        }
    }

    if (k < i)
    {
        decryptedMsg[i] = 0;
    }
    // printf("dec lg %s\n", decryptedMsg);
    // decryptedMsg[i] = '\0';

    strcpy(res, decryptedMsg);
}

int in(int len, char *target)
{
    int i;
    // printf("target %s\n", target);
    for (i = 0; i < len; i++)
    {
        if (strncmp(keterangan[i], target, strlen(target)) == 0)
        {
            // printf("arr %s\n", keterangan[i]);
            return 1;
        }
    }
    return 0;
}

void logsytem(char *level, char *cmd, char *desc)
{
    FILE *fp;
    time_t t = time(NULL);
    struct tm *ptm = localtime(&t);
    char log[200], times[100];
    strcpy(log, level);
    if (strcmp(cmd, "RENAME") == NULL)
    {
        printf("log rename\n");
        strcpy(command[jumlah], "RENAME");
        char buff[1024];
        strcpy(buff, desc);
        char tokendes[1024];
        strcpy(tokendes, strtok(buff, "::"));
        strcpy(keterangan[jumlahket], tokendes);
        printf("ket 1 %s\n", keterangan[jumlahket]);
        strcpy(tokendes, strtok(NULL, "::"));
        jumlahket++;
        strcpy(keterangan[jumlahket], tokendes);
        printf("ket 2 %s\n", keterangan[jumlahket]);
        jumlahket++;
        jumlah++;
    }

    sprintf(times, "::%02d%02d%02d-%02d:%02d:%02d::", ptm->tm_year + 1900 - 2000, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    sprintf(log, "%s%s%s::%s", level, times, cmd, desc);
    fp = fopen("/home/lambang/SinSeiFS.log", "a");
    fprintf(fp, "%s\n", log);
    fclose(fp);
}

void dirasli(char *inppath, char *outpath)
{
    char fpath[1024];
    bzero(fpath, sizeof(fpath));
    sprintf(fpath, "%s%s", dirpath, inppath);
    if (strstr(fpath, "/AtoZ_") != NULL)
    {
        // if(strstr(fpath, ".") == NULL)
        {

            char pathsetengah[1024];

            strcpy(fpath, inppath);

            // char token1[1024] ;
            // strcpy(token1, strtok(fpath, "AtoZ_"));
            // strcpy(pathsetengah, token1);
            // printf("token 1 %s\n", pathsetengah);

            //dapetin yang ga di hash
            int reallen = strlen(fpath);
            bzero(fpath, sizeof(fpath));
            //dapeting yang di hash
            strcpy(fpath, inppath);
            char *str = strstr(fpath, "/AtoZ_");
            printf("str %s\n", str);
            int m = 0;
            char arr2[100][1024];
            char *enkrip = strtok(str, "/");
            printf("enkrip %s\n", enkrip);

            if ((enkrip = strtok(NULL, "/")) != NULL)
            {
                printf("enkrip2 %s\n", enkrip);
                char str2[1024];
                bzero(fpath, sizeof(fpath));
                strcpy(fpath, inppath);
                printf("cek fpath %s\n", fpath);
                strcpy(str2, strstr(fpath, enkrip));
                printf("new enkrip %s\n", str2);
                atbash(str2);

                //cari pathsetengah
                int enkriplen = strlen(str2);
                printf("real %d enkrip %d hasil %d\n", reallen, enkriplen, reallen - enkriplen);
                bzero(pathsetengah, sizeof(pathsetengah));
                strncpy(pathsetengah, fpath, reallen - enkriplen);
                printf("pathse %s\n", pathsetengah);
                bzero(fpath, sizeof(fpath));

                sprintf(outpath, "%s%s%s", dirpath, pathsetengah, str2);
                printf("fpathreal %s\n", outpath);
            }
        }
    }
    else if (strstr(fpath, "/RX_") != NULL)
    {
        char bufpath[1024];
        bzero(fpath, sizeof(fpath));
        strcpy(fpath, inppath);
        //dapetin yang ga di hash
        int bufreallen = strlen(fpath);
        bzero(fpath, sizeof(fpath));
        //dapeting yang di hash
        strcpy(fpath, inppath);
        char *bufstr = strstr(fpath, "/RX_");
        printf("str %s\n", bufstr);
        // int m = 0;
        // char arr2[100][1024];
        char *bufenkrip = strtok(bufstr, "/");
        printf("enkrip %s\n", bufenkrip);

        if ((bufenkrip = strtok(NULL, "/")) != NULL)
        {
            printf("enkrip2 %s\n", bufenkrip);
            char bufstr2[1024];
            bzero(fpath, sizeof(fpath));
            strcpy(fpath, inppath);
            printf("cek fpath %s\n", fpath);
            strcpy(bufstr2, strstr(fpath, bufenkrip));
            printf("new enkrip %s\n", bufstr2);

            // rot13(bufstr2);
            //cari bufpath
            int bufenkriplen = strlen(bufstr2);
            printf("real %d enkrip %d hasil %d\n", bufreallen, bufenkriplen, bufreallen - bufenkriplen);
            bzero(bufpath, sizeof(bufpath));
            strncpy(bufpath, fpath, bufreallen - bufenkriplen - 1);
            printf("pathse %s\n", bufpath);

            bzero(fpath, sizeof(fpath));
            sprintf(fpath, "%s%s", dirpath, bufpath);
        }

        if (in(jumlahket, fpath))
        {
            printf("dirasli\n");
            bzero(fpath, sizeof(fpath));
            char pathsetengah[1024];

            strcpy(fpath, inppath);

            // char token1[1024] ;
            // strcpy(token1, strtok(fpath, "AtoZ_"));
            // strcpy(pathsetengah, token1);
            // printf("token 1 %s\n", pathsetengah);

            //dapetin yang ga di hash
            int reallen = strlen(fpath);
            bzero(fpath, sizeof(fpath));
            //dapeting yang di hash
            strcpy(fpath, inppath);
            char *str = strstr(fpath, "/RX_");
            printf("str %s\n", str);
            int m = 0;
            char arr2[100][1024];
            char *enkrip = strtok(str, "/");
            printf("enkrip %s\n", enkrip);

            if ((enkrip = strtok(NULL, "/")) != NULL)
            {
                printf("enkrip2 %s\n", enkrip);
                char str2[1024];
                // bzero(fpath,sizeof(fpath));
                // strcpy(fpath, inppath);
                // printf("cek fpath %s\n", fpath);
                strcpy(str2, enkrip);
                // printf("new enkrip %s\n", str2);
                char realdecryp[100];

                bzero(realdecryp, sizeof(realdecryp));
                vigeneredecode(str2, realdecryp);
                printf("decr %s\n", realdecryp);
                atbash(realdecryp);
                printf("enc %s\n", realdecryp);
                // rot13(str2);
                //cari pathsetengah
                int enkriplen = strlen(str2);
                printf("real %d enkrip %d hasil %d\n", reallen, enkriplen, reallen - enkriplen);
                bzero(pathsetengah, sizeof(pathsetengah));
                strncpy(pathsetengah, fpath, reallen - enkriplen);
                printf("pathse %s\n", pathsetengah);
                bzero(fpath, sizeof(fpath));

                sprintf(outpath, "%s%s/%s", dirpath, pathsetengah, realdecryp);
                printf("fpathreal mv %s\n", outpath);
            }
        }
        else
        {
            printf("next dir\n");
            char pathsetengah[1024];

            strcpy(fpath, inppath);

            // char token1[1024] ;
            // strcpy(token1, strtok(fpath, "AtoZ_"));
            // strcpy(pathsetengah, token1);
            // printf("token 1 %s\n", pathsetengah);

            //dapetin yang ga di hash
            int reallen = strlen(fpath);
            bzero(fpath, sizeof(fpath));
            //dapeting yang di hash
            strcpy(fpath, inppath);
            char *str = strstr(fpath, "/RX_");
            printf("str %s\n", str);
            int m = 0;
            char arr2[100][1024];
            char *enkrip = strtok(str, "/");
            printf("enkrip %s\n", enkrip);

            if ((enkrip = strtok(NULL, "/")) != NULL)
            {
                printf("enkrip2 %s\n", enkrip);
                char str2[1024];
                bzero(fpath, sizeof(fpath));
                strcpy(fpath, inppath);
                printf("cek fpath %s\n", fpath);
                strcpy(str2, strstr(fpath, enkrip));
                printf("new enkrip %s\n", str2);
                atbash(str2);
                rot13(str2);
                //cari pathsetengah
                int enkriplen = strlen(str2);
                printf("real %d enkrip %d hasil %d\n", reallen, enkriplen, reallen - enkriplen);
                bzero(pathsetengah, sizeof(pathsetengah));
                strncpy(pathsetengah, fpath, reallen - enkriplen);
                printf("pathse %s\n", pathsetengah);
                bzero(fpath, sizeof(fpath));

                sprintf(outpath, "%s%s%s", dirpath, pathsetengah, str2);
                printf("fpathreal %s\n", outpath);
            }
        }
    }
    else
    {
        sprintf(outpath, "%s%s", dirpath, inppath);
    }
}

void dirbaru(char *inppath, char *outpath)
{
    //kalo misale AtoZ didaleme foldernya di enkripsi tapi inppathnya gausah dienrkirpsi
    //tapikalo gaada atoz langsung di append pake dirpath

    char fpath[1024];
    bzero(fpath, sizeof(fpath));
    sprintf(fpath, "%s%s", dirpath, inppath);
    if (strstr(fpath, "/AtoZ_") != NULL)
    {
        char pathsetengah[1024];

        strcpy(fpath, inppath);
        int reallen = strlen(fpath);
        bzero(fpath, sizeof(fpath));
        //dapeting yang di hash
        strcpy(fpath, inppath);
        char *str = strstr(fpath, "/AtoZ_");
        printf("str %s\n", str);
        int m = 0;
        char arr2[100][1024];
        char *enkrip = strtok(str, "/");
        printf("enkrip %s\n", enkrip);

        if ((enkrip = strtok(NULL, "/")) != NULL)
        {
            printf("enkrip2 %s\n", enkrip);
            char str2[1024];
            bzero(fpath, sizeof(fpath));
            strcpy(fpath, inppath);
            printf("cek fpath %s\n", fpath);
            strcpy(str2, strstr(fpath, enkrip));
            printf("new enkrip %s\n", str2);
            // atbash(str2);

            //cari pathsetengah
            int enkriplen = strlen(str2);
            printf("real %d enkrip %d hasil %d\n", reallen, enkriplen, reallen - enkriplen);
            bzero(pathsetengah, sizeof(pathsetengah));
            strncpy(pathsetengah, fpath, reallen - enkriplen);
            printf("pathse %s\n", pathsetengah);
            bzero(outpath, sizeof(outpath));

            sprintf(outpath, "%s%s%s", dirpath, pathsetengah, str2);
            // printf("fpathreal %s\n", outpath);
        }

        else
        {
            bzero(outpath, sizeof(outpath));
            sprintf(outpath, "%s%s", dirpath, inppath);
        }
    }
    else if (strstr(fpath, "RX_") != NULL)
    {
        if (in(jumlahket, fpath))
        {
            char pathsetengah[1024];

            strcpy(fpath, inppath);
            int reallen = strlen(fpath);
            bzero(fpath, sizeof(fpath));
            //dapeting yang di hash
            strcpy(fpath, inppath);
            char *str = strstr(fpath, "/RX_");
            printf("str %s\n", str);
            int m = 0;
            char arr2[100][1024];
            char *enkrip = strtok(str, "/");
            printf("enkrip %s\n", enkrip);

            if ((enkrip = strtok(NULL, "/")) != NULL)
            {
                printf("enkrip2 %s\n", enkrip);
                char str2[1024];
                bzero(fpath, sizeof(fpath));
                strcpy(fpath, inppath);
                printf("cek fpath %s\n", fpath);
                strcpy(str2, strstr(fpath, enkrip));
                printf("new enkrip %s\n", str2);
                // atbash(str2);

                //cari pathsetengah
                int enkriplen = strlen(str2);
                printf("real %d enkrip %d hasil %d\n", reallen, enkriplen, reallen - enkriplen);
                bzero(pathsetengah, sizeof(pathsetengah));
                strncpy(pathsetengah, fpath, reallen - enkriplen);
                printf("pathse %s\n", pathsetengah);
                bzero(outpath, sizeof(outpath));

                sprintf(outpath, "%s%s%s", dirpath, pathsetengah, str2);
                // printf("fpathreal %s\n", outpath);
            }
            else
            {
                bzero(outpath, sizeof(outpath));
                sprintf(outpath, "%s%s", dirpath, inppath);
            }
        }
        else
        {
            char pathsetengah[1024];

            strcpy(fpath, inppath);
            int reallen = strlen(fpath);
            bzero(fpath, sizeof(fpath));
            //dapeting yang di hash
            strcpy(fpath, inppath);
            char *str = strstr(fpath, "/RX_");
            printf("str %s\n", str);
            int m = 0;
            char arr2[100][1024];
            char *enkrip = strtok(str, "/");
            printf("enkrip %s\n", enkrip);

            if ((enkrip = strtok(NULL, "/")) != NULL)
            {
                printf("enkrip2 %s\n", enkrip);
                char str2[1024];
                bzero(fpath, sizeof(fpath));
                strcpy(fpath, inppath);
                printf("cek fpath %s\n", fpath);
                strcpy(str2, strstr(fpath, enkrip));
                printf("new enkrip %s\n", str2);
                // atbash(str2);

                //cari pathsetengah
                int enkriplen = strlen(str2);
                printf("real %d enkrip %d hasil %d\n", reallen, enkriplen, reallen - enkriplen);
                bzero(pathsetengah, sizeof(pathsetengah));
                strncpy(pathsetengah, fpath, reallen - enkriplen);
                printf("pathse %s\n", pathsetengah);
                bzero(outpath, sizeof(outpath));

                sprintf(outpath, "%s%s%s", dirpath, pathsetengah, str2);
                // printf("fpathreal %s\n", outpath);
            }
            else
            {
                bzero(outpath, sizeof(outpath));
                sprintf(outpath, "%s%s", dirpath, inppath);
            }
        }
    }
    else
    {
        bzero(outpath, sizeof(outpath));
        sprintf(outpath, "%s%s", dirpath, inppath);
    }
}

void recursidir(char *path)
{
    DIR *indir;
    struct dirent *entr;
    char temppath[1024];
    indir = opendir(path);
    if (indir == NULL)
        return -errno;

    while ((entr = readdir(indir)) != NULL)
    {
        if (!strcmp(entr->d_name, ".") || !strcmp(entr->d_name, ".."))
            continue;
        if (entr->d_type == 4)
        {
            char namafile[1024];
            strcpy(namafile, entr->d_name);
            atbash(namafile);
        }
        else
        {
            char namadir[1024];
            sprintf(temppath, "%s/%s", path, entr->d_name);
            printf("temp %s\n", temppath);
            strcpy(namadir, entr->d_name);
            atbash(namadir);
            recursidir(temppath);
        }
    }
}

static int xmp_unlink(const char *path)
{
    char fpath[1000];
    bzero(fpath, sizeof(fpath));
    dirasli(path, fpath);

    int res;

    res = unlink(fpath);

    char desc[1024];
    strcpy(desc, fpath);
    logsytem("WARNING", "UNLINK", desc);

    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_access(const char *path, int mask)
{
    char fpath[1000];
    sprintf(fpath, "%s%s", dirpath, path);
    if (access(fpath, F_OK) == -1)
    {
        memset(fpath, 0, 1000);
        dirasli(path, fpath);
    }

    int res;

    res = access(fpath, mask);

    char desc[1024];
    strcpy(desc, path);
    logsytem("INFO", "ACCESS", desc);

    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_rmdir(const char *path)
{
    char fpath[1000];
    dirasli(path, fpath);
    int res;

    res = rmdir(fpath);

    char desc[1024];
    strcpy(desc, fpath);
    logsytem("WARNING", "RMDIR", desc);

    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_rename(const char *from, const char *to, unsigned int flags)
{

    printf("from %s\n", from);
    printf("to %s\n", to);

    char ffrom[1000];
    sprintf(ffrom, "%s%s", dirpath, from);
    if (access(ffrom, F_OK) == -1)
    {
        memset(ffrom, 0, sizeof(ffrom));
        dirasli(from, ffrom);
    }

    char fto[1000];
    sprintf(fto, "%s%s", dirpath, to);
    if (access(fto, F_OK) == -1)
    {
        memset(fto, 0, sizeof(fto));
        dirbaru(to, fto);
    }

    int res;
    printf("from %s and to %s\n", ffrom, fto);
    res = rename(ffrom, fto);

    const char desc[1024];
    sprintf(desc, "%s::%s", ffrom, fto);
    logsytem("INFO", "RENAME", desc);

    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    char fpath[1000];
    char temp[1024];
    strcpy(temp, path);
    // strcpy(fpath,path);
    printf("getattr\n");
    printf("path %s\n", path);
    sprintf(fpath, "%s%s", dirpath, path);
    // changePath(fpath, path, 0, 1);
    if (access(fpath, F_OK) == -1)
    {
        bzero(fpath, sizeof(fpath));
        printf("gabisa akses\n");
        dirasli(temp, fpath);
    }
    // tryaccess
    int res;

    printf("acces fpath %s\n", fpath);
    res = lstat(fpath, stbuf);

    char desc[1024];
    strcpy(desc, path);
    logsytem("INFO", "GETATTR", desc);

    if (res != 0)
    {
        printf("error\n");
        return -ENOENT;
    }

    return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                       off_t offset, struct fuse_file_info *fi)
{
    int res;
    DIR *dp;
    struct dirent *de;

    (void)offset;
    (void)fi;
    char fpath[1000];
    char name[1000];
    char cekrename[128];
    printf("namadifusedir: %s\n", path);
    sprintf(fpath, "%s%s", dirpath, path);
    printf("fpathread ent %s\n", fpath);
    // strcpy(fpath, path);
    if (strcmp(path, "/") == 0)
    {
        printf("cuma /\n");
        bzero(fpath, sizeof(fpath));
        sprintf(fpath, "%s", dirpath);
    }
    else
    {
        printf("nggak cuma /\n");
        // sprintf(name,"%s",path);
        // // enc(name);
        // sprintf(fpath, "%s%s",dirpath,name);
        // printf("namadifusedir after dec: %s\n", name);
        // printf("fpath after dec: %s\n", fpath);

        if (strstr(fpath, "/AtoZ_") != NULL)
        {
            {
                bzero(fpath, sizeof(fpath));
                char pathsetengah[1024];
                strcpy(fpath, path);

                //dapetin yang ga di hash
                int reallen = strlen(fpath);

                //dapeting yang di hash
                bzero(fpath, sizeof(fpath));
                strcpy(fpath, path);
                char *str = strstr(fpath, "/AtoZ_");
                printf("str %s\n", str);
                int m = 0;
                char arr2[100][1024];
                char *enkrip = strtok(str, "/");
                printf("enkrip %s\n", enkrip);

                if ((enkrip = strtok(NULL, "/")) != NULL)
                {
                    printf("enkrip2 %s\n", enkrip);
                    char str2[1024];
                    bzero(fpath, sizeof(fpath));
                    strcpy(fpath, path);
                    printf("cek fpath %s\n", fpath);
                    bzero(str2, sizeof(str2));
                    strcpy(str2, strstr(fpath, enkrip));
                    printf("new enkrip %s\n", str2);
                    atbash(str2);

                    //cari pathsetengah
                    int enkriplen = strlen(str2);
                    printf("real %d enkrip %d hasil %d\n", reallen, enkriplen, reallen - enkriplen);
                    bzero(pathsetengah, sizeof(pathsetengah));
                    strncpy(pathsetengah, fpath, reallen - enkriplen);
                    printf("pathse %s\n", pathsetengah);
                    bzero(fpath, sizeof(fpath));

                    sprintf(fpath, "%s%s%s", dirpath, pathsetengah, str2);
                    // printf("fpathreal %s\n", fpath);
                }
                else
                {
                    sprintf(fpath, "%s%s", dirpath, path);
                }
            }
        }
        else if (strstr(fpath, "/RX_") != NULL)
        {
            bzero(fpath, sizeof(fpath));
            char bufpathsetengah[1024];
            strcpy(fpath, path);

            //dapetin yang ga di hash
            int bufreallen = strlen(fpath);

            //dapeting yang di hash
            bzero(fpath, sizeof(fpath));
            strcpy(fpath, path);
            char *bufstr = strstr(fpath, "/RX_");
            printf("str %s\n", bufstr);

            char *bufenkrip = strtok(bufstr, "/");
            printf("bufenkrip %s\n", bufenkrip);

            if ((bufenkrip = strtok(NULL, "/")) != NULL)
            {
                printf("bufenkrip2 %s\n", bufenkrip);
                char bufstr2[1024];
                bzero(fpath, sizeof(fpath));
                strcpy(fpath, path);
                printf("cek fpath %s\n", fpath);
                bzero(bufstr2, sizeof(bufstr2));
                strcpy(bufstr2, strstr(fpath, bufenkrip));
                printf("new bufenkrip %s\n", bufstr2);
                atbash(bufstr2);
                char anotherbuf[100];
                bzero(anotherbuf, sizeof(anotherbuf));
                vigeneredecode(bufstr2, anotherbuf);
                atbash(anotherbuf);
                //cari bufpathsetengah
                int enkriplen = strlen(bufstr2);
                printf("real %d enkrip %d hasil %d\n", bufreallen, enkriplen, bufreallen - enkriplen);
                bzero(bufpathsetengah, sizeof(bufpathsetengah));
                strncpy(bufpathsetengah, fpath, bufreallen - enkriplen - 1);
                printf("pathse %s\n", bufpathsetengah);
                bzero(fpath, sizeof(fpath));

                sprintf(fpath, "%s%s", dirpath, bufpathsetengah);

                sprintf(cekrename, "%s%s", dirpath, bufpathsetengah);
                // printf("fpathreal %s\n", fpath);
            }
            printf("fpath rx %s\n", fpath);

            if (in(jumlahket, fpath))
            {
                printf("detect move\n");
                bzero(fpath, sizeof(fpath));
                char pathsetengah[1024];
                strcpy(fpath, path);

                //dapetin yang ga di hash
                int reallen = strlen(fpath);

                //dapeting yang di hash
                bzero(fpath, sizeof(fpath));
                strcpy(fpath, path);
                char *str = strstr(fpath, "/RX_");
                printf("str %s\n", str);
                int m = 0;
                char arr2[100][1024];
                char *enkrip = strtok(str, "/");
                printf("enkrip %s\n", enkrip);

                if ((enkrip = strtok(NULL, "/")) != NULL)
                {
                    printf("enkrip2 %s\n", enkrip);
                    char str2[1024];
                    bzero(fpath, sizeof(fpath));
                    strcpy(fpath, path);
                    printf("cek fpath %s\n", fpath);
                    bzero(str2, sizeof(str2));
                    strcpy(str2, strstr(fpath, enkrip));
                    printf("new enkrip %s\n", str2);
                    // atbash(str2);
                    char buf[100];
                    bzero(buf, sizeof(buf));
                    vigeneredecode(str2, buf);
                    atbash(buf);
                    //cari pathsetengah
                    int enkriplen = strlen(str2);
                    printf("real %d enkrip %d hasil %d\n", reallen, enkriplen, reallen - enkriplen);
                    bzero(pathsetengah, sizeof(pathsetengah));
                    strncpy(pathsetengah, fpath, reallen - enkriplen);
                    printf("pathse %s\n", pathsetengah);
                    bzero(fpath, sizeof(fpath));

                    sprintf(fpath, "%s%s%s", dirpath, pathsetengah, buf);
                    // printf("fpathreal %s\n", fpath);
                }
                else
                {
                    sprintf(fpath, "%s%s", dirpath, path);
                }
            }
            else
            {
                bzero(fpath, sizeof(fpath));
                char pathsetengah[1024];
                strcpy(fpath, path);

                //dapetin yang ga di hash
                int reallen = strlen(fpath);

                //dapeting yang di hash
                bzero(fpath, sizeof(fpath));
                strcpy(fpath, path);
                char *str = strstr(fpath, "/RX_");
                printf("str %s\n", str);
                int m = 0;
                char arr2[100][1024];
                char *enkrip = strtok(str, "/");
                printf("enkrip %s\n", enkrip);

                if ((enkrip = strtok(NULL, "/")) != NULL)
                {
                    printf("enkrip2 %s\n", enkrip);
                    char str2[1024];
                    bzero(fpath, sizeof(fpath));
                    strcpy(fpath, path);
                    printf("cek fpath %s\n", fpath);
                    bzero(str2, sizeof(str2));
                    strcpy(str2, strstr(fpath, enkrip));
                    printf("new enkrip %s\n", str2);
                    atbash(str2);
                    rot13(str2);

                    //cari pathsetengah
                    int enkriplen = strlen(str2);
                    printf("real %d enkrip %d hasil %d\n", reallen, enkriplen, reallen - enkriplen);
                    bzero(pathsetengah, sizeof(pathsetengah));
                    strncpy(pathsetengah, fpath, reallen - enkriplen);
                    printf("pathse %s\n", pathsetengah);
                    bzero(fpath, sizeof(fpath));

                    sprintf(fpath, "%s%s%s", dirpath, pathsetengah, str2);
                    // printf("fpathreal %s\n", fpath);
                }
                else
                {
                    sprintf(fpath, "%s%s", dirpath, path);
                }
            }
        }
        else
        {
            sprintf(fpath, "%s%s", dirpath, path);
        }
    }
    printf("fpath readdir %s\n", fpath);
    dp = opendir(fpath);
    if (dp == NULL)
        return -errno;

    while ((de = readdir(dp)) != NULL)
    {
        printf("INI DE : %s\n", de->d_name);

        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        char fullpathname[1000];

        // sprintf(fullpathname, "%s/%s", fpath, de->d_name);
        char temp[1000];
        strcpy(temp, de->d_name);
        // printf("full1 %s\n", fullpathname);
        // atbash(temp);
        if (strstr(path, "/AtoZ_") != NULL && strcmp(".", temp) != 0 && strcmp("..", temp) != 0)
        {
            if (strstr(temp, ".") == NULL)
            {
                printf("chr %s\n", strstr(temp, "."));
                atbash(temp);
            }

            else
            {
                printf("chr2 %s\n", strstr(temp, "."));
                char ekstensi[100];
                strcpy(ekstensi, strstr(temp, "."));
                char arr[100][1024], namafile[1024];
                // strcpy(namafile, temp);
                char *potongtitik = strtok(temp, ".");
                int n = 0;
                while (potongtitik != NULL)
                {
                    // arr[n] = potongtitik;
                    strcpy(arr[n], potongtitik);
                    printf("tongtik %s\n", arr[n]);
                    n++;
                    potongtitik = strtok(NULL, ".");
                }
                strcpy(namafile, arr[n - 2]);
                atbash(namafile);
                bzero(temp, sizeof(temp));
                sprintf(temp, "%s.%s", namafile, arr[n - 1]);
                printf("temp %s\n", temp);
            }
        }
        else if (strstr(path, "/RX_") != NULL && strcmp(".", temp) != 0 && strcmp("..", temp) != 0)
        {

            if (in(jumlahket, cekrename))
            {
                printf("detect move tampil\n");
                if (strstr(temp, ".") == NULL)
                {
                    printf("chr %s\n", strstr(temp, "."));
                    char res[100];
                    bzero(res, sizeof(res));
                    strcpy(res, temp);
                    atbash(res);
                    char buf[100];
                    bzero(buf, sizeof(buf));
                    vigenereencode(res, buf);
                    bzero(temp, sizeof(temp));
                    strcpy(temp, buf);
                }

                else
                {
                    printf("chr2 %s\n", strstr(temp, "."));
                    char ekstensi[100];
                    strcpy(ekstensi, strstr(temp, "."));
                    char arr[100][1024], namafile[1024];
                    // strcpy(namafile, temp);
                    char *potongtitik = strtok(temp, ".");
                    int n = 0;
                    while (potongtitik != NULL)
                    {
                        // arr[n] = potongtitik;
                        strcpy(arr[n], potongtitik);
                        printf("tongtik %s\n", arr[n]);
                        n++;
                        potongtitik = strtok(NULL, ".");
                    }
                    strcpy(namafile, arr[n - 2]);
                    atbash(namafile);
                    char buf[100];
                    bzero(buf, sizeof(buf));
                    vigenereencode(namafile, buf);
                    bzero(temp, sizeof(temp));
                    sprintf(temp, "%s.%s", buf, arr[n - 1]);
                    printf("temp %s\n", temp);
                }
            }
            else
            {
                printf("else move\n");
                if (strstr(temp, ".") == NULL)
                {
                    printf("chr %s\n", strstr(temp, "."));
                    atbash(temp);
                    rot13(temp);
                }

                else
                {
                    printf("chr2 %s\n", strstr(temp, "."));
                    char ekstensi[100];
                    strcpy(ekstensi, strstr(temp, "."));
                    char arr[100][1024], namafile[1024];
                    // strcpy(namafile, temp);
                    char *potongtitik = strtok(temp, ".");
                    int n = 0;
                    while (potongtitik != NULL)
                    {
                        // arr[n] = potongtitik;
                        strcpy(arr[n], potongtitik);
                        printf("tongtik %s\n", arr[n]);
                        n++;
                        potongtitik = strtok(NULL, ".");
                    }
                    strcpy(namafile, arr[n - 2]);
                    atbash(namafile);
                    rot13(namafile);
                    bzero(temp, sizeof(temp));
                    sprintf(temp, "%s.%s", namafile, arr[n - 1]);
                    printf("temp %s\n", temp);
                }
            }
        }
        printf("type %d\n", de->d_type);
        printf("INI DE AFTER ENC: %s\n", temp);
        char desc[1024];
        strcpy(desc, path);
        logsytem("INFO", "READDIR", desc);
        res = (filler(buf, temp, &st, 0));
        if (res != 0)
            break;
    }
    closedir(dp);
    return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
    char fpath[1000];
    bzero(fpath, sizeof(fpath));
    dirbaru(path, fpath);
    printf("mkdir %s", path);
    // char *ptr = strrchr(path, '/');
    // char *filePtr = strstr(ptr, "/AtoZ_");
    // if (filePtr != NULL) {
    // 	if (filePtr - ptr == 0) {

    // 	}
    // }

    int res;
    printf("fpath mkdir %s\n", fpath);
    res = mkdir(fpath, mode);
    char desc[1024];
    strcpy(desc, fpath);
    logsytem("INFO", "MKDIR", desc);
    if (res == -1)
        return -errno;

    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    char name[1000];
    char open_name[1000];
    sprintf(fpath, "%s%s", dirpath, path);
    if (strcmp(path, "/") == 0)
    {
        sprintf(fpath, "%s", dirpath);
    }
    else
    {
        dirasli(path, fpath);
    }
    int res = 0;
    int fd = 0;

    (void)fi;
    printf("READ FPATH: %s\n", fpath);
    fd = open(fpath, O_RDONLY);
    printf("READ -> %d", fd);
    if (fd == -1)
        return -errno;

    res = pread(fd, buf, size, offset);
    if (res == -1)
        res = -errno;
    char desc[1024];
    strcpy(desc, fpath);
    logsytem("INFO", "READ", fpath);
    close(fd);
    return res;
}

static int xmp_create(const char *path, mode_t mode,
                      struct fuse_file_info *fi)
{
    char fpath[1000];
    dirbaru(path, fpath);

    int res;

    res = open(fpath, fi->flags, mode);

    char desc[1024];
    strcpy(desc, fpath);
    logsytem("INFO", "CREAT", desc);

    if (res == -1)
        return -errno;

    fi->fh = res;
    return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
    char fpath[1000];
    dirasli(path, fpath);

    int res;

    res = open(fpath, fi->flags);

    char desc[1024];
    strcpy(desc, fpath);
    logsytem("INFO", "OPEN", desc);

    if (res == -1)
        return -errno;

    fi->fh = res;
    return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
    char fpath[1024];
    dirbaru(path, fpath);

    int res;
    if (S_ISFIFO(mode))
        res = mkfifo(fpath, mode);
    else
        res = mknod(fpath, mode, rdev);
    if (res == -1)
        return -errno;
    return 0;

    char desc[1024];
    strcpy(desc, fpath);
    logsytem("INFO", "MKNOD", desc);
    return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size,
                     off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    dirbaru(path, fpath);

    int fd;
    int res;

    (void)fi;
    if (fi == NULL)
        fd = open(fpath, O_WRONLY);
    else
        fd = fi->fh;

    if (fd == -1)
        return -errno;

    res = pwrite(fd, buf, size, offset);

    char desc[1024];
    strcpy(desc, fpath);
    logsytem("INFO", "WRITE", desc);

    if (res == -1)
        res = -errno;

    if (fi == NULL)
        close(fd);
    return res;
}

static const struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .access = xmp_access,
    .readdir = xmp_readdir,
    .mkdir = xmp_mkdir,
    .unlink = xmp_unlink,
    .rmdir = xmp_rmdir,
    .rename = xmp_rename,
    .create = xmp_create,
    .read = xmp_read,
    .mknod = xmp_mknod,
    .write = xmp_write};

int main(int argc, char *argv[])
{
    umask(0);
    char deb[100] = "yrhzyrhz/zmf";
    atbash(deb);
    printf("debug %s\n", deb);
    jumlah = 0;
    jumlahket = 0;
    return fuse_main(argc, argv, &xmp_oper, NULL);
}
