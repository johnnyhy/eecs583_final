# How to run demonstration executables

The following files are provided as demonstration. 

- ptr_exploit.insecure = benchmarks/ptr_exploit.cpp compiled with Clang without any stack protections
- ptr_exploit.cpilib = benchmarks/ptr_exploit.cpp compiled with Clang without default stack protections, but intrumented and linked with CPI-lib
- return_exploit.insecure = benchmarks/return_exploit.cpp compiled with Clang without any stack protections
- return_exploit.cpilib = benchmarks/return_exploit.cpp compiled with Clang without default stack protection, but instrumented and linked with CPI-lib

See build.sh for each specific step in the build process

For normal behavior (without default stack protections and without CPI-lib):

`$ ./ptr_exploit.insecure < ptr_payload_safe.txt`

`$ ./return_exploit.insecure return_payload_safe.txt`

`$ ./ptr_exploit.insecure < ptr_payload_danger.txt`

`$ ./return_exploit.insecure return_payload_danger.txt`

To compare CPI-lib behavior:

`$ ./ptr_exploit.cpilib < ptr_payload_safe.txt`

`$ ./return_exploit.cpilib return_payload_safe.txt`

`$ ./ptr_exploit.cpilib < ptr_payload_danger.txt`

`$ ./return_exploit.cpilib return_payload_danger.txt`

Note: The reason we provide pre-compiled executables is because each payload has to be crafted specifically to how Clang decides to layout the variables in memory at each compile. Feel free to compile your own versions of ptr_exploit.cpp and return_exploit.cpp if you'd like, but be prepared to spend 30 minutes or more in GDB in order to craft a payload that exhibits any interesting behavior!
