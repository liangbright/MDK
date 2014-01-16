FILE(GLOB ALL_FILES  "*.h" "*.cpp" "*.hpp")

message(STATUS Inside_Source_Dir)

message(STATUS ${ALL_FILES})

target_link_libraries(MDK_LIB_CORE ${ALL_FILES}) 