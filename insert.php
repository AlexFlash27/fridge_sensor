<?php  

header("Refresh: 290");

$db=new PDO('mysql:dbname=yii2test;host=','','');  
$row=$db->prepare('SELECT * FROM arduino_test ORDER BY id DESC LIMIT 1');  

$row->execute();
$json_data=array();
foreach($row as $rec)
{  
	$json_array['id']=$rec['id'];  
	$json_array['idSensor']=$rec['idSensor'];  
	$json_array['fridgeName']=$rec['fridgeName'];
	$json_array['date']=$rec['date'];    
	$json_array['temperature']=$rec['temperature'];  
	$json_array['humidity']=$rec['humidity'];
	$json_array['heat_index']=$rec['heat_index'];    
  
	array_push($json_data,$json_array);  
}  

echo json_encode($json_data, JSON_UNESCAPED_UNICODE);   

?>
