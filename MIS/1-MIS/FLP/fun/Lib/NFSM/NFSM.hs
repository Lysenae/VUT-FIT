-- Project: FLP #1 - plg-2-nka
-- Author:  Daniel Klimaj; xklima22@stud.fit.vutbr.cz

module Lib.NFSM.NFSM where

import Lib.Type.Symbol
import Lib.Type.State

data Transition = Transition
    { from    :: State
    , through :: Symbol
    , to      :: State
    }
  deriving (Show, Eq, Ord)

data NFSM = NFSM
    { states      :: [State]
    , alphabet    :: [Symbol]
    , transitions :: [Transition]
    , startSt     :: State
    , endSt       :: [State]
    }
  deriving (Show)
