;;; Copyright 2021 Tadashi G. Takaoka
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

        cpu     68HCS08
        org     $0100
abs:    equ     $C000
        include "test_mc6805.inc"
        include "test_mc68hc05.inc"
        include "test_mc68hc08.inc"
        include "test_mc68hcs08.inc"
