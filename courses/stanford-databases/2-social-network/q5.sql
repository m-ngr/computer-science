-- For every situation where student A likes student B, but we have no information about whom B likes 
-- (that is, B does not appear as an ID1 in the Likes table), return A and B's names and grades.

SELECT A.name, A.grade, B.name, B.grade
FROM Likes L
JOIN Highschooler A ON A.ID = L.ID1
JOIN Highschooler B ON B.ID = L.ID2
WHERE B.ID NOT IN (SELECT ID1 FROM Likes);
