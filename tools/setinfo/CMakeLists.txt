set(SOURCE_FILES
	${SOURCE_FILES}

	FormattedOutputter.hpp
	HumanReadableOutputter.hpp
	main.cpp
	PerformanceV1Outputter.cpp
	PerformanceV1Outputter.hpp
	StyleOutputter.cpp
	StyleOutputter.hpp
	XMLOutputter.hpp
	SoundOutputter.cpp
	SoundOutputter.hpp MultiSamplesOutputter.hpp MultiSamplesOutputter.cpp SampleOutputter.hpp SampleOutputter.cpp PerformanceOutputter.hpp PerformanceOutputter.cpp PerformanceV0Outputter.hpp PerformanceV2Outputter.hpp PerformanceV0Outputter.cpp PerformanceV2Outputter.cpp)

add_executable(setinfo ${SOURCE_FILES})
target_include_directories(setinfo PRIVATE "../../include")
target_link_libraries(setinfo LINK_PUBLIC Std++ Std++Static libkorg)