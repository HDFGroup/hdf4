# This script signs the targets for the package
message(STATUS "Signing script for ${CPACK_EXPORT_LIBRARIES} and ${CPACK_EXPORT_UTILS}")
foreach (target "${CPACK_EXPORT_LIBRARIES}")
    if (WIN32)
        # Sign the targets
        execute_process(COMMAND $ENV{SIGNTOOLDIR}/signtool
          sign /v /debug /fd SHA256 /tr http://timestamp.acs.microsoft.com /td SHA256
          /dlib "Microsoft.Trusted.Signing.Client/bin/x64/Azure.CodeSigning.Dlib.dll" /dmdf ${CMAKE_CURRENT_SOURCE_DIR}/credentials.json
          $<TARGET_FILE:${target}>
          WORKING_DIRECTORY ${CPACK_TEMPORARY_INSTALL_DIRECTORY}/ALL_COMPONENTS_IN_ONE/${CPACK_PACKAGE_INSTALL_DIRECTORY}/bin
        )
        message(STATUS "Signing the target $<TARGET_FILE:${target}>")
    elseif (APPLE)
        # Sign the targets
        execute_process(COMMAND codesign
          --force --timestamp --options runtime --entitlements ${CMAKE_CURRENT_SOURCE_DIR}/config/cmake/distribution.entitlements 
          --verbose=4 --strict --sign "$ENV{SIGNER}"
          $<TARGET_FILE:${target}>
          WORKING_DIRECTORY ${CPACK_TEMPORARY_INSTALL_DIRECTORY}/ALL_COMPONENTS_IN_ONE/${CPACK_PACKAGE_INSTALL_DIRECTORY}/lib
        )
        message(STATUS "Signing the target $<TARGET_FILE:${target}>")
    else ()
        set (target_path ${CPACK_TEMPORARY_INSTALL_DIRECTORY}/ALL_COMPONENTS_IN_ONE/${CPACK_PACKAGE_INSTALL_DIRECTORY}/lib)
        message(STATUS "Signing the target $<TARGET_FILE:${target}> using ${target_path}")
    endif ()
endforeach ()

foreach (target "${CPACK_EXPORT_UTILS}")
    if (WIN32)
        # Sign the targets
        execute_process(COMMAND $ENV{SIGNTOOLDIR}/signtool
          sign /v /debug /fd SHA256 /tr http://timestamp.acs.microsoft.com /td SHA256
          /dlib "Microsoft.Trusted.Signing.Client/bin/x64/Azure.CodeSigning.Dlib.dll" /dmdf ${CMAKE_CURRENT_SOURCE_DIR}/credentials.json
          $<TARGET_FILE:${target}>
          WORKING_DIRECTORY ${CPACK_TEMPORARY_INSTALL_DIRECTORY}/ALL_COMPONENTS_IN_ONE/${CPACK_PACKAGE_INSTALL_DIRECTORY}/bin
        )
        message(STATUS "Signing the target $<TARGET_FILE:${target}>")
    elseif (APPLE)
        # Sign the targets
        execute_process(COMMAND codesign
          --force --timestamp --options runtime --entitlements ${CMAKE_CURRENT_SOURCE_DIR}/config/cmake/distribution.entitlements 
          --verbose=4 --strict --sign "$ENV{SIGNER}"
          $<TARGET_FILE:${target}>
          WORKING_DIRECTORY ${CPACK_TEMPORARY_INSTALL_DIRECTORY}/ALL_COMPONENTS_IN_ONE/${CPACK_PACKAGE_INSTALL_DIRECTORY}/bin
        )
        message(STATUS "Signing the target $<TARGET_FILE:${target}>")
    else ()
        set (target_path ${CPACK_TEMPORARY_INSTALL_DIRECTORY}/ALL_COMPONENTS_IN_ONE/${CPACK_PACKAGE_INSTALL_DIRECTORY}/bin)
        message(DEBUG "Signing the target $<TARGET_FILE:${target}> using ${target_path}")
    endif ()
endforeach ()
