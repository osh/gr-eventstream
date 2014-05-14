
# Set up CFLAGS
include_directories(
    ${EVENTSTREAM_INCLUDE_DIRS}
    ${GNURADIO_PMT_INCLUDE_DIRS}
    ${GNURADIO_RUNTIME_INCLUDE_DIRS}
    ${GNURADIO_BLOCKS_INCLUDE_DIRS}
    ${PYTHON_INCLUDE_DIRS}
)

# Set up LDFLAGS
list(APPEND eventstream_libs
    ${CPPUNIT_LIBRARIES}
    ${PMT_LIBRARIES}
    ${Boost_LIBRARIES}
    ${GNURADIO_RUNTIME_LIBRARIES}
    ${GNURADIO_BLOCKS_LIBRARIES}
    ${GNURADIO_PMT_LIBRARIES}
    ${PYTHON_LIBRARIES}
)

list(APPEND eventstream_sources
    es_common.cc
    es_eh_pair.cc
    es_event_loop_thread.cc
    es_source_thread.cc
    es_handler.cc
    es_handler_flowgraph.cc
    es_handler_print.cc
    es_handler_file.cc
    es_handler_pdu.cc
    es_queue.cc
    es_sink.cc
    es_source.cc
    es_vector_source.cc
    es_vector_sink.cc
    es_gen_vector.cc
    es_handler_insert_vector.cc
    es_trigger.cc
    es_trigger_edge_f.cc
    es_trigger_sample_timer.cc
    es_pyhandler_def.cc
    es_patterned_interleaver.cc
    es_exceptions.cc
    es_event_acceptor.cc
)

list(APPEND test_eventstream_sources
    qa_es.cc
    qa_es_common.cc
    qa_es_source.cc
    qa_es_sink.cc
    qa_es_trigger.cc
)

add_library(eventstream SHARED ${eventstream_sources})
target_link_libraries(eventstream ${eventstream_libs})

add_executable(test_eventstream ${test_eventstream_sources} test_all.cc)
target_link_libraries(test_eventstream eventstream ${CPPUNIT_LIBRARIES})



########################################################################
# Install built library files
########################################################################
INSTALL(TARGETS eventstream
    LIBRARY DESTINATION lib${LIB_SUFFIX} # .so/.dylib file
    ARCHIVE DESTINATION lib${LIB_SUFFIX} # .lib file
    RUNTIME DESTINATION bin              # .dll file
)


########################################################################
# Build and register unit test
########################################################################
FIND_PACKAGE(Boost COMPONENTS unit_test_framework)

INCLUDE(GrTest)
SET(GR_TEST_TARGET_DEPS eventstream)
#turn each test cpp file into an executable with an int main() function
ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK -DBOOST_TEST_MAIN)

