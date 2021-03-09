<?php  

header("Refresh: 50");

$db=new PDO('mysql:dbname=yii2test;host=192.168.7.1;','root','root');  
$row=$db->prepare('SELECT * FROM arduino_test ORDER BY id DESC LIMIT 1');  

$row->execute();//execute the query  
$json_data=array();//create the array  
foreach($row as $rec)//foreach loop  
{  
	$json_array['id']=$rec['id'];  
	$json_array['idSensor']=$rec['idSensor'];  
	$json_array['fridgeName']=$rec['fridgeName'];
	$json_array['date']=$rec['date'];    
	$json_array['temperature']=$rec['temperature'];  
	$json_array['humidity']=$rec['humidity'];
	$json_array['heat_index']=$rec['heat_index'];    
//here pushing the values in to an array  
	array_push($json_data,$json_array);  
}  

//built in PHP function to encode the data in to JSON format  
echo json_encode($json_data, JSON_UNESCAPED_UNICODE);   

?>
