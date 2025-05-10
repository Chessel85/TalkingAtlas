--select values matching search text preferring key of name:en but using name if it is not available 
select COALESCE( t_en.value, t.value ) AS return_value,
  X( p.point ), Y( p.point )   
  from spt_Polygons AS  P 
  JOIN  tbl_tags AS t ON p.elementID = t.elementID 
  LEFT JOIN tbl_Tags as t_en  ON t.elementID = t_en.elementID  AND t_en.key='name:en' 
  where 
  t.key='name' 
  AND   ( t.value like ?  || '%'   OR t_en.value like ? || '%'  )
order by return_value limit 30;
