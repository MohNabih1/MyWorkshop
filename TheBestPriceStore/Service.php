<?php
require './Database.php';
require './Item.php';

class Service {
    function fetchAllItems() {
        $dbObject = new Database();
        $dbConnection = $dbObject->getDatabaseConnection();

        $sql = "SELECT * FROM item"; 
        $stmt = $dbConnection->prepare($sql);
        $stmt->setFetchMode(PDO::FETCH_CLASS, 'Item'); 
        if ($stmt->execute()) {
            return $stmt->fetchAll();
        } else {
            return [];
        }
    }

    function fetchItem($key, $value) {
        $dbObject = new Database();
        $dbConnection = $dbObject->getDatabaseConnection();
    
        
        $allowedKeys = ['iId', 'Iname'];
        if (!in_array($key, $allowedKeys)) {
            return [];
        }
    
        
        $sql = "SELECT iId, Iname, Sprice, Idescription FROM item WHERE $key = :value";
        $stmt = $dbConnection->prepare($sql);
        $stmt->bindParam(':value', $value);
    
        $stmt->setFetchMode(PDO::FETCH_CLASS, 'Item');
        if ($stmt->execute()) {
            return $stmt->fetchAll();
        } else {
            return [];
        }
    }    function addItem() {
        if (isset($_POST['exit'])) {
            header("Location: http://localhost/menu.php");
            exit;
        }

        $Iname = $_POST['name'];
        $iId = $_POST['iId'];

        $dbObject = new Database();
        $dbConnection = $dbObject->getDatabaseConnection();

        $sql = "INSERT INTO item (Iname, iId) VALUES (:Iname, :iId)";
        $stmt = $dbConnection->prepare($sql);
        $stmt->bindParam(':Iname', $Iname);
        $stmt->bindParam(':iId', $iId);

        return $stmt->execute() ? "Item added successfully." : "Failed to add item.";
    }

    function deleteItem() {
        if (isset($_POST['exit'])) {
            header("Location: http://localhost/menu.php");
            exit;
        }
    
        
        $iId = $_POST['iId'];
    
        $dbObject = new Database();
        $dbConnection = $dbObject->getDatabaseConnection();
    
        
        $sql = "DELETE FROM item WHERE iId = :iId";
        $stmt = $dbConnection->prepare($sql);
        $stmt->bindParam(':iId', $iId);
    
        return $stmt->execute() ? "Item deleted successfully." : "Failed to delete item.";
    }
    

    function updateItem() {
        if (isset($_POST['exit'])) {
            header("Location: http://localhost/menu.php");
            exit;
        }

        
        $iId = $_POST['iId'];
        $Iname = $_POST['Iname'];

        $dbObject = new Database();
        $dbConnection = $dbObject->getDatabaseConnection();

        
        $sql = "UPDATE item SET Iname = :Iname WHERE iId = :iId";
        $stmt = $dbConnection->prepare($sql);
        $stmt->bindParam(':Iname', $Iname);
        $stmt->bindParam(':iId', $iId);

        return $stmt->execute() ? "Item updated successfully." : "Failed to update item.";
    }
}
?>
