
add_executable(CustomDivePlan CustomDivePlan.cpp)

target_link_libraries(CustomDivePlan
    ${QT_LIBRARIES}
    subsurface_commands
    subsurface_corelib
    ${SUBSURFACE_LINK_LIBRARIES}
)
