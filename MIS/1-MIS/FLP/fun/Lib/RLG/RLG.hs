-- Project: FLP #1 - plg-2-nka
-- Author:  Daniel Klimaj; xklima22@stud.fit.vutbr.cz

module Lib.RLG.RLG where

import Lib.Type.Symbol

data Rule = Rule
    { left  :: Symbol
    , right :: [Symbol]
    }
  deriving (Show, Eq, Ord)

data RLG = RLG
    { nonterminals :: [Symbol]
    , terminals    :: [Symbol]
    , rules        :: [Rule]
    , start        :: Symbol
    }
  deriving (Show)

-- Returns length of right side of the rule
lengthR :: Rule -> Int
lengthR r = length (right r)

-- Returns nth symbol of right side of the rule or empty string if
-- index is invalid
nthRSym :: Int -> Rule -> Symbol
nthRSym i r
  | i > (lengthR r) - 1 = ""
  | i < 0               = ""
  | otherwise           = (right r)!!i

lastRSym :: Rule -> Symbol
lastRSym r = nthRSym ((lengthR r)-1) r

firstRSym :: Rule -> Symbol
firstRSym r = nthRSym 0 r

secondRSym :: Rule -> Symbol
secondRSym r = nthRSym 1 r
