#ifndef INCLUDED_QA_EVENTSTREAM_TRIGGER_H
#define INCLUDED_QA_EVENTSTREAM_TRIGGER_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCase.h>

class qa_es_trigger : public CppUnit::TestCase {

  CPPUNIT_TEST_SUITE (qa_es_trigger);
  CPPUNIT_TEST (t1);
  CPPUNIT_TEST_SUITE_END ();

 private:
  void t1 ();
};


#endif 

