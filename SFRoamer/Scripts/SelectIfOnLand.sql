--Return 1 if given point is in a land polygon or 0 otherwise 
SELECT   CASE WHEN EXISTS (
  SELECT 1 
  FROM spt_LandPolygons  AS l 
  WHERE l.ROWID IN (
    SELECT ROWID FROM SpatialIndex WHERE (
      ( f_table_name = 'spt_LandPolygons' )
      AND ( search_frame = MakePoint( ?,? ) )
    ) 
  )
 AND ST_Contains( l.lpolygon , MakePoint(  ?,? ))
  )
THEN 1 ELSE 0 END AS PointInside;
