module Hw1 where

data Cell = H | P | O | X deriving (Read,Show,Eq)
data Result = Fail | Caught (Int,Int) deriving (Read,Show,Eq)
data Direction = N | S | E | W deriving (Read,Show,Eq)

simulate :: [[Cell]] -> [(Direction, Direction)] -> Result
-- DO NOT CHANGE ABOVE THIS LINE, WRITE YOUR CODE BELOW --



f :: [Cell] -> Cell -> Int -> Int
f [] _ _  = 1000
f (a:sublist) b x 
    | a==b = x
    | otherwise = (f sublist b (x+1))

find :: [[Cell]] -> Cell -> Int -> Int -> (Int,Int)
find [] _ _ _= (1000,1000)
find (c:nestedlist) d x y
    | zey == 1000 = (find nestedlist d x (y+1))
    | otherwise = (zey,y)
    where zey = (f c d x)

update :: [[Cell]] -> Direction -> (Int , Int) -> (Int,Int)
update [] _ _  = (1000,1000)
update list kanye yer
    | kanye == N && (snd yer) /= 0 && ((concat list)!!nereye1) /= X = (fst yer,(snd yer) -1)
    | kanye == S && (length list) > (snd yer) +1 && ((concat list)!!nereye2) /= X = (fst yer,(snd yer) +1)
    | kanye == E && (lan list) > (fst yer) +1 && ((concat list)!!nereye3) /= X = ((fst yer) +1,snd yer)
    | kanye == W && (fst yer) /= 0 && ((concat list)!!nereye4) /= X = ((fst yer) -1,snd yer)
    | otherwise = yer 
    where nereye1=((snd yer)-1)*(lan list) + fst yer
          nereye2=((snd yer)+1)*(lan list) + fst yer
          nereye3=((snd yer))*(lan list) + (fst yer)+1
          nereye4=((snd yer))*(lan list) + (fst yer)-1

lan :: [[Cell]] -> Int
lan (x:list) = (length x)

nested :: [Cell] -> Int -> [[Cell]]
nested [] _ = []
nested list n = (take n list):(nested (drop n list) n )

remove :: [[Cell]] -> Cell -> (Int,Int) -> [[Cell]]
remove list cl nerede = (nested (fst notGlued ++ [cl] ++ snd notGlued) (lan list ))
    where notGlued = (take (yer) (concat list), drop (yer+1) (concat list))
          yer = (snd nerede)*(lan list) + (fst nerede)

move :: [[Cell]] -> (Int,Int)  -> Direction -> Cell -> [[Cell]]
move list yer yon cl
    | yer == (update list yon yer) = list
    | otherwise = remove (remove list O yer) cl (update list yon yer)


simulate [] _ = error "empty cell"
simulate _ [] = Fail
simulate list (x:yon) 
    | update list (fst x) hunter == update list (snd x) prey = Caught (update list (snd x) prey)
    | hunter == update list (snd x) prey && prey == update list (fst x) hunter = simulate(remove(remove list H prey) P hunter) yon
    | hunter == update list (snd x) prey = simulate (move (move list hunter (fst x) H) prey (snd x) P) yon
    | prey == update list (fst x) hunter = simulate (move (move list prey (snd x) P) hunter (fst x) H) yon
    | otherwise = simulate (move (move list prey (snd x) P) hunter (fst x) H) yon
     where hunter=(find list H 0 0)
           prey = (find list P 0 0)

--    | (find list P 0 0) == hunter = Caught (find list P 0 0)
--    | prey == find list H 0 0 = Caught prey