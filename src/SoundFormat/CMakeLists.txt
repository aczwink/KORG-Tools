set(SOURCE_FILES
	${SOURCE_FILES}

	${CMAKE_CURRENT_SOURCE_DIR}/Format2.0/SoundFormat2_0Reader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format2.0/SoundFormat2_0Reader.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/Format3.0/SoundFormat3_0Reader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format3.0/SoundFormat3_0Reader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format3.0/SoundFormat3_0Writer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format3.0/SoundFormat3_0Writer.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/SoundFormatWriter.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/SoundReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/SoundWriterFactory.hpp

	PARENT_SCOPE)