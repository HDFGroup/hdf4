# runTest.cmake executes a command and captures the output in a file. File is then compared
# against a reference file. Exit status of command can also be compared.
cmake_policy(SET CMP0007 NEW)

# arguments checking
if (NOT TEST_PROGRAM)
  message (FATAL_ERROR "Require TEST_PROGRAM to be defined")
endif (NOT TEST_PROGRAM)
#if (NOT TEST_ARGS)
#  message (STATUS "Require TEST_ARGS to be defined")
#endif (NOT TEST_ARGS)
if (NOT TEST_FOLDER)
  message ( FATAL_ERROR "Require TEST_FOLDER to be defined")
endif (NOT TEST_FOLDER)
if (NOT TEST_OUTPUT)
  message (FATAL_ERROR "Require TEST_OUTPUT to be defined")
endif (NOT TEST_OUTPUT)
#if (NOT TEST_EXPECT)
#  message (STATUS "Require TEST_EXPECT to be defined")
#endif (NOT TEST_EXPECT)
#if (NOT TEST_FILTER)
#  message (STATUS "Require TEST_FILTER to be defined")
#endif (NOT TEST_FILTER)
if (NOT TEST_SKIP_COMPARE AND NOT TEST_REFERENCE)
  message (FATAL_ERROR "Require TEST_REFERENCE to be defined")
endif (NOT TEST_SKIP_COMPARE AND NOT TEST_REFERENCE)

if (NOT TEST_ERRREF)
  set (ERROR_APPEND 1)
endif (NOT TEST_ERRREF)

message (STATUS "COMMAND: ${TEST_PROGRAM} ${TEST_ARGS}")

if (TEST_ENV_VAR)
  set (ENV{${TEST_ENV_VAR}} "${TEST_ENV_VALUE}") 
endif (TEST_ENV_VAR)

if (NOT TEST_INPUT)
  # run the test program, capture the stdout/stderr and the result var
  EXECUTE_PROCESS (
      COMMAND ${TEST_PROGRAM} ${TEST_ARGS}
      WORKING_DIRECTORY ${TEST_FOLDER}
      RESULT_VARIABLE TEST_RESULT
      OUTPUT_FILE ${TEST_OUTPUT}
      ERROR_FILE ${TEST_OUTPUT}.err
      OUTPUT_VARIABLE TEST_ERROR
      ERROR_VARIABLE TEST_ERROR
  )
else (NOT TEST_INPUT)
  # run the test program with stdin, capture the stdout/stderr and the result var
  EXECUTE_PROCESS (
      COMMAND ${TEST_PROGRAM} ${TEST_ARGS}
      WORKING_DIRECTORY ${TEST_FOLDER}
      RESULT_VARIABLE TEST_RESULT
      INPUT_FILE ${TEST_INPUT}
      OUTPUT_FILE ${TEST_OUTPUT}
      ERROR_FILE ${TEST_OUTPUT}.err
      OUTPUT_VARIABLE TEST_ERROR
      ERROR_VARIABLE TEST_ERROR
  )
endif (NOT TEST_INPUT)

message (STATUS "COMMAND Result: ${TEST_RESULT}")

if (ERROR_APPEND)
  FILE (READ ${TEST_FOLDER}/${TEST_OUTPUT}.err TEST_STREAM)
  FILE (APPEND ${TEST_FOLDER}/${TEST_OUTPUT} "${TEST_STREAM}") 
endif (ERROR_APPEND)

if (TEST_APPEND)
  FILE (APPEND ${TEST_FOLDER}/${TEST_OUTPUT} "${TEST_APPEND} ${TEST_RESULT}\n") 
endif (TEST_APPEND)

# if the return value is !=${TEST_EXPECT} bail out
if (NOT ${TEST_RESULT} STREQUAL ${TEST_EXPECT})
  message ( FATAL_ERROR "Failed: Test program ${TEST_PROGRAM} exited != ${TEST_EXPECT}.\n${TEST_ERROR}")
endif (NOT ${TEST_RESULT} STREQUAL ${TEST_EXPECT})

message (STATUS "COMMAND Error: ${TEST_ERROR}")

if (TEST_MASK)
  FILE (READ ${TEST_FOLDER}/${TEST_OUTPUT} TEST_STREAM)
  #STRING(REGEX REPLACE "Modified:[^\n]+\n" "Modified:  XXXX-XX-XX XX:XX:XX XXX\n" TEST_STREAM "${TEST_STREAM}") 
  STRING(REGEX REPLACE "Storage:[^\n]+\n" "Storage:   <details removed for portability>\n" TEST_STREAM "${TEST_STREAM}") 
  FILE (WRITE ${TEST_FOLDER}/${TEST_OUTPUT} "${TEST_STREAM}")
endif (TEST_MASK)

if (TEST_FILTER)
  FILE (READ ${TEST_FOLDER}/${TEST_OUTPUT} TEST_STREAM)
  STRING(REGEX REPLACE "${TEST_FILTER}" "" TEST_STREAM "${TEST_STREAM}") 
  FILE (WRITE ${TEST_FOLDER}/${TEST_OUTPUT} "${TEST_STREAM}")
endif (TEST_FILTER)

if (NOT TEST_SKIP_COMPARE)
  if (WIN32 AND NOT MINGW)
    FILE (READ ${TEST_FOLDER}/${TEST_REFERENCE} TEST_STREAM)
    FILE (WRITE ${TEST_FOLDER}/${TEST_REFERENCE} "${TEST_STREAM}")
  endif (WIN32 AND NOT MINGW)

  # now compare the output with the reference
  EXECUTE_PROCESS (
      COMMAND ${CMAKE_COMMAND} -E compare_files ${TEST_FOLDER}/${TEST_OUTPUT} ${TEST_FOLDER}/${TEST_REFERENCE}
      RESULT_VARIABLE TEST_RESULT
  )
  if (NOT ${TEST_RESULT} STREQUAL 0)
  set (TEST_RESULT 0)
  FILE (STRINGS ${TEST_FOLDER}/${TEST_OUTPUT} test_act)
  LIST (LENGTH test_act len_act)
  FILE (STRINGS ${TEST_FOLDER}/${TEST_REFERENCE} test_ref)
  LIST (LENGTH test_ref len_ref)
  if (NOT ${len_act} STREQUAL "0")
    MATH (EXPR _FP_LEN "${len_ref} - 1")
    foreach (line RANGE 0 ${_FP_LEN})
      LIST (GET test_act ${line} str_act)
      LIST (GET test_ref ${line} str_ref)
      if (NOT "${str_act}" STREQUAL "${str_ref}")
        if (NOT "${str_act}" STREQUAL "")
          set (TEST_RESULT 1)
          message ("line = ${line}\n***ACTUAL: ${str_act}\n****REFER: ${str_ref}\n")
         endif (NOT "${str_act}" STREQUAL "")
      endif (NOT "${str_act}" STREQUAL "${str_ref}")
    endforeach (line RANGE 0 ${_FP_LEN})
  endif (NOT ${len_act} STREQUAL "0")
  if (NOT ${len_act} STREQUAL ${len_ref})
    set (TEST_RESULT 1)
  endif (NOT ${len_act} STREQUAL ${len_ref})
  endif (NOT ${TEST_RESULT} STREQUAL 0)

  message (STATUS "COMPARE Result: ${TEST_RESULT}")

  # again, if return value is !=0 scream and shout
  if (NOT ${TEST_RESULT} STREQUAL 0)
    message (FATAL_ERROR "Failed: The output of ${TEST_OUTPUT} did not match ${TEST_REFERENCE}")
  endif (NOT ${TEST_RESULT} STREQUAL 0)
  
  if (TEST_ERRREF)
    if (WIN32 AND NOT MINGW)
      FILE (READ ${TEST_FOLDER}/${TEST_ERRREF} TEST_STREAM)
      FILE (WRITE ${TEST_FOLDER}/${TEST_ERRREF} "${TEST_STREAM}")
    endif (WIN32 AND NOT MINGW)

    # now compare the error output with the error reference
    EXECUTE_PROCESS (
        COMMAND ${CMAKE_COMMAND} -E compare_files ${TEST_FOLDER}/${TEST_OUTPUT}.err ${TEST_FOLDER}/${TEST_ERRREF}
        RESULT_VARIABLE TEST_RESULT
    )
    if (NOT ${TEST_RESULT} STREQUAL 0)
    set (TEST_RESULT 0)
    FILE (STRINGS ${TEST_FOLDER}/${TEST_OUTPUT}.err test_act)
    LIST (LENGTH test_act len_act)
    FILE (STRINGS ${TEST_FOLDER}/${TEST_ERRREF} test_ref)
    LIST (LENGTH test_ref len_ref)
    MATH (EXPR _FP_LEN "${len_ref} - 1")
    if (NOT ${len_act} STREQUAL "0")
      MATH (EXPR _FP_LEN "${len_ref} - 1")
      foreach (line RANGE 0 ${_FP_LEN})
        LIST (GET test_act ${line} str_act)
        LIST (GET test_ref ${line} str_ref)
        if (NOT "${str_act}" STREQUAL "${str_ref}")
          if (NOT "${str_act}" STREQUAL "")
            set (TEST_RESULT 1)
            message ("line = ${line}\n***ACTUAL: ${str_act}\n****REFER: ${str_ref}\n")
           endif (NOT "${str_act}" STREQUAL "")
        endif (NOT "${str_act}" STREQUAL "${str_ref}")
      endforeach (line RANGE 0 ${_FP_LEN})
    endif (NOT ${len_act} STREQUAL "0")
    if (NOT ${len_act} STREQUAL ${len_ref})
      set (TEST_RESULT 1)
    endif (NOT ${len_act} STREQUAL ${len_ref})
    endif (NOT ${TEST_RESULT} STREQUAL 0)

    message (STATUS "COMPARE Result: ${TEST_RESULT}")

    # again, if return value is !=0 scream and shout
    if (NOT ${TEST_RESULT} STREQUAL 0)
      message (FATAL_ERROR "Failed: The error output of ${TEST_OUTPUT}.err did not match ${TEST_ERRREF}")
    endif (NOT ${TEST_RESULT} STREQUAL 0)
  endif (TEST_ERRREF)
endif (NOT TEST_SKIP_COMPARE)

# everything went fine...
message ("Passed: The output of ${TEST_PROGRAM} matches ${TEST_REFERENCE}")
