-- Find the names of all reviewers who rated Gone with the Wind.

SELECT DISTINCT name
FROM Rating 
JOIN Reviewer USING (rID)
WHERE mID = (
  SELECT mID FROM Movie WHERE title = 'Gone with the Wind'
);