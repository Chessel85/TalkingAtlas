SELECT t.value, p.area,  
 ST_Distance
 ( 
 ST_Transform( polygon, 32631 ),  
 St_Transform( MakePoint( ?,? , 4326  ), 32631 ) 
) / 1000 AS distance ,
 ST_Azimuth
(
MakePoint( ?,? ) ,
ClosestPoint
(
  p.polygon, 
  MakePoint( ?,? )
 ) 
) * 180 / PI() AS bearing   
 FROM knn2 AS k
 JOIN spt_Polygons AS p ON k.FID = p.id 
 JOIN  tbl_Tags AS t ON t.elementID = p.elementID 
 WHERE f_table_name = 'spt_Polygons' AND ref_geometry = MakePoint ( ?,? ) AND radius = 2.0 AND max_items = 8 AND expand = 1
 AND t.key='name:en'
 AND distance > 0 ;

