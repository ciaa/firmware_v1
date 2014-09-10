/*
  ivtincludes.h: Includes all neccessary headers

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IVTINCLUDES
#define IVTINCLUDES

#include "ivttypes.h"

#include "ivtobjectbutton_c.h"
#include "ivtobjectdatamask_c.h"
#include "ivtobjectfontattributes_c.h"
#include "ivtobjectgraphicscontext_c.h"
#include "ivtobjectmacro_c.h"
#include "ivtobjectfillattributes_c.h"
#include "ivtobjectinputattributes_c.h"
#include "ivtobjectlineattributes_c.h"
#include "ivtobjectpicturegraphic_c.h"
#include "ivtobjectsoftkeymask_c.h"
#include "ivtobjectworkingset_c.h"

#ifdef USE_VTOBJECT_alarmmask
  #include "ivtobjectalarmmask_c.h"
#endif
#ifdef USE_VTOBJECT_archedbargraph
  #include "ivtobjectarchedbargraph_c.h"
#endif
#ifdef USE_VTOBJECT_auxiliaryfunction
  #include "ivtobjectauxiliaryfunction_c.h"
#endif
#ifdef USE_VTOBJECT_auxiliaryinput
  #include "ivtobjectauxiliaryinput_c.h"
#endif
#ifdef USE_VTOBJECT_container
  #include "ivtobjectcontainer_c.h"
#endif
#ifdef USE_VTOBJECT_ellipse
  #include "ivtobjectellipse_c.h"
#endif
#ifdef USE_VTOBJECT_inputboolean
  #include "ivtobjectinputboolean_c.h"
#endif
#ifdef USE_VTOBJECT_inputlist
  #include "ivtobjectinputlist_c.h"
#endif
#ifdef USE_VTOBJECT_inputnumber
  #include "ivtobjectinputnumber_c.h"
#endif
#ifdef USE_VTOBJECT_inputstring
  #include "ivtobjectinputstring_c.h"
#endif
#ifdef USE_VTOBJECT_key
  #include "ivtobjectkey_c.h"
#endif
#ifdef USE_VTOBJECT_line
  #include "ivtobjectline_c.h"
#endif
#ifdef USE_VTOBJECT_linearbargraph
  #include "ivtobjectlinearbargraph_c.h"
#endif
#ifdef USE_VTOBJECT_meter
  #include "ivtobjectmeter_c.h"
#endif
#ifdef USE_VTOBJECT_numbervariable
  #include "ivtobjectnumbervariable_c.h"
#endif
#ifdef USE_VTOBJECT_objectpointer
  #include "ivtobjectobjectpointer_c.h"
#endif
#ifdef USE_VTOBJECT_outputnumber
  #include "ivtobjectoutputnumber_c.h"
#endif
#ifdef USE_VTOBJECT_outputstring
  #include "ivtobjectoutputstring_c.h"
#endif
#ifdef USE_VTOBJECT_outputlist
  #include "ivtobjectoutputlist_c.h"
#endif
#ifdef USE_VTOBJECT_polygon
  #include "ivtobjectpolygon_c.h"
#endif
#ifdef USE_VTOBJECT_rectangle
  #include "ivtobjectrectangle_c.h"
#endif
#ifdef USE_VTOBJECT_stringvariable
  #include "ivtobjectstringvariable_c.h"
#endif
#ifdef USE_VTOBJECT_auxiliaryfunction2
  #include "ivtobjectauxiliaryfunction2_c.h"
#endif
#ifdef USE_VTOBJECT_auxiliaryinput2
  #include "ivtobjectauxiliaryinput2_c.h"
#endif
#ifdef USE_VTOBJECT_auxiliarycontroldesignatorobjectpointer
  #include "ivtobjectauxiliarypointer_c.h"
#endif

#include "ivtclientobjectpool_c.h"
#include "ivtclient_c.h"

#endif
