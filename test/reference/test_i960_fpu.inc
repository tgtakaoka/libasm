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

;;; The 80960KB adds the floating-point instructions and three decimal ones to
;;; the base set; GNU as gathers exactly those under -AKB.  A source operand
;;; takes a global or local register, a floating-point register, or one of the
;;; two real literals +0.0 and +1.0; a destination takes no literal (Manual
;;; Table B-1, Table B-2 and the text below them).  A long real held in the
;;; general registers needs a pair and an extended real a quad.

;;; Real arithmetic
        addr    fp0, fp1, fp2
        addr    g0, g1, g2
        addr    1.0, g1, fp0
        addrl   g0, g2, fp3
        addrl   fp0, fp1, g4
        subr    0.0, fp0, fp1
        subrl   g0, g2, g4
        mulr    g0, g1, g2
        mulrl   fp0, fp1, fp2
        divr    g0, g1, g2
        divrl   g0, g2, g4
        remr    g0, g1, g2
        remrl   g0, g2, g4
        roundr  g0, g1
        roundrl g0, g2
        sqrtr   g0, g1
        sqrtrl  g0, g2
        scaler  g0, fp1, fp2
        scalerl g0, g2, g4

;;; Transcendental
        atanr   fp0, fp1, fp2
        atanrl  g0, g2, g4
        logr    g0, g1, g2
        logrl   g0, g2, g4
        logbnr  fp0, fp1
        logbnrl g0, g2
        logepr  g0, g1, g2
        logeprl g0, g2, g4
        expr    g0, g1
        exprl   g0, g2
        sinr    g0, g1
        sinrl   g0, g2
        cosr    g0, g1
        cosrl   g0, g2
        tanr    g0, g1
        tanrl   g0, g2

;;; Compare and classify
        cmpr    fp0, fp1
        cmprl   g0, g2
        cmpor   g0, g1
        cmporl  g0, g2
        classr  fp0
        classrl g0

;;; Move and convert
        movr    fp0, fp1
        movrl   g0, fp2
        movre   g0, g4
        cpysre  g0, g4, g8
        cpyrsre g0, g4, g8
        cvtir   g0, fp0
        cvtilr  g0, g2
        cvtri   fp0, g1
        cvtril  fp0, g2
        cvtzri  fp0, g1
        cvtzril fp0, g2

;;; Decimal
        daddc   g0, g1, g2
        dsubc   g0, g1, g2
        dmovt   g0, g1
