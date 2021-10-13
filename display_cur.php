<?php
header("Refresh: 150");
?>
<!DOCTYPE HTML>
<html>
<style type="text/css">
	body {
		font-family: Calibri, sans-serif;
	}

	@media only screen and (max-width: 768px) {
  
  	}

  	#fridgeName {
  		font-size: 25px;
  	}

  	#temp, #hum {
  		font-size: 25px;
  	}


</style>


<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
<script>
	$(document).ready(function() {
		$.ajax({
			type: 'GET',
			dataType: 'json',
			url: '/insert.php',
			data: 'json',
			success: function(data){
    			console.log(data[0]);
    			document.title = 'Датчик № ' + data[0].idSensor;
    		for(var key in data[0]) { 
    			try { document.getElementById(key).innerHTML=data[0][key]; } 
    			catch(err) { console.log(key+' не найден'); } 
    		}
    	}
	})
});
</script>

<hr>
<b id='fridgeName'></b>
<div id='temp'>Температура ℃: <b id="temperature"></b></div>
<div id='hum'>Влажность %: <b id="humidity"></b></div>
<hr>

</html>