-- Project: FLP #1 - plg-2-nka
-- Author:  Daniel Klimaj; xklima22@stud.fit.vutbr.cz

module Main
    (main)
  where

import Control.Applicative
import Control.Exception
import System.Environment
import System.IO
import System.IO.Error

import Lib.Misc.Misc
import Lib.CmdArgs.Parser
import Lib.CmdArgs.Config
import Lib.RLG.RLG
import Lib.RLG.Handler

-- Main
main :: IO ()
main = do
  c <- parseArguments <$> getArgs
  either putStrLn handleCmdArgs c `catch` readExHandler

-- Parse command line arguments and do desired action
handleCmdArgs :: Config -> IO ()
handleCmdArgs conf = do
  input <- getInput (infile conf)
  putStrLn (handleRlg conf input)

-- Gets input from specified file or stdin
getInput :: FilePath -> IO String
getInput inpath
  | emptyStr inpath = getContents
  | otherwise       = readFile inpath

-- Exception handler for open file
readExHandler :: IOError -> IO ()
readExHandler e = putStrLn ("ERROR: " ++ show e)
