-- Find all years that have a movie that received a rating of 4 or 5, and sort them in increasing order.

SELECT DISTINCT year 
FROM Movie JOIN Rating USING (mID)
WHERE stars >= 4 ORDER BY year;