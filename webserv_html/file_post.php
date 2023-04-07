<?php

if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    // Handle the POST request

    // Get the data from the request
    $name = $_POST['name'];
    $email = $_POST['email'];

    // Do something with the data
    // For example, you could save it to a database

    // Send a response back to the client
    $html = "
    <!DOCTYPE html>
    <html>
    <head>
    	<title>My HTML Page</title>
    </head>
    <body>
    	<h1>Welcome to my HTML page $name!</h1>
    	<p>This is some example text. $email</p>
    </body>
    </html>
    ";

    // Return the HTML content
    echo $html;

} else {
    // Send an error response if the request method is not POST
    http_response_code(405); // Method Not Allowed
    echo "Error: Only POST requests are allowed";
}

?>