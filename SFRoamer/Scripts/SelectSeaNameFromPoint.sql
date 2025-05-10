--Get name of IHO polygon containing given point  
--If using inflated sea polygons can overlap a bit so order by area 
SELECT pk_uid, name 
 FROM spt_IHOSeas 
 WHERE ROWID IN 
 ( 
  SELECT ROWID FROM SpatialIndex WHERE 
  ( 
   (f_table_name = 'spt_IHOSeas') AND 
   (search_frame = MakePoint(?, ? ) )
  ) 
 ) 
 AND 
 ( ST_Intersects(ihopolygon, MakePoint(?, ?))
)
ORDER BY area ASC LIMIT 1;