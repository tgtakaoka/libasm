/* Linker script for the Am29000 family tests.
 *
 * a29k-coff-ld's built-in script ends .text with the __CTOR_LIST__ and
 * __DTOR_LIST__ words of its CONSTRUCTORS directive, which no assembler in
 * this test emits.  This script keeps the same section order and drops them,
 * so the linked image holds exactly the assembled instructions.  The text
 * origin stays in <stem>.ld_flags, as --section-start=.text=<org>.
 */
SECTIONS
{
  .text : { *(.text) }
  .data : { *(.data) }
  .bss  : { *(.bss) }
}
