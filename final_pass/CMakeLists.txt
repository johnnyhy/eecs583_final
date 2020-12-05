add_llvm_library(FINAL MODULE            # Name of the shared library that you will generate
       final_pass.cpp                          # Your pass
       PLUGIN_TOOL
       opt
)