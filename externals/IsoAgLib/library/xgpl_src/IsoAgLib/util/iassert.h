/*
  iassert.h: assertions

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#ifndef IASSERT_H
#define IASSERT_H

#include "compiler_adaptation.h"


// NOTE: isoaglib_assert(..) in HEADER-files is currently DISABLED
//       and would need to be implemented!!!
#define isoaglib_header_assert(e)


#if defined(NDEBUG)
#  define isoaglib_assert(e)
#elif defined(__GNUC__) || defined(WIN32)
# include <assert.h>

// Don't allow multiple simultaneous assert msgs!
// It causes the application to crash (stack overflow) when too many
// happen in a row, and then you can't even see the reason for the assert
// This happens everytime when sending at high rates, 
// because the send buffer overflows before the assert msg can be displayed on the screen.

// Put a breakpoint on one of these functions to catch isoaglib_assert calls
extern bool isoaglib_assert_callStart();  // Before the assert MsgBox is displayed
extern void isoaglib_assert_callFinish(); // After the assert MsgBox is closed (unless user hits Abort)

# define isoaglib_assert(_Expression)           \
{                                               \
  const bool assert_result = (!!(_Expression)); \
  if( !(assert_result) )                        \
  {                                             \
    if( !isoaglib_assert_callStart() )          \
    {                                           \
      assert(_Expression);                      \
      isoaglib_assert_callFinish();             \
    }                                           \
    /* else: Already in assert - ignore */      \
  }                                             \
}

#elif defined(__TSW_CPP__)
/* strings are too expensive => simple assert_fail*/
void assert_fail();
# define isoaglib_assert(e) \
  ((e) ? static_cast< void >(0) : assert_fail())
#else /* support file, line and expression */
void assert_fail(char const *pcs_file, int i_line, char const *pcs_expression);
# define isoaglib_assert(e) \
  ((e) ? static_cast< void >(0) : assert_fail(__FILE__, __LINE__, #e))
#endif

#endif
