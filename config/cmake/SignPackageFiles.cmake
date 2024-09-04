# This script signs the targets for the package
message(STATUS "Signing script for ${CPACK_EXPORT_LIBRARIES} in ${CPACK_TEMPORARY_INSTALL_DIRECTORY}/${CPACK_PACKAGE_INSTALL_DIRECTORY}")
file(GLOB target_list LIST_DIRECTORIES false "${CPACK_TEMPORARY_INSTALL_DIRECTORY}/${CPACK_PACKAGE_INSTALL_DIRECTORY}/lib/*.*" "${CPACK_TEMPORARY_INSTALL_DIRECTORY}/${CPACK_PACKAGE_INSTALL_DIRECTORY}/bin/*.*")
foreach (targetfile IN LISTS target_list)
    if (WIN32)
        # Sign the targets
        execute_process(COMMAND $ENV{SIGNTOOLDIR}/signtool
          sign /v /debug /fd SHA256 /tr http://timestamp.acs.microsoft.com /td SHA256
          /dlib "Microsoft.Trusted.Signing.Client/bin/x64/Azure.CodeSigning.Dlib.dll" /dmdf ${CMAKE_CURRENT_SOURCE_DIR}/credentials.json
          ${targetfile}
        )
        execute_process(
          COMMAND ${CMAKE_COMMAND} -E echo "Signing the target ${targetfile}"
        )
    elseif (APPLE)
        # Sign the targets
        execute_process(COMMAND codesign
          --force --timestamp --options runtime --entitlements ${CMAKE_CURRENT_SOURCE_DIR}/config/cmake/distribution.entitlements 
          --verbose=4 --strict --sign "$ENV{SIGNER}"
          ${targetfile}
        )
        execute_process(
          COMMAND ${CMAKE_COMMAND} -E echo "Signing the target ${targetfile}"
        )
    else ()
        execute_process(
          COMMAND ${CMAKE_COMMAND} -E echo "Signing the target ${targetfile}"
        )
    endif ()
endforeach ()
