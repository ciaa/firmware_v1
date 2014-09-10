/*
  vt2isoimagebase_c.cpp: image handling for vt2iso - base class

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#include "vt2isoimagebase_c.h"
#include "vt2iso-defines.hpp"
#include <iostream>


BGR_s vtColourTable[256]=
{ /// ATTENTION: This is stored "Blue-Green-Red" !!!
  {0x00,0x00,0x00},//0
  {0xFF,0xFF,0xff},//1
  {0x00,0x99,0x00},//2
  {0x99,0x99,0x00},//3
  {0x00,0x00,0x99},//4
  {0x99,0x00,0x99},//5
  {0x00,0x99,0x99},//6
  {0xCC,0xCC,0xCC},//7
  {0x99,0x99,0x99},//8
  {0xFF,0x00,0x00},//9
  {0x00,0xFF,0x00},//10
  {0xFF,0xFF,0x00},//11
  {0x00,0x00,0xFF},//12
  {0xFF,0x00,0xFF},//13
  {0x00,0xFF,0xFF},//14
  {0x99,0x00,0x00},//15
  {0x00,0x00,0x00},//16
  {0x33,0x00,0x00},//17
  {0x66,0x00,0x00},//18
  {0x99,0x00,0x00},//19
  {0xCC,0x00,0x00},//20
  {0xFF,0x00,0x00},//21
  {0x00,0x33,0x00},//22
  {0x33,0x33,0x00},//23
  {0x66,0x33,0x00},//24
  {0x99,0x33,0x00},//25
  {0xCC,0x33,0x00},//26
  {0xFF,0x33,0x00},//27
  {0x00,0x66,0x00},//28
  {0x33,0x66,0x00},//29
  {0x66,0x66,0x00},//30
  {0x99,0x66,0x00},//31
  {0xCC,0x66,0x00},//32
  {0xFF,0x66,0x00},//33
  {0x00,0x99,0x00},//34
  {0x33,0x99,0x00},//35
  {0x66,0x99,0x00},//36
  {0x99,0x99,0x00},//37
  {0xCC,0x99,0x00},//38
  {0xFF,0x99,0x00},//39
  {0x00,0xCC,0x00},//40
  {0x33,0xCC,0x00},//41
  {0x66,0xCC,0x00},//42
  {0x99,0xCC,0x00},//43
  {0xCC,0xCC,0x00},//44
  {0xFF,0xCC,0x00},//45
  {0x00,0xFF,0x00},//46
  {0x33,0xFF,0x00},//47
  {0x66,0xFF,0x00},//48
  {0x99,0xFF,0x00},//49
  {0xCC,0xFF,0x00},//50
  {0xFF,0xFF,0x00},//51
  {0x00,0x00,0x33},//52
  {0x33,0x00,0x33},//53
  {0x66,0x00,0x33},//54
  {0x99,0x00,0x33},//55
  {0xCC,0x00,0x33},//56
  {0xFF,0x00,0x33},//57
  {0x00,0x33,0x33},//58
  {0x33,0x33,0x33},//59
  {0x66,0x33,0x33},//60
  {0x99,0x33,0x33},//61
  {0xCC,0x33,0x33},//62
  {0xFF,0x33,0x33},//63
  {0x00,0x66,0x33},//64
  {0x33,0x66,0x33},//65
  {0x66,0x66,0x33},//66
  {0x99,0x66,0x33},//67
  {0xCC,0x66,0x33},//68
  {0xFF,0x66,0x33},//69
  {0x00,0x99,0x33},//70
  {0x33,0x99,0x33},//71
  {0x66,0x99,0x33},//72
  {0x99,0x99,0x33},//73
  {0xCC,0x99,0x33},//74
  {0xFF,0x99,0x33},//75
  {0x00,0xCC,0x33},//76
  {0x33,0xCC,0x33},//77
  {0x66,0xCC,0x33},//78
  {0x99,0xCC,0x33},//79
  {0xCC,0xCC,0x33},//80
  {0xFF,0xCC,0x33},//81
  {0x00,0xFF,0x33},//82
  {0x33,0xFF,0x33},//83
  {0x66,0xFF,0x33},//84
  {0x99,0xFF,0x33},//85
  {0xCC,0xFF,0x33},//86
  {0xFF,0xFF,0x33},//87
  {0x00,0x00,0x66},//88
  {0x33,0x00,0x66},//89
  {0x66,0x00,0x66},//90
  {0x99,0x00,0x66},//91
  {0xCC,0x00,0x66},//92
  {0xFF,0x00,0x66},//93
  {0x00,0x33,0x66},//94
  {0x33,0x33,0x66},//95
  {0x66,0x33,0x66},//96
  {0x99,0x33,0x66},//97
  {0xCC,0x33,0x66},//98
  {0xFF,0x33,0x66},//99
  {0x00,0x66,0x66},//100
  {0x33,0x66,0x66},//101
  {0x66,0x66,0x66},//102
  {0x99,0x66,0x66},//103
  {0xCC,0x66,0x66},//104
  {0xFF,0x66,0x66},//105
  {0x00,0x99,0x66},//106
  {0x33,0x99,0x66},//107
  {0x66,0x99,0x66},//108
  {0x99,0x99,0x66},//109
  {0xCC,0x99,0x66},//110
  {0xFF,0x99,0x66},//111
  {0x00,0xCC,0x66},//112
  {0x33,0xCC,0x66},//113
  {0x66,0xCC,0x66},//114
  {0x99,0xCC,0x66},//115
  {0xCC,0xCC,0x66},//116
  {0xFF,0xCC,0x66},//117
  {0x00,0xFF,0x66},//118
  {0x33,0xFF,0x66},//119
  {0x66,0xFF,0x66},//120
  {0x99,0xFF,0x66},//121
  {0xCC,0xFF,0x66},//122
  {0xFF,0xFF,0x66},//123
  {0x00,0x00,0x99},//124
  {0x33,0x00,0x99},//125
  {0x66,0x00,0x99},//126
  {0x99,0x00,0x99},//127
  {0xCC,0x00,0x99},//128
  {0xFF,0x00,0x99},//129
  {0x00,0x33,0x99},//130
  {0x33,0x33,0x99},//131
  {0x66,0x33,0x99},//132
  {0x99,0x33,0x99},//133
  {0xCC,0x33,0x99},//134
  {0xFF,0x33,0x99},//135
  {0x00,0x66,0x99},//136
  {0x33,0x66,0x99},//137
  {0x66,0x66,0x99},//138
  {0x99,0x66,0x99},//139
  {0xCC,0x66,0x99},//140
  {0xFF,0x66,0x99},//141
  {0x00,0x99,0x99},//142
  {0x33,0x99,0x99},//143
  {0x66,0x99,0x99},//144
  {0x99,0x99,0x99},//145
  {0xCC,0x99,0x99},//146
  {0xFF,0x99,0x99},//147
  {0x00,0xCC,0x99},//148
  {0x33,0xCC,0x99},//149
  {0x66,0xCC,0x99},//150
  {0x99,0xCC,0x99},//151
  {0xCC,0xCC,0x99},//152
  {0xFF,0xCC,0x99},//153
  {0x00,0xFF,0x99},//154
  {0x33,0xFF,0x99},//155
  {0x66,0xFF,0x99},//156
  {0x99,0xFF,0x99},//157
  {0xCC,0xFF,0x99},//158
  {0xFF,0xFF,0x99},//159
  {0x00,0x00,0xCC},//160
  {0x33,0x00,0xCC},//161
  {0x66,0x00,0xCC},//162
  {0x99,0x00,0xCC},//163
  {0xCC,0x00,0xCC},//164
  {0xFF,0x00,0xCC},//165
  {0x00,0x33,0xCC},//166
  {0x33,0x33,0xCC},//167
  {0x66,0x33,0xCC},//168
  {0x99,0x33,0xCC},//169
  {0xCC,0x33,0xCC},//170
  {0xFF,0x33,0xCC},//171
  {0x00,0x66,0xCC},//172
  {0x33,0x66,0xCC},//173
  {0x66,0x66,0xCC},//174
  {0x99,0x66,0xCC},//175
  {0xCC,0x66,0xCC},//176
  {0xFF,0x66,0xCC},//177
  {0x00,0x99,0xCC},//178
  {0x33,0x99,0xCC},//179
  {0x66,0x99,0xCC},//180
  {0x99,0x99,0xCC},//181
  {0xCC,0x99,0xCC},//182
  {0xFF,0x99,0xCC},//183
  {0x00,0xCC,0xCC},//184
  {0x33,0xCC,0xCC},//185
  {0x66,0xCC,0xCC},//186
  {0x99,0xCC,0xCC},//187
  {0xCC,0xCC,0xCC},//188
  {0xFF,0xCC,0xCC},//189
  {0x00,0xFF,0xCC},//190
  {0x33,0xFF,0xCC},//191
  {0x66,0xFF,0xCC},//192
  {0x99,0xFF,0xCC},//193
  {0xCC,0xFF,0xCC},//194
  {0xFF,0xFF,0xCC},//195
  {0x00,0x00,0xFF},//196
  {0x33,0x00,0xFF},//197
  {0x66,0x00,0xFF},//198
  {0x99,0x00,0xFF},//199
  {0xCC,0x00,0xFF},//200
  {0xFF,0x00,0xFF},//201
  {0x00,0x33,0xFF},//202
  {0x33,0x33,0xFF},//203
  {0x66,0x33,0xFF},//204
  {0x99,0x33,0xFF},//205
  {0xCC,0x33,0xFF},//206
  {0xFF,0x33,0xFF},//207
  {0x00,0x66,0xFF},//208
  {0x33,0x66,0xFF},//209
  {0x66,0x66,0xFF},//210
  {0x99,0x66,0xFF},//211
  {0xCC,0x66,0xFF},//212
  {0xFF,0x66,0xFF},//213
  {0x00,0x99,0xFF},//214
  {0x33,0x99,0xFF},//215
  {0x66,0x99,0xFF},//216
  {0x99,0x99,0xFF},//217
  {0xCC,0x99,0xFF},//218
  {0xFF,0x99,0xFF},//219
  {0x00,0xCC,0xFF},//220
  {0x33,0xCC,0xFF},//221
  {0x66,0xCC,0xFF},//222
  {0x99,0xCC,0xFF},//223
  {0xCC,0xCC,0xFF},//224
  {0xFF,0xCC,0xFF},//225
  {0x00,0xFF,0xFF},//226
  {0x33,0xFF,0xFF},//227
  {0x66,0xFF,0xFF},//228
  {0x99,0xFF,0xFF},//229
  {0xCC,0xFF,0xFF},//230
  {0xFF,0xFF,0xFF},//231
  //Proprietary
  {0x00,0x00,0x00},//232
  {0x00,0x00,0x00},//233
  {0x00,0x00,0x00},//234
  {0x00,0x00,0x00},//235
  {0x00,0x00,0x00},//236
  {0x00,0x00,0x00},//237
  {0x00,0x00,0x00},//238
  {0x00,0x00,0x00},//239
  {0x00,0x00,0x00},//240
  {0x00,0x00,0x00},//241
  {0x00,0x00,0x00},//242
  {0x00,0x00,0x00},//243
  {0x00,0x00,0x00},//244
  {0x00,0x00,0x00},//245
  {0x00,0x00,0x00},//246
  {0x00,0x00,0x00},//247
  {0x00,0x00,0x00},//248
  {0x00,0x00,0x00},//249
  {0x00,0x00,0x00},//250
  {0x00,0x00,0x00},//251
  {0x00,0x00,0x00},//252
  {0x00,0x00,0x00},//253
  {0x00,0x00,0x00},//254
  {0x00,0x00,0x00} //255
};

/** constructor which can directly open the corresponding file */
Vt2IsoImageBase_c::Vt2IsoImageBase_c( void )
  : b_isInvalidPalette(false)
  , i_currentThreshold( -1 )
  , ui_width( 0 )
  , ui_height( 0 )
  , mp_ostream( NULL )
{
}

void Vt2IsoImageBase_c::close( void )
{
}

void Vt2IsoImageBase_c::reset( void )
{
  i_currentThreshold = -1;
  ui_width = 0;
  ui_height = 0;
}


void Vt2IsoImageBase_c::resetLengths( void )
{
  objRawBitmapBytes[0] = 0;
  objRawBitmapBytes[1] = 0;
  objRawBitmapBytes[2] = 0;
}


/** deliver the b/w thresholded value at given bitmap position
  ( calculate the optimal bitmap threshold if not yet defined )
  */
unsigned int Vt2IsoImageBase_c::get1BitPixel( unsigned int aui_x, unsigned int aui_y )
{
  if ( i_currentThreshold < 0 )  getOptimalBwThreshold();
  return ( int( ( getR( aui_x, aui_y ) + getG( aui_x, aui_y ) + getB( aui_x, aui_y ) ) / 3 ) >= i_currentThreshold )?1U:0U;
}

/** get the ISO virtual table indexed bitmap value for 4Bit ( 16colour ) target bitmap */
unsigned int Vt2IsoImageBase_c::get4BitPixel( unsigned int aui_x, unsigned int aui_y )
{
  return rgbtopalette16 ( getR( aui_x, aui_y ), getG( aui_x, aui_y ), getB( aui_x, aui_y ) );
}

/** get the ISO virtual table indexed bitmap value for 8Bit ( 256colour ) target bitmap */
unsigned int Vt2IsoImageBase_c::get8BitPixel( unsigned int aui_x, unsigned int aui_y )
{
  int idx = getPaletteIndex (aui_x, aui_y);
  if (idx >= 0)
  { // we're palettized!
    // 0..255 possible - directly taken out of the bitmap!
    return idx;
  }
  else 
  { // we're NOT palettized, calculate a palette index!
    const uint8_t ui8_red = getR( aui_x, aui_y );
    const uint8_t ui8_green = getG( aui_x, aui_y );
    const uint8_t ui8_blue = getB( aui_x, aui_y );

    static unsigned int directMatchIdx = 0;
    // try previous value (prevent iteration for speed up)
    if ((vtColourTable[directMatchIdx].bgrRed == ui8_red) && (vtColourTable[directMatchIdx].bgrGreen == ui8_green) && (vtColourTable[directMatchIdx].bgrBlue == ui8_blue)) 
      return directMatchIdx;

    for ( directMatchIdx = 0; directMatchIdx < 256; directMatchIdx++)
    {
      if ((vtColourTable[directMatchIdx].bgrRed == ui8_red) && (vtColourTable[directMatchIdx].bgrGreen == ui8_green) && (vtColourTable[directMatchIdx].bgrBlue == ui8_blue)) 
        return directMatchIdx;
    }

    idx = 16 + ( componenttoindex6 ( getR( aui_x, aui_y ) )*36 )
             + ( componenttoindex6 ( getG( aui_x, aui_y ) )*6  )
             + ( componenttoindex6 ( getB( aui_x, aui_y ) )    );
    // 16..231 possible - mapped to this area!
    switch (idx)
    { // now try to map down those colours that exactly match to the range 0..15!
      // because those colours will be used in the 16-colour version, too. This makes it easier for the transparency colour then!
      case  16: idx = 0; break;
      case 231: idx = 1; break;
      case  34: idx = 2; break;
      case  37: idx = 3; break;
      case 124: idx = 4; break;
      case 127: idx = 5; break;
      case 142: idx = 6; break;
      case 188: idx = 7; break;
      case 145: idx = 8; break;
      case  21: idx = 9; break;
      case  46: idx =10; break;
      case  51: idx =11; break;
      case 196: idx =12; break;
      case 201: idx =13; break;
      case 226: idx =14; break;
      case  19: idx =15; break;
    }
    return idx;
  }
  return 0; // Only for compiler satisfaction.
}

/** write the Bitmap to the given buffer and return amount of written Bytes */
unsigned int Vt2IsoImageBase_c::write1BitBitmap( unsigned char* pui_bitmap, unsigned int aui_maxSize )
{
  objRawBitmapBytes [0] = 0;
  if ( ( i_currentThreshold < 0 ) || ( i_currentThreshold == 128 ) ) getOptimalBwThreshold();

  unsigned int roundedWidth = ( ( getWidth() + 7U ) & ( 0xFFFFFFFFU - 7U ) );

  for (unsigned int ui_y=0; ui_y< getHeight(); ui_y++) {
    for (unsigned int ui_x=0; ui_x < roundedWidth; ui_x+=8) {
      unsigned int pixel1PaletteIndex = get1BitPixel( ui_x, ui_y );
      unsigned int pixel2PaletteIndex = get1BitPixel( ui_x+1, ui_y );
      unsigned int pixel3PaletteIndex = get1BitPixel( ui_x+2, ui_y );
      unsigned int pixel4PaletteIndex = get1BitPixel( ui_x+3, ui_y );
      unsigned int pixel5PaletteIndex = get1BitPixel( ui_x+4, ui_y );;
      unsigned int pixel6PaletteIndex = get1BitPixel( ui_x+5, ui_y );
      unsigned int pixel7PaletteIndex = get1BitPixel( ui_x+6, ui_y );
      unsigned int pixel8PaletteIndex = get1BitPixel( ui_x+7, ui_y );
      unsigned int byte = (pixel1PaletteIndex << 7) + (pixel2PaletteIndex << 6) + (pixel3PaletteIndex << 5) + (pixel4PaletteIndex << 4)
                + (pixel5PaletteIndex << 3) + (pixel6PaletteIndex << 2) + (pixel7PaletteIndex << 1) + pixel8PaletteIndex;
      // avoid overflow
      if ( objRawBitmapBytes [0] > aui_maxSize ) return 0;

      pui_bitmap [objRawBitmapBytes [0]] = byte;
      objRawBitmapBytes [0]++;
    }
  } // iterate loop
  return objRawBitmapBytes [0];
}


/** write the Bitmap to the given buffer and return amount of written Bytes */
unsigned int Vt2IsoImageBase_c::write4BitBitmap( unsigned char* pui_bitmap, unsigned int aui_maxSize )
{
  objRawBitmapBytes [1] = 0;
  unsigned int roundedWidth = ( ( getWidth() + 1U ) & ( 0xFFFFFFFFU - 1U ) );

  for ( unsigned int ui_y=0; ui_y< getHeight(); ui_y++) {
    for (unsigned int ui_x=0; ui_x<roundedWidth; ui_x+=2) {
      unsigned int pixel1PaletteIndex = get4BitPixel( ui_x, ui_y );
      unsigned int pixel2PaletteIndex = get4BitPixel( ui_x+1, ui_y );
      unsigned int byte = (pixel1PaletteIndex << 4) + pixel2PaletteIndex;
      // avoid overflow
      if ( objRawBitmapBytes [1] > aui_maxSize ) return 0;

      pui_bitmap [objRawBitmapBytes [1]] = byte;
      objRawBitmapBytes [1]++;
    }
  } // iterate loop
  return objRawBitmapBytes [1];
}

/** write the Bitmap to the given buffer and return amount of written Bytes */
int Vt2IsoImageBase_c::write8BitBitmap( unsigned char* pui_bitmap, unsigned int aui_maxSize )
{
  b_isInvalidPalette = false;
  objRawBitmapBytes [2] = 0;
  for ( unsigned int ui_y=0; ui_y< getHeight(); ui_y++) {
    for ( unsigned int ui_x=0; ui_x < getWidth(); ui_x++) {
      unsigned int pixelPaletteIndex = get8BitPixel( ui_x, ui_y );
      // avoid overflow
      if ( objRawBitmapBytes [2] > aui_maxSize ) return 0;

      pui_bitmap [objRawBitmapBytes [2]] = pixelPaletteIndex;
      objRawBitmapBytes [2]++;
    }
  } // iterate loop
  if( b_isInvalidPalette )
	return -1;

  return objRawBitmapBytes [2];
}

/** deliver 4Bit palette value */
unsigned int Vt2IsoImageBase_c::indexfromtable (unsigned int red, unsigned int green, unsigned int blue)
{
  for (int i=0; i<16; i++) {
    if ((colour16table [i] [0] == red) && (colour16table [i] [1] == green) && (colour16table [i] [2] == blue))
      return i;
  }
  return 0; // shouldn't happen!!
}


/** dliver 16Bit Palette value for given R/G/B values */
unsigned int Vt2IsoImageBase_c::rgbtopalette16 (unsigned int red, unsigned int green, unsigned int blue)
{
  red = componentto09CF (red);
  green = componentto09CF (green);
  blue = componentto09CF (blue);
  if ((red <= 0x99) && (green <= 0x99) && (blue <= 0x99)) return indexfromtable (red, green, blue); // 0x00s & 0x99s
  if (((red == 0xFF) || (red == 0x00)) && ((green == 0xFF) || (green == 0x00)) && ((blue == 0xFF) || (blue == 0x00))) return indexfromtable (red, green, blue); // 0x00s & 0xFFs
  if ((red == 0xCC) && (green == 0xCC) && (blue == 0xCC)) return 7; // special-case silver 0xCC 0xCC 0xCC
  red = componentto09F (red);
  green = componentto09F (green);
  blue = componentto09F (blue);
  if ((red == 0xFF) || (green == 0xFF) || (blue == 0xFF)) { // at least one component is 0xFF, so set the others to 0x00
    if (red != 0xFF) red = 0x00;
    if (green != 0xFF) green = 0x00;
    if (blue != 0xFF) blue = 0x00;
  }
  return indexfromtable (red, green, blue);
}


/** calculate the optimal threshold for conversion to b/w */
void Vt2IsoImageBase_c::getOptimalBwThreshold( void )
{
  int threshold128AllWhite = ( ( getWidth() * getHeight() ) / 8 ),
  threshold128AllBlack = ( ( getWidth() * getHeight() ) / 8 ),
  thresholdOptimalAllWhite = ( ( getWidth() * getHeight() ) / 8 ),
  thresholdOptimalAllBlack = ( ( getWidth() * getHeight() ) / 8 ),
  thresholdLoopAllWhite = 0,
  thresholdLoopAllBlack = 0,
  optimalThreshold = 0;
  unsigned int roundedWidth = ( ( getWidth() + 7U ) & ( 0xFFFFFFFFU - 7U ) );

  for (unsigned int threshold = 32; threshold <= 224; threshold += 16 ) {
  thresholdLoopAllWhite = 0;
  thresholdLoopAllBlack = 0;
  for (unsigned int ui_y=0; ui_y<getHeight(); ui_y++) {
    for (unsigned int ui_x=0; ui_x<roundedWidth; ui_x+=8) {
      unsigned int pixel1PaletteIndex = ( ( ( getR( ui_x, ui_y ) + getG( ui_x, ui_y ) + getB( ui_x, ui_y ) ) / 3 ) >= threshold )?1:0;
      unsigned int pixel2PaletteIndex = ( ( ( getR( ui_x+1, ui_y ) + getG( ui_x+1, ui_y ) + getB( ui_x+1, ui_y ) ) / 3 ) >= threshold )?1:0;
      unsigned int pixel3PaletteIndex = ( ( ( getR( ui_x+2, ui_y ) + getG( ui_x+2, ui_y ) + getB( ui_x+2, ui_y ) ) / 3 ) >= threshold )?1:0;
      unsigned int pixel4PaletteIndex = ( ( ( getR( ui_x+3, ui_y ) + getG( ui_x+3, ui_y ) + getB( ui_x+3, ui_y ) ) / 3 ) >= threshold )?1:0;
      unsigned int pixel5PaletteIndex = ( ( ( getR( ui_x+4, ui_y ) + getG( ui_x+4, ui_y ) + getB( ui_x+4, ui_y ) ) / 3 ) >= threshold )?1:0;
      unsigned int pixel6PaletteIndex = ( ( ( getR( ui_x+5, ui_y ) + getG( ui_x+5, ui_y ) + getB( ui_x+5, ui_y ) ) / 3 ) >= threshold )?1:0;
      unsigned int pixel7PaletteIndex = ( ( ( getR( ui_x+6, ui_y ) + getG( ui_x+6, ui_y ) + getB( ui_x+6, ui_y ) ) / 3 ) >= threshold )?1:0;
      unsigned int pixel8PaletteIndex = ( ( ( getR( ui_x+7, ui_y ) + getG( ui_x+7, ui_y ) + getB( ui_x+7, ui_y ) ) / 3 ) >= threshold )?1:0;
      unsigned int byte = (pixel1PaletteIndex << 7) + (pixel2PaletteIndex << 6) + (pixel3PaletteIndex << 5) + (pixel4PaletteIndex << 4)
                + (pixel5PaletteIndex << 3) + (pixel6PaletteIndex << 2) + (pixel7PaletteIndex << 1) + pixel8PaletteIndex;
      if ( byte == 0 ) thresholdLoopAllWhite++;
      else if ( byte == 0xFF ) thresholdLoopAllBlack++;
    } // loop one columns of one line
  } // iterate loop for all lines
  if ( ( thresholdLoopAllWhite + thresholdLoopAllBlack ) < ( thresholdOptimalAllWhite + thresholdOptimalAllBlack ) ) {
    // new optimum found
    thresholdOptimalAllWhite = thresholdLoopAllWhite;
    thresholdOptimalAllBlack = thresholdLoopAllBlack;
    optimalThreshold = threshold;
  }
  if ( threshold == 128 ) {
    // currently 128 - default - analysed
    threshold128AllWhite = thresholdLoopAllWhite;
    threshold128AllBlack = thresholdLoopAllBlack;
  }
  }
  // now decide about optimal threshold -> use 128 if optimum is not BETTER
  if ( ( thresholdOptimalAllWhite + thresholdOptimalAllBlack ) < ( threshold128AllWhite + threshold128AllBlack ) )
  { // optimal threshold is really better
    if (isOstream())
      getOstream() << "Found Optimal Threshold: " << optimalThreshold << " which has only " << thresholdOptimalAllWhite << " all white and "
                   << thresholdOptimalAllBlack << " all black entries" << std::endl;
    i_currentThreshold = optimalThreshold;
  }
  else {
    if (isOstream())
      // no better threshold than standard 128 found
      getOstream() << "Default 128 Threshold is optimal as it has only " << threshold128AllWhite << " all white and "
                   << threshold128AllBlack << " all black entries\n"
                   << "Optimal Threshold has " << thresholdOptimalAllWhite << " allWhite and " << thresholdOptimalAllBlack
                   << " all black"
                   << std::endl;
    i_currentThreshold = 128;
  }
}

unsigned int Vt2IsoImageBase_c::componenttoindex6 (unsigned int component)
{
  if (component < (0x33 - 0x33/2)) return 0;
  if (component < (0x66 - 0x33/2)) return 1;
  if (component < (0x99 - 0x33/2)) return 2;
  if (component < (0xCC - 0x33/2)) return 3;
  if (component < (0xFF - 0x33/2)) return 4;
  return 0x5;
}


unsigned int Vt2IsoImageBase_c::componentto09CF (unsigned int comp)
{
  if (comp < (0x99/2)) return 0x00;
  if (comp < (0xCC-0x99)/2+0x99) return 0x99;
  if (comp < (0xFF-0xCC)/2+0xCC) return 0xCC;
  return 0xFF;
}

unsigned int Vt2IsoImageBase_c::componentto09F (unsigned int comp)
{
  if (comp < (0x99/2)) return 0x00;
  if (comp < (0xFF-0x99)/2+0x99) return 0xCC;
  return 0xFF;
}
