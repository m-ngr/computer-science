/*
Find the movie(s) with the lowest average rating. Return the movie title(s) and average rating. 
(Hint: This query may be more difficult to write in SQLite than other systems; you might think of it as finding the lowest average rating and then choosing the movie(s) with that average rating.)
*/

SELECT title, rating
FROM (
  SELECT mID, AVG(stars) AS rating 
  FROM Rating GROUP BY mID
) AS MV JOIN Movie USING(mID)
WHERE rating = (
  SELECT MIN(rating) FROM (
    SELECT mID, AVG(stars) AS rating 
    FROM Rating GROUP BY mID
  )
);