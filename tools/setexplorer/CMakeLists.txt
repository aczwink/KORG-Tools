set(SOURCE_FILES
        ${SOURCE_FILES}

        UI/BankCollectionView.hpp
        main.cpp
        UI/SetWindow.cpp
        UI/SetWindow.hpp
        SetController.hpp
        UI/BanksController.hpp UI/BankObjectsController.hpp UI/BankObjectsView.hpp UI/PlayBackView.hpp UI/PlayBackView.cpp PlayBack/loading/SampleLoader.hpp PlayBack/loading/SampleLoader.cpp PlayBack/loading/StyleMIDILoader.hpp PlayBack/loading/StyleMIDILoader.cpp PlayBack/PlayBackSet.hpp PlayBack/audio/PlayBackFactory.hpp PlayBack/PlaybackManager.hpp PlayBack/audio/SamplesManager.hpp PlayBack/audio/SamplesManager.cpp PlayBack/players/SingleSamplePlayer.hpp UI/KeyboardView.hpp UI/KeyboardView.cpp PlayBack/players/KeyboardRealtimePlayer.hpp PlayBack/players/VirtualInstrument.hpp PlayBack/players/VirtualInstrument.cpp UI/TrackSettingsView.hpp UI/TrackSettingsView.cpp PlayBack/players/StylePlayer.hpp PlayBack/players/StylePlayer.cpp UI/BankCollectionController.hpp UI/GlobalBankObjectSelectionController.hpp)

add_executable(setexplorer ${SOURCE_FILES})
target_include_directories(setexplorer PRIVATE "../../include")
target_link_libraries(setexplorer LINK_PUBLIC Std++ Std++Static libkorg)