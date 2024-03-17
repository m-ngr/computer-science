-- Find the titles of all movies not reviewed by Chris Jackson.

SELECT DISTINCT title
FROM Movie 
WHERE mID NOT IN (
  SELECT mID FROM Rating 
  JOIN Reviewer USING(rID)
  WHERE name = 'Chris Jackson' 
);