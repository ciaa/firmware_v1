/*
  vt2iso-globals.hpp

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/

#ifndef VT2ISGLOBALS_H
#define VT2ISGLOBALS_H

#include "vt2iso-defines.hpp"
#include <iostream>
#include <string>



extern bool itocolour(unsigned int ui_index, std::string& c_outputText);
extern bool itofontsize(unsigned int ui_index, std::string& c_outputText);
extern bool itofonttype(unsigned int ui_index, std::string& c_outputText);
extern bool itoformat(unsigned int ui_index, std::string& c_outputText);
extern bool itohorizontaljustification(unsigned int ui_index, std::string& c_outputText);
extern bool itoverticaljustification(unsigned int ui_index, std::string& c_outputText);
extern bool itopolygontype(unsigned int ui_index, std::string& c_outputText);
extern bool itoellipsetype(unsigned int ui_index, std::string& c_outputText);
extern bool itoacousticsignal(unsigned int ui_index, std::string& c_outputText);
extern bool itopriority(unsigned int ui_index, std::string& c_outputText);
extern bool itoauxfunctiontype(unsigned int ui_index, std::string& c_outputText);
extern bool itoaux2functiontype(unsigned int ui_index, std::string& c_outputText);
extern bool itoauxfunction2options(unsigned int ui_index, std::string& c_outputText);
extern bool itoauxinput2options(unsigned int ui_index, std::string& c_outputText);
extern bool itoauxcondesignobjptrtype(unsigned int ui_index, std::string& c_outputText);
extern bool itofilltype(unsigned int ui_index, std::string& c_outputText);
extern bool itoevent(unsigned int ui_index, std::string& c_outputText);
extern bool itovalidationtype(unsigned int ui_index, std::string& c_outputText);
extern bool itolineardirection(unsigned int ui_index, std::string& c_outputText);
extern bool itometeroptions(uint8_t ui8_options, std::string& c_outputText);
extern bool itoarchedbargraphoptions(uint8_t ui8_options, std::string& c_outputText);
extern bool itostringoptions(uint8_t ui8_options, std::string& c_outputText);
extern bool itoinputnumberoptions(uint8_t ui8_options, std::string& c_outputText);
extern bool itonumberoptions(uint8_t ui8_options, std::string& c_outputText);
extern bool itopicturegraphicoptions(uint8_t ui8_options, std::string& c_outputText);
extern bool itopicturegraphicrle(uint8_t ui8_options, std::string& c_outputText);
extern bool itolinearbargraphoptions(uint8_t ui8_options, std::string& c_outputText);
extern bool itokeygroupoptions(uint8_t ui8_options, std::string& c_outputText);
extern bool itogcoptions(uint8_t ui8_options, std::string& c_outputText);
extern bool itoinputobjectoptions(uint8_t ui8_options, std::string& c_outputText);
extern bool itobuttonoptions(uint8_t ui8_options, std::string& c_outputText);
extern bool itolinesuppression(uint8_t ui8_options, std::string& c_outputText);
extern bool itofontstyle(uint8_t ui8_options, std::string& c_outputText);
extern bool itolineart(int i_lineart, std::string& c_outputText);
extern bool itomasktype(uint8_t ui8_index, std::string& c_outputText);
extern bool itocolourdepth(uint8_t ui8_options, std::string& c_outputText);
extern bool itomacrocommand(uint8_t ui8_command, std::string& c_outputText);


extern int colourtoi (const char* text_colour);
extern int masktypetoi (const char* masktype);
extern int colourdepthtoi (const char* text_colourdepth);
extern signed int fonttypetoi (const char* text_fonttype, unsigned int aui_revision);
extern signed int booltoi (const char *text_bool);
extern signed int fontsizetoi (const char *text_fontsize);
extern signed int formattoi (const char *text_format);
extern signed int horizontaljustificationtoi (const char *text_horiz);
extern signed int verticaljustificationtoi (const char *text_vert, unsigned int aui_revision);
extern signed int stringoptionstoi (const char *text_options, unsigned int aui_revision);
extern signed int inputnumberoptionstoi (const char *text_options, unsigned int aui_revision);
extern signed int numberoptionstoi (const char *text_options, unsigned int aui_revision);
extern signed int picturegraphicoptionstoi (const char *text_options);
extern signed int picturegraphicrletoi (const char *text_options);
extern signed int meteroptionstoi (const char *text_options);
extern signed int linearbargraphoptionstoi (const char *text_options);
extern signed int archedbargraphoptionstoi (const char *text_options);
extern signed int prioritytoi (const char *text_priority);
extern signed int acousticsignaltoi (const char *text_acousticsignal);
extern signed int fontstyletoi (const char *text_fontstyle);
extern signed int linedirectiontoi (const char *text_linedirection);
extern signed int linearttoi (const char *text_lineart);
extern signed int linesuppressiontoi (const char *text_linesuppression);
extern signed int ellipsetypetoi (const char *text_ellipsetype);
extern signed int polygontypetoi (const char *text_polygontype);
extern signed int validationtypetoi (const char *text_validationtype);
extern signed int filltypetoi (const char *text_filltype);
extern signed int eventtoi (const char *text_eventName);
extern signed int auxfunctiontypetoi(const char *text_auxFunctionType);
extern signed int aux2functiontypetoi(const char *text_auxFunctionType);
extern signed int auxfunction2optionstoi(const char *text_auxFunctionType);
extern signed int auxinput2optionstoi(const char *text_auxFunctionType);
extern signed int auxcondesignobjptrtypetoi(const char *text_auxConDesignObjPtrType);
extern signed int keygroupoptionstoi (const char *text_options);
extern signed int gcoptionstoi (const char *text_options);
extern signed int inputobjectoptiontoi (const char *text_inputobjectoptions, unsigned int aui_revision);
extern signed int buttonoptiontoi (const char *text_buttonoptions, unsigned int aui_revision);

#endif // #endif VT2ISGLOBALS_H
