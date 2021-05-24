set(SOURCE_FILES
	${SOURCE_FILES}

	${CMAKE_CURRENT_SOURCE_DIR}/Format0.0/SoundFormat0_0Reader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format0.0/SoundFormat0_0Reader.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/Format1.0/SoundFormat1_0Reader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format1.0/SoundFormat1_0Reader.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/Format2.0/SoundFormat2_0Reader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format2.0/SoundFormat2_0Reader.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/Format3.0/SoundFormat3_0Reader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format3.0/SoundFormat3_0Reader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format3.0/SoundFormat3_0Writer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format3.0/SoundFormat3_0Writer.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/LegacyFormat0.0/LegacySoundFormat0_0Reader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/LegacyFormat0.0/LegacySoundFormat0_0Reader.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/LegacyFormat0.3/LegacySoundFormat0_3Reader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/LegacyFormat0.3/LegacySoundFormat0_3Reader.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/SoundFormatWriter.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/SoundReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/SoundWriterFactory.hpp

	PARENT_SCOPE)