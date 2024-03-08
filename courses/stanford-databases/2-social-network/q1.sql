-- Find the names of all students who are friends with someone named Gabriel.

SELECT name
FROM Friend JOIN Highschooler ON Friend.ID2 = Highschooler.ID
WHERE ID1 IN (
  SELECT ID FROM Highschooler WHERE name = 'Gabriel'
);