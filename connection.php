<?php
$username = "root";
$pass = "root";
$host = "192.168.7.1";
$db_name = "yii2test";
$con = mysqli_connect ($host, $username, $pass);
$db = mysqli_select_db ( $con, $db_name );
?>