# PSXEMU
Playstation emulator. Very much a work-in-progress.

A personal project to learn about 3D graphics (hardware and software), computer architecture and software project management.


## TODO:

### Code:

* GTE: finish off first few instructions
* Fixed point and repeating sections of GTE instructions need refinement
* CPU Tests
* GTE Tests
* GPU and window interface (opengl and vulkan)
* MDEC needs finishing
* DMA needs testing
* Method of loading data from ROMs/CDs
* Interrupts
* Controllers: both control system and maps to hardware devices

### Project Management
* Internally: Some sort of error system. (e.g. memory addresses)
* Sorting out graphics APIs and locations
* CMAKE!! Get cmake working for the project. This (may be) crucial for linking with graphics libs

#### Personal notes:
For compiling graphics: _pkg-config --static --libs glfw3_. Everything that comes up needs to be linked.

glfw3 libraries are found in /usr/local/lib


CPU needs to be tested in depth: the testbench is written and tests can be written too!


## DONE:

* Memory base
* Registers
* CPU
* CPU testbench
* GTE instructions and flags
* Fixed point for GTE
* GTE testbench
* DMA base


## Documentation:

[Everything you have always wanted to know...](http://gamehacking.org/faqs/PSX.pdf)

[In depth documentation](http://problemkaputt.de/psx-spx.htm)

[Component docs](http://psx.rules.org/psxrul2.shtml)

[Vulkan Spec](https://www.khronos.org/registry/vulkan/specs/1.0/pdf/vkspec.pdf)

[Vulkan Spec html](https://www.khronos.org/registry/vulkan/specs/1.0/xhtml/vkspec.html)

[This forum is useful for MDEC](http://www.psxdev.net/forum/viewtopic.php?f=70&t=551&start=20)
