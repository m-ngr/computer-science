/*
What is the average number of friends per student? (Your result should be just one number.)
*/
SELECT AVG(cnt)
FROM (
  SELECT COUNT(*) cnt
  FROM Friend 
  GROUP BY ID1
);