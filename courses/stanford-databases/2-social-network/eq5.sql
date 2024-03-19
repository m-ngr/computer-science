/*
Find the name and grade of the student(s) with the greatest number of friends.
*/

SELECT name, grade
FROM Highschooler
JOIN Friend ON ID = ID1
GROUP BY ID1
HAVING COUNT(*) = (
  SELECT MAX(cnt) FROM (
    SELECT COUNT(*) AS cnt FROM Friend GROUP BY ID1
  )
);