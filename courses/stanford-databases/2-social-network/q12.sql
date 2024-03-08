-- For all cases where A is friends with B, and B is friends with C, add a new friendship for the pair A and C. 
-- Do not add duplicate friendships, friendships that already exist, or friendships with oneself.

INSERT INTO Friend
SELECT DISTINCT F1.ID1, F2.ID2 
FROM Friend F1 
JOIN Friend F2 ON (F1.ID2 = F2.ID1 AND F2.ID2 <> F1.ID1)
WHERE F2.ID2 NOT IN (
  SELECT F3.ID2 FROM Friend F3 WHERE F3.ID1 = F1.ID1
);
