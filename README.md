## soal-shift-sisop-modul-4-IT02-2021
---

Penjelasan dan penyelesaian Soal Shift 4 Sistem Operasi 2021\
Kelompok IT02

1. Muhammad Hilmi Ramadhan (05311940000044)
2. Lambang Akbar Wijayadi (05311940000006)
3. Mulki Kusumah (05311940000043)

---

## Daftar Isi

- [Soal 1](#soal-1)

  - [Soal 1.a.](#soal-1a)
  - [Soal 1.b.](#soal-1b)
  - [Soal 1.c.](#soal-1c)
  - [Soal 1.d.](#soal-1d)
  - [Soal 1.e.](#soal-1e)


- [Soal 2](#soal-2)

  - [Soal 2.a.](#soal-2a)
  - [Soal 2.b.](#soal-2b)
  - [Soal 2.c.](#soal-2c)
  - [Soal 2.d.](#soal-2d)
  - [Soal 2.e.](#soal-2e)

- [Soal 3](#soal-3)
  - [Soal 3.a.](#soal-3a)
  - [Soal 3.b.](#soal-3b)
  - [Soal 3.c.](#soal-3c)
  - [Soal 3.d.](#soal-3d)
  - [Soal 3.e.](#soal-3e)

- [Soal 4](#soal-4)
  - [Soal 4.a.](#soal-4a)
  - [Soal 4.b.](#soal-4b)
  - [Soal 4.c.](#soal-4c)
  - [Soal 4.d.](#soal-4d)
  - [Soal 4.e.](#soal-4e)


---

<br>


## Soal 1
---

Source Code tersedia pada : [Codingannya](./SinSeiFS_IT02.c)

## **Analisa Soal**

Secara umum, kami menangkap bahwa program yang harus dibuat merupakan sebuah gabungan dari *fuse* dan *filesystem* dimana program dapat mengubah semua file yang ada pada direktori menjadi ter-*encode* menggunakan `Atbash cipher(mirror)`

Hal-hal yang perlu diperhatikan diantaranya :
1. *Filesystem* berfungsi normal layaknya linux pada umumnya, *Mount source (root) filesystem* adalah *directory* `/home/[USER]/Downloads`.
2. Dalam penamaan file '`/`' diabaikan, dan ekstensi tidak perlu di-*encode*.
3. Referensi : `https://www.dcode.fr/atbash-cipher`
4. Contoh *encode* yang diharapkan : \
terdapat `file` bernama `kucinglucu123.jpg` pada direktori `DATA_PENTING`.\
`"AtoZ_folder/DATA_PENTING/kucinglucu123.jpg"` -> `“AtoZ_folder/WZGZ_KVMGRMT/pfxrmtofxf123.jpg”`
5. Tidak diperkenankan menggunakan `system()` dan `exec()`, terkecuali terdapat pengecualian di butir soal.
6. Pengerjaan hanya dilakukan dalam 1 `file program C` dengan format nama `SinseiFS_[Kelompok].c`.


<br>

**Cara pengerjaan**
---

Dalam pengerjaan soal 1 ini kami menggunakan library :

```cpp
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

static const char *dirpath = "/home/lambang/Downloads/fuse";
```


Selain library diatas, kami juga mendefinisikan dirpath atau direktori yang akan digunakan sebagai mount pada FUSE systemnya.

<br>

## Soal 1.a.
## **Analisa Soal**
Pada soal 1.a sebagai permulaan dari soal ini kami diminta untuk membuat direktori yang diawali dengan `"AtoZ_"`, yang mana direktori ini akan menjadi direktori ter-*encode*. Semua file ataupun folder yang terdapat dalam directory dengan awalan `AtoZ_` akan terencode begitu juga dengan subfile dan subdirectorynya.

**Cara Pengerjaan**
---
Untuk membuat FUSE system sesuai dengan linux pada umumnya kita menggunakan referensi template dari modul-4 begitu juga dari contoh passthrough pada dokumentasi FUSE. Untuk menginisiasi awal FUSE kami perlu mendaklarasikan fuse_operations seperti code dibawah ini.

```cpp
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
```
_Pada struct diatas akan menjadi fungsi yang bisa berjalan dalam FUSE-nya, misalnya bisa melakukan rename, mkdir ls dan lain lain_



Untuk menjaalankan fungsi yang bisa berjalan terutama fungsi listingnya kita perlu mendefinisikan fuse operations getattr untuk mendapatkan attribut dalam sebuah direktori, attribut sendiri adalah detail detail dari apapun dalam direktori. Jika fungsi getattr tidak didefinisikan maka fungsi fuse tidak akan bisa berjalan.
```cpp
static int xmp_getattr(const char *path, struct stat *stbuf)
{
    char fpath[1000];
    char temp[1024];
    strcpy(temp, path);
    printf("getattr\n");
    printf("path %s\n", path);
    sprintf(fpath, "%s%s", dirpath, path);
    if (access(fpath, F_OK) == -1)
    {
        bzero(fpath, sizeof(fpath));
        printf("gabisa akses\n");
        dirasli(temp, fpath);
    }
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
```
_Dalam fungsi diatas kita akan melakukan pengecekan terlebih dahulu apakah path tersebut bisa dibuka atau tidak, jika tidak bisa dibuka kemungkinkan adalah file dengan nama terenkripsi supaya bisa sesuai dengan file asli pada dirpath_

```cpp
void dirasli(char *inppath, char *outpath)
{
    char fpath[1024];
    bzero(fpath, sizeof(fpath));
    sprintf(fpath, "%s%s", dirpath, inppath);
    if (strstr(fpath, "/AtoZ_") != NULL)
    {
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
                atbash(str2);
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
        int bufreallen = strlen(fpath);
        bzero(fpath, sizeof(fpath));
        strcpy(fpath, inppath);
        char *bufstr = strstr(fpath, "/RX_");
        printf("str %s\n", bufstr);
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
                strcpy(str2, enkrip);
                char realdecryp[100];

                bzero(realdecryp, sizeof(realdecryp));
                vigeneredecode(str2, realdecryp);
                printf("decr %s\n", realdecryp);
                atbash(realdecryp);
                printf("enc %s\n", realdecryp);
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
```
_Fungsi diatas mengubah sebuah nama direktori yang terencode dalam mount filesystemnya, menjadi nama file asli pada dirpath supaya file tersebut bisa dibuka, Fungsi ini akan mengecek apakah awal direktori terdiri dari kata AtoZ atau RX jika iya akan dilakukan decrypting sesuai dengan metode yang digunakan atau arahan dari soal misalnya jika awal dari direktorinya AtoZ maka didekrip dengan atbash, jika RX maka akan dicek terlebih dahulu apakah perubahannya berasal dari rename atau bukan jika iya maka akan menggunakan vigenere cipher dan atbahs, jika tidak maka akan didecrypt dengan rot13 dan atbash. Fungsi ini akan mengembalikan fullpath asli terdekripsi dari dirpathnya._

Dalam command `ls` kita akan dimunculkan nama nama file yang harusnya sudah sesuai dengan permintaan soal. maka dari itu kita akan berangkat dari fungsi readdir dan getattr. Dimana dalam fungsi readdir ini nantinya akan menampilkan nama file yang sudah terenkripsi kepada user. Pertama akan dicek terlebih dahulu apakah awalan dari directorynya merupaka directory khusus atau bukan. jika iya maka akan di proses dalam listing filenya, jika bukan akan ditampilkan seperti biasa. 
```cpp
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
                    char *potongtitik = strtok(temp, ".");
                    int n = 0;
                    while (potongtitik != NULL)
                    {
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
```
_implementeasi command LS dalam fuse, dimana perlu dilakukan pengecekan secara berlapis apakah directory yang dibaca terdapat string `AtoZ` atau `RX` nya jika iya maka akan di proses, selanjutnya jika berawlan `RX` akan dicek apakah merupakan hasil produksi dari rename atau bukan, jika iya akan menggunakan vigenere cipher dan jika tidak akan menggunakan rot13 dan atbash saja._
<br>

## Soal 1.b.
## **Analisa Soal**
Pada soal 1.b sebagai lanjutan dari soal yang sebelumnya, yang mana pada dasarnya jika terdapat sebuah direktori di-*rename* dengan awalan `"AtoZ_"`, maka direktori akan berubah menjadi direktori yang ter-*encode*/ 

**Cara Pengerjaan**
---
Dalam pengerjaanya hanya terpengaruh dalam fungsi getattr dan readdir saja dimana sudah dijelaskan dalam poin sebelumnya.
<br>

## Soal 1.c.
## **Analisa Soal**
Pada soal 1.c kami diminta untuk melakukan proses *encode* jika suatu direktori yang telah terenkripsi dilakukan perubahan nama atau *rename*, maka direktori tersebut akan seluruhnya ter-*decode*.

**Cara Pengerjaan**
---

<br>

## Soal 1.d.
## **Analisa Soal**
Pada soal 1.d yang mana diminta untuk melakukan pencatatan jika terjadi pembuatan sebuah direktori ter-*encode* ke dalam sebuah log. Dengan format : `/home/[USER]/Downloads/[Nama Direktori] → /home/[USER]/Downloads/AtoZ_[Nama Direktori]`

**Cara Pengerjaan**
---

<br>

## Soal 1.e.
## **Analisa Soal**
Pada soal 1.e perlakuan *encode* dengan metode *Atbash cipher* juga berlaku pada semua yang ada di dalamnya atau dengan melakukan *recursive*.

**Cara Pengerjaan**
---

<br>

<br>


**Kendala**
---
Pada awalnya dari kelompok kami kesulitan untuk mengatasi

<br>


**Screenshoot Hasil Run Program Soal Nomor 1 untuk multi connection**\
![Foto](./img/soal1/multicon.png)
---

<br>


## Soal 2
---

Source Code terdapat pada : [Codingannya](./SinSeiFS_IT02.c)

## **Analisa Soal**

Dari kami merasa soal ini lebih utamanya meminta tambahan metode enkripsi agar semakin aman yang sesuai rancangan oleh Sei pada soal.


<br>


**Cara pengerjaan**
---

<br>

## Soal 2.a.
## **Analisa Soal**
Pada soal 2.a diminta untuk melakukan perubahan nama isi sesuai kasus pada [Soal 1](#soal-1) yang mana diberi sebuah tambahan `ROT13 (Atbash + ROT13)`. Hal ini terjadi jika direktori dibuat dengan awalan `"RX_[Nama]"`.

**Cara Pengerjaan**
---

<br>

## Soal 2.b.
## **Analisa Soal**
Pada soal 2.b serupa dengan [soal 2.a](#soal-2a), hanya yang menjadi titik berat perbedaan adalah ketika hal tersebut terjadi ketika sebuah direktori di-*rename*, maka direktori tersebut akan ter-*encode* beserta isinya dengan perubahan nama isi sesuai ketentuan pada [Soal 1](#soal-1) dengan algoritma tambahan `Vigenere Cipher` dengan *Key* `"SISOP" (Case-sensitive, Atbash + Viegenere)`.

**Cara Pengerjaan**
---

<br>

## Soal 2.c.
## **Analisa Soal**
Pada soal 2.c diharapkan dalam perubahan nama direktori `(Dihilangkan "RX_" nya)`, maka folder keseluruhan menjadi tidak ter-*encode* dan isi direktori tersebut akan ter-*decode* berdasarkan nama aslinya.

**Cara Pengerjaan**
---

<br>

## Soal 2.d.
## **Analisa Soal**
Pada soal 2.d diminta untuk setiap aktivitas menyangkut direktori ter-*encode* baik itu `mkdir` ataupun `rename` akan tercatat dengan baik pada sebuah `log file` beserta dengan metode nya (apakah itu `mkdir` atau `rename`).

**Cara Pengerjaan**
---

<br>

## Soal 2.e.
## **Analisa Soal**
Pada soal 2.e menurut kami lumayan rumit karena dari soal diminta untuk melakukan pemecahan dari `file-file` asli menjadi `file-file kecil` yang sebesar `1024 bytes`. Jika diakses melalui `filesystem` rancangan `Sin` dan `Sei` akan menjadi normal. Guna memperjelas berikut merupakan visualisasinya :\
`Suatu_File.txt` berukuran 3 kiloBytes pada direktori asli akan menjadi 3 file kecil yakni : <br>
`Suatu_File.txt.000`\
`Suatu_File.txt.001`\
`Suatu_File.txt.002`\
<br>

Ketika diakses melalui `filesystem` hanya akan muncul `Suatu_File.txt`


**Cara Pengerjaan**
---

<br>

<br>
**Kendala**
---
Tidak ada
<br>

**Screenshoot Hasil Run Program Soal2 dengan IPC [soal2](./soal2/)**

![Foto](./img/soal2/nomer2.png)
---

---


<br>
<br>

## Soal 3
---

Source Code tersedia pada : [Codingannya](./SinSeiFS_IT02.c)

## **Analisa Soal**
Secara umum, kami menangkap bahwa program yang harus dibuat merupakan sebuah fitur tambahan 
<br>

**Cara pengerjaan**
---

Dalam menyelesaikan program yang diminta oleh 
<br>


## Soal 3.a.
## **Analisa Soal**
Pada soal 3.a kami diminta untuk menyeleksi apakah sebuah direktori tersebut `direktori spesial` atau bukan. Syarat menjadi hal itu adalah direktori dibuat dengan awalan `"A_is_a_"`.

**Cara Pengerjaan**
---
Dalam menjawab soal 3.a. kami membuat
<br>

## Soal 3.b.
## **Analisa Soal**
Pada soal 3.b serupa dengan [Soal 3.a.](#soal-3a), yang membedakannya hanya lah jika aksi yang diharapkan pada soal sebelumnya adalah membuat direktori atau `mkdir` pada soal ini diminta dengan aksi penamaan ulang atau `rename` dengan awalan `"A_is_a_"`

**Cara Pengerjaan**
---

<br>

## Soal 3.c.
## **Analisa Soal**
Pada soal 3.c meminta kami untuk melakukan mempersiapkan kemungkinan jika direktori yang telah ter-*enkripsi* dilakukan perubahan nama atau *rename* dari `"A_is_a_"` pada bagian awal nama folder maka direktori tersebut akan *rever* menjadi sebuah direktori yang normal.

**Cara Pengerjaan**
---

<br>

## Soal 3.d.
## **Analisa Soal**
Pada soal 3.d terkait dengan [Soal 3.a.](#soal-3a), yang mana akan mengembalikan enkripsi atau *encoding* pada direktori `“AtoZ_”` maupun `“RX_”`. Akan tetapi aturan masing-masing dari mereka tetap berjalan pada direktori di dalamnya `(sifat recursive "AtoZ_" dan "RX_" tetap berjalan pada subdirektori)`.

**Cara Pengerjaan**
---

<br>

## Soal 3.e.
## **Analisa Soal**
Pada soal 3.e juga masih terkait dengan [Soal 3.a.](#soal-3a), pada direktori spesial semua nama file (tidak termasuk ekstensi) pada `fuse` akan berubah menjadi `lowercase insensitive` dan diberi ekstensi baru berupa nilai desimal dari binner perbedaan namanya. Contohnya:\
Pada direktori asli nama filenya adalah `"File_CoNtoH.txt"`. Maka pada `fuse` akan menjadi `"file_contoh.txt.1321"`. Dimana `1321` berasal dari biner `10100101001`. 

**Cara Pengerjaan**
---

<br>


**Kendala**
---
Pada awalnya dari kelompok kami kesulitan untuk mengatasi
<br>

**Screenshoot Hasil Run Program Soal3 untuk fitur '*' [soal3](./soal3/soal3.c)**
![Foto](./img/soal3/bintang.png)
---

<br>
<br>

## Soal 4
---

Source Code tersedia pada : [Codingannya](./SinSeiFS_IT02.c)

## **Analisa Soal**
Secara umum, kami menangkap bahwa program yang diminta merupakan sebuah program yang dapat mengontrol dan memonitor kegiatan yang ada pada `filesystem` mereka Sin dan Sei.
<br>

**Cara pengerjaan**
---

Dalam menyelesaikan program yang diminta oleh soal untuk membuat catatan log dimana setiap perintah yang diproses pada FUSE systemnya. logsystem ini akan memilik format khusus seperti 
`[Level]::[dd][mm][yyyy]-[HH]:[MM]:[SS]:[CMD]::[DESC :: DESC]`

level didefinisikan sebagai "INFO" atau "WARNING" namun untuk "WARNING" hanya digunakan ketika terjadi penghapusan directory atau file dalam FUSE sedangkan yang lainya menggunakan level "INFO" selanjutnya dipisahkan dengan `::` untuk mendapatkan waktu dari kejadian fungsi tersebut dipanggil, `CMD` mendefinisikan command apa yang sedang terjadi, dan `DESC` adalah keterangan, pada umumnya keterangan ini diisi oleh path dari file yang diproses. 

```cpp
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
```
_Pada fungsi diatas adalah fungsi keseluruhan bagaimana system loggingnya bekerja secara utuh. Dimana kita perlu menggunakan bantuan fungsi localtime() dari library untuk mendapatkan waktu sekarang untuk catat lognya, kemudian dengan bantuan `sprinf` akan melakukan concating string sesuai dengan format log yang sudah ditentukan. Selanjutnya dengan bantuan `fopen` akan melakukan appending untuk lognya secara berkala dari string yang sudah disusun tadi._
<br>


## Soal 4.a.
## **Analisa Soal**
Pada soal 4.a merupakan spesifikasi pertama yang dibutuhkan dalam `file monitor` yang akan dibuat, `Log System` akan terbentuk dengan nama file `"SinSeiFS.log"` pada direktori `home` pengguna `(/home/[USER]/SinSeiFS.log)`. Log system ini akan menyimpan daftar perintah `system call` yang telah dijalankan pada `filesystem`.

**Cara Pengerjaan**
---
Dalam menjawab soal 4.a. kami membuat hanya perlu mendifiniskan dimana path dari log system berada. Pada code line 298 didefinisikan path dari lognya yang berada pada `/home/lambang/SinSeiFS.log` 

```cpp
void logsytem(char *level, char *cmd, char *desc)
{
    FILE *fp;
    ....
    fp = fopen("/home/lambang/SinSeiFS.log", "a");
    fprintf(fp, "%s\n", log);
    fclose(fp);
}
```
_Pendefinisian path untuk log system_
<br>

## Soal 4.b.
## **Analisa Soal**
Pada soal 4.b kami dituntut untuk merapihkan `log` yang telah dibuat akan terbagi menjadi dua level secara garis besar, yaitu `INFO` dan `WARNING`.

**Cara Pengerjaan**
---
karena dalam deklarasi fungsi logsystemnya terdapat argumen `char* level` sebagai input LEVEL dalam lognya nanti. berikut adalah penggunakan logsystem dengan level INFO dan WARNING.

```cpp
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
```
_Penggunaan logsystem WARNING_

```cpp
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
```
_Penggunaan logsystem INFO_

<br>

## Soal 4.c.

## **Analisa Soal**
Pada soal 4.c diminta khusus dalam `log level WARNING` mencatat syscall `rmdir` dan `unlink`.

**Cara Pengerjaan**
---
Berikut adalah implementasi log level WARNING untuk fungsi rmdir dan unlink.

```cpp
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
```
_Implementasi level WARNING pada fungsi rmdir_

```cpp
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
```
_Implementasi level WARNING pada fungsi unlink_

<br>

## Soal 4.d.
## **Analisa Soal**
Pada soal 4.d akan diminta melakukan pencatat yang `non-WARNING` atau sisanya akan tercatat pada `level INFO`.

**Cara Pengerjaan**
---
berikut adalah implementasi dari level INFO.

```cpp
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
```
_Implementasi level INFO pada fungsi Open_
<br>

## Soal 4.e.
## **Analisa Soal**
Pada soal 4.e diminta untuk melakukan perincian logika dalam melakukan format dari `logging filesystem` yaitu :\
<br>
`[Level]::[dd][mm][yyyy]-[HH]:[MM]:[SS]:[CMD]::[DESC :: DESC]`
<br>

`Level : Level logging, dd : 2 digit tanggal, mm : 2 digit bulan, yyyy : 4 digit tahun, \
 HH : 2 digit jam (format 24 Jam),MM : 2 digit menit, SS : 2 digit detik, \
 CMD : System Call yang terpanggil, DESC : informasi dan parameter tambahan`

`INFO::28052021-10:00:00:CREATE::/test.txt`\
`INFO::28052021-10:01:00:RENAME::/test.txt::/rename.txt`

**Cara Pengerjaan**
---
Untuk mendapatkan logsystem sesuai dengna formatnya maka diperlukan fungsi untuk mendapatkan waktu sekarang dimana kami dengan bantuan library dan fungsi `localtime()` berhasil mendapatkanya dan berikut adalah implementasinya dalam fungsi logsystem.

```cpp
void logsytem(char *level, char *cmd, char *desc)
{
    time_t t = time(NULL);
    struct tm *ptm = localtime(&t);
    char log[200], times[100];
    ....
    sprintf(times, "::%02d%02d%02d-%02d:%02d:%02d::", ptm->tm_year + 1900 - 2000, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
    sprintf(log, "%s%s%s::%s", level, times, cmd, desc);
    fp = fopen("/home/lambang/SinSeiFS.log", "a");
    fprintf(fp, "%s\n", log);
    fclose(fp);
}
```
_Pada fungsi diatas kami mendapatkan waktu sekarang dengan deklarasi struct `time_t` dan struct tm yang telah terproduksi ketika kita medefinisikan `time_t` sebelumnya. Setelahnya dengan fungsi localtime untuk mendapatkan waktu sekarang. Untuk sesuai dengan format kita menggunakan bantuan fungsi sprintf untuk concatenasinya._
<br>




**Kendala**
---
Pada awalnya dari kelompok kami kesulitan untuk mengatasi
<br>

**Screenshoot Hasil Run Program Soal3 untuk fitur '*' [soal3](./soal3/soal3.c)**
![Foto](./img/soal3/bintang.png)
---