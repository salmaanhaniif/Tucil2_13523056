
# QuadTree Image Compression

## Deskripsi Singkat
Program ini merupakan implementasi algoritma *Divide and Conquer* untuk melakukan kompresi gambar menggunakan metode *QuadTree*. Program bekerja dengan membagi gambar menjadi blok-blok berdasarkan keseragaman warna, sehingga dapat mengurangi ukuran file gambar.

Program mendukung 4 metode Error Measurement:

| Metode                          | Kelebihan                                  | Kekurangan                          |
|---------------------------------|--------------------------------------------|------------------------------------|
| Variance                       | Akurat mendeteksi warna                    | Kurang baik untuk gradasi warna    |
| Mean Absolute Deviation (MAD)  | Mirip Variance                             | Sama kekurangannya dengan Variance |
| Max Pixel Difference (MPD)     | Baik dalam mendeteksi gradasi warna        | Akurasi warna kurang               |
| Entropy                        | Terbaik untuk detail, gradasi, dan akurasi | -                                  |

Metode QuadTree ini sangat efektif untuk gambar dengan banyak area homogen (contoh: gambar PNG berukuran besar). Namun kurang optimal untuk:
- Gambar detail tinggi
- Gambar noise merata
- Gambar ukuran kecil  
Karena hasil kompresi bisa merusak detail gambar.

> *Catatan:*  
Ukuran file PNG cenderung besar karena PNG menggunakan *lossless compression* (tanpa kehilangan kualitas) yang menyimpan semua data pixel secara utuh.

---

## Requirement Program
- OS: Windows / Linux
- Compiler: g++ (C++)
- Terminal / Command Prompt
- Library standar C++ (tanpa library tambahan)

---

## Struktur Folder
```
project/
│
├── src/       → Source code (.cpp dan .h)
├── bin/       → Output file hasil kompresi (.png)
├── test/      → File hasil kompilasi (.exe)
└── run.bat    → File otomatis compile & run
```

---

## Cara Kompilasi Program

### Kompilasi Manual
Masuk ke folder `src/`:
```
cd src
g++ main.cpp block.cpp pixel.cpp point.cpp quadtree.cpp -o ../bin/main
```

---

## Cara Menjalankan Program

### Menggunakan run.bat (Windows)
```
Klik 2x file run.bat
```

### Manual (Linux/Windows)
```
cd src
g++ main.cpp block.cpp pixel.cpp point.cpp quadtree.cpp -o ../bin/main
cd ../bin
./main
```

Program akan meminta input:
1. Filepath gambar input
2. Metode Error Measurement
3. Threshold
4. Minimum Block Size
5. Filepath untuk output gambar (akan validasi overwrite jika sudah ada)

Setelah input selesai, layar akan dibersihkan (*clear screen*) dan seluruh input akan ditampilkan kembali untuk konfirmasi.


## Cara Menjalankan Program

1. Pastikan sudah dikompilasi.
2. Jalankan file output:
```
cd bin
./main
```
Atau jika menggunakan `run.bat` akan otomatis menjalankan program.

3. Input Data Sesuai Instruksi Program:
- Masukkan path gambar input
- Pilih metode error measurement (1-4)
- Masukkan nilai threshold (akan divalidasi sesuai metode)
- Masukkan minimal area block
- Masukkan path output hasil rekonstruksi (akan divalidasi jika file sudah ada)

4. Hasil gambar kompresi akan tersimpan di lokasi output yang sudah ditentukan.

---

## Catatan
- Semakin besar threshold → Kompresi semakin kuat, detail makin hilang.
- Semakin kecil minimum block → Ukuran file hasil makin besar.
- Folder output dan input harus sudah tersedia atau dibuat manual.
- Jika file output sudah ada, program akan meminta konfirmasi overwrite
