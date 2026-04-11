# GameSpy - GameSpy интеграция (Peer, Chat, Pinger, QueryReporting, cengine)
# Согласно GameSpy.dsp

cmake_minimum_required(VERSION 2.4)

project(GameSpy)

# Исходные файлы из GameSpy.dsp
set(GAMESPY_SOURCES
    # Core
    darray.c
    hashtable.c
    md5c.c
    nonport.c

    # cengine
    cengine/gserver.c
    cengine/gserverlist.c
    cengine/gutil.c

    # chat
    chat/chatCallbacks.c
    chat/chatChannel.c
    chat/chatCrypt.c
    chat/chatHandlers.c
    chat/chatMain.c
    chat/chatSocket.c

    # peer
    peer/peerCallbacks.c
    peer/peerCEngine.c
    peer/peerGlobalCallbacks.c
    peer/peerKeys.c
    peer/peerMain.c
    peer/peerMangle.c
    peer/peerOperations.c
    peer/peerPing.c
    peer/peerPlayers.c
    peer/peerRooms.c

    # pinger
    pinger/pingerMain.c

    # queryreporting
    queryreporting/gqueryreporting.c
)

# Заголовки
set(GAMESPY_HEADERS
    darray.h
    hashtable.h
    md5.h
    nonport.h
    cengine/darray.h
    cengine/goaceng.h
    cengine/gserver.h
    cengine/gutil.h
    cengine/hashtable.h
    cengine/nonport.h
    chat/chat.h
    chat/chatCallbacks.h
    chat/chatChannel.h
    chat/chatCrypt.h
    chat/chatHandlers.h
    chat/chatMain.h
    chat/chatSocket.h
    peer/peer.h
    peer/peerCallbacks.h
    peer/peerCEngine.h
    peer/peerGlobalCallbacks.h
    peer/peerKeys.h
    peer/peerMain.h
    peer/peerMangle.h
    peer/peerOperations.h
    peer/peerPing.h
    peer/peerPlayers.h
    peer/peerRooms.h
    pinger/pinger.h
    queryreporting/gqueryreporting.h
)

add_library(GameSpy STATIC ${GAMESPY_SOURCES} ${GAMESPY_HEADERS})

# Include директории - нужны для peer, chat и т.д.
target_include_directories(GameSpy PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/cengine
    ${CMAKE_CURRENT_SOURCE_DIR}/chat
    ${CMAKE_CURRENT_SOURCE_DIR}/peer
    ${CMAKE_CURRENT_SOURCE_DIR}/pinger
    ${CMAKE_CURRENT_SOURCE_DIR}/queryreporting
)

# Определения для Windows
target_compile_definitions(GameSpy PRIVATE
    WIN32
    _WINDOWS
    _LIB
    _CRT_SECURE_NO_WARNINGS
    _CRT_NONSTDC_NO_DEPRECATE
)
