<?php
require './Service.php';

$service = new Service();

$searchResults = [];
$error = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    if (isset($_POST['searchKey']) && isset($_POST['searchValue']) && !empty($_POST['searchValue'])) {
        $searchKey = $_POST['searchKey'];
        $searchValue = $_POST['searchValue'];

        
        if (in_array($searchKey, ['iId', 'Iname'])) {
            $searchResults = $service->fetchItem($searchKey, $searchValue);
        } else {
            $error = "Invalid search key.";
        }
    } else {
        $error = "Please provide a valid search key and value.";
    }
}
?>

<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>Display Item</title>
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
        input[type="text"], select {
            width: calc(100% - 120px);
            padding: 5px;
            margin-bottom: 10px;
            border: 1px solid #ccc;
            border-radius: 4px;
        }
        input[type="submit"], button {
            margin-top: 10px;
            padding: 8px 16px;
            border: none;
            border-radius: 4px;
            background-color: #007BFF;
            color: white;
            cursor: pointer;
        }
        input[type="submit"]:hover, button:hover {
            background-color: #0056b3;
        }
        table {
            width: 100%;
            border-collapse: collapse;
            margin: 20px 0;
        }
        th, td {
            border: 1px solid #ccc;
            padding: 8px;
            text-align: left;
        }
        th {
            background-color: #f2f2f2;
        }
        .button-group {
            text-align: center;
        }
    </style>
</head>
<body>
    <form method="post" action="displayItem.php">
        <fieldset>
            <legend>Search Items</legend>

            <label for="searchKey">Search by:</label>
            <select name="searchKey" id="searchKey">
                <option value="iId">Item ID</option>
                <option value="Iname">Item Name</option>
            </select><br>

            <label for="searchValue">Search Value:</label>
            <input type="text" name="searchValue" id="searchValue" placeholder="Enter value" required><br>

            <div class="button-group">
                <input type="submit" value="Search">
            </div>
        </fieldset>
    </form>

    <?php if ($error): ?>
        <p style="color: red; text-align: center;"><?= htmlspecialchars($error); ?></p>
    <?php endif; ?>

    <h2 style="text-align: center;">Results</h2>
    <table>
        <thead>
            <tr>
                <th>Item ID</th>
                <th>Item Name</th>
                <th>Price</th>
                <th>Description</th>
            </tr>
        </thead>
        <tbody>
            <?php if (!empty($searchResults)): ?>
                <?php foreach ($searchResults as $item): ?>
                    <tr>
                        <td><?= htmlspecialchars($item->iId); ?></td>
                        <td><?= htmlspecialchars($item->Iname); ?></td>
                        <td><?= htmlspecialchars($item->Sprice); ?></td>
                        <td><?= htmlspecialchars($item->Idescription); ?></td>
                    </tr>
                <?php endforeach; ?>
            <?php else: ?>
                <tr>
                    <td colspan="4" style="text-align: center;">No items found.</td>
                </tr>
            <?php endif; ?>
        </tbody>
    </table>

    <div class="button-group">
        <form method="get" action="menu.php">
            <button type="submit">Exit</button>
        </form>
    </div>
</body>
</html>
