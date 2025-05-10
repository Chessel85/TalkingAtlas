--Return the bearing in degrees between two points given in lat lon
SELECT 
  ST_Azimuth(
  MakePoint( ?,?,4326 ),
  MakePoint( ?,?,4326 ) ) * 180/PI() AS bearing;