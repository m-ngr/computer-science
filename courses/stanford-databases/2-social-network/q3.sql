-- For every pair of students who both like each other, return the name and grade of both students. 
-- Include each pair only once, with the two names in alphabetical order.

SELECT H1.name, H1.grade, H2.name, H2.grade
FROM Likes L1 JOIN Likes L2 ON L1.ID1 = L2.ID2 AND L1.ID2 = L2.ID1
JOIN Highschooler H1 ON H1.ID = L1.ID1 
JOIN Highschooler H2 ON H2.ID = L1.ID2 
WHERE H1.name < H2.name;