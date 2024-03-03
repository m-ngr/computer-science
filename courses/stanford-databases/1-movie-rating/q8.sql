-- For each movie, return the title and the 'rating spread', that is, 
-- the difference between highest and lowest ratings given to that movie.
-- Sort by rating spread from highest to lowest, then by movie title.

SELECT title, MAX(stars) - MIN(stars) AS spread
FROM Rating JOIN Movie USING(mID)
GROUP BY mID
ORDER BY spread DESC, title;