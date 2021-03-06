<?php
$url=$_SERVER['REQUEST_URI'];
header("Refresh: 150");
?>
<html>
<head>
	<title>Arduino Ethernet Database</title>
	<style type="text/css">
		.table_titles {
			//padding-right: 10px;
			//padding-left: 10px;
			color:#FFF;
			background-color:#606060;
			border: 1px solid #333;
		}
		table {
			border: 2px solid #333;
			font-size: 90%;
			font-family: calibri;
		}
		.table_cells_odd {
			background-color:#F0F0F0;
			padding-right: 10px;
			text-align: left;
			border: 1px solid #333;
		}
		.table_cells_even {
			background-color:#A0A0A0;
			padding-right: 10px;
			text-align: left;
			border: 1px solid #333;
		}
		body { font-family: "Trebuchet MS", Courier; }
	</style>
</head>
<body>
	<table border="0" cellspacing="0" cellpadding="4">
		<tr>
			<td class="table_titles">№</td>
			<td class="table_titles">Помещение - <br>Оборудование</td>
			<td class="table_titles">Дата и время</td>
			<td class="table_titles">Температура<br>°С</td>
			<td class="table_titles">Влажность<br>%</td>
		</tr>
		<?php
		include('connection.php');
		$result = mysqli_query($con,'SELECT * FROM arduino_test ORDER BY id DESC');
		$oddrow = true;

		while($row = mysqli_fetch_array($result))
		{
			if ($oddrow)
			{
				$css_class=' class="table_cells_odd"';
			}
			else
			{
				$css_class=' class="table_cells_even"';
			}
			$oddrow = !$oddrow; 
			echo "<tr>";
			echo "<td '.$css_class.'>" . $row['idSensor'] . "</td>";
			echo "<td '.$css_class.'>" . $row['fridgeName'] . "</td>";
			echo "<td '.$css_class.'>" . $row['date'] . "</td>";
			echo "<td '.$css_class.'>" . $row['temperature'] . "</td>";
			echo "<td '.$css_class.'>" . $row['humidity'] . "</td>";
			echo "</tr>"; 
		}

		mysqli_close($con);
		?>
	</table>
</body>
</html>