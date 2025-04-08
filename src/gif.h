#ifndef GIF_H
#define GIF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>

// Struktur untuk menulis GIF
typedef struct GifWriter {
    FILE* f;             // File output
    uint8_t* oldImage;   // Buffer untuk frame sebelumnya (bisa NULL jika tidak dipakai)
    int width;           // Lebar frame
    int height;          // Tinggi frame
    int delay;           // Delay per frame (dalam centisecond)
} GifWriter;

// Fungsi untuk memulai GIF, menulis header dan frame pertama
// filename: nama file output, width/height: dimensi frame, delay: delay per frame
int GifBegin(GifWriter* writer, const char* filename, int width, int height, int delay);

// Fungsi untuk menambahkan frame ke GIF
int GifWriteFrame(GifWriter* writer, const uint8_t* image, int width, int height, int delay);

// Fungsi untuk menyelesaikan file GIF
int GifEnd(GifWriter* writer);

#ifdef __cplusplus
}
#endif

#endif
