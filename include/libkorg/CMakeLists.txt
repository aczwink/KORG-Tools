add_subdirectory(BankFormat)
add_subdirectory(ChunkFormat)
add_subdirectory(Converters)
add_subdirectory(DataViews)
add_subdirectory(MultiSamples)
add_subdirectory(Performance)
add_subdirectory(Sample)
add_subdirectory(Sound)
add_subdirectory(Style)

set(SOURCE_FILES
	${SOURCE_FILES}

	${CMAKE_CURRENT_SOURCE_DIR}/FullStyle.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Model.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/UnknownChunk.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Package.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Pitch.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/ProgramChangeSequence.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Set.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/Text.hpp

	PARENT_SCOPE)