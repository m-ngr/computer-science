/*
Find those students for whom all of their friends are in different grades from themselves.
Return the students' names and grades.
*/


SELECT DISTINCT H1.name, H1.grade
FROM Friend F 
JOIN Highschooler H1 ON F.ID1 = H1.ID
WHERE H1.grade NOT IN (
  SELECT grade FROM Friend 
  JOIN Highschooler ON ID = ID2 WHERE ID1 = H1.ID
);