-- For all cases where the same reviewer rated the same movie twice 
-- and gave it a higher rating the second time
-- return the reviewer's name and the title of the movie.

SELECT name, title
FROM Rating R1
JOIN Reviewer USING(rID)
JOIN Movie USING(mID) 
WHERE R1.stars > (
  SELECT stars 
  FROM Rating R2 
  WHERE R1.rID = R2.rID AND R1.mID = R2.mID AND R1.ratingDate > R2.ratingDate
);

-- OR

SELECT name, title
FROM Rating R1 JOIN Rating R2 USING(rID, mID)
JOIN Reviewer USING(rID)
JOIN Movie USING(mID)
WHERE R1.stars > R2.stars AND R1.ratingDate > R2.ratingDate;