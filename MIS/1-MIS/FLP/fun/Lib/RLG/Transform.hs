-- Project: FLP #1 - plg-2-nka
-- Author:  Daniel Klimaj; xklima22@stud.fit.vutbr.cz

module Lib.RLG.Transform where

import Lib.Misc.Misc

import Lib.RLG.RLG
import Lib.RLG.Merge

import Lib.RLG.Transform.PreservedRules
import Lib.RLG.Transform.AlphaNRules
import Lib.RLG.Transform.AlphaRules
import Lib.RLG.Transform.SimpleRules

-- Transform RLG into RLG with rules of form A->xB or A->#, where A and B are
-- nonterminals and x is single terminal
transformRlg :: RLG -> Either String RLG
transformRlg rlg = Right $ simpleRules' rlg ((preservedRules' rlg) `mg`
  (alphaNRules' rlg) `mg` (alphaRules' rlg))
