-- Project: FLP #1 - plg-2-nka
-- Author:  Daniel Klimaj; xklima22@stud.fit.vutbr.cz

module Lib.CmdArgs.Help where

helpMsg :: String
helpMsg = "plg-2-nka [options] [input]\n\n\
  \   Options:\n\
  \     -i   Print inner representation of RLG\n\
  \     -1   Transform RLG into grammar with only rules of the form A->xB or A->#\n\
  \     -2   Convert RLG to NFSM\n\n\
  \   input  Input file"
