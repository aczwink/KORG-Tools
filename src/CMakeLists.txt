add_subdirectory(BankFormat)
add_subdirectory(Converters)
add_subdirectory(DataViews)
add_subdirectory(Models)
add_subdirectory(MultiSamplesFormat)
add_subdirectory(Package)
add_subdirectory(PCMFormat)
add_subdirectory(PerformanceFormat)
add_subdirectory(Reading)
add_subdirectory(SoundFormat)
add_subdirectory(StyleFormat)

set(SOURCE_FILES
	${SOURCE_FILES}

	${CMAKE_CURRENT_SOURCE_DIR}/Package.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PerformanceObject.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Pitch.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Set.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SongBook.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SongBookEntry.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Text.cpp

	PARENT_SCOPE)