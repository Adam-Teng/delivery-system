# CMakeList.txt: delivery 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("delivery")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(GTest CONFIG REQUIRED)
find_package(Qt5Sql CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)

# 将源代码添加到此项目的可执行文件。
add_executable (delivery 
		"src/main.cpp" "include/data.h"
		"include/UI/cli.h" "src/UI/cli.cpp" "src/UI/utils.cpp" "src/UI/user.cpp" "src/UI/list.cpp" "src/UI/delivery.cpp"
		"include/backend/usermanager.h" "src/backend/usermanager.cpp" "include/common/usermanager"
		"include/misc/user.h" "src/misc/user.cpp"
		"include/misc/duser.h" "src/misc/duser.cpp" "include/misc/manager.h" "src/misc/manager.cpp" 
		"include/misc/delivery.h" "src/misc/delivery.cpp" 
		"include/common/deliverymanager.h" "include/backend/deliverymanager.h" "src/backend/deliverymanager.cpp" "src/UI/print.cpp" 
		"include/misc/cuser.h" "src/misc/cuser.cpp" 
		"include/misc/deliverybook.h" "include/misc/deliverynormal.h" "include/misc/deliveryfragile.h" "src/misc/deliverynormal.cpp" "src/misc/deliverybook.cpp" "src/misc/deliveryfragile.cpp")

target_link_libraries(delivery PRIVATE GTest::gmock GTest::gtest GTest::gmock_main GTest::gtest_main)
target_link_libraries(delivery PRIVATE Qt::Widgets Qt5::Widgets Qt::WidgetsPrivate Qt5::WidgetsPrivate)
 target_link_libraries(delivery PRIVATE Qt::Sql Qt5::Sql Qt::SqlPrivate Qt5::SqlPrivate)

# TODO: 如有需要，请添加测试并安装目标。

