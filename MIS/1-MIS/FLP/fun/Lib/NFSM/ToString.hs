-- Project: FLP #1 - plg-2-nka
-- Author:  Daniel Klimaj; xklima22@stud.fit.vutbr.cz

module Lib.NFSM.ToString where

import Lib.NFSM.NFSM
import Lib.Type.Symbol
import Lib.Type.State

-- Converts NFSM structure int readable form
nfsm2str :: Either String NFSM -> String
nfsm2str (Left msg)   = msg
nfsm2str (Right nfsm) =
  stateList2str (states nfsm) "," ++ "\n" ++
  --symList2str (alphabet nfsm) "," ++ "\n" ++
  show (startSt nfsm) ++ "\n" ++
  stateList2str (endSt nfsm) "," ++ "\n" ++
  cvtTransitions (transitions nfsm)

-- Converts transitions into form StateFrom,Symbol,StateTo
cvtTransitions :: [Transition] -> String
cvtTransitions []     = ""
cvtTransitions (t:ts) = (transition2str t) ++ (cvtRestTrsts ts)

cvtRestTrsts :: [Transition] -> String
cvtRestTrsts []     = ""
cvtRestTrsts (t:ts) = "\n" ++ (transition2str t) ++ (cvtRestTrsts ts)

transition2str :: Transition -> String
transition2str t = (show $ from t) ++ "," ++ through t ++ "," ++ (show $ to t)
