set(SOURCE_FILES
	${SOURCE_FILES}

	${CMAKE_CURRENT_SOURCE_DIR}/AccompanimentSettingsFormat/AccompanimentSettingsReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/AccompanimentSettingsFormat/AccompanimentSettingsReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/AccompanimentSettingsFormat/AccompanimentSettingsV0_0Reader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/AccompanimentSettingsFormat/AccompanimentSettingsV0_0Reader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/AccompanimentSettingsFormat/AccompanimentSettingsV1_0Reader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/AccompanimentSettingsFormat/AccompanimentSettingsV1_0Reader.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/Format0.3/PerformanceFormatV0_3Reader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format0.3/PerformanceFormatV0_3Reader.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/Format1.0/PerformanceFormat1_0VReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format1.0/PerformanceFormat1_0VReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format1.0/PerformanceWriterV1_0Writer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format1.0/PerformanceWriterV1_0Writer.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/Format2.0/PerformanceFormat2_0VReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format2.0/PerformanceFormat2_0VReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format2.0/UnknownAdditionalReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format2.0/UnknownAdditionalReader.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/Format2.1/PerformanceFormat2_1VReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format2.1/PerformanceFormat2_1VReader.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/KeyboardSettingsFormat/KeyboardSettingsReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/KeyboardSettingsFormat/KeyboardSettingsReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/KeyboardSettingsFormat/KeyboardSettingsV0_0Reader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/KeyboardSettingsFormat/KeyboardSettingsV0_0Reader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/KeyboardSettingsFormat/KeyboardSettingsV1_0Reader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/KeyboardSettingsFormat/KeyboardSettingsV1_0Reader.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/InsertFXReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/MasterFXReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/PadSettingsReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/PerformanceFormatReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PerformanceFormatReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/PerformanceFormatWriter.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/PerformanceReaderZeroChunkReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/PerformanceWriterFactory.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/TrackPropertiesReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/TrackPropertiesReader.hpp

	PARENT_SCOPE)