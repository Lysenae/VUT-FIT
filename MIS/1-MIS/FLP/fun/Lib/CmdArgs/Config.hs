-- Project: FLP #1 - plg-2-nka
-- Author:  Daniel Klimaj; xklima22@stud.fit.vutbr.cz

module Lib.CmdArgs.Config where

data Config = Config
    { inner     :: Bool      -- Show inner represenration of RLG
    , transform :: Bool      -- Show transformed RLG
    , nfsm      :: Bool      -- Show equivalent NFSM
    , infile    :: FilePath  -- File containing input RLG
    }
  deriving (Show)
