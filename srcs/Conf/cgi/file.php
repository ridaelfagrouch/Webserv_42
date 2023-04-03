<?php

// Set the content type to HTML
header("Content-Type: text/html");
setcookie('mycookie', 'Hello, Cookie!', time() + (1 * 30), '/');
session_start();

$_SESSION['myvar'] = 'Hello, Session!';

// Generate some HTML
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

// Return the HTML content
echo $html;
?>