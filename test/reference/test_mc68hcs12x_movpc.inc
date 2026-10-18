;;; Copyright 2026 Tadashi G. Takaoka
;;;
;;; Licensed under the Apache License, Version 2.0 (the "License");
;;; you may not use this file except in compliance with the License.
;;; You may obtain a copy of the License at
;;;
;;;     http://www.apache.org/licenses/LICENSE-2.0
;;;
;;; Unless required by applicable law or agreed to in writing, software
;;; distributed under the License is distributed on an "AS IS" BASIS,
;;; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
;;; See the License for the specific language governing permissions and
;;; limitations under the License.

;;; PC-relative move
        movb    #$12,*+$85,pc
        movb    #$12,*+$1005,pc
        movb    *+5,pc,100,x
        movb    *+4,pc,$1234,x
        movb    *+4,pc,[$1234,x]
        movb    *+4,pc,[d,x]
        movb    *+$85,pc,2,x
        movb    *+$1005,pc,2,x
        movb    1,x,*+$85,pc
        movb    1,x,*+$1005,pc
        movb    *+$1005,pc,*+$2000,pc
        movw    #$1234,*+$1006,pc
        movw    *+$85,pc,$1234,y
        movw    *+$1005,pc,$1234,y
;;; Local Variables:
;;; mode: asm
;;; End:
;;; vim: set ft=asm:
