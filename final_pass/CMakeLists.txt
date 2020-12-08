add_llvm_library(FINAL MODULE            # Name of the shared library that you will generate
       final_pass.cpp                    # Your pass
       utils.cpp
       cpi.cpp
       PLUGIN_TOOL
       opt
)

target_compile_options(FINAL
       PRIVATE
       -Werror 
       -Wall 
       -Wextra
)

find_package(OpenSSL REQUIRED)
target_link_libraries(FINAL OpenSSL::SSL)
