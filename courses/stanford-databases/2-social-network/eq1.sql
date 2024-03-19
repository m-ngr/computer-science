/*
For every situation where student A likes student B, but student B likes a different student C,
return the names and grades of A, B, and C.
*/

SELECT A.name, A.grade, B.name, B.grade, C.name, C.grade
FROM Likes L1
JOIN Highschooler A ON A.ID = L1.ID1
JOIN Highschooler B ON B.ID = L1.ID2
JOIN Likes L2 ON L2.ID1 = L1.ID2 AND L2.ID2 <> L1.ID1
JOIN Highschooler C ON C.ID = L2.ID2;