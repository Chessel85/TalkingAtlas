--Return relation names that border the relation at a point  with same admin level as each other 
select DISTINCT b.toRelationName,
  X( p2.point ),
  Y( p2.point ),
 ST_Azimuth ( MakePoint( ?,? ), p2.point ) * 180/PI() AS bearing 
  FROM tbl_Borders  AS b 
  JOIN spt_Polygons AS p2 ON  toPolygonID = p2.id 
  JOIN tbl_Tags AS t ON t.elementID =  p2.elementID AND t.key='admin_level' AND ( t.value=?  OR t.value=? )
  WHERE fromPolygonID IN (
SELECT id
  FROM spt_Polygons  AS p
  WHERE
    p.id IN (
      SELECT p.id 
        FROM spt_Polygons AS p
  JOIN tbl_Tags AS t ON  t.elementID = p.elementID AND key='admin_level' 
        WHERE p.ROWID IN (
          SELECT ROWID FROM SpatialIndex WHERE(( f_table_name = 'spt_Polygons' )
          AND  f_geometry_column = 'polygon' 
          AND ( search_frame = MakePoint( ?,? ) ))
        )
      AND ST_Contains(p.polygon , MakePoint( ?,? ))
  AND ( t.value = ? OR t.value = ?  ) 
  ) 
 ) ORDER BY bearing;
