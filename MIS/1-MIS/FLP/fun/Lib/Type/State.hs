-- Project: FLP #1 - plg-2-nka
-- Author:  Daniel Klimaj; xklima22@stud.fit.vutbr.cz

module Lib.Type.State where

type State = Int

-- stateList2str symbolList delimiter
stateList2str :: [State] -> String -> String
stateList2str [] _ = ""
stateList2str (s:ss) d
  | ss == []  = show s
  | otherwise = show s ++ d ++ (stateList2str ss d)
