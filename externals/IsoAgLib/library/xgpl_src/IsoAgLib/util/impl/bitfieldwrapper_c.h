/*
  bitfieldwrapper_c.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef BITFIELDWRAPPER_C_H
#define BITFIELDWRAPPER_C_H

#include <IsoAgLib/isoaglib_config.h>

// ISOAgLib
#include <IsoAgLib/util/iassert.h>

// STL
#include <vector>


template <unsigned N> class Isoaglib32Bitset {
  private:
    STL_NAMESPACE::vector<uint32_t> _v;
  public:
    Isoaglib32Bitset( void ): _v((N+31)>>5, uint32_t(0) ) {}
    Isoaglib32Bitset( const Isoaglib32Bitset& ar_src ) : _v(ar_src._v){};

    Isoaglib32Bitset<N>& set( void ) {
      STL_NAMESPACE::vector<uint32_t>::iterator i;
      for (i=_v.begin(); i!=_v.end(); ++i) *i = 0xFFFFFFFFUL;
      return *this;
    }

    Isoaglib32Bitset<N>& reset( void ) {
      STL_NAMESPACE::vector<uint32_t>::iterator i;
      for (i=_v.begin(); i!=_v.end(); ++i) *i = 0;
      return *this;
    }

    bool test(unsigned n) const {
      return bool((_v[n>>5] & (uint32_t(1)<<(n&0x1F)))>0);
    }

    Isoaglib32Bitset<N>& set( unsigned n, int val = 1 ) {
      if (0 == val)
        _v[n>>5] &= ~(uint32_t(1)<<(n&0x1F));
      else
        _v[n>>5] |=  (uint32_t(1)<<(n&0x1F));
      return *this;
    }

    Isoaglib32Bitset<N>& reset( unsigned n ) {
      return set(n,0);
    }

    unsigned count( void ) const {
      unsigned n,c=0;
      for (n=0; n<N; ++n) if (test(n)) ++c;
      return c;
    }

};


template <unsigned N> class IsoaglibBitset {
  private:
    STL_NAMESPACE::vector<uint8_t> _v;
  public:
    IsoaglibBitset( void ): _v((N+7)>>3, uint8_t(0) ) {}
    IsoaglibBitset( const IsoaglibBitset& ar_src ) : _v(ar_src._v){};

    IsoaglibBitset<N>& set( void ) {
      STL_NAMESPACE::vector<uint8_t>::iterator i;
      for (i=_v.begin(); i!=_v.end(); ++i) *i = 0xFFu;
      return *this;
    }

    IsoaglibBitset<N>& reset( void ) {
      STL_NAMESPACE::vector<uint8_t>::iterator i;
      for (i=_v.begin(); i!=_v.end(); ++i) *i = 0x00u;
      return *this;
    }

    bool isBitSet(unsigned n) const {
      return ((_v[n>>3] & (uint8_t(1)<<(7-(n&0x07))))>0);
    }

    IsoaglibBitset<N>& setBit( unsigned n, int val = 1 ) {
      if (0 == val)
        _v[n>>3] &= ~(uint8_t(1)<<(7-(n&0x07)));
      else
        _v[n>>3] |=  (uint8_t(1)<<(7-(n&0x07)));
      return *this;
    }

    IsoaglibBitset<N>& reset( unsigned n ) {
      return setBit(n, 0);
    }

    unsigned count( void ) const {
      unsigned n,c=0;
      for (n=0; n<N; ++n) if (isBitSet(n)) ++c;
      return c;
    }

    uint8_t getByte(unsigned n) const {
      isoaglib_header_assert (n < ((N+7)>>3));
      return _v[n];
    }
};


template <unsigned N> class IsoaglibArrayBitset {
  private:
    uint8_t arr[ (N+7)>>3 ];
  public:
    IsoaglibArrayBitset() { reset(); }
    IsoaglibArrayBitset( const IsoaglibArrayBitset& rhs ) {
      for( unsigned i=0; i<((N+7)>>3); ++i)
        arr[ i ] = rhs.arr[ i ];
    }

    unsigned bits() const { return N; }

    IsoaglibArrayBitset<N>& set() {
      for( unsigned i=0; i<((N+7)>>3); ++i)
        arr[ i ] = 0xFFu;
      return *this;
    }

    IsoaglibArrayBitset<N>& reset( void ) {
      for( unsigned i=0; i<((N+7)>>3); ++i)
        arr[ i ] = 0x00u;
      return *this;
    }

    bool isBitSet(unsigned n) const {
      return ((arr[n>>3] & (uint8_t(1)<<(7-(n&0x07))))>0);
    }

    IsoaglibArrayBitset<N>& setBit( unsigned n, int val = 1 ) {
      if (0 == val)
        arr[n>>3] &= ~(uint8_t(1)<<(7-(n&0x07)));
      else
        arr[n>>3] |=  (uint8_t(1)<<(7-(n&0x07)));
      return *this;
    }

    IsoaglibArrayBitset<N>& reset( unsigned n ) {
      return setBit(n, 0);
    }

    unsigned count( void ) const {
      unsigned n,c=0;
      for (n=0; n<N; ++n)
        if (isBitSet(n))
          ++c;
      return c;
    }

    uint8_t getByte(unsigned n) const {
      isoaglib_assert (n < ((N+7)>>3));
      return arr[n];
    }
};



//! Example:
//! 11 bit
//! gets put into 2 bytes counted from 0 starting at the outmost left:
//! 0123 4567  89Ax xxxx
//!
//! Example-usage:
//! struct Certification_s
//! {
//!   typedef Certification_t enum_type;
//!   enum { number_of_bits = 24 };
//! };
//! typedef BitFieldWrapper_c<Certification_s> CertificationBitMask_t;


template <class T>
class BitFieldWrapper_c
{
  protected:
    enum {sizeInBits = T::number_of_bits};
    enum {sizeInBytes = ( T::number_of_bits+7 ) >> 3 };

    STL_NAMESPACE::vector<uint8_t> m_bitField;

  public:
    /** Constructor */
    BitFieldWrapper_c ():
      m_bitField() // intentionally not put here due to IAR compiler problem!
    {
      m_bitField.resize( sizeInBytes, 0x00 );
    }

    /** Destructor */
    ~BitFieldWrapper_c()
	{
	}
    
    unsigned int getSizeInBytes() const { return sizeInBytes; }

    /** set all bits - be sure to check if this is appropriate,
        or if there are any reserved bits which should not be set */
    BitFieldWrapper_c& setAllBits()
    {
      STL_NAMESPACE::vector<uint8_t>::iterator i;
      for (i=m_bitField.begin(); i!=m_bitField.end(); ++i) *i = 0xFFu;
      return *this;
    }

    /** do bitwise AND assignment */
    void operator &= ( const BitFieldWrapper_c& c_refBitField )
    {
      STL_NAMESPACE::vector<uint8_t>::iterator i = m_bitField.begin();
      STL_NAMESPACE::vector<uint8_t>::const_iterator j = c_refBitField.m_bitField.begin();
      for (; i!=m_bitField.end(); ++i, ++j)
        *i &= *j;
    }

    /** do bitwise OR assignment */
    void operator |= ( const BitFieldWrapper_c& c_refBitField )
    {
      STL_NAMESPACE::vector<uint8_t>::iterator i = m_bitField.begin();
      STL_NAMESPACE::vector<uint8_t>::const_iterator j = c_refBitField.m_bitField.begin();
      for (; i!=m_bitField.end(); ++i, ++j)
        *i |= *j;
    }

    /** do bitwise XOR assignment */
    void operator ^= ( const BitFieldWrapper_c& c_refBitField )
    {
      STL_NAMESPACE::vector<uint8_t>::iterator i = m_bitField.begin();
      STL_NAMESPACE::vector<uint8_t>::const_iterator j = c_refBitField.m_bitField.begin();
      for (; i!=m_bitField.end(); ++i, ++j)
        *i ^= *j;
    }

    /** do bitwise left shift */
    void operator <<= ( size_t num ); // not implemented currently!
//    { // for bitset, bit 0 is the LSB => we need to invert <<= to >>=
// TODO      m_bitField >>= num;
//    }

    /** do bitwise right shift */
    void operator >>= ( size_t num ); // not implemented currently!
//    { // for bitset, bit 0 is the LSB => we need to invert >>= to <<=
// TODO      m_bitField <<= num;
//    }

    /** A bytewise getter */
    uint8_t getByte ( uint8_t aui8_byteOffset ) const
    {
// TODO      isoaglib_assert (n < ((N+7)>>3));
      return m_bitField[ aui8_byteOffset ];
    }
};

template <class T>
class BitFieldWrapperLeft_c : public BitFieldWrapper_c<T>
{
  using BitFieldWrapper_c<T>::m_bitField;
  using BitFieldWrapper_c<T>::sizeInBits;

public:
  /** Set the given bit to 1. Bits are counted from the left
      @param  a_bitsFromTheLeft  bit position from the left, counting from 0
      @return  reference to BitFieldWrapper_c
    */
  BitFieldWrapperLeft_c& setBit( typename T::enum_type a_bitsFromTheLeft )
  {
    m_bitField[a_bitsFromTheLeft>>3] |= (uint8_t(1)<<(7-(a_bitsFromTheLeft&0x07)));
    return *this;
  }
  /** Checks wheether the given bit is set to 1. Bits are counted from the left
      @param  a_bitsFromTheLeft  bit position from the left, counting from 0
                                 if it is out-of-range, false (0 bit) is returned
      @return  true if the bit is set
  */
  bool isBitSet( typename T::enum_type a_bitsFromTheLeft )
  {
    return ((m_bitField[a_bitsFromTheLeft>>3] & (uint8_t(1)<<(7-(a_bitsFromTheLeft&0x07))))>0);
  }
};

template <class T>
class BitFieldWrapperRight_c : public BitFieldWrapper_c<T>
{
  using BitFieldWrapper_c<T>::m_bitField;
  using BitFieldWrapper_c<T>::sizeInBits;

public:
  /** Set the given bit to 1. Bits are counted from the right
      @param  a_bitsFromTheRight  bit position from the right, counting from 0
      @return  reference to BitFieldWrapper_c
    */
  BitFieldWrapperRight_c& setBit( typename T::enum_type a_bitsFromTheRight )
  {
    m_bitField[m_bitField.size() - (a_bitsFromTheRight>>3) - 1] |= (uint8_t(1)<<(a_bitsFromTheRight&0x07));
    return *this;
  }
  /** Checks wheether the given bit is set to 1. Bits are counted from the right
      @param  a_bitsFromTheLeft  bit position from the right, counting from 0
                                 if it is out-of-range, false (0 bit) is returned
      @return  true if the bit is set
  */
  bool isBitSet( typename T::enum_type a_bitsFromTheRight )
  {
    return ((m_bitField[m_bitField.size() - (a_bitsFromTheRight>>3) - 1] & (uint8_t(1)<<(a_bitsFromTheRight&0x07)))>0);
  }
};

#endif
