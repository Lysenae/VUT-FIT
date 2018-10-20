-- Project: FLP #1 - plg-2-nka
-- Author:  Daniel Klimaj; xklima22@stud.fit.vutbr.cz

module Lib.RLG.Parser where

import Text.ParserCombinators.ReadP

import Lib.RLG.RLG
import Lib.Type.Symbol
import Lib.Misc.Misc

newLine = char '\n'
comma   = char ','
arrow   = string "->"

-- Parse input grammar
parseRlg :: String -> Either String RLG
parseRlg s = case readP_to_S rlgParser s of
   [(rlg, _)] -> Right rlg
   _ -> Left "Failed to parse RLG"

rlgParser :: ReadP RLG
rlgParser = do
    nonterminals <- parseNonterminals
    newLine
    terminals <- parseTerminals
    newLine
    start <- parseNonterminal
    newLine
    rules <- parseRules
    eof
    return $ RLG nonterminals terminals rules start

parseNonterminal  = many1 $ satisfy (isNterm)
parseNonterminals = sepBy1 parseNonterminal comma
parseTerminal     = many1 $ satisfy (isTerm)
parseTerminals    = sepBy parseTerminal comma
parseSymbols      = many1 $ satisfy (isSymbol)

parseRules = many1 $ do
    t <- parseRule
    newLine
    return t
  where
    parseRule = do
        left <- parseNonterminal
        arrow
        right <- parseSymbols
        return $ Rule left (splitStr right)
