-- Project: FLP #1 - plg-2-nka
-- Author:  Daniel Klimaj; xklima22@stud.fit.vutbr.cz

module Lib.NFSM.Helpers where

import Data.List
import Data.Maybe

import Lib.Type.Symbol
import Lib.Type.State
import Lib.RLG.RLG

-- Converts nonterminal into state
nt2state :: Symbol -> [Symbol] -> State
nt2state _ []     = 0
nt2state s l@(_:_)
  | elemIndex s l == Nothing = 0
  | otherwise                = (fromJust (elemIndex s l)) + 1

-- Is epsilon rule?
epsRule :: Rule -> Bool
epsRule r
  | lengthR r == 1 && firstRSym r == "#" = True
  | otherwise                            = False
