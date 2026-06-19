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

;;; MC68HCS08 additions over MC68HC08
        sthx    $90
        ldhx    $90
        cphx    $90
        ldhx    $1000
        cphx    $2000
        sthx    $8000
        bgnd
        ldhx    ,x
        ldhx    $1234,x
        ldhx    $12,x
        ldhx    5,sp
        sthx    5,sp
        cphx    5,sp
