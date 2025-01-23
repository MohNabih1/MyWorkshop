<?php

class Database {

	public function getDatabaseConnection(){

		$dbHost = "localhost";
		$dbName = "bestpricestore";
		$dbUser = "root";
		$dbPassword = "";

		try {
			
			$dbConnection = new PDO('mysql:host='.$dbHost.';dbname='.$dbName, $dbUser, $dbPassword);
			$dbConnection->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
			return $dbConnection;
		} catch (PDOException $e) {
			echo $e->getMessage();
		}
	}
}