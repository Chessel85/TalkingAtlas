--Select name of polygon matching coordinates preferring the name:en over just name key 
SELECT DISTINCT COALESCE(t2.value, t.value) AS value, 
  p.area as area 
FROM spt_Polygons AS p
JOIN tbl_Tags AS t ON p.elementID = t.elementID
LEFT JOIN tbl_Tags AS t2 ON p.elementID = t2.elementID AND t2.key = 'name:en'
WHERE 
     p.id IN (
         SELECT p.id 
         FROM spt_Polygons AS p
         WHERE ROWID IN (
             SELECT ROWID FROM SpatialIndex WHERE(( f_table_name = 'spt_Polygons' )
  AND ( f_geometry_column = 'polygon'  )
             AND ( search_frame = MakePoint( ?,? ) ))
         )
  AND p.point IS NOT NULL 
         AND ST_Contains(p.polygon , MakePoint( ?,? ))
    )   AND t.key IN ('name', 'name:en')
 ORDER BY p.area ASC;
