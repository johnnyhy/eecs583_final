add_llvm_library(CPI MODULE            # Name of the shared library that you will generate
       cpi_pass.cpp                    # Your pass
       utils.cpp
       PLUGIN_TOOL
       opt
)

target_compile_options(CPI
       PRIVATE
       -Werror 
       -Wall 
       -Wextra
)
