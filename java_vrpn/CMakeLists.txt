# 2009-2010 Ryan Pavlik <rpavlik@iastate.edu>
# http://academic.cleardefinition.com
# Iowa State University HCI Graduate Program/VRAC

# XXX TODO Install these bindings, don't just build them!

if(JAVA_FOUND AND JNI_FOUND AND JAVAH_EXECUTABLE AND VRPN_BUILD_JAVA)

	# Set up file lists
	set(JAVA_CLASSES
		AnalogOutputRemote
		AnalogRemote
		AnalogRemoteListener
		AuxiliaryLoggerRemote
		ButtonRemote
		ButtonRemoteListener
		ForceDeviceRemote
		ForceDeviceRemoteListener
		FunctionGeneratorRemote
		PoserRemote
		TextReceiver
		TextSender
		TrackerRemote
		TrackerRemoteListener
		VRPN
		VRPNDevice)
	set(JAVAC_OUTPUT)
	set(JAVAC_INPUT)
	set(JAVA_JNI_HEADERS)
	foreach(class ${JAVA_CLASSES})
		list(APPEND JAVAC_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/vrpn/${class}.class")
		list(APPEND JAVAC_INPUT "${CMAKE_CURRENT_SOURCE_DIR}/vrpn/${class}.java")
		list(APPEND JAVA_JNI_HEADERS "${CMAKE_CURRENT_BINARY_DIR}/vrpn_${class}.h")
	endforeach()

	# *.java -> *.class
	add_custom_command(OUTPUT ${JAVAC_OUTPUT}
		DEPENDS ${JAVAC_INPUT}
		COMMAND "${Java_JAVAC_EXECUTABLE}" -d "${CMAKE_CURRENT_BINARY_DIR}" ${JAVAC_INPUT}
		COMMENT "Compiling Java sources to class files...")

	# *.class -> vrpn.jar
	add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/vrpn.jar"
		DEPENDS ${JAVAC_OUTPUT}
		COMMAND ${CMAKE_COMMAND}
		ARGS -E chdir "${CMAKE_CURRENT_BINARY_DIR}"
		${Java_JAR_EXECUTABLE} -cvf "${CMAKE_CURRENT_BINARY_DIR}/vrpn.jar" "${CMAKE_CURRENT_BINARY_DIR}/vrpn"
		COMMENT "Generating JAR file...")

	# *.class -> vrpn_*.h
	foreach(in ${JAVA_CLASSES})
		add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/vrpn_${in}.h"
			DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/vrpn/${in}.class"
			COMMAND "${JAVAH_EXECUTABLE}" -d "${CMAKE_CURRENT_BINARY_DIR}" -jni -classpath "${CMAKE_CURRENT_BINARY_DIR}" vrpn.${in}
			COMMENT "Generating JNI header file for vrpn.${in}")
	endforeach()

	# target for vrpn.jar
	add_custom_target(java_vrpn_jar
		DEPENDS
		"${CMAKE_CURRENT_BINARY_DIR}/vrpn.jar")

	# java_vrpn library
	set(LIB_SOURCES
		java_vrpn.cpp
		vrpn_AnalogOutputRemote.cpp
		vrpn_AnalogRemote.cpp
		vrpn_AuxiliaryLoggerRemote.cpp
		vrpn_ButtonRemote.cpp
		vrpn_ForceDeviceRemote.cpp
		vrpn_PoserRemote.cpp
		vrpn_TextReceiver.cpp
		vrpn_TextSender.cpp
		vrpn_TrackerRemote.cpp
		vrpn_VRPNDevice.cpp
		vrpn_FunctionGeneratorRemote.cpp)

	include_directories("${CMAKE_CURRENT_BINARY_DIR}" ${JNI_INCLUDE_DIRS})
	add_library(java_vrpn SHARED ${LIB_SOURCES} ${JAVA_JNI_HEADERS})
	target_link_libraries(java_vrpn vrpnserver ${JNI_LIBRARIES})
	add_dependencies(java_vrpn java_vrpn_jar)

	install(TARGETS java_vrpn
		ARCHIVE DESTINATION lib COMPONENT java
		LIBRARY DESTINATION lib COMPONENT java
		RUNTIME DESTINATION lib COMPONENT java)

	install(FILES "${CMAKE_CURRENT_BINARY_DIR}/vrpn.jar"
		DESTINATION share/java
		COMPONENT java
		RENAME vrpn-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.jar)

endif()
