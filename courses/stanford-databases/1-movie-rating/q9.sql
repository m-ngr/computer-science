-- database: rating.db
-- Find the difference between the average rating of movies released before 1980 and 
-- the average rating of movies released after 1980. 
-- (Make sure to calculate the average rating for each movie, then 
-- the average of those averages for movies before 1980 and movies after. 
-- Don't just calculate the overall average rating before and after 1980.)
SELECT
  AVG(Old.avg)
FROM
  (
    SELECT
      AVG(stars) AS avg
    FROM
      Rating
      JOIN Movie USING (mID)
    WHERE
      year < 1980
    GROUP BY
      mID
  ) AS Old;

SELECT
  (
    SELECT
      AVG(Old.avg)
    FROM
      (
        SELECT
          AVG(stars) AS avg
        FROM
          Rating
          JOIN Movie USING (mID)
        WHERE
          year < 1980
        GROUP BY
          mID
      ) AS Old
  ) - (
    SELECT
      AVG(New.avg)
    FROM
      (
        SELECT
          AVG(stars) AS avg
        FROM
          Rating
          JOIN Movie USING (mID)
        WHERE
          year > 1980
        GROUP BY
          mID
      ) AS New
  );

FROM
  (
    SELECT
      mID,
      AVG(stars) AS avg
    FROM
      Rating
      JOIN Movie USING (mID)
    WHERE
      year > 1980
    GROUP BY
      mID
  ) New,
  (
    SELECT
      mID,
      AVG(stars) AS avg
    FROM
      Rating
      JOIN Movie USING (mID)
    WHERE
      year < 1980
    GROUP BY
      mID
  ) Old;
