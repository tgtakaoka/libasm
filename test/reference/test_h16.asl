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

        cpu     HD641016
        org     0
        include "test_h16.inc"

;;; BIT dynamic forms: ASL diverges from the HD641016 manual (encodes
;;; these at 0x64-0x66), so emit the manual's 0x68-0x6A bytes directly.
;;; The libasm wrapper assembles the equivalent mnemonics.
        dc.b    $68,$02,$01
        dc.b    $68,$22,$01
        dc.b    $68,$42,$01
        dc.b    $68,$62,$01
        dc.b    $69,$02,$01
        dc.b    $6A,$02,$01
        dc.b    $68,$0E,$10,$10
        end
