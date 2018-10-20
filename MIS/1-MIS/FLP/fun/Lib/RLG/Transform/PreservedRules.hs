-- Project: FLP #1 - plg-2-nka
-- Author:  Daniel Klimaj; xklima22@stud.fit.vutbr.cz

module Lib.RLG.Transform.PreservedRules where

import Lib.RLG.RLG
import Lib.RLG.Transform.Helpers

-- 1. Preserve rules of form A->xB and A->#
preservedRules' :: RLG -> RLG
preservedRules' rlg = rlg { rules = preservedRules (rules rlg) }

preservedRules :: [Rule] -> [Rule]
preservedRules [] = []
preservedRules (r:rs)
  | chkRuleP r = [r] ++ preservedRules rs
  | otherwise  = [] ++ preservedRules rs
