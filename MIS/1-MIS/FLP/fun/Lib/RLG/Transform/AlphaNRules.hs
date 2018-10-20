-- Project: FLP #1 - plg-2-nka
-- Author:  Daniel Klimaj; xklima22@stud.fit.vutbr.cz

module Lib.RLG.Transform.AlphaNRules where

import Lib.RLG.RLG
import Lib.Type.Symbol
import Lib.RLG.Transform.Helpers

-- 2. Replace rules of form A->alphaB into form A->aA[n]
alphaNRules' :: RLG -> RLG
alphaNRules' rlg = addNterms (alphaNRules rlg)

alphaNRules :: RLG -> RLG
alphaNRules rlg = rlg { rules = trAlphaNRules' (rules rlg) }

trAlphaNRules' :: [Rule] -> [Rule]
trAlphaNRules' r = getRules (trAlphaNRules (1, r))

trAlphaNRules :: (Int, [Rule]) -> (Int, [Rule])
trAlphaNRules (i, []) = (i, [])
trAlphaNRules (i, (r:rs))
  | chkRuleAN r = trAlphaNRule (i, r) `addT` trAlphaNRules
    (getIndex (trAlphaNRule (i, r)), rs)
  | otherwise   = (i, []) `addT` trAlphaNRules (i, rs)

trAlphaNRule :: (Int, Rule) -> (Int, [Rule])
trAlphaNRule (i, r) = getIdxRuleTuple
  (trANRuleR ((left r), (right r), (length (right r) - 1), i, []))

-- trANRuleR leftSymbol rightSide ntermCount lastIndex rules
trANRuleR :: (Symbol, [Symbol], Int, Int, [Rule]) ->
  (Symbol, [Symbol], Int, Int, [Rule])
trANRuleR (l, [], _, i, r) = (l, [], 0, i, r)
trANRuleR (l, (t:ts), n, i, r)
  | n > 1     = trANRuleR
    ((createANterm i), ts, (n-1), i+1, r ++ [Rule l [t, (createANterm i)]])
  | n == 1    = trANRuleR (l, ts, (n-1), i, r ++ [Rule l [t, (ts!!0)]])
  | otherwise = (l, [], 0, i, r)
