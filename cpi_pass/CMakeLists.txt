add_llvm_library(CPI MODULE            # Name of the shared library that you will generate
       cpi_pass.cpp                    # Your pass
       utils.cpp
       cpi.cpp
       PLUGIN_TOOL
       opt
)

target_compile_options(CPI
       PRIVATE
       -Werror 
       -Wall 
       -Wextra
)

find_package(OpenSSL REQUIRED)
target_link_libraries(CPI OpenSSL::SSL)
