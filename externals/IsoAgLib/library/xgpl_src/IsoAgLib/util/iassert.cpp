/*
  iassert.cpp: assertions

  (C) Copyright 2009 - 2014 by OSB AG and developing partners

  See the repository-log for details on the authors and file-history.
  (Repository information can be found at <http://isoaglib.com/download>)

  Use, modification and distribution are subject to the GNU General
  Public License with exceptions for ISOAgLib. (See accompanying
  file LICENSE.txt or copy at <http://isoaglib.com/download/license>)
*/
#include <IsoAgLib/isoaglib_config.h>

#if defined(NDEBUG)
// nothing to do, assertions pre-processed out completely.
#elif defined(__GNUC__) || defined(WIN32)

#include "iassert.h"

static bool alreadyInAssert = false;

bool isoaglib_assert_callStart()
{
  // This is just a function to set a breakpoint to catch isoaglib_assert calls

  bool retval = alreadyInAssert;
  alreadyInAssert = true;

  return retval;
}

void isoaglib_assert_callFinish()
{
  alreadyInAssert = false;
}

#else

#include "iassert.h"
#include <cstdlib>

#if defined(__TSW_CPP__)
void assert_fail()
{
  STL_NAMESPACE::abort();
}
#else
void assert_fail(char const *pcs_file, int i_line, char const *pcs_expression)
{
  abort();
}
#endif

#endif
