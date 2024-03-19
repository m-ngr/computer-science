/*
Find the number of students who are either friends with Cassandra or are friends of friends of Cassandra.
Do not count Cassandra, even though technically she is a friend of a friend.
*/

SELECT COUNT(*) + COUNT(DISTINCT F2.ID1)
FROM Friend F1
JOIN Friend F2 ON (F1.ID2 = F2.ID1)
WHERE F1.ID1 = (
  SELECT ID FROM Highschooler WHERE name = 'Cassandra'
) AND F2.ID2 <> F1.ID1;