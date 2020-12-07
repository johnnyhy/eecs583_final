add_llvm_library(FINAL MODULE            # Name of the shared library that you will generate
       final_pass.cpp                    # Your pass
       utils.cpp
       PLUGIN_TOOL
       opt
)

find_package(OpenSSL REQUIRED)
target_link_libraries(FINAL OpenSSL::SSL)
