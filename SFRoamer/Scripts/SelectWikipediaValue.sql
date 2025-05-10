--Given an elementType and elementID return any wikipedia value 
SELECT value
 FROM tbl_Tags 
 WHERE elementType = ? AND elementID = ? AND key='wikipedia';