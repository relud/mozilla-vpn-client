# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

find_package(Qt6 REQUIRED COMPONENTS Core Quick QuickTest)

qt_add_executable(lottie_qmltest)

target_link_libraries(lottie_qmltest PRIVATE
    lottie
    Qt6::Quick
    Qt6::QuickTest
)

target_sources(lottie_qmltest PRIVATE
    helper.h
    main.cpp
    qml.qrc
)

add_definitions(-DQUICK_TEST_SOURCE_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\")

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    list(APPEND QML_TEST_ARGS -platform offscreen)
endif()
add_test(NAME lottie_qmltest COMMAND lottie_qmltest ${QML_TEST_ARGS})
