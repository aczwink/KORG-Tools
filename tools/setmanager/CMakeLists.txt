set(SOURCE_FILES
        ${SOURCE_FILES}

        UI/BankCollectionView.hpp
        main.cpp
        UI/SetWindow.cpp
        UI/SetWindow.hpp
        SetController.hpp
        UI/BanksController.hpp UI/BankObjectsController.hpp UI/BankObjectsView.hpp UI/PlayBackView.hpp UI/PlayBackView.cpp PlayBack/SoftwareSimulatedKORG_MIDI_Synthesizer.hpp PlayBack/SoftwareSimulatedKORG_MIDI_Synthesizer.cpp PlayBack/SampleLoader.hpp PlayBack/SampleLoader.cpp PlayBack/StyleMIDILoader.hpp PlayBack/StyleMIDILoader.cpp PlayBack/PlayBackDrumKit.hpp PlayBack/PlayBackDrumKit.cpp PlayBack/PlayBackSet.hpp PlayBack/PlayBackFactory.hpp)

add_executable(setmanager ${SOURCE_FILES})
target_include_directories(setmanager PRIVATE "../../include")
target_link_libraries(setmanager LINK_PUBLIC Std++ Std++Static libkorg)