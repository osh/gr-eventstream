#ifndef INCLUDED_QA_EVENTSTREAM_SINK_H
#define INCLUDED_QA_EVENTSTREAM_SINK_H

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestCase.h>

class qa_es_sink : public CppUnit::TestCase {

  CPPUNIT_TEST_SUITE (qa_es_sink);
  CPPUNIT_TEST (t1);
  CPPUNIT_TEST_SUITE_END ();

 private:
  void t1 ();
};


#endif 

