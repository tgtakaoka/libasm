# libasm style and workflow rules

Coding style and development workflow rules for libasm. Treat everything
here as the maintainer's standing preference.

---

## Commit hygiene

### Every commit stands alone

1. Build + tests must pass at every commit -- `ninja -C cli`,
   `ninja -C test`, `ninja -C test/driver` all green.
2. No forward references: no symbols, files, or *comments* may name a
   CPU variant, function, enum, feature, or fix that doesn't exist yet
   at that commit. Comments are part of the contract too.
3. No dead code: a file or symbol added before its first use counts as
   dead in the earlier commit.
4. The `usage: asm ...` / `usage: dis ...` blocks in `README.md` are the
   actual `./cli/asm --help` / `./cli/dis --help` output for *that
   commit*. Don't hand-edit them; regenerate them from the built
   binaries. When adding/renaming a CLI option, list "regenerate README
   CLI blocks" as a follow-up step rather than patching the README
   directly.

### Commit messages

- One-line subject in the existing repo style: `[arch] Verb noun` (e.g.
  `[i8086] Add X`, `[h8500] Fix Y`). Lowercase arch tag.
- **No redundant "for <CPU>" or "for <tool>" suffix.** The tag already names
  the arch and the noun already names the thing: `[i960] Add disassembler`,
  not `[i960] Add disassembler for 80960`.
- Body is optional and brief -- communicate intent, not transformation
  lists. The diff shows what changed.
- No bulleted file lists, no sed-rule enumerations, no "Notes:"
  sections, no extended rationale.
- **Name no assembler but GNU as** -- in a commit message or a PR body, don't
  mention ASL, `*-coff-objdump`, or any other tool, and don't call output
  "gas-derived". Cite the manual instead; a manual is not a tool. Cross-tool
  findings belong in bug reports to that tool, not in the repo history. Keep
  them out of code comments too.
- **Never** add attribution trailers -- `Co-Authored-By:` or any other --
  to a commit message, and no tool-generated attribution line in a PR body.

### "Dissolve" workflow

When asked to "**dissolve** the last commit(s) into appropriate
CL(s)", that means: split the changes by file group and squash each group
into the historically-correct prior commit, so the final history looks
like the work was done correctly the first time.

Typical pattern:
1. Identify each hunk's "home" commit (`dis_*.cpp` -> the disassembler
   commit; `asm_*.cpp` -> the assembler commit; etc.).
2. Stage per-target hunks; `git commit --fixup=<target-sha>` each.
3. `git rebase -i --autosquash <base>`.
4. Verify each rebased commit still builds + tests cleanly.

When autosquash isn't safe (e.g. the target commit is going to be
force-pushed and the upstream version is different), use explicit
`git checkout <final> -- <files>` + `git commit --amend` per target.
For a non-HEAD target with a *later* commit also touching the same file:
`git reset --hard <target>`, overlay the asm-clean files from a backup
branch (`git checkout backup -- <file>`), hand-apply the in-place hunk to
the file the later commit also edits, `git commit --amend`; then rebuild
the later commit as `git checkout backup -- . && git commit -C <orig>`.
Finish by checking `git diff backup HEAD` is empty -- the final tree must
be byte-identical to before the dissolve.

If a later commit's hunk DEPENDS on a declaration introduced by a later
commit (field, enum, helper), MOVE that declaration into the EARLIER
commit where it's first used, not where it was originally added. Example:
H8/500's `insn.maxMode` field moved into the *disassembler* commit
because the dis page-jump gating is its first user; the assembler commit
then only adds `currentPageMaxOnly`. Verify BOTH commits build in
isolation -- the earlier one must compile with the moved declaration but
without the later commit's code.

### Rewriting history by hand

Fixups alone can be squashed non-interactively with
`GIT_SEQUENCE_EDITOR=: git rebase -i --autosquash <base>`. A dissolve or
reorder that autosquash cannot express is done by hand:

1. `git branch backup` first, always.
2. `git reset --hard <base>`.
3. Per commit: `git checkout <orig> -- <its files>`, overwrite the few files
   that should differ (keep the "final" versions in a scratch directory), then
   `git commit -C <orig>` to reuse the original message, or `-F -` for a new
   one.
4. `git cherry-pick -n <c>` + `git commit -m ...` is the cheap way to rewrite
   ONLY a message: it stages the tree unchanged.
5. Finish with `git diff backup HEAD` -- empty means the tree survived -- and
   re-verify EVERY commit builds and tests in isolation.

Two traps, both hit for real:
- **Never `git add -A`, and never `git add <directory>`.** The ~125 test
  binaries under `test/` and `test/driver/` are NOT gitignored and get swept
  into the commit. Always list the files. The `git diff backup HEAD` check is
  what catches it.
- A checkout in a later step can REVERT a fix applied in an earlier one (e.g.
  `git checkout <asm> -- cli/build.ninja` undoing an ordering fix made in the
  dis commit). Re-apply per commit, or fold the fix into every commit that
  touches the file.

### Splitting a feature out into its own commit

When a feature is wanted as a separate trailing commit, rebuild the
earlier commits from their pre-feature versions, but keep general bug fixes
found along the way (e.g. the truncation fixes) in the historically-correct
fix commit. Make the feature commit the exact verified final tree:
`git restore --source=<verified-tip> --staged --worktree :/` stages adds,
modifies and deletions without touching the untracked test binaries -- unlike
`git checkout <rev> -- .` followed by a bulk add. Confirm with
`git diff <verified-tip> HEAD` (empty) and `git show --stat HEAD | grep -c ' Bin '`
(0).

### Pushing

- Prefer `git push --force-with-lease` over plain `--force` for force
  pushes (catches concurrent upstream changes).
- **`gh pr edit --body` silently FAILS on this repo**, with a GraphQL
  "Projects (classic) is being deprecated" message that reads like a warning
  but aborts the edit. Use
  `gh api -X PATCH repos/OWNER/REPO/pulls/N -F body=@file` and verify with
  `gh pr view N --json body`.
- **`gh pr create` opens the PR even when a `git push` in the same command was
  REJECTED**, so the PR is built from the stale remote branch. Push as its own
  step, confirm `git rev-parse HEAD origin/<branch>` agree, then create.
- `gh`'s "N uncommitted changes" warning counts the ~125 untracked test
  binaries; check `git status --short | grep -v '^??'` before believing it.

### Backporting to `release-1`

`main` carries the 2.x line, which dropped Arduino and AVR support in
`9e34d30e`. `release-1` is the maintenance branch for the 1.x line; it
starts at `ea52593f` (Version 1.6.70), so it still has `library.properties`,
`README_.adoc`, `arduino-ci.yml`, the PROGMEM layer, and the version pinned
across 47 files instead of 3.

**A bug fix lands on `main` first, then gets backported to `release-1`.**
Cherry-pick with `-x`, so the 1.x commit records where it came from:

```
git cherry-pick -x <sha-on-main>
```

New architectures and features are NOT backported unless asked for.

Three things to expect:

- A fix touching code the Arduino drop removed or moved will not apply
  cleanly. Resolve it against the 1.x tree as it stands; do not reshape
  `release-1` toward `main` to make a cherry-pick fit.
- Arduino CI runs on `release-1` and cannot pass while drazzy.com's TLS
  certificate is expired, so `arduino-cli` fails to fetch the DxCore index
  and fail-fast cancels the other two boards. `gh pr checks` reports those
  cancellations as `fail`; read the job log before calling it a real failure.
  PlatformIO CI covers the same three boards and does not touch drazzy.com.
- A release cut from `release-1` uses the OLD procedure: 47 version files,
  `library.properties` naming every arch in its `paragraph`, and the Arduino
  Library Manager picking up the tag on its own schedule. The "Release
  procedure" section below describes `main` only.

---

## Coding style

### No lambdas

Do not use lambdas in `src/` unless a standard-library API requires one
(e.g. inline `std::sort` comparator). Even single-use closures should be
named static functions in an anonymous namespace. Closure capture is not
a "strong reason"; restructure or pass state as parameters.

### ASCII-only in comments

No non-ASCII characters in code comments. Use `->` not `->`,
`--` not `--`, `...` not `...`. Applies to all C/C++/header files.

### Explicit AddrMode

When a new opcode pattern needs an addressing-mode value in a per-CPU
`entry_*.h`, add a dedicated new `M_*` enum value rather than overloading
an existing mode with a role-swap or context convention. Readability and
self-documentation in the table win over diff-size minimisation, even
when the new mode's encoder/decoder mostly duplicates an existing one's.

### TEXT_* and PSTR_* constants stay uppercase

Unless otherwise explicitly required.

### Dotted directives and `TEXT_d*` constants

`asm_base`'s pseudo lookup is dot-tolerant, and the driver registers both
the bare and dotted spelling of `.cpu`/`.org`/`.include`/`.end`/`.equ`/
`.option`. libasm `.asm` reference + autogen wrappers follow the vendor's
native spelling: dotted where the vendor assembler is dotted (Hitachi:
`.cpu`/`.org`/`.include`/`.end`), bare for Motorola and Zilog (`cpu`/`org`/
`include`/`end`; CodeWarrior's S12(X) directives are bare). The ASL `.asl` and
GAS `.gas` wrappers keep their own syntax. Arch-local directives matched in a
`processPseudo` override (mc68hc12 `PPAGE`...) do not get the base's dot
tolerance.

When an arch defines a dotted pseudo-directive *name constant* locally
(because it isn't in `text_common`), prefix it with `d` to mark the
dotted spelling and avoid shadowing the bare common constant:
`TEXT_dDATA = ".DATA"` (vs `common::TEXT_DATA = "data"`), `TEXT_dRES`,
`TEXT_dSET`, `TEXT_dMAXMODE`.

### Parse numeric tokens with `Value::read`, not char-by-char

Prefer `Value v; v.read(scan)` (default `RADIX_10`, leaves `scan`
unchanged and returns `NOT_AN_EXPECTED` on failure) over a hand-rolled
`expect('1')`/`expect('6')` digit chain for size qualifiers (`:8`/`:16`/
`:24`) and similar.

Boundary handling is asymmetric -- reason about it per call site rather
than reflexively adding a guard:
- A suffix split OFF the mnemonic NAME (`.B`, `:G`) MUST end the name
  (`p[1] == 0`), otherwise a trailing letter is silently truncated away
  (`MOV.BX` parses as `MOV.B`). The name needs an explicit word-boundary
  guard, and it should make the whole name `UNKNOWN_INSTRUCTION`.
- A size qualifier in the OPERAND stream (`@aa:8`) does NOT: `Value::read`
  consumes only the digits, and a trailing letter is already caught by
  the end-of-line check as `GARBAGE_AT_END`. Adding a guard there is
  redundant and was rejected in review. (Still worth a test that asserts
  the `GARBAGE_AT_END`.)

### Dedupe `TEXT_*` constants through `text_common`

Run `scripts/extract-common-text.sh` from `src/` after adding a new
arch. Every match is a duplicate `extern const char TEXT_X[]`
declaration across two arch text files; the goal is one canonical
definition in `text_common.{h,cpp}` and `using common::TEXT_X;` aliases
in each arch header. Concrete steps when the script reports a dupe
between `text_<archA>.h` and `text_<archB>.h`:

1. Confirm both arches define the **same string** (e.g.,
   `text_h8300.cpp:"ROTL"` and `text_superh.cpp:"ROTL"`). Different
   strings (e.g., `"MOV:L"` vs `"MOV.L"`) are name collisions, **not**
   true duplicates -- leave those alone.
2. Add the symbol to `text_common.h` and `text_common.cpp` in
   alphabetical position. Both `.h` (`extern const char TEXT_X[];`) and
   `.cpp` (`constexpr char TEXT_X[] = "...";`).
3. In each arch header, replace `extern const char TEXT_X[];`
   with `using common::TEXT_X;` in the existing "Instructions reused
   from common" block.
4. In each arch `.cpp`, comment out the local definition --
   `// constexpr char TEXT_X[] = "X";` -- following the
   convention used for previously-moved symbols.
5. **No build configuration change needed.** `text_common.o` already
   links into every CLI and per-arch test binary, so the existing
   build files keep working. Do *not* add cross-arch `text_*.o` deps
   (e.g., `using h8300::TEXT_X` from `text_superh.h` would require
   pulling `text_h8300.o` into `test_asm_superh`'s link line and
   should be avoided -- promote to common instead).
6. Re-run the script; only legitimate string collisions should remain.

---

## Asm / Dis design: pure table-driven

The assembler and disassembler must be **pure table-driven**. The user
has rejected name-based dispatch and ad-hoc special cases across
TLCS-900, H8/500, and H8/300 reviews.

1. **No name-based dispatch.** Never `strcasecmp(name, TEXT_PJMP)` or
   `if (insn.name() == ...)` inside `encodeImpl` / `decodeImpl`. If a
   subset of instructions needs different behaviour, gate via:
   - a separate `EntryPage` with a page-level attribute (e.g.
     `maxModeOnly`, CpuType filter, PrefixMode bitmask), or
   - a bit on `Entry::Flags`, checked in `acceptOperands` /
     `matchOpCode`.
2. **No M_*-mode-specific if-clauses in encodeImpl.** Each AddrMode has
   its own `encodeOperand(mode, op)` arm. Pre-checks (LDC's CR-size,
   MAXMODE) belong in `processPseudo` or per-mode helpers.
3. **Use `pageSetup` and `readCode` callbacks** (see `table_tlcs90.cpp`)
   to dispatch prefix and per-page state at the table-search level.
   `Cpu::defaultReadCode` is available when no custom behaviour is
   needed.
4. **Generic per-AddrMode `encodeOperand` / `decodeOperand`.**
   Specialised `processX()` functions per instruction class are
   discouraged; the instruction's data lives in the table.
5. **Operand validation inside `encodeOperand` for that mode**, not as
   a pre-encode special case.

### `acceptMode` fast path

When writing `acceptMode(op, table)` or equivalent, lead with the
exact-match equality before the switch:

```cpp
static bool acceptMode(const Operand &op, AddrMode table) {
    if (op.mode == table) return true;
    switch (table) {
        // only the genuine "fits-into-this-mode" relaxations
    }
}
```

The parser almost always produces the table-expected mode; the switch
should carry only the fit-relaxations (e.g. `M_DISP8F` satisfies
`M_DISP8`, `M_IMM8` satisfies `M_BIT`).

### `acceptOperands(insn, entry)` = `acceptMode(op1) && acceptMode(op2)`

Don't write a per-architecture pair-dispatch function. The pattern is:

```cpp
static bool acceptOperands(const AsmInsn &insn, const Entry *entry) {
    const auto flags = entry->readFlags();
    return acceptMode(insn.op1, flags.src()) &&
           acceptMode(insn.op2, flags.dst());
}
```

### Sort EA-encodable modes contiguously

When an `AddrMode` enum has a subset that's "EA-encodable" (or
analogous), arrange the enum so that subset is contiguous, then test it
with a single range comparison (`mode >= M_FIRST_EA && mode <= M_LAST_EA`)
rather than a multi-case switch.

---

### Model a capability, not a part number

When a feature appears on later SILICON of an existing part rather than on a
new part, give it a capability field in `CpuSpec` and let `CpuType` supply only
the default. Add a `CpuType` only when the part has its own name users will
write AND the difference is broad or changes `Config` itself (address width,
endian, `codeMax`). A part whose capability set exactly equals an existing
CPU's is an ALIAS, not a variant -- don't add it ('VC33 == 'C32 rev 2.0+).

Never model a revision NUMBER as an entity: model what it gates, and if the
convenience is wanted add ONE `silicon`/`rev` knob that derives the
capabilities. GAS does exactly this -- `-mrev` is documented as sugar over
`-midle2`/`-mlowpower`/`-menhanced`. Prefer that single knob over per-feature
booleans: the booleans only add combinations no real silicon has.

Two traps, both hit for real:
1. `setCpuType()` re-derives defaults, so an explicitly named revision must be
   remembered or a later `cpu` directive silently discards the option.
2. ...but `reset()` must clear it, or it leaks between unit tests and across
   CPUs.

Precedents to copy: mc68000 `CpuSpec{cpu,pmmu,fpu,fpuCid}` with `fpu(...)` /
`pmmu(...)` page tables searched after the CPU pages, chained on
`UNKNOWN_INSTRUCTION`; am29000's lighter `cpuHasFpu(CpuType)` static predicates
with `setFpu()` refusing to disable what the silicon always has. Three surfaces
are needed for a round trip -- the option, the directive, and the disassembler
EMITTING it back (`dis_superh.cpp` writes `option "fpu", "true"`) -- or dis
output stops re-assembling.

---

## PC-relative addressing: target in, target out

The single most consequential round-trip rule (caught the hard way in
H16):

1. **Disassembler emits the absolute target**, not the raw signed disp.
   `target = address + length + disp` (with the platform's PC alignment
   rule applied). Use `outRelAddr(out, target, address, width)` or write
   the absolute address directly with `outHex(out, target, 32, false)`.
2. **Assembler reads the source value as the target** and computes
   `delta = target - PC` itself.

Both sides agree on "the number in the source is the target absolute
address". If the dis instead emits the raw disp (e.g. `@(-$0156, PC)`),
the asm-side re-encode treats `-$0156` as a target absolute address and
produces a completely different delta -- and the asm->dis->asm round-trip
is broken.

SuperH gets this right by design (`outRelAddr` + `*+N` location-symbol
parsing). When porting a new Hitachi-family target, check the dis
PC-relative path is emitting a TARGET, not a DISP.

### Explicit size suffix on PC-rel disp is harmful

Tempting pattern: dis emits `@($XXXX:8, PC)` so the asm picks the same
disp byte width. **Don't.**

The hint forces the assembler to use that exact disp size. If any
earlier instruction's encoding shifts in size between gen and re-asm
(address drift), the delta at the new PC may no longer fit in `:8`, and
the assembler errors with `OVERFLOW_RANGE` -- the autogen breaks.

Better: dis omits the size suffix from PC-rel output entirely. Both
libasm and ASL auto-pick the smallest fitting size based on the actual
delta. The trade-off is that re-encoded bytes may differ in size from
the gen_<arch>-generated bytes, but the source text round-trips through
asm and dis losslessly, which is the contract that matters.

The size suffix IS appropriate for absolute address operands
(`@$XXXX:16`) where the value is PC-independent.

### OVERFLOW_RANGE on explicit-size operands

When source forces an explicit `:N` size (on any operand, not just
PC-rel), the assembler must:

- Validate the delta / value fits in N bits as the encoding expects
  (signed for PC-rel disps, signed or unsigned per the operand type).
- Emit `insn.setErrorIf(op, OVERFLOW_RANGE)` -- pass the `op` (Operand),
  not the bare error code, so `ERRT(... OVERFLOW_RANGE, "<expected at
  text>", ...)` finds the position. Bare `setErrorIf(OVERFLOW_RANGE)`
  leaves the error-at empty and the test can't anchor on it.
- Continue emitting the truncated bytes (don't bail out), so the test
  can also assert the actual byte sequence the assembler chose.

Smaller-than-N values MUST still be accepted -- a user writing
`@($10:16, PC)` is asking for the 16-bit form even though 8 bits would
fit. Only reject when the value cannot fit in N.

### Never truncate silently -- implicit widths too

The same rule holds when NO size is written: an operand that does not fit the
field the matched form encodes is `OVERFLOW_RANGE`, never quietly cut to the
low bits. mc68hc12 truncated all of these with no error until 2026-10:
`LDAA $12345` (16-bit extended), `LDAA #$123` (8-bit immediate),
`LDX #$12345` (16-bit immediate) and `LDAA <$1234` (forced direct, off the
direct page -- mc6800 already rejected it). Funnel every address/immediate
emit through one helper per field (`encodeAddress`, `encodeDirect`) so a new
emit site cannot skip the check, and grep for raw
`emitOperand16(op.val.getUnsigned())` when a cherry-pick brings new sites.
GNU as rejects all four; ASL `68HC12X` silently truncates the extended one.

### Autogen ORG: don't start at 0x0000

The chip's exception / reset vector table almost always sits at low
addresses. Generating instructions starting at 0x0000 makes PC-relative
targets wrap around into the high address space and disassemble as
large negative-looking values (e.g. `-$6F = 0xFFFFFF91`).

Pick an ORG safely past the vector table:

- H16: vector table 0x000-0x3FC -> ORG `0x1000`
- H8/500: 256-byte vector table -> ORG `0x0100`

Update both the `TestGenerator(..., addr)` constructor call in
`test/gen_<arch>.cpp` AND the `org $...` line in the matching
`test/autogen/gen_<arch>.{asl,asm}` wrappers.

---

## Autogen test generation: pluggable `TokenizedText` tokenizers

The autogen generators (`test/gen_<arch>.cpp`) sweep the opcode space, feed each
candidate to `Dis<arch>`, and emit decodable results as test sources under
`test/autogen/`. To keep those files small, `TokenizedText`
(`test/tokenized_text.{h,cpp}`) collapses the variable parts of disassembly
(operand values, register/index/scale variants, PC-relative deltas) to template
tokens so near-duplicate instructions dedup.

Tokenization is **pluggable** via `test/tokenizer.{h,cpp}`, and since the 2026-07
refactor the number-consuming tokenizers are **templated on the arch's hex number
policy** (no more try-every-family singletons):

- `struct Tokenizer { const char *tokenize(const char *p, std::string &out) const; }`
  -- match a token class at `p`; on match append the template token to `out` and
  return the pointer past the consumed input, else `nullptr`.
- **Number policies** (pure `static bool match(p, r)`, NO decimal fallback baked
  in): `IntelNumber` (`1234H`/`377Q`), `MotorolaNumber` (`$1234`), `NationalNumber`
  (`0x..` + IBM `X'..'`/`H'..'`), `CstyleNumber` (`0x..`/`0b..`), and the shared
  `DecimalNumber` fallback. `matchNum<Hex>(p,r)` = `Hex::match || DecimalNumber::match`;
  hex is ALWAYS tried before decimal (decimal-first would split `0x1F`/`1234H` at
  the leading digit).
- **Templated tokenizers**: `NumberTokenizer<Number>` (-> `n`),
  `RelativeTokenizer<Hex>(loc)` (branch target `<loc>+/-n` -> `<loc>+r`),
  `IndexDispTokenizer<Hex,Index>` / `RegDispTokenizer<Hex,Reg>` (consolidate a
  signed index/base displacement to one `n` by DROPPING the sign),
  `RegisterTokenizer(prefix,maxReg,classToken[,suffix])` (collapse `R0..Rn` ->
  `Rn`), and the non-templated `FloatTokenizer`.
- **Context predicates** (2nd template arg, tiny `static bool ok(...)` structs):
  index-context `ParenIndex` (`n(Rn)`), `CommaIndex` (`n,X` no-space inside a
  mode), `OuterDisp` (mc68020 `([...],od)`), `ColonDisp` (h16 `@(n:16,...)`),
  `LeadImm` (leading bare immediate `<mnem> -8,` -- ns32000 quick); register-context
  `ParenReg` (`(reg+n)`), `BracketReg` (`[reg+n]`).
- `standardTokenizers<Hex>(char loc, {indexToks})` returns
  `{FLOAT, indexToks..., NumberTokenizer<Hex>, NumberTokenizer<DecimalNumber>, Relative<Hex>(loc)}`
  -- index/reg tokenizers FIRST (win on the whole displacement), then hex, then
  the decimal fallback, then relative. `loc` = `Disassembler::curSym()` (the origin
  char the dis emits). `TestGenerator`'s 3-arg ctor still auto-builds the default
  list for plain archs; the 4-arg ctor takes an explicit list.

**Each `gen_<arch>.cpp` picks its own hex policy** plus its index/reg tokenizer
instances: `standardTokenizers<CstyleNumber>(loc, {&INDEX_DISP, ...})`. Native vs
gas variants may need different policies -- mc68000/i8086 branch on
`driver.generateGas()` (`MotorolaNumber`/`IntelNumber` native, `CstyleNumber` gas).
Arch-specific tokenizers live in that arch's `gen_<arch>.cpp` anonymous namespace,
NOT in `tokenizer.cpp`. Current extras: mc68000 (index, movem list, `OuterDisp`),
ns32000 (`LeadImm`, `EXTERN`, `+(-n)` disp, `:s` scale), i8086 (reg8/16/32, seg,
`*s` scale, `BracketReg` reg+disp), h16 (reg/index/list, `CommaIndex`+`ColonDisp`,
drop `:N` size-suffix), z8000 (Z8001 extern-sym).

Two rules learned sizing these:
- Register collapse is EXPLICIT per family via `RegisterTokenizer` -- size it from
  `reg_<arch>.cpp:outRegName()`, NOT the enum (composed/computed names like h16
  `CR`/`PR`, superh `FR`/`DR`, tlcs900 abs-bank `<base>0..7`, i8086 `CR/DR/TR`
  aren't enumerated). The number matcher already collapses `R5`->`Rn` inside a reg,
  so a redundant RegClass tokenizer merges nothing.
- `IndexDisp`/`RegDisp` add value ONLY where the arch actually emits NEGATIVE
  displacements. A no-op one can catastrophically perturb the `calcDrop`
  exploration heuristic (pdp11 jumped 7246->926 lines) -- removed from
  pdp11/mc6800/superh/mos6502/tms9900/mn1610.

### Number format is gen-normalized, not arch-default

A gen sets `intel-style` / `c-style` / `gnu-as` so its output uses one
ASL/GAS-compatible number syntax regardless of the disassembler's default
formatter (ns32000 `c-style` -> `0x`; pdp11 `intel-style` -> octal `377Q`).
Probing `dis.formatter().formatHex()` is therefore the WRONG family signal --
the standard list carries all number families so whichever the gen emits is
collapsed. The `%`/`>`/`@`/`#` glyphs in output (tms7000 `%23H`, z8 `>01H`) are
addressing-mode PREFIXES, not number syntax; the number after them is Intel-`H`
and collapses normally. Caveat: the standard tokenizer does NOT collapse IBM
`X'..'` hex, so an arch whose native hex is IBM (e.g. CP-1600) must gen-normalize
to `c-style` (`0x..`) or its autogen explodes -- see the CP1600 gotcha.

### Float immediates must render a decimal point (so they dedup)

`FloatTokenizer` collapses a float constant (`n.n`, `n.nE+-e`, integer+exponent
`nEn`, `NaN`, `Inf`) to one token. But a WHOLE-number float rendered without a
decimal point (`13389134`, `0`, `-0`) looks like an integer, tokenizes as a plain
`n`, and stays DISTINCT from the fractional-float token -- so the same instruction
keeps a redundant test line per whole-number float operand. Fix is in the
DISASSEMBLER, not the tokenizer: `__float80::gcvt`/`fcvt` (`src/float80_*.cpp`)
append `.0` to an integral rendering (string-check the `snprintf %Lg` output; do
NOT use `modf` -- it would also perturb rounded/exponent cases). Then the whole
float renders `13389134.0`, `FloatTokenizer` folds it in with the fractional
forms, and the redundant lines collapse. Affects only the float-DISPLAYING archs
(mc68000 FMOVE, ns32000, pdp11, tms320) -- their baselines shrink on the next
regen; unit coverage is `test/unit/test_str_buffer.cpp` + `test_float80.inc`.

### Immediate-sign consolidation is ARCH-SPECIFIC (don't blanket-apply)

`LeadImm` collapses a leading bare immediate/quick sign (`ADDQ 0` / `ADDQ -8` ->
one `ADDQ n`). Safe ONLY where the signed operand maps to exactly ONE opcode
(ns32000 quick field). Do NOT apply it where the sign is an encoding discriminator:
- **mc68000** immediates are `#`-prefixed unsigned hex -- essentially no sign
  redundancy to collapse (only MOVEQ is signed, 1-2 lines).
- **i8086** signed bare immediate is the sign-extended imm8 opcode `0x83`, which
  ALIASES imm16 `0x81` on the POSITIVE side (`ADD [X],5`=0x83, `ADD [X],8887H`=0x81
  -- both tokenize to `n` and already conflate). The NEGATIVE rendering is the sole
  thing keeping 0x83 in the deduped set; collapsing its sign silently DROPS that
  encoding's round-trip coverage. Verify opcode selection with `cli/asm -l` before
  assuming an immediate sign is pure redundancy.

### The autogen is the only check that catches decode-path bugs

Unit tests, the reference test and hand round-trips all passed while two real
bugs were live; the generator found both, because it is the only thing that
walks the whole opcode space.

- A **hung sweep is a bug report**. Four h8300 sweeps burned 3h50m of CPU each
  and emitted 7 lines: a new prefix page was matching under a super-prefix and
  the exploration could not terminate. Compare progress against the committed
  baseline's line count and treat "slow" as "wrong" until proven otherwise.
- A **big diff is a bug report too**. `-3588` lines per mc68000 variant was not
  coverage rebalancing, it was the disassembler's FPU being switched off.
  Always characterise a diff per mnemonic before accepting it:
  `awk '!/^\s*;/{print $1}' | sort | uniq -c` on both sides.
- **A zero diff is a result**, not a non-event: regenerating the whole mc68000
  family byte-identically proved the committed baselines already matched the
  intended behaviour and only the shipped code had drifted.
- Finish the round trip. "The other assembler accepted it" is NOT the check --
  assemble the regenerated source with libasm too and compare the S-records or
  hex with `grep -v '^S0'` (the S0 header holds the filename and always
  differs). Skipping that on `gen_h8s*` left the strongest available evidence
  unmeasured until it was asked for.

### Do not attribute a coverage change to `calcDrop` without checking

`calcDrop` only engages once ONE tokenized skeleton has been seen 65536 times,
and it prints `@@ break drop=` when it fires. **Those prints are compiled out
unless you build with `-DDEBUG_TRACE=1`**: `DataGenerator::dump()`/`debug()`
are wrapped in `#if DEBUG_TRACE` and nothing defines it, so an `.lst` with no
`@@` lines tells you NOTHING about whether the heuristic ran. Rebuild
`test_generator.cpp` with `g++ -DDEBUG_TRACE=1` and relink the generator
before concluding it never fired. The usual cause is the **exploration tree** changing shape: the generator
descends through `NO_MEMORY` (spawn a child, extend the sequence), so an opcode
that stops erroring terminates a branch early and everything the sweep used to
reach beneath it disappears. That is what cost tlcs900 3614 deep
bank-register forms while gaining 1651 new ones. Verify the dropped forms still
assemble and disassemble by hand before accepting the trade.

### `maxBarren()`: when the drop heuristic can never fire

`calcDrop` counts only decodes that SUCCEED. Where a subtree's operands are
mostly REJECTED, the threshold is never reached and the sweep runs
effectively forever -- i8096 80196N made no progress in 29 minutes, because
its extended operands pair a register that must be divisible by eight with a
24-bit displacement, so only one byte combination in eight decodes.

`TestGenerator::maxBarren(count)` bounds the search by decodes since the last
previously-unseen skeleton. It is OFF by default and deliberately PER TARGET:
an architecture's legitimate barren stretch can itself run to millions, and
the honest maxima are close enough to the pathological case that no global
value separates them. Measured: **i8096 8,650,992**, mc68hc16 135,035,
tms9900 132,856, z8 66,563, superh 8,063. 80196N uses `12 << 20` and finishes
in 45s.

The cut costs coverage, so state what it costs. It returns `MAX_DROP_BYTE`,
which unwinds PAST the level that needs to advance: on 80196N the budget
expires inside the first misaligned `treg`, so `EST n, n[n]` (1D) and
`ELD n, n[n]` (E9) are never emitted. Unwinding less (4 is the unique value
that drops the three displacement levels but lets `treg` advance) restores
them in principle and puts the sweep back to minutes without finishing.

Dead ends, all measured, before reaching for this: counting
`OPERAND_NOT_ALIGNED` as a drop candidate is fast but LOSES REAL COVERAGE
(i8096, mc68000 `MOVEM.W`, z8000 `RES`); the `generate(opc1)` pre-fill is
front-only, so it cannot bound a cost that sits mid-instruction; lowering the
drop threshold prunes coverage without being fast. Check any change here with
a skeleton SUPERSET diff across architectures -- a line-count diff misses it.

### Verifying autogen changes: baselines can be stale

Committed `test/autogen/*.lst` are gitignored and frequently stale; the `.inc`/
`.ginc`/`.zinc`/`.asm` are the real baselines but can ALSO be stale when the
disassembler was rewritten after they were last generated. So a regen DIFF vs
the committed `.inc` does NOT by itself mean a tokenizer regression. To isolate
a tokenizer change from a stale baseline, build the pre-change binary in a
throwaway worktree and diff its output against the post-change binary:

```sh
git worktree add /tmp/pre HEAD~1
make -sC /tmp/pre/test gen_<arch>
# diff /tmp/pre's regen vs current; identical => byte-identical, baseline stale
```

Identical prefixes are enough on a slow generator (run both under `timeout` and
compare the first N lines -- the sweep is deterministic, so any tokenizer-caused
divergence shows early). Regenerate a confirmed-stale baseline with
`make -j gen_<arch>` from `test/autogen/` (drives every output + CPU variant);
`.asl`/`.asm` are static `include` wrappers, so only `.inc`/`.ginc`/`.zinc`/
`.asm` change.

### A baseline regenerated with an INTERMEDIATE tokenizer can be silently wrong

mtime does not tell you which tokenizer produced a committed `.inc`. A baseline
regenerated mid-refactor -- with a number policy that didn't yet match the arch's
hex (`IntelNumber` not matching `...H`) -- silently stops collapsing those numbers
and EXPLODES (tms320 `.inc` gained +2181 lines, the same address recurring
un-collapsed). Cheap detector: number-normalize both the working-tree and the
committed `.inc` and count near-dup skeletons -- a CONTAMINATED file has MORE
near-dups than committed (a correctly-consolidating change has fewer):

```sh
redun(){ perl -pe 's/0x[0-9A-Fa-f]+/N/g; s/\$[0-9A-Fa-f]+/N/g; s/\b[0-9A-F]+H\b/N/g;
                   s/#-?[0-9.]+/#N/g; s/\b\d+\.?\d*/N/g; s/[-+]N/N/g; s/[ \t]+/ /g;' \
        "$1" | sort | uniq -c | awk '$1>1{s+=$1-1}END{print s+0}'; }
# redun working.inc  >  redun committed.inc  =>  contaminated, regenerate fresh
```

Verify a shrink with a mnemonic-coverage check (`comm -23` committed-vs-new first
column must be empty) AND a number/sign/size-INSENSITIVE skeleton SUPERSET (a pure
consolidation makes the new set a subset of the old -- never diff raw lines; a
whole-number-float `.0` change or a no-space dis change makes every line "differ").
Where the diff shrinks more than the intended collapse, it is usually `calcDrop`
2^16-cap exploration churn: mc68020 FBcc.X drops from 11 to 3 surviving `.X`
condition mnemonics, but the churn is coverage-SAFE because the `.X` render path is
condition-INDEPENDENT (tested via the 3 survivors) and all 32 plain conditions
remain. Accept such churn only after confirming every decode path is still covered.

### Each autogen commit OWNS its regenerated baselines

A regenerated `.inc` bakes in EVERY current change at once (dis rendering +
tokenizer dedup). So a commit that changes the dis or a tokenizer must include the
baselines it affects, or an ancestor commit must supply the other half. When
splitting float-rendering / per-arch-tokenizer work into separate commits, group so
each commit is round-trip-consistent on its own: float-only archs (pdp11, tms320)
with the float commit; each arch's baseline with whatever combination of dis +
tokenizer changes it needs present at that commit or below. If the shared
`float80`/`FloatTokenizer` change and a per-arch tokenizer BOTH touch an arch's
baseline (mc68000, ns32000), those source changes must land in the same commit as
the baseline or an ancestor of it.

**Full-sweep regen cost is DAYS**; regenerate an arch only when inevitable, and run
the slow ones (mc68020/030/040, ns32000, tms320, h16) as PARALLEL background jobs.
NEVER abort a live regen -- it discards hours of unrecoverable work.

### Regenerate via the ARCH target `make -j gen_<arch>`, never individual outputs

`gen_<arch>` (underscore) is an arch-level phony whose recipe cleans that arch's
outputs and then re-invokes make with `-j` across EVERY variant and form it owns:

```make
gen_$(1): clean_$(1)
	@make -j $(foreach t,$(call arch-target,$(1)),$(call gen-out,$(t)))
```

So `make -j gen_am29000` runs all 12 am29000 sweeps (6 CPU variants x
`.inc`/`.ginc`) CONCURRENTLY: total wall time is about ONE sweep, not twelve.
Naming the outputs directly instead -- `make gen_am29050.inc gen_am29050.ginc`
-- SERIALIZES them, because make builds the named goals one after another and
the `-j` inside the arch recipe never runs at all.

Measured on the 12-core box (am29000, 2026-09): `make -j gen_am29000` =
**298 min wall for 3499 min of CPU** -- all 12 sweeps at once, each sweep being
~5h on its own. Naming the two am29050 outputs directly would have cost ~10h,
and the full arch serially ~58h. One am29050 `.inc` sweep alone had only
reached 677 of 1148 lines after 20 min. ALWAYS use the arch target.

Regenerating the whole arch also re-derives the variants the change did NOT
touch. That is a free consistency check, not waste: they must come back
byte-identical, and any diff there is pre-existing baseline drift to report
separately rather than fold into the commit.

### Waiting on a sweep: `pgrep -f` matches the waiter itself

`until ! pgrep -f 'gen_<arch> -i'; do sleep; done` never exits: the waiting
shell's own command line contains the pattern, so it matches itself forever.
Use `pgrep -x gen_<arch>` (exact process NAME) instead -- the waiter is
`bash`, so it cannot self-match.

### Regenerating autogen: rebuild the generator binary FIRST

`make -j gen_<arch>` (underscore, from `test/autogen/`) regenerates the `.inc`
but does NOT rebuild the `../gen_<arch>` binary -- the per-output Makefile rule
has the binary prerequisite commented out (`$(3): #../gen_$(1)`). So after
editing `src/` it runs a STALE binary and silently reproduces the OLD `.inc`; an
empty `git diff` then proves nothing. (Running the unit tests recompiles the
`.o` files but does NOT relink the generator.)

Correct order to actually verify a disassembler change:
1. `make gen_<arch>` from `test/` -- rebuilds+relinks the generator binary.
2. `make -j gen_<arch>` from `test/autogen/` -- regenerate the `.inc`.
3. `git diff test/autogen/gen_<arch>*.inc` -- now a real byte+text diff.
4. Round-trip: `cli/asm -C <CPU> gen_<variant>.asm -o /dev/null` == 0 errors.

A SECOND way an empty diff proves nothing: `make` may skip the regeneration
outright, printing `'gen_<arch>.inc' is up to date`, because the committed
`.inc` is newer than the prerequisites it tracks. Delete the target first --
`rm -f gen_<arch>.inc && make gen_<arch>.inc`. Also check whether the arch is in
BOTH `INC_targets` and `GINC_targets` (tms320c32, i8086 family, mc68000, ...);
then the `.ginc` needs the same treatment.

Before trusting any diff, confirm the binary is current:
`ls -la test/gen_<arch> test/table_<arch>.o test/dis_<arch>.o` -- if any `.o` is
newer than the binary, it is stale.

This hides well and has cost a full sweep at least once (i960, 2026-09).
`ninja -C test` rebuilds `dis_<arch>.o` but does NOT relink `test/gen_<arch>`,
so a ten-minute regen reproduced the previous baselines BYTE-FOR-BYTE after a
deliberate rendering change.  **Output that is unchanged across a behaviour
change is the symptom, not the reassurance** -- it reads like evidence the
change was a no-op.  Relink first (`ninja -C test gen_<arch>`) and check the
binary's mtime against the sources before drawing any conclusion from the
regenerated file.

---

## Banked memory: physical locations, CPU-address operands

libasm has no linker, so the binary must be the absolute image that goes into
the chip. For a CPU whose memory map controller pages windows of a 64K CPU
space (mc68hc12 `mmc` option) the design that needs NO base-class or driver
change is:

- **Locations are physical.** `org`, labels and `*` are physical addresses;
  the driver writes each byte at `insn.address()` as for every other arch (no
  hooks on `ConfigBase`, `Assembler`, `Disassembler` or `AsmDirective` -- an
  earlier hooks version was rejected for extending the base classes).
- **Operand rule:** a value <= $FFFF is a CPU address; a larger one is
  physical and is mapped to its CPU address through the per-CPU region table:
  a fixed region wins (same memory as its window alias), otherwise a window
  whose page equals the assumed page (directive, or for code on a program
  page its own page; undefined = any). No match -> `OVERFLOW_RANGE`; two
  windows matching (HC12's overlapping pin spaces) -> `OPERAND_NOT_ALLOWED`,
  an error for safety.
- **An instruction must sit in program space** (fixed region or PPAGE window),
  and one below $10000 must be identity-mapped -- otherwise its own labels
  would read back as different CPU addresses. Data directives may go anywhere.
- Branches and PC-relative operands work in CPU space from the instruction's
  mapped CPU address. The disassembler prints a target on the instruction's
  own program page as physical (it matches the labels) and any other as the
  CPU address, which reassembles under the operand rule.
- `PAGE(x) = (x >> 14) & $FF` and `LOW16(x) = $8000 | (x & $3FFF)` (x <=
  $FFFF: page 0, x) give the PPAGE page and window address on all three CPUs
  without CPU context -- function tables get no CPU or page state.
  One-operand `CALL` takes a physical target; a CPU address is an error (no
  page). G* operands stay global (= physical on S12X).
- Region rows are `{base, last, reg, shift, phys}`; window lookups
  (`pageWindow`) return `PAGE_NONE` for fixed rows.
- Same idea as Z180/i8086/H8/500/Z8001, whose location already is the binary
  address; the HC12 family differs only in needing the operand mapping.

## Reference test workflow

The reference test (`test/reference/`) is a hand-written source assembled by
libasm into a committed golden, cross-checked byte-for-byte against an external
assembler. It complements the autogen (exhaustive dis->asm round-trip) with
human-curated, real-syntax coverage.

**Sources** (per arch `test_<arch>`), usually thin wrappers that `include`
shared `.inc` files so variants reuse coverage:
- `test_<arch>.asm` -- libasm syntax. Always present; produces the golden.
- `test_<arch>.asl` -- ASL syntax, when it must differ from `.asm`; the ASL
  rule falls back to the `.asm` when no `.asl` exists. **Most** arches run an
  ASL cross-check.
- `test_<arch>.gas` -- GNU-as syntax. **Some** arches (mostly Hitachi/Moto
  with a GNU target) add a GAS cross-check.

**Targets** (from `test/reference/`, per-arch form skips the wipe):
- `asm-test_<arch>.<ext>` -- assemble the `.asm` via libasm; **this is the
  committed golden.** Bare `asm-test` rebuilds every arch and wipes first (see
  the `asm-test wipes ALL goldens` gotcha below).
- `asl-test_<arch>.<ext>` -- assemble via ASL (`asl ... && p2hex`).
- `gas-test_<arch>.<ext>` -- assemble via GNU-as.
- `<ext>` is per arch: `.hex` (Intel) or `.srec`/`.s28`/`.s19` (Motorola
  S-record). Use the arch's actual extension (check the committed golden).

**The golden is libasm's own output; ASL/GAS are the adjudicators.** libasm
must be byte-identical to the external tool on the golden bytes. A mismatch is
a question, not a verdict -- the manual decides who is wrong (see the
golden-diff adjudication rule below); do NOT regenerate blindly.

**Gotcha -- cross-check overwrites the golden.** `asl-test_<arch>` and
`gas-test_<arch>` emit the SAME filename as the golden (`test_<arch>.<ext>`).
So save the golden, run the cross-check, `diff`, then restore:
```sh
make -C test/reference asm-test_<arch>.<ext>          # (re)build golden
cp test/reference/test_<arch>.<ext> /tmp/lib.<ext>    # save libasm's
make -C test/reference asl-test_<arch>.<ext>          # overwrites w/ ASL's
diff /tmp/lib.<ext> test/reference/test_<arch>.<ext>  # empty == byte-identical
git checkout test/reference/test_<arch>.<ext>         # restore committed golden
```

**Linker @files are generated, never committed.** Where the GAS rule links
(`z8001`, `z8002`, `am29xxx`), `<stem>.ld_flags` holds `--section-start` lines
and any `--defsym`; both `test/reference/` and `test/autogen/` generate it on
demand from a `<ARCH>_TEXT_ORG` make variable, and `clean`/`clean-gas` wipe
`*.ld_flags`.  Keep the origin in that variable, not parsed back out of the
file, so one shared rule can serve both directories.  The origin must match the
`org` in `test_<arch>.asm`.

**Adding coverage:** add lines to the mode-appropriate shared `.inc` (not the
per-variant wrapper) that BOTH libasm and the cross-check tool assemble
identically; regenerate the golden; confirm the ASL/GAS diff is empty. When
libasm and the external tool legitimately diverge (a documented external-tool
bug), keep the reference as-is and record the divergence in a bug report to that
tool rather than contort libasm.

### Assembler-specific directives live in the wrappers

Keep shared `.inc`/`.ginc` files instruction-only; `org`, page directives,
`option` and section switches go in `test_<x>.asm` / `.gas` / `.asl`. When a
directive changes how following lines assemble (`direct $12` turns `ldaa
$FD1234` direct), set it up front in the wrapper and pick operands that stay
meaningful. Where code must sit at several origins, split the include per
block (`test_mc68hc12_mmc_p30.inc`, `_p31.inc`, per-CPU fixed-area `.inc`).

**GNU as across banks**: link with the banked emulation (`m6811-elf-ld -m
m68hc12elfb --bank-window bank`, committed `mc68hc12_mmc.ld`), place each page
section at ld's page address, then `objcopy --change-section-lma` each section
to its physical address (per-CPU rule parameters). This keeps GNU as's own
`call far`, `%page()`, `%addr()`. Write the PC-relative branches ld gets wrong
as `(.+N)`. A tool that cannot express the target at all (ASL: 16-bit address
space) is `ASL_exclude`d with the reason, not given a `dc.b` wrapper.

**libasm-only images still need an independent check**: compute the expected
bytes from the manual formulas in a throwaway script (from the listing's
logical addresses), plus a dis -> asm round trip. The round trip alone only
proves self-consistency.

### Sweeping the whole suite: compare DATA records, ignore container framing

`make -k asl-test` / `make -k gas-test` rebuild every target over the goldens;
`git status` then names each file that differs and `git checkout -- ` restores
them.  Compare only the DATA records -- two framing differences are noise and
carry no encoding information:

- **Intel-HEX type-02/04 extended-address records.** p2hex restates the active
  base at the start of every contiguous block; libasm writes one only when the
  upper 16 bits actually change.  (User's call: these do not matter.)
  A second, sharper form of this: binutils' ihex writer (`bfd/ihex.c`) picks
  the record TYPE by address -- type 02 (extended segment, `base<<4`) below
  1MB, type 04 (extended linear, `base<<16`) at or above it -- while libasm
  always writes 04.  There is no objcopy flag for it.  Where the arch allows,
  fix it by moving the test origin above 1MB rather than by post-processing:
  z8001's reference org went `0x010000` -> `0x100000` and asm/asl/gas became
  byte-identical.  For a segmented arch, 04 is also the RIGHT record -- its
  base IS the segment number, and 02 caps out near 1MB.
- **The S0 header record.** objcopy stores the output file name there, so the
  GAS pipeline writes `/dev/stdout` where libasm writes an empty S0.

```sh
f() { grep -v -E '^(:02[0-9A-Fa-f]{4}0[24]|S0)' "$1"; }
diff <(f "$golden") <(f "$produced")
```

Everything else IS a real difference.  Baseline as of 2026-08: ASL 89/91, GAS
32/40, and every remainder is an adjudicated external-tool bug.

### GAS fill and padding: libasm leaves gaps, gas materializes them

The commonest false "mismatch" against GAS is not an encoding disagreement at
all -- libasm and ASL leave an alignment gap EMPTY while gas and the linker fill
it.  Four distinct sources, each with its own fix:

- **`.align` inside a section** -> zero fill.  Give each include its own
  section instead.  But note gas then rounds the SECTION SIZE up to the section
  alignment, so `.p2align` merely trades a leading gap for a trailing pad; use
  alignment-1 sections plus explicit `--change-section-lma` when the chunk size
  is not already a multiple.
- **Linker-script rounding** (`m68hc11elf`, `m68hc12elf`) appends fill past the
  last real byte.  Convert straight from the assembler output with `objcopy
  -j .text` and skip `ld` entirely -- the ns32032 rule already did this.
- **`.org` inside `.text` double-counted with `-section-start`.**  The org gap
  and the section start ADD, so tms320's `.org 0xc0` + `.text=0x300` landed the
  code at byte `0x600`.  Drop one of them.
- **The linker script's `CONSTRUCTORS` directive** ends `.text` with the
  `__CTOR_LIST__`/`__DTOR_LIST__` words -- 16 bytes on a29k -- which no
  assembler emits.  Link with a script of your own carrying the same section
  order and no `CONSTRUCTORS` (`test/reference/am29000.ld`, copied into
  `test/autogen/` by its Makefile), or skip `ld` entirely when
  the source names no undefined symbol.
- **Object-format rounding that cannot be avoided**: ns32k a.out always pads
  `.text` to 8 bytes (a 1-byte file becomes 8) and tic4x-coff rounds the section
  too.  Accepted as a tool artifact; trailing zeros cannot be trimmed blindly
  because the last real byte is often `0x00` itself.

## Spec is source of truth, not the reference assembler

ASL is the de facto reference for most Hitachi targets and GAS for
GNU-syntax targets. They are USEFUL references but not authoritative
when the chip's user manual disagrees.

Working rule:

1. Implement what the manual says.
2. Cross-check against ASL / GAS to surface ambiguities.
3. When they disagree, follow the manual and document the divergence:
   - A note in the arch's section of this file recording the discrepancy.
   - Optionally a bug report to forward upstream.

H16 example: the bank prefix (`<CRn>`, `<PRn>`) before an EA with no Rn
field (immediate, absolute, PC-relative) is spec-legal per Manual Note 3
to Table 4-7. ASL silently strips it. libasm rejects only the truly
non-round-trippable cases via `ILLEGAL_OPERAND_MODE` in the
disassembler, so `gen_h16.inc` doesn't emit sources that would lose the
prefix on re-asm.

Accumulator-form (A) bit placement is another spec-vs-intuition case:
H16 Figure 4-41 and Note 4 to Table 4-7 put the A bit on bit 7 of the
FIRST byte of the EA field. When the EA leads with a bank-prefix byte,
the A bit goes on the prefix byte, not the inner EA byte.

### Don't infer "illegal" from the reference assembler REJECTING something

A reference assembler refusing a form proves only that *it* can't assemble
it -- not that the encoding is architecturally illegal. Treating an ASL/GAS
rejection as proof of illegality is the inverse of the rule above and leads to
wrongly restricting the disassembler.

TLCS-900 example: the disassembler emits previous-bank register operands
(`DJNZ A'`, via the `C7`/`D7`/`E7` absolute-register prefix with prev-bank
selectors `D0..DF`). ASL `96c141` rejects `DJNZ A'` ("invalid symbol name")
while accepting `LD A,A'` / `EX A,A'`. That looked like a libasm bug -- but the
manual ("Register Bank Switching": all bank registers usable, prev-bank written
`A'`/`WA'`; `DJNZ r,$+3+d8` with no restriction on `r`) shows the form is LEGAL
and libasm's notation is correct. ASL has an instruction-specific limitation;
libasm's dis is right. Verdict: keep the dis as-is, report it to ASL,
and let the autogen round-trip on libasm only (some manual-legal forms simply
aren't ASL-portable; that's fine -- the autogen's job is libasm asm<->dis
inverse, not ASL parity). Always adjudicate against the manual before deciding
a decoded form is malformed.

### `test_illegal` opcode sets come from the manual, NOT the disassembler (Ouroboros)

The illegal-opcode set asserted in `test_dis_<arch>.cpp` must be derived
from the chip manual's opcode map. Do NOT build it by brute-forcing
"which opcodes does `cli/dis` reject?" and asserting exactly those --
that is circular (the snake eats its tail): a wrong table entry (an
opcode marked illegal that the manual defines, or a spurious decode the
manual leaves undefined) launders straight into a green test that
validates nothing.

The disassembler sweep is a CROSS-CHECK only. Build the defined-opcode
set from the manual's opcode map / instruction-set summary, take the
complement over the opcode space, and DIFF that against what the
disassembler emits `UNKN` for. Where they disagree, the manual wins --
fix libasm (or document a deliberate divergence). Only a hand-written,
manual-sourced `test_illegal` (or a verified-against-manual one) counts;
"it matches the current disassembler" is not verification.

Corollary: an ABSENT `test_illegal` can be CORRECT. If the manual says
the opcode space is fully populated, there is nothing to assert -- do
not fabricate one. mc68hc12 (2026-06): the CPU12RM states page 1 is
fully used (256/256) and page 2 = 54 instructions + the TRAP range
(`18 30-39`, `18 40-FF`, all decoding as `TRAP $tn`), so there are no
illegal opcodes and the absence of `test_illegal` is right. The lone
page-1 `$3C` the sweep flagged is the `wavr` WAV-recovery pseudo-op (a
real opcode); left undecoded because ASL also lacks it -- avoid a
libasm-only mnemonic that breaks the ASL round-trip.

Verified-correct manual cross-checks (2026-06): ins8060 `0x88` (SC/MP
Table 5-2 has only `8F DLY` in the `0x8x` row); mc6809 `10 20` and base
`0x18` (Programmer's Reference opcode-map notes: "the 6309 considers
this an illegal instruction"); i8080 V30EMU `0xED` is the CALLN/RETEM
prefix, not illegal (NEC V20/V30 manual); i8096 `0xFE` signed prefix
legal only over the MUL/DIV block (`h4 in 0x40..0x9F, l4 >= 0xC`).

Scanned-image manuals (no text layer) defeat `pdftotext`; read the
opcode-map / instruction-summary page with the PDF image reader instead.

#### i8096 family: which manual covers which CPU

Getting this wrong produces a confidently wrong illegal set, because the
family's manuals do NOT line up with libasm's CPU names.

| source | actually covers |
|---|---|
| `8x9x User's Manual.pdf` section 6.0 OPCODE TABLE (pp. 3-17..3-19) | **8096BH** -- the base `8096` |
| `Intel 16-Bit Embedded Controller Handbook Volume 2.pdf`, 80C196KA "NEW INSTRUCTIONS" | **the KB delta** |
| `8xC196Kx User's Manual.pdf` | KQ/KR/KS/KT/JQ/JR/JT/JV/CA -- **NOT the KB or KC** |
| `8XC196NP User's Manual.pdf` Table A-1 | **80196N** |
| `80C196 Assembler User's Guide.pdf` Appendix D | useless for this: its `*` = "24-bit models", `**` = "specific to 80C196", so it lumps BMOV/CMPL in with XCH/TIJMP/BMOVI/DPTS/EPTS and never splits KB from KC |
| `S80C196KB.pdf`, `S80C196KC.pdf` | electrical datasheets only, no instruction data |

The 8x9x and Handbook scans are OCR-mangled -- read them as images.

Verified sets. 8096BH reserved (8x9x section 6.0, footnote "\*\*Opcodes which do
not have a corresponding instruction will not generate an interrupt if
executed"): `0B 10 1B 1C-1F C1 C5 CD E1 E2 E4 E5 E6 E8-EB EC ED EE F1 F4 F5
F6`. The KB adds exactly six over it, with object-code formats given:
`BMOV C1`, `CMPL C5`, `DJNZW E1`, `PUSHA F4`, `POPA F5`, `IDLPD F6`. 80196N
reserved (NP Table A-1): `10 E5 EE` -- which also confirms `RETI` is NOT on
the N, so ASL admitting it from `CPU80196N` is one CPU too low.

**The 8x9x opcode table is WRONG about XCH, and silicon says so (2026-10).**
It lists `04 XCH` and `14 XCHB` as real 8096BH instructions. On a real
P8095BH none of `04`, `14`, `0B`, `1B` executes as XCH/XCHB: `04` takes two
bytes, not three, and leaves both registers untouched; `14` and `1B` transfer
control somewhere else entirely; `0B` takes three bytes where the real
indexed form is five. All four behave as reserved, matching the table's own
footnote that reserved opcodes do not interrupt but do something undefined.
So libasm is RIGHT to keep all four XCH forms out of the base and KB sets,
and `test_illegal_8096` is right to assert `04`/`14` illegal.

Method worth reusing: a length test pins an opcode's size without decoding it
-- put a breakpoint (`TRAP`, `F7`) on each byte after the opcode and see
which one it stops at, since a longer instruction swallows the intervening
breakpoints. Pair it with a register test (load known values, execute, store
and read back).

Measured on one P8095BH, one run per case, on a retro-bionic i8096 board.
No KB or KC opcode map has been found, so which CPU first had `0B`/`1B` is
still unsourced -- but it does not matter for the KB, whose set is the BH's
plus exactly six.

### Silicon settles what the manual leaves open

When manual, GNU as and ASL disagree on behavior (not syntax), a bench run on
a real part is the final word. A retro-bionic bench (Teensy-hosted, with
bus-cycle capture) runs exact byte sequences against the expected effective
addresses per hypothesis; record the part number and mask set with the result
(the HC912BD32 run confirmed M68HC12 Table 3-3 MOVx offsets).

### Asm and dis must be mutual inverses; adjudicate golden diffs with dis

For a LOSSLESS encoding, `dis(asm(source))` reproduces `source` and
`asm(dis(bytes))` reproduces `bytes`. So when a reference round-trip diffs
-- `make -C test/reference asm-test_<v>.hex` no longer matches the
committed golden `.hex` on a clean tree -- do NOT assume the golden is
stale and regenerate it, and do NOT assume the golden (or the external
assembler that made it) is right either. A golden mismatch is a question
("who is manual-conformant?"), never an instruction ("regenerate the
golden"). Use the disassembler to LOCALIZE the disagreement, then let the
manual decide:

1. Disassemble the committed golden bytes: `dis(golden)`.
2. Disassemble libasm's freshly assembled bytes: `dis(asm(source))`.
3. Whichever bytes libasm's own dis flags `ILLEGAL`/decodes to a value the
   manual forbids is the non-conformant side -- often the GAS/ASL golden,
   not libasm. Check the encoding against the chip manual before changing
   anything. Regenerating the golden blindly launders an asm bug into a
   green test (Ouroboros, above); "fixing" libasm to match a buggy golden
   is the same error in reverse.

CAVEAT -- intrinsically lossy instructions. Source-text round-trip
failure is NOT proof of a bug. Some instructions discard part of the
operand BY DESIGN, so `dis(asm(x)) != x` is correct behaviour. Confirm the
instruction is lossless before treating a non-round-trip as a defect.

TMS320F LDM (2026-06) is the cautionary case where I initially got this
wrong. The c32 round-trip diffed on `ldm` short-float immediates;
`asm("ldm -0.50024")` = `0x09600FFF`, `dis(0x09600FFF)` = `-1.0005`, so I
flagged an asm bug. WRONG: LDM = "load floating-point MANTISSA"; its
immediate is a 12-bit mantissa (exponent 0, value in [1.0, 2.0)), and
`-1.0005` is exactly the mantissa of `-0.50024` -- LDM is lossy by design.
libasm is the manual-conformant side (dis correctly rejects a non-zero LDM
exponent: `mode == M_FLDM && (opr & ~0xFFF) != 0 -> ILLEGAL_OPERAND`); GAS and
ASL were both outliers, storing a full 16-bit short float without zeroing bits
15-12. ASL fixed this in Bld 310, so it is now a GAS-only divergence.
Manual SPRU031F p.13-135 (LDM): "If immediate addressing mode is used, bits
15-12 of the instruction word are forced to 0 by the assembler." Decisive
repro `ldm 156.75,R2`: libasm/manual `0x096201CC` vs ASL `0x096271CC` (GAS
`0x0960FFFF` for the FFFF case); values in [1.0,2.0) have a 0 exponent
nibble so all three coincidentally agree there. Reported to ASL. The only open
libasm question is cosmetic: an out-of-[1,2) LDM operand is silently
mantissa-truncated rather than rejected (the unit test
`TEST("LDM 2.5508, R6", 0x09660234)` enshrines the truncation as intended).
(The branch-displacement lines in the same diff were the cleaner case:
libasm round-tripped to the correct target and only the `.gas` golden
differed -- also a GAS/source divergence, not a libasm bug.)

---

## Asm / Dis classes are pure const (no `mutable` state)

`AsmXxx` / `DisXxx` instances must be logically immutable across
`encode` / `decode` calls -- they hold only configuration (CPU type,
options). Any *cross-instruction* state (prefix carry-over like Z380's
DDIR, CP1600's SDBD, TMS320F's parallel-pair partner) lives on the
`Insn` itself via `Insn::state<T>()`, not as `mutable Xxx _state` on the
assembler / disassembler.

Design:

- `Insn::state<T>()` provides per-Insn typed storage. Small `T`
  (`sizeof(T) <= sizeof(uintptr_t)`) lives inline in an Insn slot; larger
  `T` is heap-allocated and destroyed in `~Insn`. No `<new>` /
  `<type_traits>` includes; placement-new is avoided.
- Each arch that needs cross-instruction state declares a `using State =
  Foo;` (or `struct State { ... };`) inside the `AsmXxx` / `DisXxx`
  class. Access via `_insn.state<State>()` inside `encodeImpl` /
  `decodeImpl`, or `insn.insnBase().state<State>()` from per-mode
  helpers that take an `AsmInsn` / `DisInsn`.
- Helpers that previously read the mutable state member (e.g. Z380's
  `wordMode()`, `lwordMode()`) take an `AsmInsn` / `DisInsn` parameter
  so they can reach the per-Insn state.

Driver pattern: the CLI assembler / disassembler loops reuse one `Insn`
across consecutive calls, so the carried state survives naturally. Tests
that exercise prefix carry-over must do the same -- see "Test code
readability" for the `DTEST` / `DERRT` / `PTEST` pattern.

Status (verified `grep -rn 'mutable ' src/`): no `mutable` member exists
anywhere in `src/`. The only arches that ever needed per-instruction
state -- CP1600 (SDBD), TMS320F (parallel-pair partner), Z380 (DDIR) --
have all been migrated to `Insn::state<T>()`. New arches should follow
the same pattern; do not introduce `mutable` on an `AsmXxx` / `DisXxx`.

---

## Listing a split continuation: `Insn::continueOffset`

A prefix that owns its follower's bytes (CP1600 SDBD decodes the whole
SDBD+follower as one `Insn` so the decode loop advances past both) still
wants to *list* as separate per-instruction lines. `Insn::continueOffset`
(byte offset, default 0) tells the dis formatter where the second display
segment begins, without changing the Insn's `length()`:

- `0` (overlap): the continuation re-displays the owning word at the same
  address -- TMS320F's parallel `||` partner (bytes suppressed on the 2nd line).
- `> 0` (sequential): the owning line shows bytes `[0, continueOffset)` and
  the continuation shows `[continueOffset, length)` at
  `address + continueOffset / addressUnit()` -- CP1600 SDBD (offset = the
  1-word prefix), so `sdbd` lists as its own word and the follower at the
  next address.

Set it next to `setContinueMark_P`:
`insn.setContinueMark_P(mark); insn.setContinueOffset(sizeof(opcode_t));`.
(The multi-line byte-overflow row also divides the byte offset by
`addressUnit()` so word-addressed CPUs show the right address.)

## `StrBuffer::over(heir)` hijacks the buffer -- save and recover

`a.over(b)` copies `a`'s write pointers into `b`. So after
`nameBuffer().reset().over(out)`, the case-aware `out` points into the
*name* buffer, not the operands buffer. Routing a prefix name through the
case buffer (so it honors upper/lower case) must save and recover `out`
and hand the buffer back to the name buffer -- the i8086 LOCK/REP prefix
is the template:

```cpp
auto save{out};
insn.nameBuffer().reset().over(out).text_P(TEXT_PREFIX).over(insn.nameBuffer());
save.over(out);
```

Forgetting `save`/recover leaves `out` aliased to the name storage and
silently corrupts the operands. Writing the name directly with `text_P`
(no `.over(out)`) skips case conversion -- the bug that left CP1600 `SDBD`
upper-case in a lower-case listing.

---

## Test code readability

### Multi-step tests share one `Insn` via `DTEST` / `DERRT` / `PTEST`

When two encodes must share state (prefix carry-over, parallel-pair
continuation), write per-arch wrapper macros that allocate one local
`Insn` and pass it to both `asm_assert` / `cont_assert` calls. Examples
in `test_asm_cp1600.cpp` (SDBD), `test_asm_z380.cpp` (DDIR), and
`test_asm_tms320f.cpp` (parallel `||`).

The Z380-style signature is the convention:

```cpp
DTEST(addr1, src1, dd, addr2, eaddr2, src2, ...op_bytes);
DERRT(addr1, src1, dd, addr2, eaddr2, src2, err, at, ...op_bytes);
```

CP1600's `DTEST` / `DERRT` mirror the same parameter order so the two
arches read identically.

### Two-line PTEST-style layout

For two-step tests, lay each call across two source lines -- one per
assembled instruction -- so the source strings and emitted bytes are
visually paired:

```cpp
DTEST(0x1000, "DDIR W",    DDIR_W,
      0x1002, 0x1002, "LD  I, HL", 0xDD, 0x47);
DTEST(0x1000, "DDIR LW",   DDIR_LW,
      0x1002, 0x1002, "LD  HL, I", 0xDD, 0x57);
```

Within an adjacent run of `DTEST` / `DERRT` calls, column-align the
`src1`, `dd`, `src2`, and first trailing byte/prefix-macro so that
optional `DDIR_*` prefixes line up with bare-byte rows -- same
convention as existing `TEST` runs (`"LD IX, ...", DDIR_IB, 0xDD, ...`).

### `asm_assert` / `cont_assert` take `Insn &`

There is one signature: caller supplies the `Insn`. Ordinary
`TEST`/`ERRT` macros allocate a local `Insn` per call; multi-step
`DTEST`/`PTEST` reuse one across both halves. No no-Insn overload.

### `test_asm_*` must cover data constants AND errors

Every `test_asm_<arch>.cpp` needs (in addition to the happy-path
instruction sections):

- **`test_data_constant`** -- exercises the architecture's data-defining
  pseudos (`dc.b` / `dc.w` / `dc.l`, `fcb` / `fdb`, `.data`, etc.) plus
  any space-reserving directive (`rmb`, `.res`). Cover single values,
  comma-separated lists, and big-endian / little-endian word/long
  encoding. The user has flagged the absence of this section in
  reviews more than once.
- **`test_errors`** -- exercises asm error paths with `ERRT` /
  `AERRT`:
    - `ILLEGAL_SIZE` for missing-suffix-on-sized, wrong-suffix-on-fixed,
      any-suffix-on-no-size, and forbidden-suffix (e.g. `.L` on
      MUL/DIV that only accept `.B`/`.W`).
    - `OVERFLOW_RANGE` for explicit-size operands whose value cannot fit
      (PC-rel `:8` with delta > 127, immediate `:8` out of -128..127).
    - `UNKNOWN_INSTRUCTION` for an unknown mnemonic (a single sample is
      enough).
    - `OPERAND_NOT_ALLOWED` or `UNKNOWN_OPERAND` for non-existent
      registers or out-of-table register numbers (e.g. R16 on a 0..15
      register file).

Happy-path-only `test_asm_*` is incomplete. The error sections catch
both encoder regressions AND missing-position bugs (where the asm sets
the error code but doesn't pass `op` / `scan` so the error-at text is
empty).

### `test_dis_*` must include positive cases

Several arches (SuperH currently) have `test_dis_<arch>.cpp` files
populated only with illegal-instruction tests. That defers all positive
disassembler validation to autogen, which is a much slower feedback
loop and doesn't survive a clean autogen rebuild gone wrong.

Each test_dis_<arch>.cpp should include a representative positive test
per instruction class (one MOV, one ADD, one branch, etc.) so dis
regressions surface in `ninja -C test` rather than in `make autogen`.

### Driver behavior needs `test_formatter_<arch>` cases

Anything the drivers do (address mapping, re-`org`, driver-level errors) is
invisible to `test_asm_*`/`test_dis_*`. Add cases to
`test/driver/test_formatter_<arch>.cpp`; `ASM_MEMORY(cpu, src, listing,
image)` hands back the assembled `BinMemory` so the test can check bytes at
their physical addresses, and `DIS8` at a physical origin checks the logical
`org`s printed. The user likes these.

### CPU-dependent tests branch on the CPU predicate

`run_tests` runs every function once per CPU of the arch; a case valid on one
variant only must sit under `if (m68hcs12x())` etc., with the other variants'
expectation (often `UNKNOWN_INSTRUCTION`) in the `else`.

---

## Per-arch gotchas

### H8/300 / H8/300H / H8S

- Option is `advanced-mode` (NOT `maxmode`); ASL uses that name and we
  avoid the clash.
- BRA auto-promotion is **unconditional** on H8/300H -- BRA/Bcc/BSR with
  8-bit overflow always promote to `:16`. No `smart-branch` flag needed.
  H8/300 still errors with `OPERAND_TOO_FAR` (no long branch).
- Advanced-mode `@aa:16` valid: `[0, 0x7FFF]` or `[0xFF8000, 0xFFFFFF]`;
  assembler errors on values in the gap.
- Advanced-mode `@aa:8` valid: `[0, 0xFF]` or `[0xFFFF00, 0xFFFFFF]`;
  disassembler widens `0xnn` to `0xFFFFnn`.
- M_ADREG decodes R7/ER7 as `SP`; plain M_REG16/M_REG32 keep `R7`/`ER7`.
- **H8S bit operations on `@aa:16`/`@aa:32` put the address BEFORE the
  operation code**: `6A10 aaaa 7300` is `BTST #0,@aa:16`. The prefix carries
  the address width (`6A10`/`6A30`) and whether the operation writes memory
  (`6A18`/`6A38`); four pages reuse the two member lists the `@Rd` and `@aa:8`
  pages already share. No other H8 instruction has an operand ahead of its
  code, so `decodeImpl` reads the address when it consumes the prefix, and the
  assembler's `operandPos()`/`emitInsn()` place the code after it.
- **`isPrefix()` is NOT super-prefix aware.** `pageMatcher` compares
  `insn.superPrefix`, but `isPrefix(code)` only sees the code, so adding a page
  makes its prefix match everywhere. Adding `0x6A10` therefore hijacked
  `0100 6A10` -- `MOV.L @aa:16` under the 0x0100 super-prefix -- and the
  generator explored a space it could never exhaust: four sweeps ran 3h50m each
  and emitted 7 lines. Any new prefix page needs the CPU and the super-prefix
  tested explicitly:
  `hasExr() && insn.superPrefix == SPRX_NONE && isBitAddrPrefix(opc)`.
  Without the CPU half, plain H8/300 reports NO_MEMORY where it should report
  UNKNOWN_INSTRUCTION.
- ASL has **no H8S support** (`codeh8_3.c` stops at H8/300H), which is why the
  h8s variants are in `ASL_exclude` and have no `.asl` reference. GNU as
  (`h8300-elf-as`) is the only adjudicator there. `test_h8300h` vs ASL differs
  on main already -- a pre-existing divergence, not something a branch broke.

### TLCS-900

- Two-level encoding: prefix byte (operand context) -> second byte
  (operation). PrefixMode: `PM_NONE`, `PM_REG{8,16,32}`,
  `PM_MEM{8,16,32}`, `PM_MEMD`, `PM_BLOCK`, `PM_RETCC`.
- **Page order is the encoder's preference lever**, and the file already says
  so ("TABLE_DST_MEM precedes TABLE_SRC_MEM so the encoder prefers the
  ASL-style form"). To prefer ONE entry over an earlier page, give it a page of
  its own rather than moving a whole page: promoting all of `TABLE_SRC_REG`
  ahead of the unprefixed table costs `POP A` its one-byte `15` and `PUSH XWA`
  its `38`. `TABLE_SRC_REG_SHORT` holds just `LD r,#3` and `CP r,#3` for that
  reason.
- A short form living alongside a full-width encoding of the same instruction
  needs a value check in `acceptMode` (which sees the Operand), not in
  `modeMatch` (which does not). Without it `LD A0,034H` silently became
  `C7 00 AC` -- `0x34 & 7`.
- The disassembler must never leave a valid encoding undecodable just because
  the assembler prefers another: both `C8 AB` and `C8 DB` decode, and because
  the short form is the one the encoder picks, both round trip.

#### Operand-size restrictions are table-driven (`SizeMask`) -- and PERVASIVE

Many TLCS-900 instructions accept only a subset of B/W/L. The size an entry
permits lives in a 3-bit `SizeMask` on `Entry::Flags` (`entry_tlcs900.h`): the
`E0/E1/E2` macros take it as an explicit arg RIGHT AFTER the mnemonic (no `EnS`
variant), `SM_ANY` = unrestricted. Enforcement is a one-hot intersection: infer
the operand size from the register name (or the prefix mode) and intersect with
`entry.sizes()` -- empty => `ILLEGAL_SIZE` (asm, `acceptOperands`) /
`UNKNOWN_INSTRUCTION` (dis, `opcMatchCode`). This replaced the old size-specific
AddrModes `M_SRCB/W/WL/BW` (folded into plain `M_SRC` + mask). Manual B/W/L flags
(H2 Instruction List, ~CPU900-150; authoritative for size, NOT opcodes):

- `--L` (long only): `LINK`, `UNLK`.
- `-W-` (word only): `MIRR`, `MINC*`/`MDEC*`, `BS1F`/`BS1B` (source reg is r16),
  `PUSH/POP SR`, `MUL/MULS/DIV/DIVS` (result-doubling).
- `BW-`: `DJNZ`, `NEG`/`CPL`, `EX` (reg,reg AND mem,reg), `SCC cc,r`, the bit
  ops in the REGISTER form (`ANDCF/ORCF/XORCF/LDCF/STCF/RES/SET/CHG/BIT/TSET
  #b,r`), all `<W>`-capable memory ops (byte + `W`-mnemonic word).
- `-WL`: `EXTZ`/`EXTS`/`PAA`, `PUSH/POP R` (compact 18+zz/38+zz), `LDA`.
- `B--` (byte only): `DAA`, `LDX`, `RLD`/`RRD`, the bit ops in the MEMORY form
  (`ORCF/RES/SET/CHG/TSET #b,(mem)` = `B0+mem:...`).

BEWARE: the byte/word split of an instruction may be spread across BOTH forms
(register `BW-` vs memory `B--`), so one `SizeMask` per Entry, not per mnemonic.
`SM_ANY` on a genuinely-restricted entry is a latent over-generation bug (the
disassembler emits illegal-size encodings) -- see the ASL cross-check below.

#### The ASL autogen cross-check is what catches size over-generation

Unit + reference tests only cover the forms you thought to write; they will NOT
catch a disassembler that emits an illegal-size variant of an instruction you
didn't restrict. `make -C test/autogen asl-gen_tlcs900*.hex` assembles EVERY
disassembled instruction through ASL and is the tool that surfaces this:
`invalid operand size` / `undefined operand size` from ASL == a missing
`SizeMask` in libasm (verify against the manual, then restrict). This is how the
EX/bit-op/BS1F/SCC restrictions above were found -- the libasm autogen
round-trips clean (asm(dis(x))==x) yet ASL rejects the over-generated sizes.
Distinguish real bugs from ASL limitations by the message:
- `invalid/undefined operand size` -> almost always a real missing `SizeMask`.
- `wrong number of operands` on prev-bank `(Xrr'+)` forms -> ASL parser
  limitation; libasm is manual-correct (keep, libasm-only round-trip).
- MINC/MDEC hang in ASL is a NON-power-of-2 buffer operand; libasm's dis only
  emits valid `2^n` operands so the autogen never triggers it.
- **Pre-dec/post-inc has NO single-byte compact form -- the two-byte
  `C4/C5` form is the only manual encoding (2026-06 correction).** The
  TLCS-900 CPU manual opcode map (16x16, identical in 900 and 900/L1;
  see "TLCS-900 CPU.pdf") assigns the `0x80-0xBF`
  range to register-indirect and `(Xrr+d8)`: rows 8/9/A/B (scr.B/W/L, dst)
  low-nibble `0-7` = `(XWA)..(XSP)` register-indirect, `8-F` = `(Xrr+d8)`.
  So `0x80` = `(XWA)` register-indirect, `0x84` = `(XIX)`. Pre-dec/post-inc
  live in rows C/D/E/F: low-nibble `4` = `(-xrr)`, `5` = `(xrr+)`, i.e.
  `(-XIX)` = `C4 F0`, `(XIX+)` = `C5 F0`. The "mem specify code" table
  lists `(-r32)`/`(r32+)` ONLY as that two-byte form. libasm's old
  single-byte "compact" path (`asm_tlcs900.cpp` `memPmSimpleBase|reg` ->
  `0x80` for `-XIX`) COLLIDES with `(XWA)` register-indirect and is
  non-conformant: on silicon `LD A,(-XIX)` -> `80 21` decodes as
  `LD A,(XWA)`. ASL's `C4 F0` is correct; the `complex-indir` option had
  the sense backwards (the "complex" form is the only legal one). Fix
  tracked on branch `tlcs900`. This RETRACTS the former "ASL emits long
  form" report (ASL was right).
- `0x83` (would-be `(-XSP)` if a compact form existed) and `0xB0` are the
  `PM_BLOCK` / `PM_RETCC` prefixes; this is why the range can't carry a
  compact pre-dec form there either.
- MINC/MDEC instructions cause ASL infinite loop -- excluded from the
  hand-written reference test (but present in the autogen).
- `MINC{1,2,4}`/`MDEC{1,2,4}` buffer operand (`M_BUF`) must be `2^n` with
  `2*step <= size <= 0x8000` (`step = 1<<(opc&3)`; manual n=(opc&3)+1..15),
  encoded as `size-step`. The dis must reject other encodings (`ILLEGAL_OPERAND`)
  or it emits non-round-trippable operands; the asm enforces the same bound.
- `LDC` control-register size check (`M_CREG`) must map ALL register prefix
  modes, including `PM_ABREG8/16` (absolute-register form), to the expected
  size -- not just `PM_REG8/16`. Missing the ABREG cases let a byte/word
  prev-bank register match a 32-bit control reg (`LDC DMAS0, A'`).
- Prefix-operand decode errors must propagate: an invalid `PM_MEMD` complex
  sub-byte sets the error on the prefix `Operand`, so `readPrefix` must
  `return insn.setError(prefixOp)` (not `insn.getError()`, which stays OK).
- Previous-bank (`'`) registers (`DJNZ A'`, `LD A'`, via the `C7/D7/E7`
  absolute-register prefix) are LEGAL per the manual -- do NOT restrict the
  dis. ASL rejects some (DJNZ/LINK); that's an ASL limitation,
  so the autogen round-trips libasm-only.
  See "Don't infer illegal from the reference assembler REJECTING something".
- `MULA xrr` is encoded `D8+r 19` (PM_REG16 prefix carrying a 32-bit operand,
  per manual `MULA rr  D8+r :19`); there is NO E8/PM_REG32 form. The
  `M_R32SRC` mode handles this: `resolveMode` returns `M_REG32` only at
  `PM_REG16` (so the asm picks D8), and the dis rejects `M_R32SRC` at any
  other prefix mode. Operand is 32-bit (`MULA XBC`, not `MULA BC`).
- **Register-mode model (per the L1 manual comparison table, CPU900L 1-1 --
  the "900/H, 900/L1" cells are ONE combined column; do NOT read them as
  separate variants):** base `900` boots MINIMUM and supports both modes;
  `900/L` boots MAXIMUM and supports both (it is NOT min-only); `/H`, `/H2`
  are MAXIMUM-only (reject `MAXMODE OFF`). `setCpuType` default is
  `_maxMode = (cpuType != TLCS900)`. **`TLCS900L1` was dropped** (same core as
  `/H`). Earlier notes/code said "/L is MIN-only" -- that was WRONG.
- **Do NOT cite a maximum-mode-only manual (900/L1, 900/H2) for minimum-mode
  behavior -- they don't describe min mode.** Use the base 900 manual, which is
  the authority for min mode. Manual text-layer status (2026-07):
  "TLCS-900 CPU.pdf" (base, min authority), "TLCS-900H CPU.pdf", and
  "TLCS-900H2 CPU.pdf" all have `pdftotext`-able text now; only "TLCS-900L CPU.pdf"
  is image-only. The OCR'd opcode-hex columns are noisy (e.g. "QIWA" for XWA) --
  trust the instruction-LIST size column and prose over the OCR'd hex. The base
  manual sec.3.4 Note 2 is the min-mode 16-bit-addressing citation.
- **Minimum-mode memory addressing is 16-bit for the GENERAL pairs.** A memory
  base register is written with its mode-appropriate width: min mode uses the
  16-bit name for XWA/XBC/XDE/XHL (`(HL)`, `(-HL)`, `(HL+)`, `(HL+d)`, block
  `(DE+),(HL+)`, and previous-bank `(HL')`); the index registers
  XIX/XIY/XIZ/XSP are mode-agnostic ALWAYS 32-bit (`(XIX)`). Same encoding as
  the 32-bit spelling (`LD A,(HL)` == `LD A,(XHL)` == `83 21`). Implemented as
  mode-dependent width: asm `parseMemBaseReg(..., maxMode)` + `baseWidthOk`
  (index -> size 2, else min -> size 1), dis `baseRegSize()` renders the base
  by mode. A general register is still illegal as a 32-bit DATA operand in min
  mode -- only the MEMORY-base use drops to 16-bit. Don't leave a stale
  "general memory base is illegal in min mode" guard in the dis; it must
  decode and render 16-bit. ASL matches.
- Reference wrappers set `MAXMODE` explicitly per section because the ASL cpu
  selectors (`93c141`/`96c141`) are GENERIC, not variant-specific. `_common`
  is mode-independent (included in BOTH the ON and OFF sections); `_maxmode`
  goes only in the ON section, `_minmode` (16-bit general forms) only in the
  OFF section. The `MIN`/`MAX`/`NORMAL` instructions are opcodes under test,
  NOT assembler-mode switches -- only the `MAXMODE` directive changes mode.
- `maxMode()` lives on `Config`, not the `AsmTlcs900` class.

### H8/500

- Mode is set by the `maximum-mode` option (asm + dis; the gen tool gets
  it via `OPT_h8500 = -O maximum-mode=on`) OR the assembler-only
  `.maxmode on` pseudo. The bare `maxmode` was dropped: libasm `.asm`
  wrappers use `.maxmode`, ASL `.asl` wrappers use bare `maxmode`.
- Minimum mode is a flat 16-bit PHYSICAL space (no page registers);
  maximum mode reaches the wider physical width (24-bit H8/500, 20-bit
  H8/520 and H8/530) via page registers. An org past 0xFFFF only makes sense in
  maximum mode.
- Page jumps (PJMP/PJSR/PRTS/PRTD) are `maxModeOnly` pages, gated by BOTH
  the assembler (`acceptOperands`) AND the disassembler (`pageMatcher`):
  both skip the page when `!insn.maxMode`. (The dis used to decode them
  regardless; that asymmetry was removed so minimum-mode autogen
  round-trips -- min-mode pjmp bytes now decode as UNKNOWN.)
- Driver: `H8500Directive : HitachiDirective` -- `.assign` + Motorola
  S-record default output. It `disablePseudo(".set")` because H8/500 owns
  `.set <page register>, <value>`, which records an assumed DP/EP/TP/CP/BR
  value so absolute operands shorten to `@aa:16` (DP) / `@aa:8` (BR).
  Undefined (initial, value < 0) drops the high bits silently; once
  defined, mismatched high bits are `OVERFLOW_RANGE` -- the Z8 `rp` /
  mc6809 `setdp` pattern, implemented INSIDE `AsmH8500` (not the driver).
- Minimum-mode targets `h8500n`/`h8520n`/`h8530n` mirror `h8300hn`: same
  CPU, min mode, S19. `TGT_h8500` lists all six; `OPT_h8500/h8520/h8530 =
  -O maximum-mode=on` (n-variants get no OPT); add `CPU_h8500n` etc.;
  S28_OUTS = the max variants, S19_OUTS += the n variants. Per-variant
  reference + autogen `.asm/.asl/.gas` wrappers; mode set in the wrapper.
- Physical-width-aware autogen org: `gen_h8500.cpp` picks `0x010000`
  (max, S2 records) vs `0x0100` (min, S1) from `dish8500.maxMode()`. A
  hardcoded high org breaks min-mode generation (>0xFFFF overflows the
  16-bit space, so the generator emits nothing).
- libasm == ASL byte-identical on the reference goldens (S2 max / S1 min,
  via `-F Moto +5`). GAS (h8500-hms) DIVERGES and is in autogen
  `GAS_exclude`: it encodes `:g` immediates with a leading EA prefix
  (`A0 04 55`) where libasm/ASL use the EA-suffix form (`04 55 70`). The
  exhaustive autogen is also in `ASL_exclude`: the dis emits MOV:F
  negative displacements as `@(-0x7F,FP)`, which ASL rejects (it wants
  `@((-0x7F),fp)`); the curated reference spells them ASL-compatibly.
- `fp-alias` (default on) shows R6 as `FP` in all contexts;
  `sp-alias` (default on) shows R7 as `SP` *in indirect addressing only*
  (so `CMP:I SP, R0` stays `R7` -- the helper is `outAddrReg`, not
  `outReg`).

### i80386 / i8086

- `M_CX`/`M_ECX` in the table with a `// Assembler` comment marker means
  those modes are used only in `asm_i8086`'s `parseOperand` (not
  encoded by the table dispatch).
- LOOP CX/ECX: NASM accepts pre-80386 too; mirror that. Reference tools
  for verification: `i386-elf-as` and `nasm`.
- Prefix emission order in `emitInsn`: segment override, addr32(0x67),
  data32(0x66), lock(0xF0), rep(0xF2/0xF3), opcode prefix. Matches GAS
  output (load-bearing constraint for the gen reference tests); the
  Intel SDM order is the reverse but both decode identically.
- EBP as base register always requires `mod>=1` in SIB encoding (avoid
  disp32-only interpretation).
- D-suffix opcodes route via `TCODE32_00` secondary search when
  `cpuSpec.has32bit()`.
- **Use `-mtune=i386` for the i80486 gas rule.** `-mtune` here only picks
  between equivalent encodings (it does NOT gate the instruction set -- that is
  `-march`, which is too strict for this reference because it rejects the
  8087-only `FENI`/`FDISI`/`FNSETPM` the test exercises). The i486 tuning makes
  gas prefer the 3-byte `C0 /4 ib` shift form over the canonical 2-byte
  `D0 /4` that libasm and nasm emit for a shift count of 1. `-O0` does not
  disable it.
- `TGT_i8086` covers the whole family: `i8086 i80186 i80286 i80386
  i80486 v30`. There is no separate `TGT_i80386` / `TGT_i80486`; new
  variants extend this one list and the single `gen_i8086` binary
  generates for all of them.
- `gen_i80386.ginc` and `gen_i80486.ginc` each take ~3-4 hours to
  regenerate (~225k / ~236k lines). The other i8086-family `.ginc`/`.inc`
  files complete in seconds. Plan accordingly when reseting size-check
  semantics or any change that invalidates the cached outputs.
- `TestGenerator::ignoreSizeVariation()` (when present) is a no-op for
  i8086/i80186/i80286/V30 output -- those CPUs have no ambiguous-size
  instructions -- but materially changes i80386/i80486 output (extra
  `and ax`, `shl ax`, `mul ax` variants). If you toggle this flag,
  only the 32-bit `.ginc` files need regeneration.

### CP1600

- **SDBD is a prefix with continuation, NOT stateful.** The dis reads and
  decodes the WHOLE following SDBD-aware instruction in one decode, emits
  `SDBD`, and sets a continuation mark (`TEXT_null`); the continuation
  re-decodes the prefixed instruction from the insn's OWN bytes (skip the SDBD
  word) via a throwaway Insn. This is why `hasContinue` is safe here and does
  NOT break TMS320F parallel: the continuation never advances the caller's
  reader or changes `insn.length()`. (The old `mutable bool _sdbdPrefix`
  carry-state design polluted the standalone decode of the opcode right after
  SDBD in the autogen sweep -- `0x0002`=EIS vanished.) Gotchas: propagate
  `NO_MEMORY` (don't `resetError` it) so the generator supplies the follower;
  set `sdbd_prefix = is_sdbdAware()`, not unconditionally true. `is_sdbdAware()`
  per Manual 3.3.4: ext-ref `MVI`..`XOR` (excludes `MVO`), immediate (mode 7),
  indirect modes 1-5; EXCLUDES mode 0 (direct) and mode 6 (R6 stack).
- **Every table EA mode needs a `decodeOperand` case.** `M_STACK` (R6-stack
  source; table entries `0136x` for `ADD@`/`SUB@`/`CMP@`/`AND@`/`XOR@ R6`) had
  no case and silently emitted an empty operand -- invisible until the autogen
  `.inc` failed to re-assemble. Fix: `case M_STACK: outReg(out, 6)`.
- `--sp-alias` / `--pc-alias` default OFF. The original reason (ASL took only
  `R0`-`R7`) is GONE -- ASL Bld 311 accepts `SP`/`PC`, and the reference now
  covers them (`MVI@ SP,R0` = `02B0` = `MVI@ R6,R0` in both tools). Flipping the
  dis defaults to ON would match the manual (R6=SP, R7=PC) but changes dis
  output and needs a cp1600 autogen regen, so it is still an open decision. The
  assembler accepts `SP`/`PC` unconditionally as `R6`/`R7` synonyms.
- `gen_cp1600.cpp` sets `c-style` so immediates emit as `0x..`: CP-1600's
  native hex is IBM `X'..'`, which the standard tokenizer does NOT collapse, so
  the autogen explodes to 25k+ lines without it. See "ASL native hex" below.
- **All three CP-1600 items reported to ASL are FIXED in ASL
  Bld 311**, plus the SP/PC feature request: the branch-condition aliases
  `BLGE`/`BLLT`/`BEQ`/`BNEQ` assemble, and the SDBD negative-immediate high byte
  no longer sign-extends (`MVII -1,R1` and `MVII X'FFFF',R1` both give
  `02B9 00FF 00FF`). So there is no `WORD`-substitution workaround any more --
  `.asm` and `.asl` share one `test_cp1600.inc`.

### H16 (HD641016)

- Source of truth is the **HD641016 User's Manual**, not ASL. ASL has
  bugs around bank-prefix encoding (silently strips `<CRn>`/`<PRn>` for
  certain following EA modes); libasm follows the manual.
- **DADD is 0xA3 and DSUB 0xA7**, not the 0x83/0x87 that ASL uses and libasm
  once had. Row A is `BEQ, DADD, Bcc:G, DSUB, BSR, SA, EXTU, DNEG`, so each
  decimal instruction sits in the reserved size-3 slot of the branch before it
  (`BEQ` spans 0xA0-0xA2, `Bcc:G` 0xA4-0xA6). h16 is in `ASL_exclude`
  because of it, and
  the curated reference omits the two so it stays byte-identical with ASL.
- Reading the manual's garbled instruction-format boxes: **skip the leading
  table-border character, take the next 8 bits**. That rule reproduces the
  opcode libasm already had for 17 of the 24 boxes that extract cleanly, which
  is what makes it trustworthy on the two that differ. Of the rest, four are
  page prefixes the extractor misreads as sub-table entries and one is a size
  variant (`SWAP` is `ISZ_OBIT0`, so 0xEB is 0xEA's other size).
- **A reserved size code can be a real opcode.** 0xA7 is `Bcc:G`'s reserved
  Sz=11 prefix; `pageMatcher` excluded it but `prefixMatcher` did not, so DSUB
  was swallowed as a prefix byte and died as a two-byte unknown. Both now share
  `reservedSize()`. Check both matchers when a table entry lands on a
  prefix page's reserved code.
- Accumulator-form (A) bit lives on bit 7 of the FIRST byte of the EA
  field. When a bank prefix is present, A goes on the prefix byte
  (e.g. `0xF4` not `0x74` when the destination is R0). Manual ref:
  4.5.2 Figure 4-41 + Note 4 to Table 4-7.
- Bank prefix (`<CRn>` = 0x70, `<PRn>` = 0x74) before an EA with no Rn
  field (PC-relative 0x7D, immediate 0x71-0x73, absolute 0x75-0x77, or
  another prefix 0x70/0x74) is spec-legal but cannot round-trip the
  source text. The disassembler rejects those byte combinations as
  `ILLEGAL_OPERAND_MODE` so `gen_h16.inc` doesn't emit unbuildable
  sources.
- PC-rel: dis emits absolute target without size suffix; asm reads
  source as target and auto-picks size. Strict `OVERFLOW_RANGE` check
  for explicit `:8`/`:16` hints. See "PC-relative addressing" above.
- CRn / PRn are register names (not just bank-prefix syntactic sugar);
  parser accepts `R<n>` / `CR<n>` / `PR<n>` for n = 0..15.
- Mnemonic parser allows `:` and `.` as instruction letters
  (`HitachiSymbolParser`-style) so `ADD:G.W` parses as one mnemonic.
- ISZ_AUTO (RTD / LINK) auto-picks the displacement size from the
  operand's value when no explicit suffix is given.
- Don't-care (`*`) bits in EA expansion bytes are zero-required by the
  disassembler (`requireZero` with the appropriate mask). Stricter than
  the manual but ensures canonical encoding.
- Autogen ORG is `0x1000` (past the 0x000-0x3FC exception vector table).

### SuperH (SH-1 / SH-2 / SH-DSP / SH-2E / SH-2A)

- libasm CPU names use the hyphenated form `SH-1` / `SH-2`; ASL's
  family-code names `SH7000` / `SH7600` belong only in `*.asl`
  source wrappers (and in `searchCpuName` if you need an alias, but the
  current code does not -- per user direction "no need to handle
  ASL-compatible CPU name").
- `*` is registered as the location symbol via `SuperHSymbolParser`
  (`locationSymbol(scan, '*')`), so the assembler's `parseExpr`
  resolves `*+N` to the absolute target. Do *not* literally match `*`
  in `parseOperand` for PC-relative addressing -- let the expression
  parser handle it.
- PC-relative operand emission in `DisSuperH` uses `outRelAddr` (not
  `outAbsAddr`). With `--relative` + `--origin-char=*` the
  disassembler emits `@(*+N,PC)` (Hitachi/ASL idiom); with `--gnu-as`
  it emits the absolute target form GAS expects. Drive both modes
  from `gen_superh.cpp`: GAS branch sets `gnu-as`, the default branch
  sets `relative`, `origin-char`, and `motorola-style` (ASL requires
  `$NN` hex, not `H'NN`).
- **`gen_superh.cpp` sets `origin-char` AFTER `driver.main()` has parsed the
  command line**, so `-O origin-char=` / a `GENOPT_*` entry cannot override it
  and fails silently. It long held `$` while this document already claimed `*`
  -- the drift cost a whole arch its autogen ASL check. `*` is the right value
  regardless of ASL: the generated text is Motorola style, so `$` is ALSO the
  hex prefix and `$+4` is ambiguous with a hex literal in a file that writes
  `$00101`. libasm's assembler takes `$`, `*` and `.` alike.
- Inside `@(...)` operand lists use `out.letter(',')` rather than
  `out.comma()` -- ASL rejects the space that `comma()` inserts
  between `N` and `Rn`. Outer (between-operands) commas can still
  use `comma()`.
- The reference `.s37` DOES byte-match all three tools now. Once
  `SuperHSymbolParser` derives from `HitachiSymbolParser` (location
  counter `$`/`*`/`.`), the `.asl` can `include` the same `.inc` the
  `.asm` uses instead of carrying its own copy, and ASL agrees
  byte-for-byte. (The older note here claiming ASL encodes `@(*+N,PC)`
  as an absolute `d = (current_addr + N) / 2` no longer applies.)
- **The `.ginc` must mirror the `.inc` instruction-for-instruction** or the
  byte streams cannot match. GAS accepts a NUMERIC PC displacement, and
  `@(N,pc)` means exactly what libasm's `*+N` means (`mov.w @(4,pc),r0` =
  `9000`); branches take `.+N` / `.-N`. Write those rather than real literal
  pools with labels, delay-slot NOPs and `.word`/`.long` data -- pools shift
  every following address and make the file uncomparable. GAS still rejects an
  unaligned `MOV.L`/`MOVA` PC-relative target ("offset to unaligned
  destination"), so those operands must land 4-aligned.
- Pass `-j .text` to `sh-elf-objcopy`: the linker script emits a
  `DEADDEAD` stack marker at `0x3FFFFF00`, which both adds a spurious
  record AND forces the whole file to S3 (32-bit address) records.
- The autogen GAS targets DO build: `gas-gen_sh1`, `gas-gen_sh2`,
  `gas-gen_shdsp` and `gas-gen_sh2e` all assemble clean. (An older note here
  said they could not, because the stream had no NOP padding for unaligned
  MOV.L PC-relative loads; that is no longer true.) `sh2a` is the one
  exception and is in autogen `GAS_exclude` -- see the MOVI20S bullet below.
- `superh` belongs in both `INC_targets` and `GINC_targets` in
  `test/autogen/Makefile` (not `NOASM_targets`). `gen_superh -i`
  produces `gen_sh*.inc` (ASL-syntax) and `gen_superh -i -g` produces
  `gen_sh*.ginc` (GAS-syntax); the hand-written `gen_sh*.{asm,asl}`
  wrappers `include` the appropriate `.inc`/`.ginc` for the asm tool.

#### ASL Bld 312: SH-DSP / SH-2E / SH-2A adjudication

Bld 312 reworked ASL's SH target (SH-2A, FPU, DSP) and gave `shdsp`, `sh2e`
and `sh2a` an external adjudicator for the first time.

- **ASL CPU mapping**: `SH7615` = SH-2 + DSP, `SH7201` = SH-2A no FPU,
  `SH7205` = SH-2A + fpu32|fpu64. `FPU` and `DSP` both default OFF, so the
  `.asl` wrapper needs `fpu on` / `dsp on`. `get_act_cpu_flags()` only ever
  REMOVES flags, so an FPU cannot be added to a core that lacks one: SH-2E
  (SH-2 core + single FPU) has no exact ASL CPU and is checked against
  `SH7205` as a superset -- which cannot catch an SH-2A instruction wrongly
  accepted there.
- Four ASL bugs: `NOTT`
  encoded `00D0` where the manual gives `0068`; `PCSymbol = "*"` where the
  SuperH Assembler User's Manual 1.5 makes `$` the location counter; SH-DSP
  missing the `MOD`/`RS`/`RE` control registers and `LDRS`/`LDRE`/`SETRC`.
  The last two are BUGS, not absent features -- ASL declares DSP support and
  then rejects documented members of the same set. Do not soften a
  declared-but-incomplete target into a "feature gap".
- `test_sh2a` and `shdsp` stay in `ASL_exclude` for those; everything else
  passes. SH-2E is byte-identical over 852/852 bytes, SH-2A over 989/990
  (only `NOTT`), SH-DSP over 960/960 once the 46 unsupported forms are set
  aside.
- **`MOVI20S` takes the 20-bit field, not the shifted result** (SH-2A manual
  6.3.22: `MOVI20S #imm20,Rn`, example `MOVI20S H'7FFFF,R0` leaves
  `R0 = H'07FFFF00`). ASL agrees; **GNU as is the wrong one here** -- it wants
  the operand pre-shifted and rejects the manual's own example with
  "misaligned offset". So
  `test_sh2a.ginc` carries the shifted spelling BY HAND and `sh2a` is in the
  autogen `GAS_exclude`. Otherwise `sh-elf-as` agrees byte-for-byte on all
  five variants and is the better SuperH adjudicator.
- Two more manual-conformance fixes found the same way: the `TST`/`AND`/`OR`/
  `XOR` immediate is ZERO-extended (the manual sign-extends only for the
  arithmetic ops), and `FMAC` disassembles its implied `FR0` in full
  (`FMAC FR0,FRm,FRn`). The SuperH table is `(src,dst)` only, so the latter is
  a flag bit at `_attr[15]`, not a third operand.
- **`ASL_exclude` in `test/autogen/Makefile` used to be bypassed** for any
  target owning a `.asl` wrapper -- the second `foreach` re-added it
  unconditionally, so exclusions never took effect for those (`h16` was
  leaking too). `test/reference/Makefile` had always applied the filter to the
  union; autogen now matches. Check both when adding an exclusion.

### MC68000 family FPU / PMMU defaults

- **A CPU's on-chip units follow from the CPU**, not from `reset()`:
  `setFpuType(mc68040() || _cpuSpec.fpu != FPU_NONE ? FPU_ON : FPU_NONE)` and
  `setPmmuType(mc68030() || mc68040() || _cpuSpec.pmmu != PMMU_NONE ? PMMU_ON
  : PMMU_NONE)`. Turning either off names the cut-down parts: `--fpu=none` is
  an MC68LC040, `--fpu=none --pmmu=none` an MC68EC040.
- **`reset()` runs before a CPU is chosen**, and `PMMU_ON` matches only
  MC68020/030/040 -- so a PMMU asked for there degrades to `PMMU_NONE`, and
  selecting the CPU later maps that to the PMMU-less `PMMU_MC68EC040`. That is
  why `dis -C MC68040` could not read back what `asm -C mc68040` had just
  written. `FPU_ON` does NOT have this problem: it resolves to an MC68881 for
  any CPU, so it survives `reset()`.
- The assembler only *appeared* correct through an accident: `setCpuType`
  tested `pmmu == PMMU_NONE ? NONE : ON`, and `PMMU_MC68EC040` is not
  `PMMU_NONE`, so a later CPU selection silently promoted the EC part to a full
  PMMU. Do not reason from `reset()` alone -- probe the object.
- **Preserve what was already requested.** `mc68040() ? FPU_ON : FPU_NONE`
  looks equivalent and is not: it takes the FPU away from the disassembler on
  every earlier CPU and drops 3588 F-line instructions from each generated
  test. The disassembler decoding MC68881 floating point on any CPU is
  deliberate.

### MC68851 (MC68020 PMMU)

- **The immediate function code AND the mask are both 4 bits.** MC68851 User's
  Manual A-6 gives the extension word as
  `001 | mode(3) | 0 | mask(4)@8-5 | fc(5)@4-0`, and A-7 (PFLUSH), A-10 (PLOAD)
  and A-22 (PTEST) all spell the FC field as
  `1DDDD - Function code is specified as four bits DDDD`. The part drives FC0
  *through FC3* (3.4, plus the p.1-1 feature line "4-Bit Function Code"). libasm
  is already right: `op.val.overflow(mc68030() ? 7 : 15)` in asm,
  `mc68030() && >= 8` in dis.
- **MC68030 narrows BOTH to 3 bits**: 2-5 says SFC/DFC have "only bits 2:0
  implemented ... for MOVES, PLOAD, PFLUSH, and PTEST"; Table 5-1 (5-2) calls
  FC0-FC2 a "3-bit function code"; and the TT registers' FC BASE / FC MASK are
  each "This 3-bit field" (9-58). The MC68030 UM has no encoding appendix, so
  the bit positions are inferred from the widths, not quoted.
  (Erratum worth knowing: that manual's Table 3-10 on 3-13 lists the MC68040
  syntax `PFLUSH (An)` / `PTEST (An)`, contradicting its own timing table on
  11-59 which correctly shows `PFLUSH <fc>,#<mask>`.)
- **MC68040 has neither field.** Its PMMU forms are `PFLUSHA` / `PFLUSHAN` /
  `PFLUSH (An)` / `PFLUSHN (An)` / `PTESTR|W (An)` (M68040 UM 1-19, 3.7.2/3.7.3
  on 3-33), and PTEST takes the function code from DFC. libasm keeps them in a
  separate table that never uses `M_IMFC`/`M_IMFM` -- do not "fix" the range
  checks for this part.
- **ASL caps the immediate fc at #7** (`DecodeFC()` evaluates it as `UInt3`,
  then does `*erg = Val + 16`, so `11000`..`11111` is unreachable). 16 lines of
  `test_mc68851.inc` therefore do not assemble and
  `make asl-test_mc68020.s37` is **expected red** until ASL widens it. The sharpest argument for reporting it is ASL's own inconsistency: its
  mask check is already `> 15`, so it accepts `#7,#15` but rejects `#15,#7`.
- `test_mc68020` is deliberately **NOT** in `ASL_exclude` -- excluding it would
  drop the ASL cross-check for the whole 68000/68010/68020/68882/68851 corpus
  over one operand. GNU as assembles every case and agrees with libasm
  byte-for-byte (`pflush #15,#4` -> `f000 309f`). Note the gas pipeline's
  `.s37` differs from the committed reference only in the S0 header record
  (objcopy writes `/dev/stdout` as the module name); compare data records only.
- The commit that had narrowed the reference to `#7` was **dropped from
  history** -- its message wrongly claimed "the mask is 3 bits". The reference
  now exercises the full 4-bit range for both fields.

### TMS320C3x (TMS320F)

- Source of truth is **SPRU031F, TMS320C3x User's Guide**, NOT ASL. Both ASL
  and GAS have remaining bugs here. The committed golden `.hex` is libasm's own (`asm-test`).
- **p2hex cannot do 32-bit words, but p2bin can -- the reference IS byte
  comparable.** (Corrects an earlier note here that said a byte-level diff was
  unachievable; that claim hid three real ASL bugs.) p2hex's Intel writer keeps
  only the LOW 16 bits of each target word and still counts the dropped bytes in
  the record length, so the file is malformed. `p2bin` keeps the whole word and
  `objcopy -I binary -O ihex --change-addresses=<byte org>` produces an exact
  Intel HEX -- tms320c30 went from 4530 differing bytes to 34. Wired up as
  `P2BIN_OUTS` + `asl-conv` in `test/Makefile.asl`, used by both
  `reference/Makefile` and `autogen/Makefile`. Two traps: `asl-conv` must be
  called as `$$(call ...)` from the eval'd rule so it expands at RECIPE time
  (eval-time expansion double-expands the flags and eats the `\$-\$`), and
  `-m <0..3>` is **PIC-specific** (INHX8M/16M/8L/8H), not a general multibyte
  switch. `-F DSK` does preserve the full word (low/high half pairs); only the
  Intel writer truncates.
- **Three ASL encoding bugs remain at Bld 311**, all cross-checked against
  `tic4x-coff-as` (which agrees with libasm and the manual): `TRAPcond N` low
  byte is `N` not `N + 20h` (p.13-244, all 20 conditions); `ROL`/`ROLC`/`ROR`/
  `RORC` emit a zero count field where the manual fixes bits 15:0 at `0001H`
  resp. `FFFFH` (pp.13-205/206/208/209); short-float immediate loses the SIGN
  for powers of two (`ADDF -1.0` -> `+1.0`, `-2.0` -> `+2.0`, `-0.5` -> `+0.5`
  -- mantissa exactly -2.0, i.e. s=1 f=0; non-powers-of-two are fine).
  The TRAPcond one is NOT the Bld 310 "allow trap vectors 0..31" fix -- that
  only widened the operand RANGE (`UInt4`->`UInt5`).
- **Fixed upstream, do NOT re-report.** Bld 310: TRAPcond range, `CMPF3`/`CMPI3`
  two indirect operands, parallel register-first order, parallel 2-operand short
  form, and **LDM exponent**. Bld 311: the bare no-`3` mnemonic for the
  non-commutative parallel computes (`ASH`/`LSH`/`SUBF`/`SUBI`). So the old
  "keep the reference as-is, ASL is buggy" note on the bare aliases is obsolete,
  and the earlier parallel-alias and LDM reports are all closed.
- **LDM immediate is lossy BY DESIGN** (loads a 12-bit mantissa, exponent forced
  to 0); `dis(asm(x)) != x` for out-of-[1,2) operands is correct. libasm is
  manual-conformant. ASL agrees since Bld 310 -- this is now a **GAS-only**
  divergence (GAS still keeps the exponent nibble).
- **Capabilities follow the SILICON REVISION, not the CPU** (2026-08-29;
  supersedes the old "augmented operands are C32-only" note). IDLE2,
  LOPOWER/MAXSPEED and the augmented parallel operands each arrived with later
  silicon rather than a new part, so `CpuSpec` carries them and a `silicon`
  option + directive selects them; `CpuType` only supplies the default. Manual
  basis: 7.9 (p.7-48) lists IDLE2+LOPOWER on 'C30 version 7.0+, 'LC31, 'C31 rev
  5.0+, 'C32 and 'VC33; the augmented note repeated on all 28 parallel pages
  lists 'C31 rev 6.0+, 'C32 rev 2.0+, 'VC33. Defaults preserve the previous
  behaviour exactly: C30 -> 1, C31 -> 5, C32 -> 2.
  - **Revisions are 1-based**, so 0 is free as the "not named" sentinel and
    `setSilicon` rejects it.
  - A named revision is **sticky** across a later `cpu` directive -- otherwise
    `setCpuType` re-derives defaults and silently discards the option -- but is
    **cleared by `reset()`**, otherwise it leaks between unit tests and across
    CPUs. That leak really did break `test_parallel` until `reset()` called
    `resetSilicon()`; the unit test caught it.
  - **No `VC33` CpuType**: its capabilities are exactly 'C32 rev 2.0+, so it
    would be an alias, not a variant.
  - All CPUs share `TMS320C3X_PAGES`; `idle2()` / `lopower()` / `para()` add the
    rest, chained on `UNKNOWN_INSTRUCTION` like mc68000's `fpu()` / `pmmu()`.
  - Only the wrappers that need a revision declare one (`silicon 5`,
    `silicon 2`); `test_tms320c30.asm` declares nothing because it uses nothing
    revision-gated. A libasm-only directive is safe there because c31/c32 are in
    `ASL_exclude` and c30 has its own `.asl`.
  - `acceptMode` used to let `M_FREG` satisfy a table `M_IDIR`, leaking
    augmented operands onto every device while the dis correctly rejected the
    bytes -- fixed by splitting the range so `M_IDIR` is skipped.

- **`M_FIDR` is deliberately NARROWER than the manual and GAS -- keep it.** The
  augmented slot encodes `encodeRegName(reg) | 0xE0` over a full 5-bit field,
  and all 28 augmented pages read "indirect ... or any CPU register", for the
  float ops too; GAS `-menhanced` accepts `absf ar0,r1 || stf r2,*-ar3`
  (`c8420be8`). libasm still restricts the FLOAT slots to R0-R7, because reading
  a 32-bit `AR0`/`ST` as a 40-bit float operand is encodable nonsense. `M_IIDR`
  (integer) stays manual-faithful at any CPU register and matches GAS
  byte-for-byte across all 28 registers. Structural payoff: every operand a
  parallel head can take is then also legal stand-alone, so line 1 always
  parses -- see the next bullet for why that matters.

- **The assembler parses a parallel pair line-by-line, and line 1 must stand
  alone.** `encodeImpl` matches it with `acceptSingle` (which excludes every
  parallel entry) and returns BEFORE saving `state.prev` if that fails, so the
  `||` line then has nothing to pair with. Line 1's bytes are provisional --
  line 2 re-searches both halves and re-emits at `state.prev.address()`.
  Consequence: widening a parallel slot past what the stand-alone table accepts
  makes those forms unreachable in the assembler while the dis still decodes
  them. Fixing that properly would need a provisional line 1 plus a flush hook,
  and the flush cannot help the direct `encode(line, insn)` API (a dangling head
  would return OK with zero bytes), so it was NOT done -- keeping `M_FIDR`
  avoids the whole problem.

- **GAS needs `-menhanced`** for the augmented operands (`-midle2` /
  `-mlowpower` for the power modes); `-mrev` is documented sugar that sets them
  from `-mcpu`+`-mrev`. Without the flag GAS rejects every augmented form, which
  is what makes them look unsupported at first glance. GAS bug:
  `-mcpu=33` never enables `-mlowpower` at
  any `-mrev`, so LOPOWER/MAXSPEED are "Unknown opcode" on the one part 7.9 and
  7.9.2 name explicitly for both; passing `-mlowpower` by hand assembles them
  and the encodings then match libasm.

- **ASL has NO C3x variant model.** `code3203x.c` registers 320C30, 320C31,
  320C40, 320C44 -- no 320C32, no VC33 -- and `CPU32031` is declared and
  registered then never used as a MinCPU, so 320C30 and 320C31 produce
  byte-identical output (verified on `test_tms320c30.inc`). The only real gate
  is `ChkMinCPU(CPU32040)` for C4x-only instructions, and IDLE2 / LOPOWER /
  MAXSPEED are absent from the file entirely. ASL therefore cannot cross-check
  any of the revision work; GAS is the only usable second source.
- Reference parallel coverage is 100% (every `X||Y` pair in SPRU031F, most in
  both operand orders). `SUBF3||STF` fixes `src1`=register/`src2`=indirect
  (p.13-232), so the 2-operand short form genuinely does NOT apply to
  `subf`/`subi` -- its absence is correct, not a gap. `tms320c31`/`c32` stay in
  `ASL_exclude`: ASL has no `320C32` CPU, lacks `IDLE2`/`LOPOWER`/`MAXSPEED`,
  and rejects the augmented operands on every CPU.

### Am29000 (Am29xxx family)

- Source of truth is the AMD **Am29000 / Am29200 / Am29240 / Am29030 / Am29050
  User's Manuals**; ASL's `code29k.c` and GNU `a29k-coff-as` (binutils 2.11.2)
  are the adjudicators.
- **`CONSTH` operand convention differs and both are defensible**, so it is
  kept OUT of the shared reference include: the manual and ASL take the 16-bit
  field value (`CONSTH gr96,0x5678`), GNU as takes a full 32-bit value and uses
  its high half (`consth gr96,0x56780000`).  libasm follows the manual.
- **`a29k-coff-objdump` 2.11.2 segfaults on a `CONSTH` at address 0** -- and
  only there.  It reads the preceding word to recognize a `CONST`/`CONSTH`
  pair, and `memaddr - 4` underflows past `buffer_read_memory`'s bounds check
  (the check adds `length` back, which wraps).  A non-zero section vma or any
  one preceding instruction avoids it, so the disassembly step is safe at the
  test origins and IS worth running -- an earlier note here wrongly said the
  GNU disassembler was unusable on `CONSTH` code.
- **Branch targets come from the linker, not the source.**  GNU as treats the
  absolute `CALL`/`JMP` word address as signed and rejects anything over
  `0x1FFFC`, though the manual zero-extends it to `0x3FFFC`.  So the generated
  source NAMES its targets and `scripts/scrape-am29000-syms.sh` turns each into
  a `--defsym`; `a29k-coff-ld` then resolves the full range, preferring the
  absolute form.  The reference sources name no such symbol.
- The gas pipeline links with `am29000.ld` (see the `CONSTRUCTORS` bullet
  under GAS fill and padding) and disassembles the `.hex`, which carries no
  architecture -- pass `-m a29k` explicitly.

### i960 (Intel 80960)

- Source of truth is the **80960KB Programmer's Reference Manual** (order
  270567) plus the **i960 Processor Assembler User's Guide** (485867), .  **GNU `i960-coff-as` is the adjudicator, NOT
  ASL**: ASL's 80960 target has eight documented issues, six of them silent
  wrong-code, plus two convention differences that are NOT bugs
  (reported to ASL).
  The OCR'd Appendix B tables are GARBLED -- read the prose beneath them.
- **`exp(ip)` is the OFFSET, not the target.** Table 5-1 gives the syntax as
  `exp (IP)` for "IP + displacement + 8", every sibling mode puts the ADDED
  value in `exp`, 5-8 says so outright, and the Assembler Guide writes
  `st g5, 4(ip)` ("store based on ip").  Applying the libasm "PC-relative:
  target in, target out" house rule here was WRONG -- that rule is for
  BRANCHES.  Getting it backwards made the identical source line assemble to
  different bytes under libasm and GNU as with no diagnostic, which is worse
  than a rejection.  ASL alone reads it as the target; `ip-operand=target` /
  `.ipoperand target` selects that reading.  ASL is not obliged to follow the
  Intel assembler here and the target form is friendlier for hand-written
  code, so the report files it as a convention difference, not a bug.
- **The index multiplier is always written, including `*1`.**  Table 5-1
  spells every indexed mode `[reg*scale]` and defines no scale-less form.  The
  two assemblers extend the omission incompatibly: ASL reads `[g4]` as x1
  always, GNU as reads it as the INSTRUCTION'S OPERAND SIZE (`ld` -> x4,
  `ldl` -> x8, `ldob` -> x1).  gas's own disassembler omits `*1` and so does
  not survive its own round trip.
- **The mode bit of an unused operand field is free**, and the two assemblers
  choose differently: GNU as and the Intel assembler set it (binutils
  `include/opcode/i960.h` builds it into the opcode via `R_0`/`R_1`/`R_2D`),
  ASL leaves it clear.  Tables B-1/B-2 define the bit only for fields an
  instruction USES, so neither is wrong.  libasm follows GNU as by default and
  the `mode-bits` option / `.modebits` directive selects the other.  Put the
  set bits in the table's opcode, as binutils does, and `matchOpCode` only has
  to clear all three when the option is off.  `mode-bits` is an INT taking 0
  or 1 -- the value the bit carries -- not a bool, so it reads as the bit.
- A bare `(g5)` is MEMB mode 4 while `0(g5)` is MEMA, so `Operand::hasDisp`
  has to record whether a displacement was actually WRITTEN, not just its
  value.
- **Do NOT reject a MEMB displacement that also fits MEMA.**  It looks like
  the h16 reject-what-cannot-round-trip case and is not: GNU as and the Intel
  assembler reserve the 32-bit displacement for ANY operand naming a symbol,
  however low it resolves (`tc-i960.c` "we HAVE to reserve 32 bits"; Guide
  5-39 `.lomem` "Otherwise the assembler defaults to MEMB format"), so it is
  ORDINARY CODE and rejecting it made libasm refuse routine gas output.  The
  h16 rule is for encodings NO assembler emits.  Decode it by default and let
  the generator opt into strictness -- `canonical-mem`, a dis-only option that
  `gen_i960.cpp` sets.  Contrast mc6800, where the `>` prefix (`ldaa >$0010`)
  pins the long form so both round-trip; i960 has no such operand syntax, only
  `.lomem`, a per-symbol directive selecting the SHORT form.
- **The 80960KB extensions** (49 floating point + 3 decimal = exactly GNU as
  `-AKB` minus `-AKA`) sit behind `fpu` / `.fpu on|off`.  **Its default
  differs by class: ON for the disassembler, OFF for the assembler** -- a
  disassembler is handed whatever the target ran, an assembler should be told
  which processor is being written for.  Each sets its own in `reset()`, so
  the `Config` constructor's value is irrelevant.  CONSEQUENCE: every autogen
  variant is driven by the DIS, so a variant wanting the base set alone must
  say `-O fpu=off` explicitly or it silently gains the extensions.
- FP operand encoding (Table B-1/B-2 and the prose below them): a source field
  takes g/r when its mode bit is clear and `fp0`-`fp3` or the real literals
  +0.0 (field 16) / +1.0 (field 22) when set; a DESTINATION takes an fp
  register but NO literal.  Long real in general registers is a pair, extended
  real a quad -- and the pair/quad alignment rule must be SKIPPED when the mode
  bit selects an fp register or a literal, or `addrl g0,g2,fp3` assembles but
  will not disassemble.
- With an extension turned off, a gated mnemonic should report
  UNKNOWN_INSTRUCTION, not OPERAND_NOT_ALLOWED: re-search with the extension
  on and, if that would have matched, the instruction is turned off rather than
  misspelled (the am29000 pattern; reset the error before the retry).
- A dotted directive needs a `SymbolParser` whose `instructionLetter` accepts
  `.` (`I960SymbolParser`); without it `.org`, `.end` and `.include` all fail
  with "Not an expected entity" even though `.cpu` works.
- GNU as needs **no linker and no `--defsym`** here, unlike am29000: nothing it
  sees is out of range.  Do not reach for that machinery -- `i960-coff-ld`
  mis-resolves PC-relative relocations against `--defsym` absolute symbols
  (CTRL silently encodes the wrong target, COBR errors "relocation truncated
  to fit"), and gas refuses an absolute expression or `.equ` symbol as a
  CTRL/COBR target outright ("must be a label").  What it needs instead is
  RENDERING: `relative` + `origin-char "."` so branches read `.+N`.
- Register spelling: GNU as rejects `r0`, `r1`, `r2` and `g15` and takes only
  `pfp`, `sp`, `rip` and `fp`; ASL takes either.  The dis emits the alias
  names so its output assembles under both.
- Comment characters are incompatible (`#` for gas, `;` for ASL/libasm, and
  gas treats `;` as a statement separator), so a shared source is derived by
  a `sed` in `test/Makefile.gas`.  That is the ONLY difference between the
  native and GNU as autogen source, so one generated `.ginc` serves both.

### Z8000

- **BOTH ASL (`codez8000.c`) and GNU as (`z8k-coff-as`) cross-check this
  target**, and the reference is byte-identical to both. The goldens are
  `.hex` (not `.s37`), and the z8001 org sits at `0x100000` on purpose -- see
  the Intel-HEX type-02/04 note above.
- **Keyword names are also legal symbol names.** `VI`/`NVI` (EI/DI) and the
  flag names `P`/`S`/`V` must work as labels: ASL and gas both assemble
  `jp vi` and `jr p` against a label of that name. Registers stay reserved
  everywhere, as in every assembler. Implementation: an operand read as
  `M_CC`/`M_FLAG`/`M_INTR` fits wherever `M_DA` does (delegate to
  `acceptMode(M_DA, table)`, do not copy its list), and `emitOperand` re-reads
  the text as an address when the matched entry asked for one -- it fires only
  on the label path, since the keyword path keeps `mode == op.mode`.
- **Condition codes go by ARITY, following ASL.** `C` and `Z` are flag names
  as well as condition codes, and the references disagree here: gas rejects a
  label so named (`jr c` -> "missing operand"), while ASL reads `jr c` as a
  jump to label `c` and `jr c,x` as the conditional jump. libasm follows ASL,
  so every cc name works as a label; the operand count decides. Verified
  byte-identical to ASL on `jr c` / `jr z` / `jr c,c` / `jr nz,z` / `jp c` /
  `ld r0,c` / `setflg c` / `ret c`. The manual does not settle this -- symbol
  naming is assembler policy, not an ISA fact -- so it is a deliberate choice
  of reference, and the reference goldens stay byte-identical to BOTH tools.
- **`FLAG_P = 1` and `FLAG_V = 1 + 16` encode to the SAME bit** (`& 0xF`):
  parity and overflow share one flag. So a flag list can carry FIVE names --
  `SETFLG C,Z,S,P,V` is legal and gas emits `8DF1`, identical to the four-name
  form. Any design that splits the list across operand slots is therefore dead
  on arrival: `encodeImpl` parses four.
- **`ex1` and `ex2` hold two bits and one bit** (`ex1_gm = 0x3`,
  `ex2_gm = 0x1`), so the third and fourth table operands can only name modes
  0-3 and 0-1. `M_INTR = 21` and `M_FLAG = 23` do not fit, and `_mode` is full
  (dst 5 + src 5 + ex1 2 + ex2 1 + size 3 = 16). Operand arity past two cannot
  be declared in the entry.
- Consequence for the keyword/label ambiguity: `parseOperand` runs BEFORE
  `searchName`, so it must guess, and un-guessing has to happen after the
  match. Checking the symbol table at parse time is not an escape -- it breaks
  forward references, since the label is undefined on pass 1. `acceptMode`
  cannot re-read the text either: it lives in `table_z8000.cpp` with no access
  to the parser or symbol table. The resolution is
  `acceptMode(M_INTR|M_FLAG, table) -> acceptMode(M_DA, table)` (delegate, do
  not copy the M_DA list) plus a re-read at the top of `emitOperand` when the
  matched entry asked for an address. That fires only on the label path; the
  keyword path keeps `mode == op.mode` and runs the original code.

### MC68HC12 / MC68HCS12 / MC68HCS12X

- CpuTypes MC68HC12, MC68HCS12, MC68HCS12X (`68HC12X` alias for ASL). HCS12
  ISA == HC12; only PC-relative MOVB/MOVW differ: M68HC12 base = end + Table
  3-3 correction (MOVB # +1, MOVW # +2, IDX->IDX -1/+1; silicon-confirmed on
  MC68HC912BD32), HCS12 none, S12X source base excludes the destination's
  offset bytes. MOVW #,idx is 18 00 (18 03 is #,ext).
- Don't-care postbyte bits are must-be-zero; illegal sets from the manual.
- `mmc` physical maps (locations are physical; see "Banked memory"): HC12 =
  MC68HC812A4 Table 8-1 pins (outside windows
  $3F0000|a, PPAGE<<14, DPAGE $300000+DPAGE<<12, EPAGE $3C0000+EPAGE<<10);
  HCS12 linear ($4000 = page $3E, $C000 = $3F, < $4000 identity, PPAGE 6
  bits, reset 0); S12X global (PPAGE $400000+, RPAGE <<12, EPAGE $100000+,
  fixed $0C00/$2000/$4000/$C000 = E$FF / R$FE-$FF / P$FD / P$FF).
- Syntax reference is CodeWarrior's S12(X) assembler (bare directives;
  `HIGH()`, `LOW()` operators match; CW's `PAGE()` assumes logical addresses,
  libasm's works on physical ones, and `LOW16()` and the page-register
  directives are libasm extensions). Not adopted, by the
  maintainer's choice: CW's `.B`/`.W` force suffixes, CPU names `CPU12`/`HCS12`/
  `HCS12X`, and a `-CpDirect`-style option (DIRECT stays a directive).
- DIRECT exists on S12X only; HC12/HCS12 direct page is fixed at $00xx. G*
  instructions get explicit `M_GDIR`/`M_GEXT` (GPAGE check, no window lookup).
- ASL: no HCS12 CPU; 16-bit address space even for `68HC12X`; `ASSUME` only
  DIRECT (compatible) and GPAGE (stored, never checked); ignores an
  unsatisfiable `<`; `68HC12X` truncates `ldaa $12345`. GNU as: GSTS ext
  18 6F, no CCRH/XL-style registers, symbolic PC-rel MOVx = raw offset; ld
  banked mode numbers pages from $10000 and botches page<->fixed PC-relative
  branches. Nothing m68hc1x-relevant changed in binutils 2.44 -> 2.47.

---

## Helper scripts (`scripts/`)

Hand-sorting libasm's tables is error-prone; always run the
script. **Regenerate after ANY entry change, removals included** -- deleting
two entries without re-running `make-index-array.sh` left the index pointing
past them and broke 23 tlcs900 tests at once. The rule is not "regenerate when
you add".

| Script | Sorts / lists |
|---|---|
| `make-index-array.sh` | `INDEX[]` for an `Entry` table (alphabetical by `TEXT_*`) |
| `sort-name-entry.sh` | `NameEntry` arrays in `reg_*.cpp` |
| `sort-pseudo-entry.sh` | `Pseudo` arrays in `asm_*.cpp` |
| `sort-text-constant.sh` | `TEXT_*` constants in `text_*.h/cpp` |
| `extract-common-text.sh` | `TEXT_*` constants shared across multiple arch text files (for promotion into `text_common.h/cpp`) |

Idiomatic usage: Emacs `C-u M-x shell-command-on-region` after marking
the table region.

Example:

```sh
awk '/^constexpr Entry MYTABLE\[\]/,/^};$/' src/table_XXX.cpp \
  | scripts/make-index-array.sh
```

---

## Build / test commands

| Command | Effect |
|---|---|
| `ninja -C cli` | Build the `asm` and `dis` CLI binaries |
| `ninja -C test` | Build + run all unit tests |
| `ninja -C test/driver` | Build + run formatter / driver tests |
| `make test` | Full test suite from project root |
| `make test_<arch>` | Per-arch unit tests only |
| `make gen_<arch>` (in `test/`) | Rebuild + relink the generator binary from changed `.o` files |
| `make -j gen_<arch>` (in `test/autogen/`) | Regenerate every variant's `.inc`; parallel-safe. Does NOT rebuild the binary -- run the `test/` build first (see autogen section) |
| direct: `test/gen_<arch> -g -i -C <CPU> -o gen_<CPU>.ginc -l gen_<CPU>_ginc.lst` (from `test/autogen/`) | Single-file regen; workaround for the per-file `)`-suffix bug below (the arch-level `gen_<arch>` target above avoids it) |
| `make -C test/reference asm-test_<target>.<ext>` | Assemble reference via libasm (the golden); `<ext>` = `hex` (Intel) or `srec`/`s28`/`s19` (Moto) per arch |
| `make -C test/reference asl-test_<target>.<ext>` | Cross-check via ASL (most arches); overwrites the golden filename -- save + diff |
| `make -C test/reference gas-test_<target>.<ext>` | Cross-check via GAS (some arches); overwrites the golden filename -- save + diff |
| `./cli/dis` / `./cli/asm` | Print the canonical `--help` blocks used to regenerate README |

### `long double` is 80-bit only on x86 -- test the SOFT float80 path too

`src/float80.h` picks the implementation from `LDBL_MANT_DIG == 64`, which is
true only for x86's 80-bit extended `long double`. Everywhere else -- aarch64
and any other IEEE binary128 host (`LDBL_MANT_DIG == 113`) -- the build uses
`__float80_soft` instead of `__float80_hard`. **The two are separate decimal
readers**: the hard one delegates to `strtold`, the soft one scans the digits
itself in `__fixed64::read`. A divergence between them is invisible on an x86
developer machine and fails only on the other host.

That is a real bug class, not a hypothetical: `__fixed64::read` required a
digit before the `.`, so `F'.123D3` (`.123E3` after normalization) parsed on
x86 and reported `ILLEGAL_CONSTANT` on aarch64 -- the one float literal in the
whole suite written without a leading digit.

Reproduce the other host's build on x86 by forcing the soft path:

```sh
make -C test test -j12 \
    CXXFLAGS='-std=c++14 -Wall -O -DLIBASM_DEBUG_VALUE -DLIBASM_SOFT_FLOAT80'
```

Float-reader tests belong in `test/unit/test_float80.inc`, which
`test_float80_hard` and `test_float80_soft` BOTH include -- a case added there
covers the soft path from x86. An arch-level `test_asm_*` case cannot: it
exercises whichever implementation the host selected. After adding one, revert
the fix and confirm `test_float80_soft` actually fails; a test that passes
either way is not a guard. No CI job builds the soft path today.

### Grep the build for `warning:`, not just `error`

The tree builds warning-free with `-Wall`, so any warning is a regression --
but `ninja ... | grep -E 'error|FAILED'` throws them away, and a whole arch can
be developed without ever seeing one.  Grep for `warning:` too, or just count
`grep -cE 'warning:|error:'` and expect 0.

The one that hides best is **`-Wswitch`**: the per-mode `switch` in
`decodeOperand` / `encodeOperand` deliberately has NO `default:` so that a new
`AddrMode` is caught, and adding a parser-only mode (i960's `M_FREG`, `M_REG`,
`M_VALUE`) trips it even though such a mode can never reach those functions.
Give the parser-only modes one explicit case that returns, rather than adding
a `default:` -- the missing `default:` is the safety net.

### A crashed test binary prints no FAIL

`./test_asm_<arch> | grep -c FAIL` reports 0 when the binary SEGFAULTs
mid-run (buffered output is lost). Check the exit status (`; echo rc=$?`) or
look for the last `PASS`/`TEST` line, especially right after a cherry-pick
that adds a CPU to a per-CPU table lookup (a missing row -> nullptr).

### ASL flags

- `asl source.asl -L listing.lst` -- listing to *file*
- `asl -l source.asl` -- listing to *stdout*

Source file is positional. Both flags take no argument.

### ASL native hex syntax differs per arch

ASL's per-CPU `SwitchTo_*` (in `code*.c`) sets the native integer-constant mode;
non-native forms parse ONLY under `-relaxed`. The reference harness passes
`-relaxed`, so `.asl` sources can silently drift to non-native syntax -- prefer
the native form so a source also assembles WITHOUT `-relaxed`. Native hex by
family:

- **Motorola** (`SetIntConstMode(eIntConstModeMoto)`): hex `$xxxx`, bin `%`,
  oct `@`. H8/300, H8/500, H16, SuperH (most Hitachi/Moto targets).
- **IBM** (CP-1600: explicit `eIntFormatIBMXHex`): hex `X'xxxx'`; `$`/`0x`/`H`
  need `-relaxed`.

Loc-counter symbol is `*` (not `$`) for all of the above. `IntConstModeIBMNoTerm`
(H8/300, H8/500) only loosens the non-native IBM parser to allow a missing
closing quote -- it does not change the native form.

### Regenerating the README CLI blocks

`asm --help` / `dis --help` write to **stderr**, and the first line is
`no input file`; the wrapped CPU-list lines also carry **trailing spaces**.
Capture stdout+stderr, drop that line, `rstrip()` every line, and replace the
WHOLE indented run -- anchoring on the `libasm ... (version` line instead lets
stray lines accumulate above it. Regenerate at each commit that changes a CLI
option or adds a CPU, since the blocks must be that commit's actual output.

### `make -C test/reference asm-test` wipes ALL goldens first

The `asm-test` target runs `clean-asm` (which `rm`s *every* arch's
committed golden) and then regenerates them in alphabetical order. If any
arch fails mid-run -- e.g. a pre-existing `OVERFLOW_RANGE` on `test_h16`
-- it ABORTS and leaves all the not-yet-regenerated goldens DELETED.
Recover with `git checkout HEAD -- test/reference/`. To (re)build one
arch's goldens without the wipe, use the per-target rule, which skips
`clean-asm`:

```sh
make -C test/reference asm-test_<target>.<ext>   # e.g. asm-test_h8500.s28
make -C test/reference asl-test_<target>.<ext>   # ASL cross-check
make -C test/reference gas-test_<target>.<ext>   # GAS cross-check
```

The committed golden is libasm's (`asm-test`); `asl-test`/`gas-test`
overwrite the same file, so save a copy and `diff` to compare a tool
without losing the golden. The autogen tracks only `.inc`/`.ginc` (no
committed binary golden); its `asm-test` just has to assemble cleanly.

### `test/autogen/Makefile` `gen-out` stray paren

The `define gen-out` macro has four closing parens for three `$(if` opens,
so every generated target name (`gen_i8086.inc`, `gen_i80386.ginc`, ...)
ends with a literal `)` in the Makefile's rule tables. Consequence:
`make gen_i8086.inc` reports "Nothing to be done" because the real rule
key is `gen_i8086.inc)`. This hits only the per-FILE targets
(`gen_<CPU>.inc`); the arch-level `make -j gen_<arch>` target (which
regenerates every variant) is a different rule and is UNAFFECTED -- prefer
it. Workarounds for a single file: run the generator binary directly (see
Build / test commands above), or `make -B 'gen_i8086.inc)'` with the stray
paren quoted. Real fix: drop one `)` on the last line of the `gen-out` macro.

---

## Release procedure

Releases are cut on `main` and published to the PlatformIO registry. Each
release is a single bump commit, a LIGHTWEIGHT tag on it, a GitHub release, and
a PlatformIO publish. Verified against 1.6.66 (2026-09-27).

### 1. The version bump commit

All mechanical:

| File | What changes |
|---|---|
| `src/version.h` | `LIBASM_VERSION_PATCH` and `LIBASM_VERSION_STRING` |
| `library.json` | `"version"`, plus `keywords` if an arch was added |
| `README.md` | the CLI blocks, REGENERATED not hand-edited |

Three traps, all hit for real:

- **`LIBASM_VERSION_PATCH` is a BARE number**, so a blanket
  `sed s/1.6.65/1.6.66/g` silently misses it and leaves the macro one behind
  the string. Check `grep -E 'PATCH|STRING' src/version.h` after the sed.
- **`library.json` `keywords` must cover every arch in `ARCHS`**
  (`src/Makefile.arch`), in ARCHS order uppercased -- NOT alphabetical.
  Nothing checks it, and i960 went a whole release with the keyword missing.
  `len(keywords)` should be the arch count + 2 (`Assembler`, `Disassembler`).
- **The README CLI blocks embed the version string**, so they change on EVERY
  release. Regenerate from the rebuilt binaries (see "Regenerating the README
  CLI blocks"); never hand-patch the version line. On an otherwise-current tree
  a correct regeneration yields a 2-line diff per file -- more than that means
  the blocks had drifted, which is a separate finding worth reporting.

Message format (see `git log v1.6.65` for precedent):

    Version 1.6.66

    - Addition: [i960] Support Intel 80960
    - Bugfix: [H16] Fix DADD and DSUB opcodes

Only USER-VISIBLE changes are listed -- `[ci]`, `[test]`, `[platformio]`,
autogen regenerations and internal refactors are left out.
"Add missing <instruction>" is a **Bugfix**, not an Addition (precedent:
`Bugfix: [Z280] Add missing ADD/ADC/...`). A whole new architecture is still
a PATCH bump: 1.6.64 added six of them.

### 2. PR, merge, tag

The bump goes through a PR like anything else. After it merges:

1. `git pull --ff-only`, then **confirm `git diff <local-sha> origin/main` is
   empty** before tagging. The tag is the step you cannot quietly redo.
2. Re-run the checks on the MERGED commit, not the pre-merge one: both binaries
   print the new version, `ninja -C test` green, no old version string left
   (`grep -rl` over `*.h *.json *.md`).
3. `git tag vX.Y.Z <merged-sha>` -- lightweight, like every prior tag -- then
   `git push origin vX.Y.Z`.

Watch the merge dialog: a squash merge that appends `(#NNN)` would leave the
release commit reading `Version 1.6.66 (#146)`, unlike every other release.

### 3. GitHub release

    gh release create vX.Y.Z --title "release X.Y.Z" --notes-file notes.md

Title is `release X.Y.Z` (lowercase word, no `v`); only the tag carries the
`v`. Body is exactly the commit's bullet list, no heading (1.6.67 and 1.6.68):
`git log -1 --format=%b vX.Y.Z | gh release create ... --notes-file -`. No
assets, not a draft, not a prerelease.

### 4. PlatformIO: publish `git archive <tag>`, NEVER the working tree

`library.json` has **no `export` section**, so `pio pkg publish` packages
whatever directory it is handed -- and the working clone carries ~650MB of
build output and ~1100 gitignored files.

Do NOT `git clean` to fix that: it destroys the build state and still cannot
promise a pristine tree. Export the tag instead, which emits only TRACKED files
so nothing untracked or ignored can leak:

```sh
mkdir -p /tmp/pub && git archive --format=tar vX.Y.Z | tar -x -C /tmp/pub
cd /tmp/pub && pio pkg pack -o /tmp/libasm-X.Y.Z.tar.gz   # INSPECT before publishing
pio pkg publish /tmp/libasm-X.Y.Z.tar.gz --no-interactive
```

Checks worth the minute they cost:

- **Size.** `test/` (109MB) is stripped by PlatformIO's default excludes, so a
  115MB export packs to ~720KB. Compare with the previously published tarball;
  a jump means something bulky got in.
- **Diff the file list against the LAST PUBLISHED package**, not against the
  repo:

      PREV=1.6.65
      URL=https://dl.registry.platformio.org/download/tgtakaoka/library/libasm
      curl -sL -o prev.tar.gz $URL/$PREV/libasm-$PREV.tar.gz
      LC_ALL=C comm -3 <(tar tzf prev.tar.gz | grep -v '/$' | LC_ALL=C sort) \
                       <(tar tzf new.tar.gz  | grep -v '/$' | LC_ALL=C sort)

  `LC_ALL=C` on BOTH sorts and the `comm`, or `comm` rejects the input as
  unsorted. Every added file should be one you expect.

  This is how the 1.6.65 leak was found: it shipped
  a local editor settings file and `src/.gdb_history` -- the latter matched by
  `.gitignore` and packaged anyway, because that release was published from the
  live working directory. The export is exactly what prevents it.
- The registry serves the new version some minutes after "the package has been
  accepted"; poll
  `https://api.registry.platformio.org/v3/packages/tgtakaoka/library/libasm`
  instead of assuming.

**Still shipped, arguably should not be**: `.github/`, `.gemini/`, `.clangd`,
`.clang-format`, `.clang-tidy`, `.gitignore`, `scripts/`. An `export.exclude`
list in `library.json` would make the package contents deterministic rather
than a property of PlatformIO's defaults plus a clean source tree.

---

## Quick checklist before a commit lands

- [ ] Subject is `[arch] Verb ...` lowercase tag
- [ ] No `Co-Authored-By:`
- [ ] `ninja -C cli && ninja -C test && ninja -C test/driver` all green
- [ ] No forward references in code or comments
- [ ] README CLI blocks regenerated *if* CLI options changed
- [ ] Any new `Entry`/`NameEntry`/`Pseudo` table has its `INDEX[]`
      regenerated via the matching `scripts/sort-*.sh`
- [ ] No name-based dispatch and no per-instruction if-clauses in
      encodeImpl/decodeImpl
- [ ] ASCII-only in any comment touched
- [ ] `INDEX[]` regenerated after entries were REMOVED, not only added
- [ ] Disassembler change: autogen regenerated, its diff characterised per
      mnemonic, and the round trip closed by assembling the regenerated source
      with libasm and byte-comparing against the reference toolchain
- [ ] A new prefix page: checked against the CPU list AND the super-prefix, and
      confirmed it does not shadow an existing code
- [ ] Third-party check named honestly -- which of ASL / GNU as actually
      supports this CPU, and was it a byte comparison or just "it assembled"
- [ ] Float parsing/rendering touched: rebuilt with `-DLIBASM_SOFT_FLOAT80`
      too, and the case added to `test/unit/test_float80.inc` (shared by the
      hard and soft unit tests), not only to an arch `test_asm_*`
- [ ] New architecture: added to `library.json` `keywords` as well as to
      `ARCHS` -- nothing checks that they agree, and a release will ship
      without it

---

## What to skip / avoid

- Don't write large multi-paragraph commit messages.
- Don't add lambdas to `src/`.
- Don't hand-edit README CLI usage blocks.
- Don't leave review/fixup commits on top of the branch after they
  are asked to be "dissolved".
- Don't reach for Unicode glyphs in comments.
- Don't add `// removed X` / `// renamed Y` comments for things you
  deleted -- the diff documents it.
- Don't put arch-specific autogen filters in `tokenized_text.cpp`; new
  per-arch `Tokenizer`s go in that arch's `gen_<arch>.cpp`.
- Don't treat a regen DIFF vs a committed autogen `.inc` as a regression
  without isolating it with a `HEAD~1` build -- the committed baseline may
  be stale from a disassembler rewrite.
- Don't add attribution trailers to a commit or a PR body.
- Don't assume a float bug is arch-specific because only one arch's test
  fails -- check whether the host picked `__float80_soft`.
- Don't let the operand parser decide between a keyword and a same-spelled
  symbol by consulting the symbol table: on pass 1 a forward-referenced label
  is undefined, so the keyword always wins and the label never assembles. The
  entry has to make the call, after the match.
- Don't conclude a keyword is reserved because the parser eats it. Ask the
  reference assembler whether a label of that name assembles -- and ask BOTH
  where both exist: on Z8000 they agreed on VI/NVI/P/S/V and disagreed on
  C/Z, which made that half a judgement call rather than a lookup (resolved
  by following ASL's arity rule).
- Don't trust a bullet in THIS file over the code: the SuperH `origin-char`
  note described `*` for months while `gen_superh.cpp` set `$`. When a claim
  here is load-bearing, re-verify it, then fix whichever side is wrong.
- Don't run `pio pkg publish` in the working clone, and don't `git clean` it to
  make that safe -- publish an export of the tag (see Release procedure).
