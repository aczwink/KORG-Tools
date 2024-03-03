set(SOURCE_FILES
        ${SOURCE_FILES}

        UI/BankCollectionView.hpp
        main.cpp
        UI/SetWindow.cpp
        UI/SetWindow.hpp
        SetController.hpp
        UI/BanksController.hpp UI/BankObjectsController.hpp UI/BankObjectsView.hpp UI/PlayBackView.hpp UI/PlayBackView.cpp PlayBack/loading/SampleLoader.hpp PlayBack/loading/SampleLoader.cpp PlayBack/_legacy/StyleMIDILoader.hpp PlayBack/_legacy/StyleMIDILoader.cpp PlayBack/PlayBackSet.hpp PlayBack/audio/PlayBackFactory.hpp PlayBack/PlaybackManager.hpp PlayBack/audio/SamplesManager.hpp PlayBack/audio/SamplesManager.cpp PlayBack/players/SingleSamplePlayer.hpp UI/KeyboardView.hpp UI/KeyboardView.cpp PlayBack/players/KeyboardRealtimePlayer.hpp PlayBack/players/VirtualInstrument.hpp PlayBack/players/VirtualInstrument.cpp UI/TrackView.hpp UI/TrackView.cpp)

add_executable(setmanager ${SOURCE_FILES})
target_include_directories(setmanager PRIVATE "../../include")
target_link_libraries(setmanager LINK_PUBLIC Std++ Std++Static libkorg)