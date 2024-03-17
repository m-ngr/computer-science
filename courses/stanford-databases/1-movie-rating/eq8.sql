/*
Find the names of all reviewers who have contributed three or more ratings.
(As an extra challenge, try writing the query without HAVING or without COUNT.)
*/

SELECT DISTINCT name
FROM Reviewer
WHERE rID IN (
  SELECT rID FROM Rating GROUP BY rID HAVING COUNT(*) >= 3
);