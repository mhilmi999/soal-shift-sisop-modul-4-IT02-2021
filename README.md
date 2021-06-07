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
5. Tidak diperkenankan menggunakan `system()` dan `exec()`, terkecuali terdapat pengecualian di butri soal.
6. Pengerjaan hanya dilakukan dalam 1 `file program C` dengan format nama `SinseiFS_[Kelompok].c`.


<br>

**Cara pengerjaan**
---

<br>

## Soal 1.a.
## **Analisa Soal**
Pada soal 1.a sebagai permulaan dari soal ini kami diminta untuk membuat direktori yang diawali dengan `"AtoZ_"`, yang mana direktori ini akan menjadi direktori ter-*encode*.

**Cara Pengerjaan**
---

<br>

## Soal 1.b.
## **Analisa Soal**
Pada soal 1.b sebagai lanjutan dari soal yang sebelumnya, yang mana pada dasarnya jika terdapat sebuah direktori di-*rename* dengan awalan `"AtoZ_"`, maka direktori akan berubah menjadi direktori yang ter-*encode*/ 

**Cara Pengerjaan**
---

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

Dalam menyelesaikan program yang diminta oleh 
<br>


## Soal 4.a.
## **Analisa Soal**
Pada soal 4.a merupakan spesifikasi pertama yang dibutuhkan dalam `file monitor` yang akan dibuat, `Log System` akan terbentuk dengan nama file `"SinSeiFS.log"` pada direktori `home` pengguna `(/home/[USER]/SinSeiFS.log)`. Log system ini akan menyimpan daftar perintah `system call` yang telah dijalankan pada `filesystem`.

**Cara Pengerjaan**
---
Dalam menjawab soal 3.a. kami membuat
<br>

## Soal 4.b.
## **Analisa Soal**
Pada soal 4.b kami dituntut untuk merapihkan `log` yang telah dibuat akan terbagi menjadi dua level secara garis besar, yaitu `INFO` dan `WARNING`.

**Cara Pengerjaan**
---

<br>

## Soal 4.c.

## **Analisa Soal**
Pada soal 4.c diminta khusus dalam `log level WARNING` mencatat `syscall, mkdir` dan `unlink`.

**Cara Pengerjaan**
---

<br>

## Soal 4.d.
## **Analisa Soal**
Pada soal 4.d akan diminta melakukan pencatat yang `non-WARNING` atau sisanya akan tercatat pada `level INFO`.

**Cara Pengerjaan**
---

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

<br>




**Kendala**
---
Pada awalnya dari kelompok kami kesulitan untuk mengatasi
<br>

**Screenshoot Hasil Run Program Soal3 untuk fitur '*' [soal3](./soal3/soal3.c)**
![Foto](./img/soal3/bintang.png)
---