#include "rgbimage.h"
#include "color.h"
#include "assert.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>

RGBImage::RGBImage( unsigned int Width, unsigned int Height)
{
	m_Height = Height;
    m_Width = Width;
    m_Image = new Color[m_Width*m_Height];
}

RGBImage::~RGBImage()
{
	delete this->m_Image;
}

void RGBImage::setPixelColor( unsigned int x, unsigned int y, const Color& c)
{
	m_Image[m_Width*y+x]= getPixelColor(x,y);

}

const Color& RGBImage::getPixelColor( unsigned int x, unsigned int y) const
{
	return m_Image[m_Width*y+x];
}

unsigned int RGBImage::width() const
{
	return m_Width;
}
unsigned int RGBImage::height() const
{
	return m_Height;
}

unsigned char RGBImage::convertColorChannel( float v)
{
    float f2 = std::fmax(0.0, std::fmin(1.0, v));
   return floor(f2 == 1.0 ? 255 : f2 * 256.0);

}


bool RGBImage::saveToDisk( const char* Filename)
{
    std::ofstream f;
    f.open(Filename, std::ios::out | std::ios::binary);
    if (!f.is_open()){
        std::cout << "file cant be opened";
        return false;
    }
    return true;
}
