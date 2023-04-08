<?php

// Set the content type to HTML
header("Content-Type: text/html");

$name = "sami";
// $test = ;

setcookie($name, "Hello, world!", time()+360);





$html = "
<!DOCTYPE html>
<html>
<head>

</head>
<body>
	<h1>cookie name $name</h1>
</body>
</html>
";

// Return the HTML content
echo $html;
?>