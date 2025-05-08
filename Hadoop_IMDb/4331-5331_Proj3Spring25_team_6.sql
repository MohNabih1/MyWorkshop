/*
Dev Patel
Yahia Elsaad
Mohamad Nabih Alkhateeb
Project 3 Team 6 - Task 2 SQL code



English query:
The objective of the following query is to identify the top 5 highest-rated movies released between 2011 and 2020 that belong to both the "Action" and "Thriller" genres. 
To ensure relevance and quality (correct conditions), the query only considers:
Movies (titleType = 'movie')
That have received at least 150,000 votes
With an average rating of 7.0 or higher
And includes the lead actor or actress for each movie, identified as the person with ordering = 1 in the title_principals table
The results are sorted in descending order of average rating, and only the top 5 movies are returned.*/

-- SQL Query:
SET ECHO ON;
SET LINESIZE 200;
SET PAGESIZE 100;
COLUMN title FORMAT A35;
COLUMN lead_actor_or_actress FORMAT A25;
SPOOL 4331-5331_Proj3Spring25_team_6.sql

SELECT
  b.primarytitle AS title,
  r.averagerating,
  n.primaryname AS lead_actor_or_actress
FROM
  imdb00.title_basics b
JOIN
  imdb00.title_ratings r ON b.tconst = r.tconst
JOIN
  imdb00.title_principals p ON b.tconst = p.tconst
JOIN
  imdb00.name_basics n ON p.nconst = n.nconst
WHERE
  b.titletype = 'movie'
  AND b.startyear BETWEEN 2011 AND 2020
  AND LOWER(b.genres) LIKE '%action%'
  AND LOWER(b.genres) LIKE '%thriller%'
  AND r.numvotes >= 150000
  AND r.averagerating >= 7.0
  AND p.ordering = '1'
ORDER BY
  r.averagerating DESC
FETCH FIRST 5 ROWS ONLY;


-- SQL Query output:
/*
TITLE                              AVERAGERATING   LEAD_ACTOR_OR_ACTRESS
—-------------------------------  —-------------  ----------------------------
Skyfall                                     7.8     Daniel Craig
Mission: Impossible – Fallout               7.7     Tom Cruise
The Raid: Redemption                        7.6     Iko Uwais
Train to Busan                              7.6     Gong Yoo
Upgrade                                     7.5     Logan Marshall-Green
*/

--Query plan:
EXPLAIN PLAN FOR
SELECT
  b.primarytitle AS title, r.averagerating, n.primaryname AS lead_actor_or_actress
FROM
  imdb00.title_basics b
JOIN
  imdb00.title_ratings r ON b.tconst = r.tconst
JOIN
  imdb00.title_principals p ON b.tconst = p.tconst
JOIN
  imdb00.name_basics n ON p.nconst = n.nconst
WHERE
  b.titletype = 'movie'
  AND b.startyear BETWEEN 2011 AND 2020
  AND LOWER(b.genres) LIKE '%action%'
  AND LOWER(b.genres) LIKE '%thriller%'
  AND r.numvotes >= 150000
  AND r.averagerating >= 7.0
  AND p.ordering = '1'
ORDER BY
  r.averagerating DESC
FETCH FIRST 5 ROWS ONLY;

--then run this to see results of the explanation:
SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY());


--Query plan output:
/*----------------------------------------------------------------------------------
| Id  | Operation                 | Name             | Rows | Bytes | Cost | Time |
----------------------------------------------------------------------------------
|  0  | SELECT STATEMENT         |                  |   5  | 10215 | 133K | 00:00:06 |
|* 1  |  VIEW                    |                  |   5  | 10215 | 133K | 00:00:06 |
|* 2  |   WINDOW SORT PUSHED RANK|                  |   6  | 1080  | 133K | 00:00:06 |
|  3  |    NESTED LOOPS          |                  |   6  | 1080  | 133K | 00:00:06 |
|  4  |     NESTED LOOPS         |                  |   6  | 1080  | 133K | 00:00:06 |
|  5  |      HASH JOIN           |                  |   6  |  840  | 133K | 00:00:06 |
|  6  |       NESTED LOOPS       |                  |   4  |  460  | 2614 | 00:00:01 |
|  7  |        TABLE ACCESS FULL | TITLE_RATINGS    | 657  | 11169 | 1084| 00:00:01 |
|  8  |        INDEX UNIQUE SCAN | SYS_C00547784    |   1  |       |     | 00:00:01 |
|* 9  |       TABLE ACCESS BY INDEX ROWID | TITLE_BASICS | 1 | 98   |   2 | 00:00:01 |
|*10  |     TABLE ACCESS FULL    | TITLE_PRINCIPALS | 8191K| 195M  | 131K| 00:00:06 |
| 11  |      INDEX UNIQUE SCAN   | SYS_C00547785    |   1  |       |     | 00:00:01 |
|*12  |     TABLE ACCESS BY INDEX ROWID | NAME_BASICS | 1 | 40   |   2 | 00:00:01 |
----------------------------------------------------------------------------------

Predicate Information (identified by operation id):
----------------------------------------------------
  1 - filter("from$_subquery$_008"."rowlimit_$$_rownumber"<=5)
  2 - filter(ROW_NUMBER() OVER (ORDER BY INTERNAL_FUNCTION("r"."averagerating") DESC) <=5)
  5 - access("b"."tconst"="r"."tconst")
  6 - filter("r"."numvotes">=150000 AND "r"."averagerating">=7.0)
  9 - access("b"."tconst"="r"."tconst")
 10 - filter("b"."titletype"='movie' AND TO_NUMBER("b"."startyear")>=2011 AND 
             TO_NUMBER("b"."startyear")<=2020 AND 
             LOWER("b"."genres") LIKE U'%action%' AND 
             LOWER("b"."genres") LIKE U'%thriller%')
 11 - filter("p"."ordering"='1')
 12 - access("p"."nconst"="n"."nconst")

34 rows selected.

*/