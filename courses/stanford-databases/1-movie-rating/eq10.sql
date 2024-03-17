/*
Find the movie(s) with the highest average rating. Return the movie title(s) and average rating. 
(Hint: This query is more difficult to write in SQLite than other systems; you might think of it as finding the 
 highest average rating and then choosing the movie(s) with that average rating.)
*/
SELECT title, rating
FROM (
  SELECT mID, AVG(stars) AS rating 
  FROM Rating GROUP BY mID
) AS MV JOIN Movie USING(mID)
WHERE rating = (
  SELECT MAX(rating) FROM (
    SELECT mID, AVG(stars) AS rating 
    FROM Rating GROUP BY mID
  )
);