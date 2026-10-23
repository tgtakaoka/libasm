# Copyright 2026 Tadashi G. Takaoka
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Make binutils Intel HEX read as libasm and p2hex write it: below 1MB
# objcopy gives the base as an extended segment address record (type 02),
# rewritten here as the extended linear address record (type 04), and the
# start address record ld adds (type 03) has no counterpart.

/^:02000002/ {
    seg = strtonum("0x" substr($0, 10, 4))
    if (seg % 4096 == 0) {
        hi = seg / 4096
        sum = (2 + 4 + int(hi / 256) + hi % 256) % 256
        printf ":02000004%04X%02X\n", hi, (256 - sum) % 256
        next
    }
}
/^:04000003/ { next }
{ print }
