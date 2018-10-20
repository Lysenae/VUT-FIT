-- Project: FLP #1 - plg-2-nka
-- Author:  Daniel Klimaj; xklima22@stud.fit.vutbr.cz

module Lib.RLG.Transform.AlphaRules where

import Lib.RLG.RLG
import Lib.Type.Symbol
import Lib.RLG.Transform.Helpers

-- 3. Replace rules of form A->alpha into form A->aB[n], ..., B[n]->#
alphaRules' :: RLG -> RLG
alphaRules' rlg = addNterms (alphaRules rlg)

alphaRules :: RLG -> RLG
alphaRules rlg = rlg { rules = trAlphaRules' (rules rlg) }

trAlphaRules' :: [Rule] -> [Rule]
trAlphaRules' r = getRules (trAlphaRules (1, r))

trAlphaRules :: (Int, [Rule]) -> (Int, [Rule])
trAlphaRules (i, []) = (i, [])
trAlphaRules (i, (r:rs))
  | chkRuleA r  = trAlphaRule (i, r) `addT` trAlphaRules
    (getIndex (trAlphaRule (i, r)), rs)
  | otherwise   = (i, []) `addT` trAlphaRules (i, rs)

trAlphaRule :: (Int, Rule) -> (Int, [Rule])
trAlphaRule (i, r) = getIdxRuleTuple
  (trARuleR ((left r), (right r), (length (right r)), i, []))

-- trARuleR leftSymbol rightSide ntermCount lastIndex rules
trARuleR :: (Symbol, [Symbol], Int, Int, [Rule]) ->
  (Symbol, [Symbol], Int, Int, [Rule])
trARuleR (l, [], _, i, r) = (l, [], 0, i, r ++ [Rule l ["#"]])
trARuleR (l, (t:ts), n, i, r)
  | n > 0     = trARuleR
    ((createBNterm i), ts, (n-1), i+1, r ++ [Rule l [t, (createBNterm i)]])
  | n == 0    = trARuleR (l, ts, (n-1), i, r ++ [Rule l ["#"]])
  | otherwise = (l, [], 0, i, r)
