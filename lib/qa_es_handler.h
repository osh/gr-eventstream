#ifndef INCLUDED_QA_EVENTSTREAM_HANDLER_H
#define INCLUDED_QA_EVENTSTREAM_HANDLER_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCase.h>

class qa_es_handler : public CppUnit::TestCase {

  CPPUNIT_TEST_SUITE (qa_es_handler);
  CPPUNIT_TEST (t1);
  CPPUNIT_TEST_SUITE_END ();

 private:
  void t1 ();
};


#endif 

