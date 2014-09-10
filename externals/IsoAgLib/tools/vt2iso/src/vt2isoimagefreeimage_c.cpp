/*
  vt2isoimagefreeimage_c.cpp: image handling for vt2is with
    paintlib library

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#if defined( WIN32 ) && defined( __GNUC__ )
 // MinGW as gcc compiler on Win32 platform
 // include header to get DWORD and corresponding types
 #include <windef.h>
 #include <wingdi.h>
#endif

#include "vt2isoimagefreeimage_c.h"
#include <iostream>
#include <iomanip>

#include "vt2iso-defines.hpp"
#include "vt2iso-globals.hpp"


typedef unsigned char fiuint8_t;
typedef unsigned int fiuint16_t;

extern BGR_s vtColourTable[256];

bool Vt2IsoImageFreeImage_c::b_FreeImageLibInited = false;

/** constructor which can directly open the corresponding file */
Vt2IsoImageFreeImage_c::Vt2IsoImageFreeImage_c( const char* filename )
 : Vt2IsoImageBase_c( ), i_curScanLineY( -1 )
{
 if ( ! b_FreeImageLibInited )
 {
  b_FreeImageLibInited = true;
  FreeImage_Initialise();
 }
 if ( filename != NULL )
 {
  if ( openBitmap( filename ) ) getOptimalBwThreshold();
 }
}


void Vt2IsoImageFreeImage_c::printLicenseText()
{
  if (isOstream()) getOstream() << "This software uses the FreeImage open source image library. See http://freeimage.sourceforge.net for details." << std::endl
                                << "FreeImage is used under the (GNU GPL or FIPL), version (license version). " << std::endl;
}

/** open the given bitmap file and guarantee
 that the pixels can be accessed by RGB
 ( i.e. convert if indexed )
 @return true -> opened and converted correct
*/
bool Vt2IsoImageFreeImage_c::openBitmap( const char* filename )
{
 reset();
 i_curScanLineY = -1;

 if (isOstream())
   getOstream() << std::endl << "Opening " << filename << std::endl;
 FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

 // check the file signature and deduce its format
 // (the second argument is currently not used by FreeImage)
 fif = FreeImage_GetFileType(filename, 0);
 if(fif == FIF_UNKNOWN) {
  // no signature ?
  // try to guess the file format from the file extension
  fif = FreeImage_GetFIFFromFilename(filename);
  if (isOstream()) getOstream() << "File type not recognized" << std::endl;
 }
 // check that the plugin has reading capabilities ...
 if((fif != FIF_UNKNOWN) && FreeImage_FIFSupportsReading(fif)) {
  // ok, let's load the file
  bitmap = FreeImage_Load(fif, filename, 0);
 }
 else
 { // hardcore - try with BMP
  if (isOstream()) getOstream() << "Try with BMP" << std::endl;
  bitmap = FreeImage_Load(FIF_BMP, filename, 0);
  if ( bitmap == NULL )
  { // next hardcoretry with png
    if (isOstream()) getOstream() << "Try with PNG" << std::endl;
    bitmap = FreeImage_Load(FIF_PNG, filename, 0);
  }
  if ( bitmap == NULL )
  { // next hardcoretry with jpg
    if (isOstream()) getOstream() << "Try with JPEG" << std::endl;
    bitmap = FreeImage_Load(FIF_JPEG, filename, 0);
  }
 }
 if ( bitmap == NULL )
 {
    if (isOstream()) getOstream() << "Error loading bitmap file" << std::endl;
    return false;
 }
 else if (isOstream()) getOstream() << "Bitmap successfully loaded" << std::endl;

 if (FreeImage_GetBPP(bitmap) > 8)
 { // convert to 32-Bit standard bitmap ONLY if NOT palettized!
   FIBITMAP *tmp = FreeImage_ConvertTo32Bits(bitmap);
   FreeImage_Unload(bitmap);
   bitmap = tmp;
 }

 ui_width  = FreeImage_GetWidth(bitmap);
 ui_height = FreeImage_GetHeight(bitmap);
 // Calculate the number of bytes per pixel (3 for 24-bit or 4 for 32-bit)
 bytespp = FreeImage_GetLine(bitmap) / FreeImage_GetWidth(bitmap);

 mb_palettized = false;

 if (FreeImage_GetBPP(bitmap) <= 8)
 {
   mi_colourMismatch = -1; // default: no colour mismatches...

   for (int i=0; i<(16+216); i++)
   { // check all colours if the bitmap uses the ISO-palette
     if ( (vtColourTable[i].bgrRed != FreeImage_GetPalette (bitmap)[i].rgbRed)
       || (vtColourTable[i].bgrGreen != FreeImage_GetPalette (bitmap)[i].rgbGreen)
       || (vtColourTable[i].bgrBlue != FreeImage_GetPalette (bitmap)[i].rgbBlue) )
     {
       mi_colourMismatch = i;
       break;
     }
   }
   mb_palettized = true;
   if (isOstream()) getOstream() << " palettized (depth="<<FreeImage_GetBPP(bitmap)<<"). ";
   return true;
 }
 else
 {
   if (isOstream()) getOstream() << " as RGB (depth="<<FreeImage_GetBPP(bitmap)<<"). ";
   return true;
 }
}

// this function is only called for the 8bit case!
int Vt2IsoImageFreeImage_c::getPaletteIndex (unsigned int aui_x, unsigned int aui_y)
{
  if (mb_palettized)
  {
    // do this check here, because in case we only use 4bit bitmap, we don't have to care for the palette matching...
    if (mi_colourMismatch >= 0)
    {
      if (isOstream()) getOstream() << "*** COULDN'T LOAD BITMAP: WRONG PALETTE. See (first) mismatching colour #"<<mi_colourMismatch<<" below. Please use the ISO11783-Part 6 (VT)-Palette for bitmaps you have saved palettized and use in 8bit-mode. Use 'vt2iso -p' to generate an .act file and resample your bitmap to use this palette! ***" << std::endl;
      if (isOstream()) getOstream() << "HAS TO BE | you had" << std::hex << std::setfill('0');
      for (int i=0; i<(16+216); i++)
      {
        if ((i % 8) == 0) { if (isOstream()) getOstream() << std::endl; }
        else { if (isOstream()) getOstream() << "     "; }
        if (isOstream()) getOstream() << std::setw(2) << fiuint16_t(vtColourTable[i].bgrRed) << std::setw(2) << fiuint16_t(vtColourTable[i].bgrGreen) << std::setw(2) << fiuint16_t(vtColourTable[i].bgrBlue);
        if (i == mi_colourMismatch) { if (isOstream()) getOstream() << "*|*"; } else { if (isOstream()) getOstream() << " | "; }
        if (isOstream()) getOstream() << std::setw(2) << fiuint16_t(FreeImage_GetPalette (bitmap)[i].rgbRed) << std::setw(2) << fiuint16_t(FreeImage_GetPalette (bitmap)[i].rgbGreen) << std::setw(2) << fiuint16_t(FreeImage_GetPalette (bitmap)[i].rgbBlue);
      }
      if (isOstream()) getOstream() << std::endl;
      b_isInvalidPalette = true;
      return -1;
    }

    fiuint8_t idx;
    FreeImage_GetPixelIndex (bitmap, aui_x, (ui_height - 1) - aui_y, &idx);
    return idx;
  }
  else return -1;
}


void Vt2IsoImageFreeImage_c::close( void )
{ // unload bitmap from memory
 FreeImage_Unload(bitmap);
 reset();
}


/** check and adapt scanline */
void Vt2IsoImageFreeImage_c::checkUpdateScanline( unsigned int aui_y )
{
 if ( i_curScanLineY != int(aui_y) )
 { // read scanline for given y
  // ( FreeImage library documentation states, that first scanline in memory is
  //   bottommost -> i.e. upsidedown in relation to other modellings
  //    -> change direction back to usual with ( ui_height - aui_y ) )
  scanline = FreeImage_GetScanLine(bitmap, ( (ui_height - 1) - aui_y ) );
  i_curScanLineY = aui_y;
 }
}

/** deliver R-value of bitmap at given position */
unsigned int Vt2IsoImageFreeImage_c::getR( unsigned int aui_x, unsigned int aui_y )
{
 if ( ( aui_x >= ui_width ) || ( aui_y >= ui_height ) ) return 0;

 if (mb_palettized)
 { // we can't raw-access the bitmap buffer with RGB, we need to get the RGB via the palette-index's colour
   fiuint8_t idx;
   FreeImage_GetPixelIndex (bitmap, aui_x, (ui_height - 1) - aui_y, &idx);
   return vtColourTable[idx].bgrRed;
 }
 else
 {
  #if defined( WIN32 )
  RGBQUAD temp_pixel;
  // ( FreeImage library documentation states, that first scanline in memory is
  //   bottommost -> i.e. upsidedown in relation to other modellings
  //    -> change direction back to usual with ( ui_height - aui_y ) )
  FreeImage_GetPixelColor(bitmap, aui_x, ( (ui_height - 1) - aui_y ), &temp_pixel);
  return temp_pixel.rgbRed;
  #else
  checkUpdateScanline( aui_y );
  return scanline[FI_RGBA_RED + ( bytespp * aui_x )];
  #endif
 }
}

/** deliver G-value of bitmap at given position */
unsigned int Vt2IsoImageFreeImage_c::getG( unsigned int aui_x, unsigned int aui_y )
{
 if (mb_palettized)
 { // we can't raw-access the bitmap buffer with RGB, we need to get the RGB via the palette-index's colour
   fiuint8_t idx;
   FreeImage_GetPixelIndex (bitmap, aui_x, (ui_height - 1) - aui_y, &idx);
   return vtColourTable[idx].bgrGreen;
 }
 else
 {
  if ( ( aui_x >= ui_width ) || ( aui_y >= ui_height ) ) return 0;
  #if defined( WIN32 )
  RGBQUAD temp_pixel;
  // ( FreeImage library documentation states, that first scanline in memory is
  //   bottommost -> i.e. upsidedown in relation to other modellings
  //    -> change direction back to usual with ( ui_height - aui_y ) )
  FreeImage_GetPixelColor(bitmap, aui_x, ( (ui_height - 1) - aui_y ), &temp_pixel);
  return temp_pixel.rgbGreen;
  #else
  checkUpdateScanline( aui_y );
  return scanline[FI_RGBA_GREEN + ( bytespp * aui_x )];
  #endif
 }
}

/** deliver B-value of bitmap at given position */
unsigned int Vt2IsoImageFreeImage_c::getB( unsigned int aui_x, unsigned int aui_y )
{
 if (mb_palettized)
 { // we can't raw-access the bitmap buffer with RGB, we need to get the RGB via the palette-index's colour
   fiuint8_t idx;
   FreeImage_GetPixelIndex (bitmap, aui_x, (ui_height - 1) - aui_y, &idx);
   return vtColourTable[idx].bgrBlue;
 }
 else
 {
  if ( ( aui_x >= ui_width ) || ( aui_y >= ui_height ) ) return 0;
  #if defined( WIN32 )
  RGBQUAD temp_pixel;
  // ( FreeImage library documentation states, that first scanline in memory is
  //   bottommost -> i.e. upsidedown in relation to other modellings
  //    -> change direction back to usual with ( ui_height - aui_y ) )
  FreeImage_GetPixelColor(bitmap, aui_x, ( (ui_height - 1) - aui_y ), &temp_pixel);
  return temp_pixel.rgbBlue;
  #else
  checkUpdateScanline( aui_y );
  return scanline[FI_RGBA_BLUE + ( bytespp * aui_x )];
  #endif
 }
}
