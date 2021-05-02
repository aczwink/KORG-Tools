set(SOURCE_FILES
	${SOURCE_FILES}

	FormattedOutputter.hpp
	HumanReadableOutputter.hpp
	main.cpp
	PerformanceOutputter.cpp
	PerformanceOutputter.hpp
	StyleOutputter.cpp
	StyleOutputter.hpp
	XMLOutputter.hpp
	SoundOutputter.cpp
	SoundOutputter.hpp MultiSamplesOutputter.hpp MultiSamplesOutputter.cpp SampleOutputter.hpp SampleOutputter.cpp)

add_executable(setinfo ${SOURCE_FILES})
target_include_directories(setinfo PRIVATE "../../include")
target_link_libraries(setinfo LINK_PUBLIC Std++ Std++Static libkorg)