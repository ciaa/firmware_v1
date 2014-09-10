/*
  vt2isoimagebase_c.h: image handling for vt2iso - base class

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef VT2ISOIMAGEBASE_C_H
#define VT2ISOIMAGEBASE_C_H

#include <ostream>


static const unsigned int colour16table [16] [3] = {
	{0x00,0x00,0x00},
	{0xFF,0xFF,0xFF},
	{0x00,0x99,0x00},
	{0x00,0x99,0x99},
	{0x99,0x00,0x00},
	{0x99,0x00,0x99},
	{0x99,0x99,0x00},
	{0xCC,0xCC,0xCC},
	{0x99,0x99,0x99},
	{0x00,0x00,0xFF},
	{0x00,0xFF,0x00},
	{0x00,0xFF,0xFF},
	{0xFF,0x00,0x00},
	{0xFF,0x00,0xFF},
	{0xFF,0xFF,0x00},
	{0x00,0x00,0x99}};


// ---------------------------------------------------------------------------
//  Abstract base class for image access
// ---------------------------------------------------------------------------
class Vt2IsoImageBase_c
{
 public:
 	/** constructor which can directly open the corresponding file */
	Vt2IsoImageBase_c( void );
	
	void printLicenseText() {}

  // enable informative output via the given ostream.
  // if NULL is passed, output is suppressed.
  // construction time default is "disabled".
  void setOstream(std::ostream& ap_ostream) { mp_ostream = &ap_ostream; }
  // disable informative output
  void resetOstream() { mp_ostream = NULL; }

  virtual ~Vt2IsoImageBase_c() {} // Prevent from warning
	void reset( void );
	void resetLengths( void );
	virtual void close( void );
 	/** open the given bitmap file and guarantee
		that the pixels can be accessed by RGB
		( i.e. convert if indexed )
		@return true -> opened and converted correct
	*/
	virtual bool openBitmap( const char* filename ) = 0;
	/** deliver the widht of current opened bitmap ( 0 == no open bitmap ) */
	unsigned int getWidth( void ) const { return ui_width;};
	/** deliver the height of current opened bitmap ( 0 == no open bitmap ) */
	unsigned int getHeight( void ) const { return ui_height;};
	/** deliver the b/w thresholded value at given bitmap position
		( calculate the optimal bitmap threshold if not yet defined )
	 */
	unsigned int get1BitPixel( unsigned int aui_x, unsigned int aui_y );
	/** get the ISO virtual table indexed bitmap value for 4Bit ( 16colour ) target bitmap */
	unsigned int get4BitPixel( unsigned int aui_x, unsigned int aui_y );
	/** get the ISO virtual table indexed bitmap value for 8Bit ( 256colour ) target bitmap */
	unsigned int get8BitPixel( unsigned int aui_x, unsigned int aui_y );

	/** write the Bitmap to the given buffer and return amount of written Bytes */
	unsigned int write1BitBitmap( unsigned char* pui_bitmap, unsigned int aui_maxSize);
	/** write the Bitmap to the given buffer and return amount of written Bytes */
	unsigned int write4BitBitmap( unsigned char* pui_bitmap, unsigned int aui_maxSize);
	/** write the Bitmap to the given buffer and return amount of written Bytes and -1 if palette did not match */
	int write8BitBitmap( unsigned char* pui_bitmap, unsigned int aui_maxSize);
	unsigned int objRawBitmapBytes [3];
	bool b_isInvalidPalette;


 protected:
	/** deliver R-value of bitmap at given position */
	virtual unsigned int getR( unsigned int aui_x, unsigned int aui_y ) = 0;
	/** deliver G-value of bitmap at given position */
	virtual unsigned int getG( unsigned int aui_x, unsigned int aui_y ) = 0;
	/** deliver B-value of bitmap at given position */
	virtual unsigned int getB( unsigned int aui_x, unsigned int aui_y ) = 0;

  /** deliver Palette-Index of bitmap
   @return -1 for NOT IN PALETTE MODE
   @return 0..255 for PALETTE INDEX */
  virtual int getPaletteIndex (unsigned int aui_x, unsigned int aui_y) = 0;

	/** calculate the optimal threshold for conversion to b/w */
	void getOptimalBwThreshold( void );
	/** deliver 4Bit palette value */
	unsigned int indexfromtable (unsigned int red, unsigned int green, unsigned int blue);
	/** dliver 16Bit Palette value for given R/G/B values */
	unsigned int rgbtopalette16 (unsigned int red, unsigned int green, unsigned int blue);
	unsigned int componenttoindex6 (unsigned int component);
	unsigned int componentto09CF (unsigned int comp);
	unsigned int componentto09F (unsigned int comp);

	#if 0
	/** get 4Bit palette pixel value for selected pixel */
	unsigned int get4BitPalettePixel( unsigned int aui_x, unsigned int aui_y ) = 0;
	/** get 8Bit palette pixel value for selected pixel */
	unsigned int get8BitPalettePixel( unsigned int aui_x, unsigned int aui_y ) = 0;
	#endif
	/** current threshold for b/w derive - default -1 == not yet calculated */
	int i_currentThreshold;
	/** width of bitmap */
	unsigned int ui_width;
	/** height of bitmap */
	unsigned int ui_height;

  bool isOstream() { return (mp_ostream != NULL); }
  // only use getOstream() if checked with isOstream().
  std::ostream& getOstream() { return *mp_ostream; }

private:
  std::ostream* mp_ostream;
};

#endif
