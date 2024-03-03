-- Some reviewers didn't provide a date with their rating. Find the names of all reviewers who have ratings with a NULL value for the date.

SELECT DISTINCT name
FROM Reviewer JOIN Rating USING(rID)
WHERE ratingDate IS NULL;