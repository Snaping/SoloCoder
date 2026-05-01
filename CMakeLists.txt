cmake_minimum_required(VERSION 3.10)
project(FileExplorer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
set(CMAKE_MFC_FLAG 2)

find_package(MFC REQUIRED)

add_executable(FileExplorer WIN32
    FileExplorer.cpp
    FileExplorer.h
    MainFrm.cpp
    MainFrm.h
    ChildFrm.cpp
    ChildFrm.h
    FileExplorerDoc.cpp
    FileExplorerDoc.h
    FileExplorerView.cpp
    FileExplorerView.h
    FileListCtrl.cpp
    FileListCtrl.h
    FileTreeCtrl.cpp
    FileTreeCtrl.h
    FilePreview.cpp
    FilePreview.h
    FileExplorer.rc
    Resource.h
    stdafx.h
)

target_link_libraries(FileExplorer PRIVATE comctl32 gdiplus)

set_target_properties(FileExplorer PROPERTIES
    WIN32_EXECUTABLE TRUE
)
