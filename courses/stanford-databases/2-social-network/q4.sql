-- Find all students who do not appear in the Likes table (as a student who likes or is liked) 
-- and return their names and grades. Sort by grade, then by name within each grade.

SELECT name, grade FROM Highschooler
WHERE ID NOT IN (
  SELECT ID1 FROM Likes UNION SELECT ID2 FROM Likes
) ORDER BY grade, name;

SELECT name, grade
FROM Highschooler
LEFT JOIN Likes ON ID = ID1 OR ID = ID2
WHERE ID1 IS NULL AND ID2 IS NULL
ORDER BY grade, name;
