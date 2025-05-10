--Get name of points, based on largest polygon or point, for a given position and rectangle around this point 
SELECT
    t.value AS relation_name,
  X( p.point ),
Y( p.point ), 
    MAX(p.area) AS max_area,
 ST_Distance( 
   p.point, 
  MakePoint( ?,?,4326 ) 
) AS distance,
 ST_Azimuth( MakePoint( ?,?,4326), p.point ) * 180 / PI() AS bearing  
 FROM
    spt_Polygons AS p
 JOIN tbl_Tags AS t ON t.elementID = p.elementID
 WHERE
  ST_Contains( BuildMbr( ?,?,?,? ), p.point )  
    AND t.key = 'name'
    AND p.point IS NOT NULL
 GROUP BY
    t.value
 ORDER BY distance ASC LIMIT 20	;
