<?php
$username = "";
$pass = "";
$host = "";
$db_name = "yii2test";
$con = mysqli_connect ($host, $username, $pass);
$db = mysqli_select_db ( $con, $db_name );
?>
