<?php
require './Service.php';

$service = new Service();

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    if (isset($_POST['exit'])) {
        header("Location: http://localhost/menu.php");
        exit;
    } elseif (isset($_POST['submit'])) {
        $result = $service->updateItem();
    }
}
?>

<!DOCTYPE html>
<html>
<head>
    <title>Update Item</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 20px;
        }
        form {
            max-width: 400px;
            margin: 0 auto;
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
            display: inline-block;
            width: 100px;
            margin-bottom: 10px;
            text-align: right;
        }
        input[type="text"] {
            width: calc(100% - 120px);
            padding: 5px;
            margin-bottom: 10px;
            border: 1px solid #ccc;
            border-radius: 4px;
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
        .button-group {
            text-align: center;
        }
    </style>
    <script>
        function handleExit() {
            
            document.getElementById('iId').removeAttribute('required');
            document.getElementById('Iname').removeAttribute('required');
        }
    </script>
</head>
<body>
    <form method="post">
        <fieldset>
            <legend>Update Item</legend>

            <label for="iId">Item ID:</label>
            <input type="text" name="iId" id="iId" placeholder="Item ID" required><br>

            <label for="Iname">Item Name:</label>
            <input type="text" name="Iname" id="Iname" placeholder="New Item Name" required><br>

            <div class="button-group">
                <input type="submit" name="submit" value="Update">
                <input type="submit" name="exit" value="Exit" onclick="handleExit()">
            </div>
        </fieldset>
        <?php if (isset($result)) : ?>
            <p><?= htmlspecialchars($result); ?></p>
        <?php endif; ?>
    </form>
</body>
</html>
