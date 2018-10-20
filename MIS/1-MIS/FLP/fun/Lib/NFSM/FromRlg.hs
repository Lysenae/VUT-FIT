-- Project: FLP #1 - plg-2-nka
-- Author:  Daniel Klimaj; xklima22@stud.fit.vutbr.cz

module Lib.NFSM.FromRlg where

import Data.Array
import Data.List

import Lib.RLG.RLG
import Lib.NFSM.NFSM
import Lib.NFSM.Helpers
import Lib.Type.Symbol
import Lib.Type.State

-- Converts transformed RLG into NFSM
fromRlg :: RLG -> Either String NFSM
fromRlg rlg = Right $ NFSM (getStates rlg) (terminals rlg) (createTransitions rlg)
  (getStartState rlg) (getEndStates rlg)

-- Converts nonterminals of RLG into states. Each state is represented by nonzero
-- number specified by it's index in nonterminals list increased by 1
getStates :: RLG -> [State]
getStates rlg = range (1, length (nonterminals rlg))

-- Converts rules of RLG into transitions
createTransitions :: RLG -> [Transition]
createTransitions rlg = createTransitions' (rules rlg) (nonterminals rlg)

getStartState :: RLG -> State
getStartState rlg = nt2state (start rlg) (nonterminals rlg)

getEndStates :: RLG -> [State]
getEndStates rlg = sort $ getEndStates' (rules rlg) (nonterminals rlg)

-- See createTransitions
createTransitions' :: [Rule] -> [Symbol] -> [Transition]
createTransitions' [] _ = []
createTransitions' (r:rs) s
  | epsRule r = createTransitions' rs s
  | otherwise = (Transition (nt2state (left r) s) (firstRSym r)
    (nt2state (secondRSym r) s)) : createTransitions' rs s

getEndStates' :: [Rule] -> [Symbol] -> [State]
getEndStates' [] _ = []
getEndStates' (r:rs) s
  | epsRule r = (nt2state (left r) s) : getEndStates' rs s
  | otherwise = getEndStates' rs s
