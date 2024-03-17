/*
List movie titles and average ratings, from highest-rated to lowest-rated. 
If two or more movies have the same average rating, list them in alphabetical order.
*/

SELECT title, AVG(stars) AS rating
FROM Rating JOIN Movie USING(mID)
GROUP BY mID
ORDER BY rating DESC, title;