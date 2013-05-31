#ifndef INCLUDED_QA_EVENTSTREAM_COMMON_h
#define INCLUDED_QA_EVENTSTREAM_COMMON_h

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCase.h>

class qa_es_common : public CppUnit::TestCase {

  CPPUNIT_TEST_SUITE (qa_es_common);
  CPPUNIT_TEST (t1);
  CPPUNIT_TEST_SUITE_END ();

 private:
  void t1 ();
};


#endif 

