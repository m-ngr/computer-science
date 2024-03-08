-- Find names and grades of students who only have friends in the same grade.
-- Return the result sorted by grade, then by name within each grade.

SELECT H1.name, H1.grade
FROM Friend
JOIN Highschooler H1 ON ID1 = H1.ID
GROUP BY ID1
HAVING COUNT(*) = (
  SELECT COUNT(*)
  FROM Friend F
  JOIN Highschooler H2 ON F.ID2 = H2.ID
  WHERE F.ID1 = H1.ID AND H1.grade = H2.grade 
) ORDER BY H1.grade, H1.name;


SELECT DISTINCT H1.name, H1.grade
FROM Friend
JOIN Highschooler H1 ON ID1 = H1.ID
WHERE NOT EXISTS (
  SELECT 1
  FROM Friend F
  JOIN Highschooler H2 ON F.ID2 = H2.ID
  WHERE F.ID1 = H1.ID AND H1.grade <> H2.grade 
) ORDER BY H1.grade, H1.name;