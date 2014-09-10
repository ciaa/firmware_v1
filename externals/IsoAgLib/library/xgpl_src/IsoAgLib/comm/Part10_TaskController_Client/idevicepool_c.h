/*
  idevicepool_c.h - Implementation of a device description object pool using modifiable
    C++ objects. The pool is represented of a collection of objects
    derived from a common base class. The device pool synchronization
    with the Task Controller is accomplished by uploading a partial pool
    when objects change.

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef _IDEVICEPOOL_C_H_
#define _IDEVICEPOOL_C_H_

#include <IsoAgLib/comm/Part10_TaskController_Client/iddidefinition.h>
#include <IsoAgLib/comm/Part10_TaskController_Client/impl/devicepool_c.h>

namespace IsoAgLib {
  class iProcData_c;
  class iTcClient_c;

  typedef __IsoAgLib::Localization_s Localization_s;
  typedef __IsoAgLib::StructureLabel_s StructureLabel_s;

  /* Dvc */

  class iDeviceObjectDvc_c : public __IsoAgLib::DeviceObjectDvc_c {
    public:
      iDeviceObjectDvc_c( const char* version, const char* desig ) : __IsoAgLib::DeviceObjectDvc_c( version, desig ) {}
      // TODO currently not updated during run-time.
      void setLocalization( const localSettings_s& s ) {
        __IsoAgLib::DeviceObjectDvc_c::setLocalSettings( s );
      }
      void setLocalization( const Localization_s& s ) {
        __IsoAgLib::DeviceObjectDvc_c::setLocalization( s );
      }
      void setStructureLabel( const char* s ) {
        __IsoAgLib::DeviceObjectDvc_c::setStructureLabel( s );
      }
      void setSerialNumber( const char* s ) {
        __IsoAgLib::DeviceObjectDvc_c::setSerialNumber( s );
      }
  };


  /* Dvp */

  class iDeviceObjectDvp_c : public __IsoAgLib::DeviceObjectDvp_c {
    public:
      iDeviceObjectDvp_c( float scale, int32_t offset, uint8_t decimals, const char* desig ) : __IsoAgLib::DeviceObjectDvp_c( scale, offset, decimals, desig ) {}
      void setOffset( int32_t offset ) {
        __IsoAgLib::DeviceObjectDvp_c::setOffset( offset );
      }
      void setDecimals( uint8_t decimals ) {
        __IsoAgLib::DeviceObjectDvp_c::setDecimals( decimals );
      }
      void setScale( float scale ) {
        __IsoAgLib::DeviceObjectDvp_c::setScale( scale );
      }
      void setUnitDesignator( const char* desig ) {
        __IsoAgLib::DeviceObjectDvp_c::setDesignator( desig );
      }
  };


  /* Dpd */

  class iDeviceObjectDpd_c : public __IsoAgLib::DeviceObjectDpd_c {
    public:
      iDeviceObjectDpd_c( uint16_t dpd_ddi, const ProcData::Properties_t& p, const ProcData::Methods_t& m, const char* desig, const iDeviceObjectDvp_c* dvp )
        : DeviceObjectDpd_c( dpd_ddi, p, m, desig, dvp ) {}

      //! Caution: Only use this c'tor for porting legacy-DDOPs.
      //! For normal, new applications use the type-safe version above!
      iDeviceObjectDpd_c( uint16_t dpd_ddi, uint8_t properties, uint8_t triggerMethods, const char* desig, const iDeviceObjectDvp_c* dvp )
        : DeviceObjectDpd_c( dpd_ddi, properties, triggerMethods, desig, dvp ) {}

      bool propertyDefaultSet() const {
        return __IsoAgLib::DeviceObjectDpd_c::propertyDefaultSet();
      }
  };


  /* Dpt */

  class iDeviceObjectDpt_c : public __IsoAgLib::DeviceObjectDpt_c {
    public:
      iDeviceObjectDpt_c( uint16_t dpt_ddi, int32_t value, const char* desig, const iDeviceObjectDvp_c* dvp ) : __IsoAgLib::DeviceObjectDpt_c( dpt_ddi, value, desig, dvp ) {}
      int32_t getValue() const {
        return __IsoAgLib::DeviceObjectDpt_c::getValue();
      }
  };

  /* Det */

  class iDeviceObjectDet_c : public __IsoAgLib::DeviceObjectDet_c {
    public:
      iDeviceObjectDet_c( const iDeviceObjectDet_c& parent, uint16_t element, uint8_t type, const char* desig ) : DeviceObjectDet_c( parent.getObjectId(), element, type, desig ) {}
      iDeviceObjectDet_c( const iDeviceObjectDvc_c& parent, uint16_t element, uint8_t type, const char* desig ) : DeviceObjectDet_c( parent.getObjectId(), element, type, desig ) {}
      bool addChild( const iDeviceObjectDpt_c& c ) {
        return __IsoAgLib::DeviceObjectDet_c::addChild( c.getObjectId() );
      }
      bool addChild( const iDeviceObjectDpd_c& c ) {
        return __IsoAgLib::DeviceObjectDet_c::addChild( c.getObjectId() );
      }
      uint16_t elementNumber() const {
        return __IsoAgLib::DeviceObjectDet_c::elementNumber();
      };
  };



  /* iDevicePool_c */

  class iDevicePool_c : private __IsoAgLib::DevicePool_c {
    public:
      virtual ~iDevicePool_c() {}

      virtual uint8_t* allocByteStreamBuffer( uint32_t size ) {
        return static_cast<uint8_t*>( CNAMESPACE::malloc( size ) );
      }
      virtual void freeByteStreamBuffer( uint8_t* buffer ) {
        CNAMESPACE::free( buffer );
      }

      //! not pedantic: it is safe to insert object multiple time
      //!   object is then present only once in pool
      template<class T>
      void add( T& devObj ) {
        __IsoAgLib::DevicePool_c::add( devObj );
      }
      //! Caution: Be sure one iProcData_c is only added once
      //!   Not safe to insert same iProcData_c multiple time
      void add( iProcData_c& pd );

      //! Caution: Be sure to only clear if not connected to any server!
      void clear() {
        __IsoAgLib::DevicePool_c::clear();
      }

      /* changing designators is not yet supported */
      template<class T>
      void changeDesignator( T& obj, const char* str ) {
        __IsoAgLib::DevicePool_c::changeDesignator( obj, str );
      }

      void setLocalSettings( const localSettings_s& l ) {
        __IsoAgLib::DevicePool_c::setLocalSettings( l );
      }
      void updateLocale() {
        __IsoAgLib::DevicePool_c::updateLocale();
      }
    private:
      friend class iTcClient_c;
  };

} // IsoAgLib

#endif // _IDEVICEPOOL_C_H_
