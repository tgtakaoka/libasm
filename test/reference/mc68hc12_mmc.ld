/* Linker script for the MC68HC12 family memory map controller tests.
 *
 * m68hc12elfb resolves CALL, %page() and %addr() from a page number it derives
 * as (address - 0x10000) >> 14, and maps a banked address to the bank window
 * below.  Each page section is linked at the address of its page in that
 * scheme; objcopy then moves every section to its physical address.  The
 * text origin is given with --section-start=.text=<org>.
 */
MEMORY
{
  bank (rx) : ORIGIN = 0x8000, LENGTH = 0x4000
}
SECTIONS
{
  .text  : { *(.text) }
  .p30   0x0D0000 : { *(.p30) }
  .p31   0x0D4000 : { *(.p31) }
  .fixed 0xC000 : { *(.fixed) }
}
