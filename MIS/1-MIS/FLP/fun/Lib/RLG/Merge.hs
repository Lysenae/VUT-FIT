-- Project: FLP #1 - plg-2-nka
-- Author:  Daniel Klimaj; xklima22@stud.fit.vutbr.cz

module Lib.RLG.Merge where

import Lib.RLG.RLG
import Lib.Type.Symbol
import Lib.Misc.Misc

mergeNterm :: RLG -> RLG -> [Symbol]
mergeNterm r1 r2 = listMerge (nonterminals r1) (nonterminals r2)

mergeTerm :: RLG -> RLG -> [Symbol]
mergeTerm r1 r2 = listMerge (terminals r1) (terminals r2)

-- Latter overwrites start symbol
mergeStart :: RLG -> RLG -> Symbol
mergeStart r1 r2 = start r2

mergeRules :: RLG -> RLG -> [Rule]
mergeRules r1 r2 = listMerge (rules r1) (rules r2)

mergeRlg :: RLG -> RLG -> RLG
mergeRlg r1 r2 = RLG (mergeNterm r1 r2) (mergeTerm r1 r2) (mergeRules r1 r2)
  (mergeStart r1 r2)

-- Merge 2 RLGs into 1
mg :: RLG -> RLG -> RLG
mg r1 r2 = mergeRlg r1 r2
