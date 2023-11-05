#include "rgbimage.h"
#include "color.h"
#include "assert.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>

RGBImage::RGBImage(unsigned int Width, unsigned int Height) {
    m_Height = Height;
    m_Width = Width;
    m_Image = new Color[m_Width * m_Height];
}

RGBImage::~RGBImage() {
    delete this->m_Image;
}

void RGBImage::setPixelColor(unsigned int x, unsigned int y, const Color &c) {
    m_Image[m_Width * y + x] = c;

}

const Color &RGBImage::getPixelColor(unsigned int x, unsigned int y) const {
    return m_Image[m_Width * y + x];
}

unsigned int RGBImage::width() const {
    return m_Width;
}

unsigned int RGBImage::height() const {
    return m_Height;
}

unsigned char RGBImage::convertColorChannel(float v) {
    float f2 = std::fmax(0.0, std::fmin(1.0, v));
    return floor(f2 == 1.0 ? 255 : f2 * 256.0);
    //return v*255;

}

//function to generate a 24 bit bmp file given the Filename and an Array with the pixel values
//bool RGBImage::saveToDisk( const char* Filename)
//{
//    std::ofstream f;
//    f.open(Filename, std::ios::out | std::ios::binary);
//    if (!f.is_open()){
//        std::cout << "file cant be opened";
//        return false;
//    }
//    unsigned char bmpPad[3] = {0,0,0};
//    const int paddingAmount = ((4-(m_Width*3)%4)%4);
//
//    const int fileHeaderSize=14;
//    const int informationHeaderSize=40;
//    const int fileSize = fileHeaderSize + informationHeaderSize +m_Width*m_Height * 3 +paddingAmount*m_Height;
//
//    unsigned char fileHeader[fileHeaderSize];
//    unsigned char informationHeader[informationHeaderSize];
//
//    fileHeader[0] = 'B';
//    fileHeader[1] = 'M';
//    fileHeader[2] = fileSize;
//    fileHeader[3] = fileSize >> 8;
//    fileHeader[4] = fileSize >> 16;
//    fileHeader[5] = fileSize >> 24;
//    fileHeader[10] = fileHeaderSize + informationHeaderSize;
//
//    informationHeader[0] = informationHeaderSize;
//    informationHeader[4] = m_Width;
//    informationHeader[5] = m_Width >> 8;
//    informationHeader[6] = m_Width >> 16;
//    informationHeader[7] = m_Width >> 24;
//    informationHeader[8] = m_Height;
//    informationHeader[9] = m_Height >> 8;
//    informationHeader[10] = m_Height >> 16;
//    informationHeader[11] = m_Height >> 24;
//    informationHeader[12] = 1;
//    informationHeader[14] = 3;
//
//    f.write((char*)fileHeader, fileHeaderSize);
//    f.write((char*)informationHeader, informationHeaderSize);
//
//    for(int y = 0; y < m_Height; y++)
//    {
//        for(int x = 0; x < m_Width; x++)
//        {
//            Color col = getPixelColor(x,y);
//
//
//            unsigned char r = convertColorChannel(col.R);
//            unsigned char g = convertColorChannel(col.G);
//            unsigned char b = convertColorChannel(col.B);
//            f.write((char*)&b, sizeof(b));
//            f.write((char*)&g, sizeof(g));
//            f.write((char*)&r, sizeof(r));
////unsigned char color_converted[] = {b,g,r};
////            f.write(reinterpret_cast<char*>(color_converted),3);
//        }
//        f.write((char*)bmpPad, paddingAmount);
//    }
//    f.close();
//    return true;
//}
bool RGBImage::saveToDisk(const char *Filename) {
    // Öffnen der Datei zum Schreiben im binären Modus
    FILE *file = fopen(Filename, "wb");
    if (file == nullptr) {
        std::cerr << "Fehler beim Öffnen der Datei zum Schreiben." << std::endl;
        return false;
    }

    // BMP-Dateiheader schreiben
    unsigned int fileSize = 54 + 3 * m_Width * m_Height;
    unsigned char fileHeader[14] = {
            'B', 'M',             // Dateityp (Bitmap)
            fileSize, fileSize >> 8, fileSize >> 16, fileSize >> 24, // Dateigröße
            0, 0, 0, 0,           // Reserviert
            54, 0, 0, 0           // Offset zum Bild-Datenbereich
    };

    unsigned char infoHeader[40] = {
            40, 0, 0, 0,          // Info-Headergröße
            m_Width, m_Width >> 8, m_Width >> 16, m_Width >> 24, // Bildbreite
            -m_Height, -m_Height >> 8, -m_Height >> 16, -m_Height >> 24, // Bildhöhe
            1, 0,                // Farbebenen (immer 1)
            24, 0,               // Bittiefe (24-Bit RGB)
            0, 0, 0, 0,          // Keine Kompression
            0, 0, 0, 0,          // Bildgröße (wird später berechnet)
            0, 0, 0, 0,          // X-Auflösung
            0, 0, 0, 0           // Y-Auflösung
    };

    // Bildgröße im Header aktualisieren
    infoHeader[20] = 3 * m_Width;
    infoHeader[21] = 3 * m_Width >> 8;
    infoHeader[22] = 3 * m_Width >> 16;
    infoHeader[23] = 3 * m_Width >> 24;

    // Dateiheader und Info-Header in die Datei schreiben
    fwrite(fileHeader, 1, 14, file);
    fwrite(infoHeader, 1, 40, file);

    // Bildpixel in die Datei schreiben
    for (unsigned int y = 0; y < m_Height; y++) {
        for (unsigned int x = 0; x < m_Width; x++) {
            const Color &pixelColor = getPixelColor(x, y);
            unsigned char r = convertColorChannel(pixelColor.R);
            unsigned char g = convertColorChannel(pixelColor.G);
            unsigned char b = convertColorChannel(pixelColor.B);
            fputc(b, file);
            fputc(g, file);
            fputc(r, file);
        }
    }

    // Datei schließen
    fclose(file);
    return true;
}


