# This script signs the targets for the package
foreach (target IN LISTS ${HDF4_INSTALL_TARGETS})
    if (WIN32)
        # Sign the targets
        execute_process(COMMAND ENV{SIGNTOOLDIR}/signtool
          sign /v /debug /fd SHA256 /tr http://timestamp.acs.microsoft.com /td SHA256
          /dlib "Microsoft.Trusted.Signing.Client\bin\x64\Azure.CodeSigning.Dlib.dll" /dmdf ${CMAKE_CURRENT_SOURCE_DIR}\credentials.json
          $<TARGET_FILE:${target}>
          WORKING_DIRECTORY ${CPACK_TEMPORARY_INSTALL_DIRECTORY}/packages
        )
        message(STATUS "Signing the target ${target}")
    elseif (APPLE)
        # Sign the targets
        execute_process(COMMAND codesign
          --force --timestamp --options runtime --entitlements ${CMAKE_CURRENT_SOURCE_DIR}/config/cmake/distribution.entitlements 
          --verbose=4 --strict --sign "ENV{SIGNER}"
          $<TARGET_FILE:${target}>
          WORKING_DIRECTORY ${CPACK_TEMPORARY_INSTALL_DIRECTORY}/packages
        )
        message(STATUS "Signing the target ${target}")
    endif ()
endforeach ()