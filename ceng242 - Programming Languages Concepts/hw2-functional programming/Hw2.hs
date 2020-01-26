module Hw2 where

import Data.List -- YOU MAY USE THIS MODULE FOR SORTING THE AGENTS

data Level = Newbie | Intermediate | Expert deriving (Enum, Eq, Ord, Show, Read)
data Hunter = Hunter {hID::Int, hlevel::Level, hEnergy::Int, hNumberOfCatches::Int, hActions::[Direction]} deriving (Eq, Show, Read)
data Prey = Prey {pID::Int, pEnergy::Int, pActions::[Direction]} deriving (Eq, Show, Read)
data Cell = O | X | H Hunter | P Prey | T deriving (Eq, Show, Read)
data Direction = N | S | E | W deriving (Eq, Show, Read)
type Coordinate = (Int, Int)

--simulate :: [ [ Cell ] ] -> ( [ ( Hunter , Coordinate ) ] , [ ( Prey , Coordinate ) ])
-- DO NOT CHANGE THE DEFINITIONS ABOVE. --


-- INSTANCES OF Ord FOR SORTING, UNCOMMENT AND COMPLETE THE IMPLEMENTATIONS --
letsee1::Hunter -> Hunter -> Ordering
letsee1 (Hunter id1 lev1 en1 c1 x) (Hunter id2 lev2 en2 c2 y) = if en2 > en1 then LT
	                                                            else if en1 > en2 then GT
	                                                            else if c2 > c1 then LT
	                                                            else if c1 > c2 then GT
	                                                            else if id2 > id1 then GT
	                                                            else if id1 > id2 then LT
	                                                            else EQ
letsee:: Hunter -> Hunter -> Ordering
letsee (Hunter id1 lev1 en1 c1 x) (Hunter id2 lev2 en2 c2 y)
   | (compare lev1 lev2 ) == EQ  = letsee1 (Hunter id1 lev1 en1 c1 x) (Hunter id2 lev2 en2 c2 y)
   | otherwise = (compare lev1 lev2 )

instance Ord Hunter where
    compare (Hunter id1 lev1 en1 c1 x) (Hunter id2 lev2 en2 c2 y) = letsee (Hunter id1 lev1 en1 c1 x) (Hunter id2 lev2 en2 c2 y)  

offf:: Prey -> Prey -> Ordering
offf (Prey id1 en1 x) (Prey id2 en2 y) = if en1 > en2 then GT
    	                               else if en2 > en1 then LT
    	                               else if id1 > id2 then LT
    	                               else if id2 > id1 then GT
    	                               else EQ

instance Ord Prey where
    compare (Prey id1 en1 x) (Prey id2 en2 y) = offf (Prey id1 en1 x) (Prey id2 en2 y)

-- WRITE THE REST OF YOUR CODE HERE --
fprey :: [Cell] -> Int -> Int -> [(Prey,Coordinate)]
fprey [] _ _ = []
fprey (a:xs) x y = case a of P n -> (n,(x,y)):(fprey xs (x+1) y)
                             H n -> (fprey xs (x+1) y)
                             _ -> (fprey xs (x+1) y) 
                                 

findprey ::  [[Cell]]-> Int -> Int -> [(Prey,Coordinate)]
findprey [] _ _ = [] 
findprey (a:xs) x y = (fprey a x y)++(findprey xs x (y+1))

fhunter :: [Cell] -> Int -> Int -> [(Hunter,Coordinate)]
fhunter [] _ _ = []
fhunter (a:xs) x y = case a of H n -> (n,(x,y)):(fhunter xs (x+1) y)
                               P n -> (fhunter xs (x+1) y)
                               _ -> (fhunter xs (x+1) y) 

findhunter :: [[Cell]] -> Int -> Int -> [(Hunter,Coordinate)]
findhunter [] _ _ = []
findhunter (a:xs) x y = (fhunter a x y) ++ (findhunter xs x (y+1))

lan :: [[Cell]] -> Int
lan (x:list) = (length x)


h::[[Cell]]-> Hunter -> Coordinate -> (Hunter,Coordinate)
h list (Hunter id lev en c (x:xs)) nerede
    | en < 10 = ((Hunter id lev (en+1) c xs),nerede)
    | x == N && (snd nerede) /= 0               && (list!!(second-1)!!first ) /= X = ((Hunter id lev (en-1) c xs),(fst nerede,(snd nerede)-1))
    | x == S && (length list) > (snd nerede) +1 && (list!!(second+1)!!first ) /= X = ((Hunter id lev (en-1) c xs),(fst nerede,(snd nerede)+1))
    | x == E && (lan list) > (fst nerede) +1    && (list!!second!!(first+1) ) /= X = ((Hunter id lev (en-1) c xs),((fst nerede)+1,snd nerede))
    | x == W && (fst nerede) /= 0               && (list!!second!!(first-1) ) /= X = ((Hunter id lev (en-1) c xs),((fst nerede)-1,snd nerede))
    | otherwise = ((Hunter id lev (en-1) c xs),nerede)
    where first = fst nerede 
          second = snd nerede 


updatehunter :: [[Cell]] -> [(Hunter,Coordinate)] -> [(Hunter,Coordinate)]
updatehunter _ [] = []
updatehunter list (x:piyon) = (h list (fst x) (snd x)):(updatehunter list piyon)  
     
p::[[Cell]]-> Prey -> Coordinate -> (Prey,Coordinate)
p list (Prey id en (x:xs)) nerede
    | en < 10 && (concat list)!!nerede5 == T = ((Prey id 0 xs),nerede)
    | en < 10  = ((Prey id (en+1) xs),nerede)
    | x == N && (snd nerede) /= 0               && ((concat list)!!nereye1) == T = ((Prey id (en-11) xs),(fst nerede,(snd nerede)-1))
    | x == N && (snd nerede) /= 0               && ((concat list)!!nereye1) /= X = ((Prey id (en-1) xs),(fst nerede,(snd nerede)-1))
    | x == S && (length list) > (snd nerede) +1 && ((concat list)!!nereye2) == T = ((Prey id (en-11) xs),(fst nerede,(snd nerede)+1))
    | x == S && (length list) > (snd nerede) +1 && ((concat list)!!nereye2) /= X = ((Prey id (en-1) xs),(fst nerede,(snd nerede)+1))
    | x == E && (lan list) > (fst nerede) +1    && ((concat list)!!nereye3) == T = ((Prey id (en-11) xs),((fst nerede)+1,snd nerede))
    | x == E && (lan list) > (fst nerede) +1    && ((concat list)!!nereye3) /= X = ((Prey id (en-1) xs),((fst nerede)+1,snd nerede))
    | x == W && (fst nerede) /= 0               && ((concat list)!!nereye4) == T = ((Prey id (en-11) xs),((fst nerede)-1,snd nerede))
    | x == W && (fst nerede) /= 0               && ((concat list)!!nereye4) /= X = ((Prey id (en-1) xs),((fst nerede)-1,snd nerede))
    | en==10 && (concat list)!!nerede5 == T = ((Prey id 0 xs),nerede)
    | (concat list)!!nerede5 == T = ((Prey id (en-11) xs),nerede)
    | otherwise = ((Prey id (en-1) xs),nerede)
    where nereye1=((snd nerede)-1)*(lan list) + fst nerede
          nereye2=((snd nerede)+1)*(lan list) + fst nerede
          nereye3=((snd nerede))*(lan list) + (fst nerede)+1
          nereye4=((snd nerede))*(lan list) + (fst nerede)-1
          nerede5=(snd nerede)*(lan list) + (fst nerede)


updateprey :: [[Cell]] -> [(Prey,Coordinate)] -> [(Prey,Coordinate)]
updateprey _ [] = []
updateprey list (x:piyon) = (p list (fst x) (snd x)):(updateprey list piyon)

yakaladi:: Hunter -> Hunter
yakaladi (Hunter id lev en c x) = if en + 20 > 100 then (Hunter id lev 100 (c+1) x)
	                              else (Hunter id lev (en+20) (c+1) x)
 
thisisme :: (Hunter,Coordinate) -> [(Prey,Coordinate)] -> [(Prey,Coordinate)] -> Int -> ((Hunter,Coordinate),[(Prey,Coordinate)])
thisisme hunter (p:prey) bos x
    | x==2 = (hunter,[p]++prey++bos)
    | prey == [] && snd p == snd hunter = ((yakaladi (fst hunter), snd hunter),bos) 
    | prey == [] && snd p /= snd hunter = (hunter,(p:bos)) 
    | snd p == snd hunter = thisisme (yakaladi (fst hunter), snd hunter) prey bos (x+1)
    | otherwise = thisisme hunter prey (p:bos) x

fromnowon :: [(Prey,Coordinate)] -> [(Hunter,Coordinate)] -> [(Hunter,Coordinate)] -> ([(Hunter,Coordinate)],[(Prey,Coordinate)])
fromnowon prey (h:hunter) bos
    | prey == [] = (([h]++hunter++bos),[])
    | hunter == [] = (((fst milliondreams):bos),(snd milliondreams))
    | otherwise = fromnowon (sort(snd milliondreams)) hunter ((fst milliondreams):bos)
    where milliondreams = (thisisme h prey [] 0) 

rec :: [[Cell]] -> Int -> [(Prey,Coordinate)] -> [(Hunter,Coordinate)] -> ([(Hunter,Coordinate)],[(Prey,Coordinate)])
rec list x prey hunter
    | prey == [] || x==0 = (reverse (sort hunter),sort prey)
    | otherwise = rec list (x-1) (snd harrypotter) (fst harrypotter)
    where harrypotter = fromnowon (sort (updateprey list prey)) (reverse (sort (updatehunter list hunter))) []

kackere :: Prey -> Int
kackere (Prey id en x) = length x

simulate :: [[Cell]] -> ([(Hunter,Coordinate)],[(Prey,Coordinate)])
simulate list = rec list (kackere (fst (head (findprey list 0 0)))) (findprey list 0 0) (findhunter list 0 0)





















