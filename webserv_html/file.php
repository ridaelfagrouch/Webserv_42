#!/usr/bin/php
<?php
// Set the cookie
$test = getenv("HTTP_COOKIE");



setcookie("mycookie", "Hello, world!", time()+10, '/');

$html = "
<!DOCTYPE html>
<html>
<head>
	<title>My HTML Page</title>
</head>
<body>
	<h1>Welcome to my HTML page!</h1>
	<p>This is some example text.</p>
    echo $test;
</body>
</html>
";

// Send the cookie header
// header('Location: http://www.example.com/');

// Echo a message to confirm the cookie
// echo "Cookie set!";
// echo $html
?>