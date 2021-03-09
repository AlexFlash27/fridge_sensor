<?php
//$url=$_SERVER['REQUEST_URI'];
//header("Refresh: 10; URL=$url"); // Refresh the webpage every 10 seconds
include ('connection.php');

$sql_insert = "INSERT INTO arduino_test (idSensor, fridgeName, temperature, humidity, heat_index) VALUES ('".$_GET['idSensor']."', '".$_GET['fridgeName']."', '".$_GET['temperature']."', '".$_GET['humidity']."', '".$_GET['heat_index']."')";

if (mysqli_query($con,$sql_insert))
	{
		echo "Done";
		mysqli_close($con);
	}
else
	{
		echo "error is ".mysqli_error($con);
	}
?>