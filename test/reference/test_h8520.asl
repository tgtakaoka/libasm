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

;;; H8/520 in maximum mode - identical to H8/500 except no E-clock instructions
;;; (MOVTPE/MOVFPE) because H8/520 has no E-clock output pin.
;;; HD6475328 is the only H8/500-family code ASL knows; the omitted .inc file
;;; covers the only H8/520-vs-H8/500 difference.

        cpu     HD6475328
        maxmode on
        org     H'010000
        include "test_h8500.inc"
        include "test_h8500_pjmp.inc"
        end
