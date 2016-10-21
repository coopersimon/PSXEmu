# TODO:

* GTE instructions
* Fixed point needs refining
* CPU Tests
* GTE Tests (will write with instructions)
* GPU and window interface
* GPU testbench
* Memory bus for GPU, MDEC and others (DMA)
* MDEC
* Method of loading data from ROMs/CDs
* Interrupts
* Controllers

CPU needs to be tested in depth: the testbench is written and tests can be written to! Don't be afraid to write tests

DMA hierarchy needs to be sorted. Current plan is that it will include all other devices, and map generic memory accesses to specific functions inside those devices.
For example, the CPU will write to memory address 0x1F801820, which will go to the DMA. The DMA will decode that address to mean calling the MDEC command function.


# DONE:

* Memory base
* Registers
* CPU
* CPU testbench
* GTE base
* Fixed point for GTE
* GTE testbench


# Documentation:

[Everything you have always wanted to know...](http://gamehacking.org/faqs/PSX.pdf)

[In depth documentation](http://problemkaputt.de/psx-spx.htm)

[Vulkan Spec](https://www.khronos.org/registry/vulkan/specs/1.0/pdf/vkspec.pdf)

[Vulkan Spec html](https://www.khronos.org/registry/vulkan/specs/1.0/xhtml/vkspec.html)
