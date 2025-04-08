// #define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "gif.h"
#include <stdlib.h>
#include <string.h>

int GifBegin(GifWriter* writer, const char* filename, int width, int height, int delay) {
    writer->f = fopen(filename, "wb");
    if (!writer->f) {
        return 0; // gagal buka file
    }
    writer->width = width;
    writer->height = height;
    writer->delay = delay;
    writer->oldImage = (uint8_t*)malloc(width * height * 4); // RGBA
    if (!writer->oldImage) {
        fclose(writer->f);
        return 0;
    }
    return 1; // berhasil
}

int GifWriteFrame(GifWriter* writer, const uint8_t* image, int width, int height, int delay) {
    if (!writer->f) {
        return 0;
    }
    // Tulis frame sebagai PNG (sementara karena implementasi GIF proper butuh lib tambahan)
    char buffer[256];
    static int frame = 0;
    sprintf(buffer, "frame_%03d.png", frame++);
    stbi_write_png(buffer, width, height, 4, image, width * 4);
    memcpy(writer->oldImage, image, width * height * 4);
    return 1;
}

int GifEnd(GifWriter* writer) {
    if (writer->oldImage) {
        free(writer->oldImage);
    }
    if (writer->f) {
        fclose(writer->f);
    }
    return 1;
}
