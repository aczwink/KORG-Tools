set(SOURCE_FILES
        ${SOURCE_FILES}

        BankCollectionView.hpp
        main.cpp
        SetWindow.cpp
        SetWindow.hpp
        SetController.hpp
        BanksController.hpp BankObjectsController.hpp BankObjectsView.hpp)

add_executable(setmanager ${SOURCE_FILES})
target_include_directories(setmanager PRIVATE "../../include")
target_link_libraries(setmanager LINK_PUBLIC Std++ Std++Static libkorg)