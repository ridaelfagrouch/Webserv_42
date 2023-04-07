#!/usr/bin/php
<?php
// Set the cookie


$html = "
<!DOCTYPE html>
<html>
<head>
	<title>My HTML Page</title>
</head>
<body>
	<h1>Welcome to my HTML page!</h1>
	<p>This is some example text.</p>
</body>
</html>
";

// Send the cookie header
// header('Location: http://www.example.com/');

// Echo a message to confirm the cookie
// echo "Cookie set!";
echo $html;


?>