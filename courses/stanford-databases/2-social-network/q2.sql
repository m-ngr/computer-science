-- For every student who likes someone 2 or more grades younger than themselves, 
-- return that student's name and grade, and the name and grade of the student they like.

SELECT H1.name, H1.grade, H2.name, H2.grade 
FROM Likes L 
JOIN Highschooler H1 ON L.ID1 = H1.ID
JOIN Highschooler H2 ON L.ID2 = H2.ID
WHERE H1.grade - H2.grade >= 2;