cmake_minimum_required(VERSION 3.5)

project(TelegramUtility LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)




find_package(Td 1.8.40 REQUIRED PATHS "C:/Users/Admin/Desktop/fromTDLibBuild/td/tdlib")


add_executable(TelegramUtility					    main.cpp
					abstracttask.h		    abstracttask.cpp
					telegramclient.h	    telegramclient.cpp
					closeapplicationtask.h	    closeapplicationtask.cpp
					logintask.h		    logintask.cpp
					logouttask.h		    logouttask.cpp
					personaldata.h
					findchatidtask.h	    findchatidtask.cpp
					finduseridtask.h	    finduseridtask.cpp
					finduseridinchattask.h	    finduseridinchattask.cpp)


target_link_libraries(TelegramUtility PRIVATE Td::TdJson)
target_link_libraries(TelegramUtility PRIVATE Td::tdclient)


