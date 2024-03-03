-- For each movie that has at least one rating,
-- find the highest number of stars that movie received. 
-- Return the movie title and number of stars. Sort by movie title.

SELECT title, MAX(stars) AS stars
FROM Rating JOIN Movie USING(mID)
GROUP BY mID
ORDER BY title;