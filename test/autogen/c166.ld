/* The default xc16x-elf script confines .text to a memory region the test
   origins lie outside of. */
SECTIONS
{
    .text : { *(.text) }
}
