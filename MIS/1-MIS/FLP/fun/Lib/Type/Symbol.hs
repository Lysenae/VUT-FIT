-- Project: FLP #1 - plg-2-nka
-- Author:  Daniel Klimaj; xklima22@stud.fit.vutbr.cz

module Lib.Type.Symbol where

type Symbol = String

-- symList2str symbolList delimiter
symList2str :: [Symbol] -> String -> String
symList2str [] _ = ""
symList2str (s:ss) d
  | ss == []  = s
  | otherwise = s ++ d ++ (symList2str ss d)
