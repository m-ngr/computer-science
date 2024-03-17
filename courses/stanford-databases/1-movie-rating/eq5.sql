/* For all pairs of reviewers such that both reviewers gave a rating to the same movie, 
return the names of both reviewers. Eliminate duplicates, don't pair reviewers with themselves, 
and include each pair only once. For each pair, return the names in the pair in alphabetical order.
*/


SELECT DISTINCT R1.name, R2.name
FROM (Reviewer JOIN Rating USING(rID)) AS R1,
     (Reviewer JOIN Rating USING(rID)) AS R2
WHERE R1.name < R2.name AND R1.mID = R2.mID
ORDER BY R1.name, R2.name;