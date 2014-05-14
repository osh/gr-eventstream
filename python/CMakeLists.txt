FIND_PACKAGE(SWIG)
FIND_PACKAGE(PythonLibs)
IF(NOT SWIG_FOUND OR NOT PYTHONLIBS_FOUND)
    RETURN()
ENDIF()
INCLUDE(GrSwig)
INCLUDE(GrPython)

GR_PYTHON_INSTALL(
    FILES
    es_trigger_keyboard.py
    es_trigger_timer.py
    #patterned_interleaver.py
    util.py
    DESTINATION ${GR_PYTHON_DIR}/es
    COMPONENT "es_py"
)
   
GR_ADD_TEST(qa_es ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/qa_es.py)
