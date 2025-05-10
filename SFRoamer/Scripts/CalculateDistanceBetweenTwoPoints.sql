--Return the distance in kilometres between two points given in lat lon
SELECT 
ST_Distance( 
  ST_Transform( MakePoint( ?,?, 4326 ), 32631), 
  ST_Transform( MakePoint( ?, ?, 4326), 32631) 
) / 1000 AS distance;