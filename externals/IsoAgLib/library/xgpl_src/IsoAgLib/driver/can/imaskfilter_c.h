/*
  imaskfilter_c.h: header file for iMaskFilter_c and iMaskFilterType_c

  (C) Copyright 2011 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef __IMASK_FILTER_C
#define __IMASK_FILTER_C

#include <IsoAgLib/driver/can/iident_c.h>
#include <IsoAgLib/util/iassert.h>


namespace IsoAgLib {

  //! CAN mask and filter pair for saver interface access
  class iMaskFilter_c {
    static const uint32_t s_maskInvalid = 0xC0000000UL;
    public:
      iMaskFilter_c()
          : mt_identMask( s_maskInvalid ), mt_identFilter( 0 ) {}


      iMaskFilter_c( const iMaskFilter_c& arc_hs ) {
        setMask( arc_hs.getMask() );
        setFilter( arc_hs.getFilter() );
      }


      iMaskFilter_c( uint32_t aui32_mask, uint32_t aui32_filter )
        : mt_identMask( aui32_mask ), mt_identFilter( aui32_filter ) {}


      bool operator==( const iMaskFilter_c& rhs ) const {
        return ( rhs.empty() && empty() )
               || ( rhs.getMask() == getMask()
                    && rhs.getFilter() == getFilter() );
      }


      iMaskFilter_c& operator=( const iMaskFilter_c& rhs ) {
        setMask( rhs.getMask() );
        setFilter( rhs.getFilter() );
        return *this;
      }


      bool empty() const {
        return( getMask() == s_maskInvalid );
      }


      void setEmpty() {
        setMask( s_maskInvalid );
      }


      void setMask( uint32_t aui32_mask ) {
        mt_identMask = aui32_mask;
      }


      uint32_t getMask() const {
        return mt_identMask;
      }


      void setFilter( uint32_t aui32_filter ) {
        mt_identFilter = aui32_filter;
      }


      uint32_t getFilter() const {
        return mt_identFilter;
      }


    private:
      uint32_t mt_identMask;
      uint32_t mt_identFilter;
  };


  class iMaskFilterType_c : public iMaskFilter_c {
    public:
      iMaskFilterType_c()
          : iMaskFilter_c(), mt_type( iIdent_c::StandardIdent ) {}


      iMaskFilterType_c( const iMaskFilterType_c& arc_hs )
          : iMaskFilter_c( arc_hs ), mt_type( arc_hs.getType() ) { }


      iMaskFilterType_c( const iMaskFilter_c& arc_mf, iIdent_c::identType_t at_type )
          : iMaskFilter_c( arc_mf ), mt_type( at_type ) {}


      iMaskFilterType_c( uint32_t aui32_mask, uint32_t aui32_filter, iIdent_c::identType_t at_type )
          : iMaskFilter_c( aui32_mask, aui32_filter ), mt_type( at_type ) {}


      bool operator==( const iMaskFilterType_c& rhs ) const {
        return(( iMaskFilter_c::operator==( rhs ) )
               && ( rhs.getType() == getType() ) ) ;
      }


      iMaskFilterType_c& operator=( const iMaskFilterType_c& rhs ) {
        iMaskFilter_c::operator=( rhs );
        setType( rhs.getType() );
        return *this;
      }


      iIdent_c getMaskIdent() const {
        return iIdent_c( getMask(), getType() );
      }


      iIdent_c getFilterIdent() const {
        return iIdent_c( getFilter(), getType() );
      }


      void setType( iIdent_c::identType_t at_type ) {
        mt_type = at_type;
      }


      iIdent_c::identType_t getType() const {
        return mt_type;
      }


      bool matchMsgId( const iIdent_c& arc_ident ) const {

        isoaglib_header_assert(( getFilter() & ( ~getMask() ) ) == 0 );

        bool r = ( arc_ident.identType() == mt_type );
        r &= (( getMask() & arc_ident.ident() ) == getFilter() );
        return r;
      }


    private:
      iIdent_c::identType_t mt_type;
  };


}
#endif
// eof
