#ifndef INCLUDED_QA_EVENTSTREAM_SOURCE_H
#define INCLUDED_QA_EVENTSTREAM_SOURCE_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCase.h>

class qa_es_source : public CppUnit::TestCase {

  CPPUNIT_TEST_SUITE (qa_es_source);
  CPPUNIT_TEST (t1);
  CPPUNIT_TEST (t2);
  CPPUNIT_TEST_SUITE_END ();

 private:
  void t1 ();
  void t2 ();
};


#endif 

