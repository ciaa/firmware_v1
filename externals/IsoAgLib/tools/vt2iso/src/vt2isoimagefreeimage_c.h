/*
  vt2isoimagefreeimage_c.h: image handling for vt2iso with
    paintlib library

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef VT2ISOIMAGEFREEIMAGE_C_H
#define VT2ISOIMAGEFREEIMAGE_C_H

/* *************************************** */
/* ********** include headers ************ */
/* *************************************** */
#include <FreeImage.h>

#include "vt2isoimagebase_c.h"

// ---------------------------------------------------------------------------
//  Concrete class for image access with Paintlib
// ---------------------------------------------------------------------------
class Vt2IsoImageFreeImage_c : public Vt2IsoImageBase_c
{
 public:
 	/** constructor which can directly open the corresponding file */
	Vt2IsoImageFreeImage_c( const char* filename = NULL );
	
	void printLicenseText();
	
	virtual void close( void );
 	/** open the given bitmap file and guarantee
		that the pixels can be accessed by RGB
		( i.e. convert if indexed )
		@return true -> opened and converted correct
	*/
	virtual bool openBitmap( const char* filename );
 protected:
	/** deliver R-value of bitmap at given position */
	virtual unsigned int getR( unsigned int aui_x, unsigned int aui_y );
	/** deliver G-value of bitmap at given position */
	virtual unsigned int getG( unsigned int aui_x, unsigned int aui_y );
	/** deliver B-value of bitmap at given position */
	virtual unsigned int getB( unsigned int aui_x, unsigned int aui_y );

  int getPaletteIndex (unsigned int aui_x, unsigned int aui_y);

 private:
	/** check and adapt scanline */
	void checkUpdateScanline( unsigned int aui_y );

	FIBITMAP *bitmap;
	BYTE *scanline;
	int bytespp;
	int i_curScanLineY;
	static bool b_FreeImageLibInited;
  
  bool mb_palettized;
  int mi_colourMismatch; // <0 for NO MISMATCH, 0..255 for the colour index mismatching!
};

#endif
