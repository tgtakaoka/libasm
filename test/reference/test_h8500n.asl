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

;;; H8/500 in minimum mode - base + E-clock instructions (no page jumps).
;;; HD6475328 (H8/532) is the only H8/500-family code ASL knows; both H8/500
;;; and H8/520 share the same CPU core, differing only in the E-clock pin
;;; (H8/520 lacks MOVTPE/MOVFPE).

        cpu     HD6475328
        org     H'0100
        include "test_h8500.inc"
        include "test_h8500_eclk.inc"
        end
