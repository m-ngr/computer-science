-- Find the name and grade of all students who are liked by more than one other student.

SELECT DISTINCT H.name, H.grade
FROM Likes
JOIN Highschooler H ON H.ID = ID2
GROUP BY ID2
HAVING COUNT(*) > 1;