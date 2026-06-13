add_executable(quant_cli main.cpp)
target_link_libraries(quant_cli PRIVATE quantcore)
target_compile_features(quant_cli PRIVATE cxx_std_23)

install(TARGETS quant_cli RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
