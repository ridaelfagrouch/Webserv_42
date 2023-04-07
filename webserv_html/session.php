<?php

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
	<h1>hello from session!</h1>
</body>
</html>
";

// Return the HTML content
echo $html;
?>