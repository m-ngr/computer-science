/*
For each rating that is the lowest (fewest stars) currently in the database,
return the reviewer name, movie title, and number of stars.
*/

SELECT name, title, stars
FROM Rating
JOIN Reviewer USING (rID)
JOIN Movie USING (mID)
WHERE stars = (
  SELECT MIN(stars) FROM Rating
);
