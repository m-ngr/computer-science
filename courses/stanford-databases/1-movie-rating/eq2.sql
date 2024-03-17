-- For any rating where the reviewer is the same as the director of the movie, 
-- return the reviewer name, movie title, and number of stars.

SELECT name, title, stars
FROM Rating 
JOIN Reviewer USING(rID)
JOIN Movie USING(mID)
WHERE name = director;