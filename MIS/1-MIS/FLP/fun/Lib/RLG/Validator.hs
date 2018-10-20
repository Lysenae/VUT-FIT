-- Project: FLP #1 - plg-2-nka
-- Author:  Daniel Klimaj; xklima22@stud.fit.vutbr.cz

-- Validates RLG and returns Left if given grammar is invalid RLG and returns
-- Right if grammar is valid
module Lib.RLG.Validator where

import Lib.RLG.RLG
import Lib.Type.Symbol
import Lib.Misc.Misc

-- Check left side of the rule, returns False if symbol is not nonterminal
chkLRuleSides :: [Symbol] -> [Rule] -> Bool
chkLRuleSides _ []        = True -- Everything else checked
chkLRuleSides ntms (r:rs) = (chkLRuleSide ntms r) && (chkLRuleSides ntms rs)

chkLRuleSide :: [Symbol] -> Rule -> Bool
chkLRuleSide ntms rule = elem (left rule) ntms

-- Check right side of the rule, returns False if rule consists of invalid
-- terminals or nonterminals or when rule is not form of A->alphaB, where
-- alpha is 0-n terminals and B is nonterminal or A->#
-- chkRRuleSides nonterminals terminals rules
chkRRuleSides :: [Symbol] -> [Symbol] -> [Rule] -> Bool
chkRRuleSides _ _ []     = True
chkRRuleSides n t (r:rs) = (chkRRuleSide (right r) n t) && (chkRRuleSides n t rs)

-- chkRRuleSide rule nonterminals terminals
chkRRuleSide :: [Symbol] -> [Symbol] -> [Symbol] -> Bool
chkRRuleSide [] _ _     = True
chkRRuleSide (r:rs) n t = (chkSymRRule r n t) && (chkRRuleSide rs n t)

chkSymRRule :: Symbol -> [Symbol] -> [Symbol] -> Bool
chkSymRRule s n t
  | isTermS s && elem s t  = True
  | isNtermS s && elem s n = True
  | s == "#"               = True
  | otherwise              = False

chkRRuleSidesFormat :: [Rule] -> Bool
chkRRuleSidesFormat []     = True
chkRRuleSidesFormat (r:rs) =
  (chkRRuleSideFormat (right r) False) && (chkRRuleSidesFormat rs)

-- chkRRuleSideFormat ruleRightSide nonterminalFound
chkRRuleSideFormat :: [Symbol] -> Bool -> Bool
chkRRuleSideFormat [] _            = True
chkRRuleSideFormat (r:rs) ntmFound
  | ntmFound  = False -- Nonterminal must be the last symbol
  | otherwise = chkRRuleSideFormat rs (isNterm (s2ch r))

validateRlg :: RLG -> Either String RLG
validateRlg (RLG nterm term rules start)
  | elem start nterm == False =
    Left "Start symbol is not valid nonterminal"
  | chkLRuleSides nterm rules == False =
    Left "Invalid nonterminal on the left side of the rule"
  | chkRRuleSidesFormat rules == False =
    Left "Invalid format of the right side of the rule"
  | chkRRuleSides nterm term rules == False =
    Left "Invalid symbol in the the right side of the rule"
  | otherwise = Right $ RLG nterm term rules start
