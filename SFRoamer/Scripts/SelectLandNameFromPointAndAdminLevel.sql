--Select name of polygon matching coordinates and admin level preferring the name:en over just name key 
SELECT DISTINCT COALESCE(t2.value, t.value) AS value,
        CASE WHEN a.value IN ('island', 'islet') THEN 1 ELSE 0 END AS is_island_or_islet,
 r.area AS area,
 t.elementType,
 t.elementID  
FROM spt_Polygons AS p
  JOIN tbl_Relations AS r ON r.relationID =  t.elementID 
  JOIN tbl_Tags AS a  ON a.elementID = t.elementID 
JOIN tbl_Tags AS t ON p.elementID = t.elementID
LEFT JOIN tbl_Tags AS t2 ON p.elementID = t2.elementID AND t2.key = 'name:en'
WHERE 
     p.id IN (
         SELECT p.id 
         FROM spt_Polygons AS p
         WHERE ROWID IN (
             SELECT ROWID FROM SpatialIndex WHERE(( f_table_name = 'spt_Polygons' )
  AND ( f_geometry_column = 'polygon' )
             AND ( search_frame = MakePoint( ?,? ) ))
         )
  AND p.point IS NOT NULL 
         AND ST_Contains(p.polygon , MakePoint( ?,? ))
    )   
AND t.key IN ('name', 'name:en')
  AND ( ( a.key='admin_level' AND ( a.value=? OR a.value=? ) )
   OR ( a.key='place' AND a.value='island' OR a.value='islet') )
 ORDER BY p.area ASC;
