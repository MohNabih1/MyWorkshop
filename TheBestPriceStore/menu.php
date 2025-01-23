<?php
if ($_SERVER["REQUEST_METHOD"] == "POST" && isset($_POST['choice'])) {
    $option = $_POST['choice'];
    switch ($option) {
        case 1:
            header("Location: http://localhost/displayItem.php");
            break;
        case 2:
            header("Location: http://localhost/insertItem.php");
            break;
        case 3:
            header("Location: http://localhost/updateItem.php");
            break;
        case 4:
            header("Location: http://localhost/deleteItem.php");
            break;
        case 5:
            exit;
    }
}
?>

<!DOCTYPE html>
<html>
<head>
    <title>Menu</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 20px;
        }
        form {
            max-width: 400px;
            margin: 0 auto;
            text-align: center;
        }
        fieldset {
            border: 1px solid #ccc;
            padding: 20px;
            border-radius: 8px;
        }
        legend {
            font-size: 1.2em;
            font-weight: bold;
        }
        label {
            font-size: 1.1em;
            margin-bottom: 10px;
            display: inline-block;
        }
        input[type="number"] {
            width: 50px;
            padding: 5px;
            margin: 10px 0;
            border: 1px solid #ccc;
            border-radius: 4px;
            text-align: center;
        }
        input[type="submit"] {
            margin-top: 10px;
            padding: 8px 16px;
            border: none;
            border-radius: 4px;
            background-color: #007BFF;
            color: white;
            cursor: pointer;
        }
        input[type="submit"]:hover {
            background-color: #0056b3;
        }
        ul {
            list-style-type: none;
            padding: 0;
            margin: 0 0 20px;
        }
        li {
            font-size: 1.1em;
            margin: 5px 0;
        }
    </style>
</head>
<body>
    <form method="post">
        <fieldset>
            <legend>Menu</legend>
            <ul>
                <li>1. Display Items</li>
                <li>2. Insert Item</li>
                <li>3. Update Item</li>
                <li>4. Delete Item</li>
                <li>5. Exit</li>
            </ul>
            <label for="choice">Enter your choice:</label>
            <input type="number" name="choice" id="choice" min="1" max="5" required><br>
            <input type="submit" value="Submit">
        </fieldset>
    </form>
</body>
</html>
