-- Project: FLP #1 - plg-2-nka
-- Author:  Daniel Klimaj; xklima22@stud.fit.vutbr.cz

-- Miscellaneous methods not fitting in any module but used in many of them
module Lib.Misc.Misc where

import Data.Char

-- Checks if string is empty
emptyStr :: String -> Bool
emptyStr [] = True
emptyStr _  = False

-- Splits string into array of strings, each element is string of length 1
splitStr :: String -> [String]
splitStr []    = []
splitStr (h:r) = [h]:(splitStr r)

-- Checks if given char is symbol
isSymbol:: Char -> Bool
isSymbol c
  | (isAlpha c) == True || c == '#' = True
  | otherwise                       = False

-- Checks if given char is terminal
isTerm:: Char -> Bool
isTerm c
  | c == '#'  = True
  | isLower c = True
  | otherwise = False

-- Checks if given char is nonterminal
isNterm :: Char -> Bool
isNterm c
  | isUpper c = True
  | otherwise = False

-- Checks if given char is terminal
isTermS:: String -> Bool
isTermS s
  | isLower (s2ch s) = True
  | otherwise        = False

-- Checks if given char is nonterminal
isNtermS :: String -> Bool
isNtermS s
  | isUpper (s2ch s) = True
  | otherwise        = False

-- Converts first element of the string to char
s2ch :: String -> Char
s2ch []     = ' '
s2ch (s:rs) = s

-- Checks if string consists only from digits
allNum :: String -> Bool
allNum [] = False
allNum l  = (allNum' l)

-- See above, helper function
allNum' :: String -> Bool
allNum' []     = True
allNum' (s:ss) = (isDigit s) && (allNum' ss)

-- Merges 2 lists, appends only unique values
listMerge :: Eq a => [a] -> [a] -> [a]
listMerge l1 [] = l1
listMerge l1 (l:ls)
  | elem l l1 = listMerge l1 ls
  | otherwise = listMerge (l1 ++ [l]) ls

-- Shortcut of listMerge for infix usage
lm :: Eq a => [a] -> [a] -> [a]
lm l1 l2 = listMerge l1 l2

-- Appends string to string list if not already present
addUnique :: String -> [String] -> [String]
addUnique s l
  | s == []   = l
  | elem s l  = l
  | otherwise = (s:l)

-- Shortcut of addUnique for infix usage
au :: String -> [String] -> [String]
au s l = addUnique s l
