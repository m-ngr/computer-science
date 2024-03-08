-- For each student A who likes a student B where the two are not friends,
-- find if they have a friend C in common For all such trios, return the name and grade of A, B, and C.

SELECT DISTINCT A.name, A.grade, B.name, B.grade, C.name, C.grade 
FROM Friend F
JOIN (SELECT * FROM Likes L
WHERE L.ID2 NOT IN (SELECT ID2 FROM Friend WHERE ID1 = L.ID1 )
) NF
JOIN Highschooler A ON A.ID = NF.ID1
JOIN Highschooler B ON B.ID = NF.ID2
JOIN Highschooler C ON C.ID = F.ID1
WHERE NF.ID1 IN (SELECT ID2 FROM Friend WHERE ID1 = F.ID1)
AND NF.ID2 IN (SELECT ID2 FROM Friend WHERE ID1 = F.ID1) ;
