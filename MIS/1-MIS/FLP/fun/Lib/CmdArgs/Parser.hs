-- Project: FLP #1 - plg-2-nka
-- Author:  Daniel Klimaj; xklima22@stud.fit.vutbr.cz

module Lib.CmdArgs.Parser where

import Lib.CmdArgs.Config
import Lib.CmdArgs.Help

-- Parse command line input
parseArg :: String -> String -> Either String Config
parseArg a inf
  | a == "-i" = (Right $ Config True False False inf)
  | a == "-1" = (Right $ Config False True False inf)
  | a == "-2" = (Right $ Config False False True inf)
  | otherwise = Left $ "Invalid option " ++ a

parseArguments :: [String] -> Either String Config
parseArguments [x]   = parseArg x ""
parseArguments [x,y] = parseArg x y
parseArguments _     = Left helpMsg
