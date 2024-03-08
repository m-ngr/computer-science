-- If two students A and B are friends, and A likes B but not vice-versa, remove the Likes tuple.

DELETE FROM Likes
WHERE (ID1, ID2) IN(
  SELECT * FROM Friend F
  JOIN Likes USING(ID1, ID2)
  WHERE F.ID1 NOT IN (
    SELECT ID2 FROM Likes WHERE ID1 = F.ID2
  )
);