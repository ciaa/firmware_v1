/*
  singleton.h

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef SINGLETON_H_
#define SINGLETON_H_

#include <IsoAgLib/isoaglib_config.h>

#include <new>

#if defined(OPTIMIZE_HEAPSIZE_IN_FAVOR_OF_SPEED) && defined( __GNUC__ ) && __GNUC__ >= 4
#  include <ext/malloc_allocator.h>
#endif

#define MULTITON_INST_PARAMETER_DEF               int ai_multitonInst
#define MULTITON_INST_PARAMETER_DEFAULT_NULL_DEF  int ai_multitonInst = 0
#define MULTITON_INST_PARAMETER_DEF_WITH_COMMA  , int ai_multitonInst
#define MULTITON_INST_PARAMETER_DEFAULT_NULL_DEF_WITH_COMMA , int ai_multitonInst = 0
#define MULTITON_INST_PARAMETER_USE               ai_multitonInst
#define MULTITON_INST_PARAMETER_USE_WITH_COMMA  , ai_multitonInst

#define MULTITON_PAR_ARR_DEF(PAR)                     int getMultitonInst() const { return (PAR) ? (PAR)->getMultitonInst() : 0; }
#define MULTITON_MEMBER_DEF               ClientBase c_clientBase; \
                                                    int getMultitonInst() const { return c_clientBase.getMultitonInst(); }
#define MULTITON_MEMBER_ASSIGN(PAR)     c_clientBase.setMultitonInst (PAR.c_clientBase.getMultitonInst());
#define MULTITON_MEMBER_CONSTRUCTOR    c_clientBase( ai_multitonInst ),
#define MULTITON_MEMBER_COPY_CONSTRUCTOR(PAR) c_clientBase( PAR.getMultitonInst() ),
#define MULTITON_PARENT_CONSTRUCTOR      ClientBase( ai_multitonInst ),

#define MULTITON_INST_INIT_CALL                   setMultitonInst( ai_multitonInst );
#define MULTITON_INST                             getMultitonInst()
#define MULTITON_INST_WITH_COMMA                , getMultitonInst()


#define getForeignInstance4Comm(PAR)      PAR.getMultitonInst()
#define getIsoBusInstance4Comm()          getIsoBusInstance( getMultitonInst() )
#define getCanInstance4Comm()             getCanInstance( getMultitonInst() )
#define getTimePosGpsInstance4Comm()      getTimePosGpsInstance( getMultitonInst() )
#define getTracGeneralInstance4Comm()     getTracGeneralInstance( getMultitonInst() )
#define getTracFacilitiesInstance4Comm()  getTracFacilitiesInstance( getMultitonInst() )
#define getTracMoveInstance4Comm()        getTracMoveInstance( getMultitonInst() )
#define getTracLightInstance4Comm()       getTracLightInstance( getMultitonInst() )
#define getTracAuxInstance4Comm()         getTracAuxInstance( getMultitonInst() )
#define getTracPtoInstance4Comm()         getTracPtoInstance( getMultitonInst() )
#define getTracGuidanceInstance4Comm()    getTracGuidanceInstance( getMultitonInst() )
#define getTracGuidanceCommandInstance4Comm()  getTracGuidanceCommandInstance( getMultitonInst() )
#define getIsbClientInstance4Comm()       getIsbClientInstance( getMultitonInst() )
#define getIsoMonitorInstance4Comm()      getIsoMonitorInstance( getMultitonInst() )
#define getIsoFilterManager4Comm()        getIsoFilterManagerInstance( getMultitonInst() )
#define getTcClientInstance4Comm()         getTcClientInstance( getMultitonInst() )
#define getMultiSendInstance4Comm()       getMultiSendInstance( getMultitonInst() )
#define getMultiReceiveInstance4Comm()    getMultiReceiveInstance( getMultitonInst() )
#define getVtClientInstance4Comm()        getVtClientInstance( getMultitonInst() )
#define getIsoRequestPgnInstance4Comm()   getIsoRequestPgnInstance( getMultitonInst() )
#define getIsoFilterManagerInstance4Comm() getIsoFilterManagerInstance( getMultitonInst() )
#define getProprietaryMessageHandlerInstance4Comm() getProprietaryMessageHandlerInstance( getMultitonInst() )
#define getFsManagerInstance4Comm()       getFsManagerInstance( getMultitonInst() )

/** the class ClientBase delivers the base information, to concat client class instances
 * with the corresponding server class instance. This is realized by the single
 * attribute singletonVecKey
 */
class ClientBase {
public:
  ClientBase( int ai_multitonInst = 0 ) : mi_multitonInst(ai_multitonInst){};
  ClientBase( const ClientBase& acrc_src )
    : mi_multitonInst(acrc_src.mi_multitonInst) { };
  int getMultitonInst() const { return mi_multitonInst;};
  void setMultitonInst( int ai_instance ) { mi_multitonInst = ai_instance;};
protected:
  const ClientBase& operator=( const ClientBase& acrc_src )
  { mi_multitonInst = acrc_src.mi_multitonInst; return *this;};
private:
  int mi_multitonInst;
};

/** PROP_SINGLETON defines the number of proprietary busses to be supported.
  */
#if (PROP_INSTANCE_CNT == 0)
#  define getCanInstance4Prop()   ERROR_not_configured_for_Proprietary_CAN_usage_ERROR
#elif (PROP_INSTANCE_CNT > 1)
#  define getCanInstance4Prop()   getCanInstance( (PRT_INSTANCE_CNT) + getMultitonInst() )
#else
#  if (PRT_INSTANCE_CNT == 0)
#    define getCanInstance4Prop()   getCanInstance()
#  else
#    define getCanInstance4Prop()   getCanInstance( (PRT_INSTANCE_CNT) )
#  endif
#endif


#define MACRO_MULTITON_CONTRIBUTION() \
public: \
  int getMultitonInst() const { return mi_multitonInst; } \
  void setMultitonInst(int ai_instance) { mi_multitonInst = ai_instance; } \
private: \
  int mi_multitonInst

#define DO_PLACEMENT_NEW_ON_STATIC_BUFFER 1

#if DO_PLACEMENT_NEW_ON_STATIC_BUFFER
namespace __IsoAgLib
{
union MaxAlign_u {
  long li_dummy;
  void *p_dummy;
};
} // namespace __IsoAgLib
#  define DEFINE_STATIC_BUFFER_FOR_PLACEMENT_NEW(T, SIZE) \
    static union { \
      __IsoAgLib::MaxAlign_u mu_dummy; \
      char mc_mem[SIZE * (sizeof(T)+sizeof(__IsoAgLib::MaxAlign_u))]; \
    } su_place
#  define STATIC_BUFFER_ARGUMENT_FOR_PLACEMENT_NEW(T, instance) \
  (su_place.mc_mem + (instance * (((sizeof(T) + sizeof(__IsoAgLib::MaxAlign_u))/sizeof(__IsoAgLib::MaxAlign_u))*sizeof(__IsoAgLib::MaxAlign_u)) ))
#else //DO_PLACEMENT_NEW_ON_STATIC_BUFFER
#  define DEFINE_STATIC_BUFFER_FOR_PLACEMENT_NEW(T, SIZE) (void)0
#  define STATIC_BUFFER_ARGUMENT_FOR_PLACEMENT_NEW(T, instance)
#endif //DO_PLACEMENT_NEW_ON_STATIC_BUFFER

#define MACRO_MULTITON_GET_INSTANCE_BODY(T, SIZE, instance)  \
  static T *sarrpt_instances[SIZE]; \
  static bool isInitialized = false; \
  DEFINE_STATIC_BUFFER_FOR_PLACEMENT_NEW(T, SIZE); \
  if( !isInitialized ) \
  { \
    for( unsigned instLoop=0; instLoop < SIZE; ++instLoop ) \
    { \
      sarrpt_instances[ instLoop ] = new STATIC_BUFFER_ARGUMENT_FOR_PLACEMENT_NEW(T, instLoop) T; \
      sarrpt_instances[ instLoop ]->setMultitonInst( instLoop ); \
    } \
    isInitialized = true; \
  } \
  return *sarrpt_instances[ instance ]

#define MACRO_SINGLETON_GET_INSTANCE_BODY(T)  \
  static T *rpt_instance=0; \
  if (0 == rpt_instance) { \
    DEFINE_STATIC_BUFFER_FOR_PLACEMENT_NEW(T, 1); \
    rpt_instance = new STATIC_BUFFER_ARGUMENT_FOR_PLACEMENT_NEW(T, 0) T; \
  } \
  return *rpt_instance

#endif // SINGLETON_H_
