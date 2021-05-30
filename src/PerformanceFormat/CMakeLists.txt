set(SOURCE_FILES
	${SOURCE_FILES}

	${CMAKE_CURRENT_SOURCE_DIR}/Format0.2/PerformanceFormatV0_2Reader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format0.2/PerformanceFormatV0_2Reader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format0.2/ScaleSettingsReader.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/Format0.3/PerformanceFormatV0_3Reader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format0.3/PerformanceFormatV0_3Reader.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/Format1.0/AccompanimentSettingsReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format1.0/AccompanimentSettingsV0_0Reader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format1.0/AccompanimentSettingsV0_0Reader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format1.0/FXReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format1.0/KeyboardSettingsDataReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format1.0/KeyboardSettingsDataReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format1.0/KeyboardSettingsReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format1.0/KeyboardSettingsV0_0Reader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format1.0/KeyboardSettingsV0_0Reader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format1.0/PadSettingsReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format1.0/PerformanceFormat1_0VReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format1.0/PerformanceFormat1_0VReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format1.0/PerformanceWriterV1_0Writer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format1.0/PerformanceWriterV1_0Writer.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format1.0/TrackSettingsReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format1.0/TrackSettingsReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format1.0/UnknownAdditionalReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format1.0/UnknownAdditionalReader.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/Format2.0/AccompanimentSettingsV1_0Reader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format2.0/AccompanimentSettingsV1_0Reader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format2.0/InsertFXReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format2.0/KeyboardSettingsV1_0Reader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format2.0/KeyboardSettingsV1_0Reader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format2.0/PerformanceFormat2_0VReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format2.0/PerformanceFormat2_0VReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format2.0/TrackSettingsV2Reader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format2.0/UnknownAdditionalReaderV2.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format2.0/UnknownAdditionalReaderV2.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/Format2.1/PerformanceFormat2_1VReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format2.1/PerformanceFormat2_1VReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format2.1/UnknownAdditionalReaderV2_1.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Format2.1/UnknownAdditionalReaderV2_1.hpp

	${CMAKE_CURRENT_SOURCE_DIR}/MasterFXReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/PerformanceFormatWriter.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/PerformanceReaderZeroChunkReader.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/PerformanceWriterFactory.hpp

	PARENT_SCOPE)