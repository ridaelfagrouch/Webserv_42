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
        <h1>this is POST request!</h1>
    	<h1>Welcome to my HTML page $name!</h1>
    	<p>This is some example text. $email</p>
    </body>
    </html>
    ";

    // Return the HTML content
    echo $html;

} else {


    $html = "
    <!DOCTYPE html>
    <html>
    <head>
    	<title>My HTML Page</title>
    </head>
    <body>
    	<h1>this is GET request!</h1>
    </body>
    </html>
    ";

    // Return the HTML content
    echo $html;
}

?>