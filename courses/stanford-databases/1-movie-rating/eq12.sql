/*
For each director, return the director's name together with the title(s) of the movie(s) they directed 
that received the highest rating among all of their movies, and the value of that rating.
Ignore movies whose director is NULL.
*/

-- <director> <movie-name> <max-rating>

SELECT director, title, stars
FROM Movie JOIN Rating USING(mID)
WHERE director IS NOT NULL
GROUP BY director
HAVING stars = MAX(stars);