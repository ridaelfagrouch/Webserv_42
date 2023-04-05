<?php

header("Content-Type: text/html");
// Set the content type to HTML

$name = "sami";
$value = "sami";


setcookie($name, "Hello, world!", time()+3600);





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