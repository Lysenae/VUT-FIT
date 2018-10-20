-- Project: FLP #1 - plg-2-nka
-- Author:  Daniel Klimaj; xklima22@stud.fit.vutbr.cz

module Lib.RLG.Handler where

import Lib.CmdArgs.Config

import Lib.RLG.RLG
import Lib.RLG.Parser
import Lib.RLG.Validator
import Lib.RLG.Transform
import Lib.RLG.ToString

import Lib.NFSM.NFSM
import Lib.NFSM.FromRlg
import Lib.NFSM.ToString

-- Do action specified by command line arguments
handleRlg :: Config -> String -> String
handleRlg (Config inner transform nfsm _) input
  | inner     = getStr (handleInnerRlg input)
  | transform = rlg2str (handleTransformRlg input)
  | nfsm      = nfsm2str (handleNfsm input)
  | otherwise = "Invalid config" -- Should not happen

-- Print inner representation of RLG
handleInnerRlg :: String -> Either String RLG
handleInnerRlg input = do
  inRlg <- parseRlg input
  validateRlg inRlg

-- Print transformed RLG
handleTransformRlg :: String -> Either String RLG
handleTransformRlg input = do
  inRlg <- parseRlg input
  validRlg <- validateRlg inRlg
  transformRlg validRlg

-- Print NFSM equivalent to input grammar
handleNfsm :: String -> Either String NFSM
handleNfsm input = do
  inRlg <- parseRlg input
  validRlg <- validateRlg inRlg
  trRlg <- transformRlg validRlg
  fromRlg trRlg
