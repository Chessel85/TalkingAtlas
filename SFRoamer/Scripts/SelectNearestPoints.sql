--Get the nearest points with distance and bearing info 
	SELECT t.value, 
 ST_Distance
 ( 
 ST_Transform( point, 32631 ),  
 St_Transform( MakePoint( ?,? , 4326  ), 32631 ) 
) / 1000 AS distance ,
 ST_Azimuth
(
MakePoint( ?,? ) ,
  p.point
) * 180 / PI() AS bearing   
 FROM knn2 AS k
 JOIN spt_Polygons AS p ON k.FID = p.id 
 JOIN  tbl_Tags AS t ON t.elementID = p.elementID 
 WHERE f_table_name = 'spt_Polygons' AND f_geometry_column = 'point' AND ref_geometry = MakePoint ( ?,? ) AND radius = 0.5 AND max_items = 8 AND expand = 0
 AND t.key='name:en'
 AND distance > 0 
 ORDER BY distance;

