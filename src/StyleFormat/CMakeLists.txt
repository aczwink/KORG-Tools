set(SOURCE_FILES
	${SOURCE_FILES}

	${CMAKE_CURRENT_SOURCE_DIR}/Format0.0/KORG_MIDI_EventDecoder.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format0.0/KORG_MIDI_EventDecoder.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format0.0/MIDITrackListReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format0.0/MIDITrackListReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format0.0/StyleElementReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format0.0/StyleElementReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format0.0/StyleFormatV0_0Reader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format0.0/StyleFormatV0_0Reader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format0.0/StyleFormat0_0V5_0Writer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format0.0/StyleFormat0_0V5_0Writer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format0.0/StyleInfoDataReader.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/StyleFormatReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/StyleFormatWriter.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/StyleReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/StyleWriterFactory.hpp

	PARENT_SCOPE)