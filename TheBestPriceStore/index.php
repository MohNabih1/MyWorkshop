<?php
require './Service.php';

$service = new Service();
$items = $service->fetchAllItems(); 
?>

<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8">
    <title>Items</title>
</head>
<body>
    <div>Items</div>

    <table>
        <thead>
            <tr>
                <th>iId</th>
                <th>Iname</th>
            </tr>
        </thead>
        <tbody>
            <?php if (is_array($items)) :  ?>
                <?php foreach ($items as $item) : ?>
                    <tr>
                        <td><?= htmlspecialchars($item->iId); ?></td>
                        <td><?= htmlspecialchars($item->Iname); ?></td>
                    </tr>
                <?php endforeach; ?>
            <?php else : ?>
                <tr>
                    <td colspan="2">No items found.</td>
                </tr>
            <?php endif; ?>
        </tbody>
    </table>
</body>
</html>
